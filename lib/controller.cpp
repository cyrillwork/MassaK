#include "controller.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

Controller::Controller(const std::string& port_name, bool high_speed):
        name{port_name}
      , is_connected{false}
{
    ptrSerial = serial_factory();
    //std::cout << "Controller" << std::endl;

    baud = "4800";
    if(name.find("ttyACM") != std::string::npos ) {
        baud = "57600";
    }

    is_init = open();
    if(is_init) {
        ptrSerial->close();
    }
}

Controller::~Controller()
{
    close();
}

bool Controller::isInit() const
{
    return is_init;
}

bool Controller::send(const std::vector<uint8_t>& buff)
{
    bool result = false;

    auto size_w =  ptrSerial->write(buff.data(), buff.size());
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
        LOG(INFO) << "Error reading answer send size: " << buff.size() << std::endl;
    }
    return result;
}

bool Controller::read_fd(std::vector<uint8_t>& buff, bool print)
{
    bool result = false;
    uint64_t timeout = 1000000;
    buff.clear();
    while (true) {
        if(ptrSerial->select(timeout) > 0)
        {
            uint8_t response[256];
            int bytesRead = ptrSerial->read(response, sizeof(response));
            if (bytesRead > 0) {
                std::copy(response, response + bytesRead, back_inserter(buff));
            } else {
                LOG(INFO) << "Controller::read_fd bytesRead: " << bytesRead << "\n";
                break;
            }           
        } else {
            break;
        }
    }

    if(!buff.empty()) {
        result = true;
        if(print) {
            std::stringbuf buffer;
            std::ostream os (&buffer);
            os << "IN: ";
            for (const auto& iii: buff) {
                os << std::hex << (int)iii << " ";
            }
            LOG(INFO) << buffer.str() << "\n";
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

bool Controller::open()
{
    bool result;

    #ifdef MASSAK_WINDOWS
        int flags = O_RDWR;
    #else
        int flags = O_RDWR | O_NONBLOCK;
    #endif

    result = ptrSerial->open(name.c_str(), flags);
    if (!result) {
        LOG(INFO) << "Error open port: " << name << std::endl;
        result = false;
    } else {
        LOG(INFO) << "port opened OK " << std::endl;
        //fcntl(fd, F_SETFL, FNDELAY);	//read with no delay
        result = ptrSerial->set_params( baud );
    }

    return result;
}

void Controller::close()
{
    if(ptrSerial && is_init) {
        ptrSerial->close();
    }
}
