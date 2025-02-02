#pragma once

#include <string>
#include <vector>

#include "aixlog.hpp"

#include "serial_factory.h"

class Controller
{
public:
    Controller(const std::string& port_name, bool high_speed);
    ~Controller();

    bool isInit() const;
    bool send(const std::vector<uint8_t>& buff);
    bool read(std::vector<uint8_t>& buff);
    bool read_fd(std::vector<uint8_t>& buff, bool print = true);

    bool isConnected() const;
    void setConnected(bool connected);

    bool open();
    void close();

private:
    //int fd;
    std::string name;
    std::string baud;
    bool is_connected;
    bool is_init;


    //bool open(const std::string& port_name);

    PtrSerial ptrSerial;
};
