#include "controller.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

Controller::Controller(const std::string& port_name, bool high_speed):
    is_connected{false}
{
    ptrSerial = serial_factory();
    //std::cout << "Controller" << std::endl;

    is_init = false;
    int flags = O_RDWR /*| O_NONBLOCK*/;

    is_init = ptrSerial->open(port_name.c_str(), flags);
    if (!is_init) {
        LOG(INFO) << "Error open port: " << port_name << std::endl;
    } else {
        LOG(INFO) << "port opened OK " << std::endl;
    }

    //fcntl(fd, F_SETFL, FNDELAY);	//read with no delay    
    is_init = ptrSerial->set_params(high_speed ? "57600" : "4800"/*baud_rate*/);
    //std::cout << "set_params is_init: " << is_init << std::endl;
}

Controller::~Controller()
{
    if(ptrSerial) {
        ptrSerial->close();
    }
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
    uint64_t timeout = 500000;
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
