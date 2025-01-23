#include "linuxserial.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>

#include "aixlog.hpp"

LinuxSerial::LinuxSerial()
{
}

LinuxSerial::~LinuxSerial()
{
}

bool LinuxSerial::open(const char* pathname, int flags)
{
    fd = ::open(pathname, flags);
    return (fd != -1);
}

void LinuxSerial::close()
{
    if(fd != -1) {
        ::close(fd);
    }
}

bool LinuxSerial::set_params(uint32_t baud_rate)
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
        cfsetispeed(baud_rate);
        cfsetospeed(baud_rate);
        {
            tty.c_cflag |= (CLOCAL | CREAD);

            if(baud_rate == B57600) {
                LOG(INFO) << "Set speed B57600" << std::endl;
                tty.c_cflag |= PARENB; // Enable parity
                tty.c_cflag |= PARODD; // Set odd parity                
            } else if(baud_rate == B19200) {
                LOG(INFO) << "Set speed B19200" << std::endl;
                // clear mark/space parity
                tty.c_cflag &= ~CMSPAR;
            } else if(baud_rate == B4800) {
                LOG(INFO) << "Set speed B4800" << std::endl;
                tty.c_cflag |= PARENB; // Enable parity
            } else {
                LOG(INFO) << "Error unsupported speed" << std::endl;
            }

            tty.c_cflag &= ~CSTOPB; // 1 stop bit
            tty.c_cflag &= ~CSIZE;
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

    if (tcsetattr(TCSANOW, &tty) != 0) {
        LOG(INFO) << "Error tcsetattr" << std::endl;
        return result;
    } else {
        LOG(INFO) << "tcsetattr OK" << std::endl;
    }

    result = true;
    return result;

}

int64_t LinuxSerial::read(uint8_t* buff, uint64_t len)
{	
    return ::read(fd, buff, len);
}

int64_t LinuxSerial::write(const uint8_t* buff, uint64_t len)
{
    auto res = ::write(fd, buff, len);
    fsync(fd);
    return res;
}

int LinuxSerial::select(uint64_t timeout)
{
    struct timeval tv;
    fd_set fds;
    int resfds;
    FD_ZERO (&fds);
    FD_SET (fd, &fds);

    tv.tv_sec = (size_t)(timeout / 1000000);
    tv.tv_usec = timeout - tv.tv_sec*1000000;

    resfds = ::select(fd + 1, &fds, NULL, NULL, &tv);

    return (resfds <= 0) ? 0 : 1;
}

int LinuxSerial::tcsetattr(int optional_actions, const termios* termios_p)
{
    ::memcpy(&(this->termios_p), termios_p, sizeof(termios));

    ::cfsetispeed(&(this->termios_p), ispeed);
    ::cfsetospeed(&(this->termios_p), ospeed);

    return ::tcsetattr(fd, optional_actions, &(this->termios_p));
}

int LinuxSerial::cfsetispeed(speed_t speed)
{
    ispeed = speed;
    return 1;
}

int LinuxSerial::cfsetospeed(speed_t speed)
{
    ospeed = speed;
    return 1;
}
