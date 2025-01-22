#ifndef WINSERIAL_H
#define WINSERIAL_H

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

    virtual int open(const char *pathname, int flags) override;
    virtual int close() override;

    virtual size_t read(char* buff, size_t len) override;
    virtual size_t write(const char* buff, size_t len) override;

    virtual int select(size_t timeout) override;

    virtual int tcsetattr(int optional_actions, const termios* termios_p) override;
    virtual int cfsetispeed(speed_t speed) override;
    virtual int cfsetospeed(speed_t speed) override;

private:
    HANDLE m_Handle;

unsigned char in_buffer[MAX_BUFSIZE];
unsigned long counter={0};

    DCB ComDCM = {}; //Initializing DCB structure
};

}

#endif // WINSERIAL_H
