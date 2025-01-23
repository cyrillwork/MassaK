#pragma once

#include "iserial.h"

namespace win_serial
{

#define MAX_BUFSIZE 255

class WinSerial: public ISerial
{
public:

    WinSerial();
    virtual ~WinSerial() override;

    bool IsOK() const;

    bool open(const char *pathname, int flags) override;
    void close() override;

    bool set_params(uint32_t baud_rate) override;

    size_t read(char* buff, size_t len) override;
    size_t write(const char* buff, size_t len) override;

    int select(size_t timeout) override;

    int tcsetattr(int optional_actions, const termios* termios_p) override;
    int cfsetispeed(speed_t speed) override;
    int cfsetospeed(speed_t speed) override;

private:
    HANDLE m_Handle;

    unsigned char in_buffer[MAX_BUFSIZE];
    unsigned long counter={0};

    DCB ComDCM = {}; //Initializing DCB structure
};

}
