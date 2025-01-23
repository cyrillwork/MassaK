#pragma once

#include <iostream>
#include <cstring>
#include <memory>
#include <cinttypes>

#ifdef MASSAK_WINDOWS
    //#define NOMINMAX //иначе API windows определит макросы min и max, конфликтующие с std::max и std::min в vector
    #include <windows.h>
    //#undef interface

    #include "win_serial.h"

    using namespace win_serial;
#else
    #include <termios.h>
#endif

class ISerial;
using PtrSerial = std::shared_ptr<ISerial>;

class ISerial
{
public:
    virtual ~ISerial() = default;

    virtual bool open (const char* pathname, int flags) = 0;

    virtual void close () = 0;

    virtual bool set_params(uint32_t baud_rate) = 0;

    virtual int64_t read(uint8_t* buff, uint64_t len) = 0;

    virtual int64_t write(const uint8_t* buff, uint64_t len) = 0;

    virtual int tcsetattr(int optional_actions, const termios* termios_p) = 0;

    /* Put the state of FD into *TERMIOS_P.  */
//    int tcgetattr (termios *termios_p)
//    {
//        if(!termios_p) return -1;
//        ::memcpy(termios_p, &(this->termios_p), sizeof(termios));
//        return 0;
//    }

    virtual int cfsetispeed(speed_t speed) = 0;

    virtual int cfsetospeed(speed_t speed) = 0;

    /** timeout milliseconds */
    virtual int select(uint64_t timeout) = 0;

protected:    
    int fd{0};

    termios termios_p = {};
};