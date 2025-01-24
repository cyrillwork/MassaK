#pragma once

#include "iserial.h"
#include <memory>

class LinuxSerial: public ISerial
{

public:
    LinuxSerial();

    ~LinuxSerial() override;

    bool open(const char* pathname, int flags) override;

    bool set_params(const std::string& baud_rate) override;

    void close() override;

    int64_t read(uint8_t* buff, uint64_t len) override;

    int64_t write(const uint8_t* buff, uint64_t len) override;

    int select(uint64_t timeout) override;

    int tcsetattr(int optional_actions, const termios* termios_p) override;

    int cfsetispeed(speed_t speed) override;

    int cfsetospeed(speed_t speed) override;

private:
    speed_t ispeed={0};
    speed_t ospeed={0};
};
