#pragma once

#include <string>
#include <vector>

#include "aixlog.hpp"

#include "serial_factory.h"

class Controller
{
public:
    Controller(const std::string& port_name);
    ~Controller();

    bool isInit() const;
    bool send(const std::vector<uint8_t>& buff);
    bool read(std::vector<uint8_t>& buff);
    bool read_fd(std::vector<uint8_t>& buff, bool print = true);

    bool isConnected() const;
    void setConnected(bool connected);

private:
    int fd;
    bool is_connected;
    bool is_init;    

    bool open(const std::string& port_name);
    bool set_params(uint32_t baud_rate);

    PtrSerial ptrSerial;
};
