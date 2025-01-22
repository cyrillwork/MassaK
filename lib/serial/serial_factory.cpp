#include "serial_factory.h"

#ifdef MASSAK_WINDOWS
    #include "winserial.h"
#else
    #include "linuxserial.h"
#endif

PtrSerial serial_factory()
{
#ifdef MASSAK_WINDOWS
    return std::make_shared<WinSerial>();
#else
    return std::make_shared<LinuxSerial>();
#endif

}
