#include "controller.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sstream>

Controller::Controller(const std::string& port_name):
    is_connected{false}
{
    //std::cout << "Controller" << std::endl;
    is_init = open(port_name);
}

Controller::~Controller()
{
    //std::cout << "~Controller" << std::endl;
    if(fd >= 0) {
        ::close(fd);
    }
}

bool Controller::isInit() const
{
    return is_init;
}

bool Controller::open(const std::string& port_name)
{
    bool result = false;
    fd = ::open(port_name.c_str(), O_RDWR | O_NONBLOCK /*O_NOCTTY | O_NDELAY*/);
    if (fd == -1) {
        LOG(INFO) << "Error open port: " << port_name << std::endl;
        return result;
    } else {
        LOG(INFO) << "port opened " << port_name <<  " fd: " << fd << std::endl;
    }

    //fcntl(fd, F_SETFL, FNDELAY);	//read with no delay
    result = set_params(B57600 /*baud_rate*/);
    return result;
}

bool Controller::set_params(uint32_t baud_rate)
{
    bool result = false;

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0) {
        LOG(INFO) << "Erro get attr port" << std::endl;
        return result;
    } else {
        LOG(INFO) << "tcgetattr OK" << std::endl;
    }

    {
        cfsetispeed(&tty, baud_rate);
        cfsetospeed(&tty, baud_rate);
        {
            tty.c_cflag |= (CLOCAL | CREAD);

            tty.c_cflag |= PARENB; // Enable parity
            tty.c_cflag |= PARODD; // Set odd parity
            {
                tty.c_cflag &= ~CSTOPB;
            }			//1 stop bit
            tty.c_cflag &= ~CSIZE;	//bit mask for data bits
            tty.c_cflag |= CS8;	//8 data bits
            tty.c_cflag &= ~CRTSCTS;	//disable hardware flow control
            tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);	//raw mode!
            tty.c_iflag &= ~(INPCK | ISTRIP | IUCLC | IGNCR | ICRNL | INLCR | PARMRK);	//raw mode!
            tty.c_iflag &= ~(IXON | IXOFF | IXANY);	//disable software flow control
            tty.c_oflag &= ~OPOST;

            tty.c_cc[VMIN] = 0;
            tty.c_cc[VTIME]= 0;
        }
        tty.c_cflag &= ~CBAUD;
        tty.c_cflag |= baud_rate;	//9600BAUD
    }

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        LOG(INFO) << "Error tcsetattr" << std::endl;
        return result;
    } else {
        LOG(INFO) << "tcsetattr OK" << std::endl;
    }

    result = true;
    return result;

}

bool Controller::send(const std::vector<uint8_t>& buff)
{
    bool result = false;

    auto size_w = ::write(fd, buff.data(), buff.size());
    LOG(INFO) << std::dec << "write size: " << size_w << " buf_len: " << (int)buff.size() << std::endl;

    if((size_w >= 0) && ((size_t)size_w == buff.size())) {
        result = true;
    } else {
        LOG(INFO) << "Error Controller::send size_w: " << size_w << std::endl;
    }

    return result;
}

bool Controller::read(std::vector<uint8_t>& buff)
{
    bool result = false;
    if (read_fd(buff) && (buff.size() > 0)) {
        result = true;
    } else {
        LOG(INFO) << "Error reading answer send" << std::endl;
    }
    return result;
}

bool Controller::read_fd(std::vector<uint8_t>& buff, bool print)
{
    fd_set rfds;
    struct timeval tv;
    bool result = false;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec  = 0;
    tv.tv_usec = 500000;

    if(select(fd + 1, &rfds, NULL, NULL, &tv) > 0) {
        uint8_t response[256];
        int bytesRead = ::read(fd, response, sizeof(response));
        if (bytesRead > 0) {
            if(print) {
                std::cout << "IN: ";
                for (int i = 0; i < bytesRead; ++i) {
                    std::cout << std::hex << (int)response[i] << " ";
                }
                std::cout << "\n";
            }

            std::copy(response, response + bytesRead, back_inserter(buff));
            result = true;
        }
    }
    return result;
}

bool Controller::isConnected() const
{
    return is_connected;
}

void Controller::setConnected(bool connected)
{
    is_connected = connected;
}
