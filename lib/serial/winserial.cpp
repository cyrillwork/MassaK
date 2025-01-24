#include "winserial.h"

#include <thread>

using namespace win_serial;

//OVERLAPPED overlapped;
//OVERLAPPED overlappedwr;

WinSerial::WinSerial():
    m_Handle(static_cast<HANDLE>(INVALID_HANDLE_VALUE))
{
}

WinSerial::~WinSerial()
{
    close();
}

bool WinSerial::IsOK() const
{
    return m_Handle != INVALID_HANDLE_VALUE;
}

bool WinSerial::open(const char *pathname, int flags)
{
    bool result = false;

    ++fd;

    std::string port(pathname);
    std::wstring str1(port.begin(), port.end());
    m_Handle = CreateFile( str1.c_str(),
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           nullptr,
                           OPEN_EXISTING,
                           0,//FILE_FLAG_OVERLAPPED,
                           nullptr
                           );

    if(m_Handle == INVALID_HANDLE_VALUE) {
        std::cout << "!!! Invalid open port" << std::endl;
        return -1;
    }

    return result;
}

void WinSerial::close()
{
    //CloseHandle(overlappedwr.hEvent);
    //CloseHandle(overlapped.hEvent);
    if(m_Handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_Handle);
        m_Handle = INVALID_HANDLE_VALUE;
    }
}

bool WinSerial::set_params(const std::string& baud_rate)
{
    bool result = false;

    //инициализация порта
    ComDCM.DCBlength = sizeof(ComDCM);

    if(!GetCommState(m_Handle, &ComDCM)) {
        std::cout << "!!! Invalid GetCommState" << std::endl;
        CloseHandle(m_Handle);
        m_Handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);
        return result;
    }
/*
    ComDCM.BaudRate = baud_rate;
    ComDCM.fBinary = TRUE;
    ComDCM.fOutxCtsFlow = FALSE;
    ComDCM.fOutxDsrFlow = FALSE;
    ComDCM.fDtrControl = DTR_CONTROL_DISABLE;
    ComDCM.fDsrSensitivity = FALSE;
    ComDCM.fNull = FALSE;
    ComDCM.fRtsControl = RTS_CONTROL_DISABLE;
    ComDCM.fAbortOnError = FALSE;
    ComDCM.ByteSize = 8;
    ComDCM.Parity = NOPARITY;
    ComDCM.fParity = TRUE;
    ComDCM.StopBits = ONESTOPBIT;
*/
    if(baud_rate == "57600") {
        ComDCM.BaudRate = CBR_57600;
        ComDCM.ByteSize = 8;
        ComDCM.Parity   = NOPARITY;
        ComDCM.StopBits = ONESTOPBIT;
    } else if(baud_rate == "4800") {
        ComDCM.BaudRate = CBR_4800;
        ComDCM.ByteSize = 8;
        ComDCM.Parity   = EVEN;
        ComDCM.StopBits = ONESTOPBIT;
    } else if(baud_rate == "19200") {
        ComDCM.BaudRate = CBR_19200;
        ComDCM.ByteSize = 8;
        ComDCM.Parity   = SPACE;
        ComDCM.StopBits = ONESTOPBIT;
    } else {
        std::cout << "!!! Invalid baud_rate: " << baud_rate << std::endl;
        return result;
    }

    if(!SetCommState(m_Handle, &ComDCM)) {
        CloseHandle(m_Handle);
        m_Handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);
        return result;
    }

    COMMTIMEOUTS CommTimeOuts;

    CommTimeOuts.ReadIntervalTimeout         = 100;	 	//таймаут между двумя символами
    CommTimeOuts.ReadTotalTimeoutMultiplier  = 100;	//общий таймаут операции чтения
    CommTimeOuts.ReadTotalTimeoutConstant    = 100;         //константа для общего таймаута операции чтения
    CommTimeOuts.WriteTotalTimeoutMultiplier = 100;      //общий таймаут операции записи
    CommTimeOuts.WriteTotalTimeoutConstant   = 100;        //константа для общего таймаута операции записи

    //записать структуру таймаутов в порт
    if(!SetCommTimeouts(m_Handle, &CommTimeOuts))	//если не удалось - закрыть порт и вывести сообщение об ошибке в строке состояния
    {
        CloseHandle(m_Handle);
        m_Handle = static_cast<HANDLE>(INVALID_HANDLE_VALUE);
        return result;
    }

//    SetupComm(m_Handle, 2000, 2000);
//    PurgeComm(m_Handle, PURGE_RXCLEAR);
//    overlapped.hEvent = CreateEvent(nullptr, true, true, nullptr);
//    overlappedwr.hEvent = CreateEvent(nullptr, true, true, nullptr);
//    SetCommMask(m_Handle, EV_RXCHAR|EV_TXEMPTY);

    result = true;

    return result;
}

int WinSerial::select(size_t timeout)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout*0.001));

    return 1;
/*
    COMSTAT comstat;
    DWORD btr, temp, mask, signal;
    unsigned char bufrd[MAX_BUFSIZE];

    WaitCommEvent(m_Handle, &mask, &overlapped);

    signal = WaitForSingleObject(overlapped.hEvent, static_cast<DWORD>(timeout*0.001));
    if(signal == WAIT_OBJECT_0)
    {
        if(GetOverlappedResult(m_Handle, &overlapped, &temp, true))
            if((mask & EV_RXCHAR)!=0)
            {
                ClearCommError(m_Handle, &temp, &comstat);
                btr = comstat.cbInQue;
                if(btr)
                {
                    ReadFile(m_Handle, in_buffer, btr, &temp, &overlapped);
                    counter=btr;
                    //std::cout<<"read pack"<<std::endl;
                    return fd;
                }
            }
    }

    return -1;
*/
}


int64_t WinSerial::write(const uint8_t* buff, uint64_t len)
{    
    DWORD bytesWritten = 0;

    if(m_Handle == INVALID_HANDLE_VALUE) {
        std::cout << "!!! WinSerial::write INVALID_HANDLE_VALUE" << std::endl;
        return 0;
    }

    WriteFile(m_Handle, buff, len, &bytesWritten, NULL);

//    DWORD feedback = 0;
//    DWORD temp, signal;
//    WriteFile(m_Handle, buff, static_cast<DWORD>(len), &feedback, &overlappedwr);
//    signal = WaitForSingleObject(overlappedwr.hEvent, INFINITE);
//    if((signal == WAIT_OBJECT_0) && (GetOverlappedResult(m_Handle, &overlappedwr, &temp, true))) {
//    } else {
//    }

    return bytesWritten;
}

int64_t WinSerial::read(uint8_t *buff, uint64_t len)
{
    DWORD bytesRead = 0;

    if(m_Handle == INVALID_HANDLE_VALUE) {
        std::cout << "!!! WinSerial::read INVALID_HANDLE_VALUE" << std::endl;
        return bytesRead;
    }

    ReadFile(m_Handle, buff, len, &bytesRead, NULL);

//    DWORD feedback = 0;
//    if (!counter)this->select(1500); //time out 3 byte* from 19200 = 1500
//    if (counter)
//    {
//        feedback=counter;
//        memcpy(buff,in_buffer,counter);
//        counter=0;
//    }

    return bytesRead;
}

/*nbyte 0->7*/
int getByte(tcflag_t flag, int nbyte, int nibble)
{

    int byte;
    if (nibble == 1)
        byte = (flag >> (8 * (nbyte)) & 0x0f);
    else
        byte = (flag >> (8 * (nbyte)) & 0xf0);
    return byte;
}
//INPUT FUNCTIONS

int getIXOptions(tcflag_t flag)
{
#define i_IXOFF 0x01
#define i_IXON 0x02
#define i_IXOFF_IXON 0x03
#define i_PARMRK 0x04
#define i_PARMRK_IXOFF 0x05
#define i_PARMRK_IXON 0x06
#define i_PARMRK_IXON_IXOFF 0x07

    int byte = getByte(flag, 1, 1);

    return byte;
}

//LOCALOPT FUNCTIONS

int getEchoOptions(tcflag_t flag)
{

#define l_NOECHO 0x00
#define l_ECHO 0x01
#define l_ECHO_ECHOE 0x03
#define l_ECHO_ECHOK 0x05
#define l_ECHO_ECHONL 0x09
#define l_ECHO_ECHOE_ECHOK 0x07
#define l_ECHO_ECHOE_ECHONL 0x0b
#define l_ECHO_ECHOE_ECHOK_ECHONL 0x0f
#define l_ECHO_ECHOK_ECHONL 0x0d
#define l_ECHOE 0x02
#define l_ECHOE_ECHOK 0x06
#define l_ECHOE_ECHONL 0x0a
#define l_ECHOE_ECHOK_ECHONL 0x0e
#define l_ECHOK 0x04
#define l_ECHOK_ECHONL 0x0c
#define l_ECHONL 0x08

    int byte = getByte(flag, 1, 1);
    return byte;
}

int getLocalOptions(tcflag_t flag) {

#define l_ICANON 0x10
#define l_ICANON_ISIG 0x50
#define l_ICANON_IEXTEN 0x30
#define l_ICANON_NOFLSH 0x90
#define l_ICANON_ISIG_IEXTEN 0x70
#define l_ICANON_ISIG_NOFLSH 0xd0
#define l_ICANON_IEXTEN_NOFLSH 0xb0
#define l_ICANON_ISIG_IEXTEN_NOFLSH 0xf0
#define l_ISIG 0x40
#define l_ISIG_IEXTEN 0x60
#define l_ISIG_NOFLSH 0xc0
#define l_ISIG_IEXTEN_NOFLSH 0xe0
#define l_IEXTEN 0x20
#define l_IEXTEN_NOFLSH 0xa0
#define l_NOFLSH 0x80

    int byte = getByte(flag, 1, 0);
    return byte;
}

int getToStop(tcflag_t flag) {

#define l_TOSTOP 0x01

    int byte = getByte(flag, 1, 1);
    return byte;
}

//CONTROLOPT FUNCTIONS

int getCharSet(tcflag_t flag)
{

    //FLAG IS MADE UP OF 8 BYTES, A FLAG IS MADE UP OF A NIBBLE -> 4 BITS, WE NEED TO EXTRACT THE SECOND NIBBLE (1st) FROM THE FIFTH BYTE (6th).
    int byte = getByte(flag, 1, 1);

    switch (byte) {

        case 0X0:
        return CS5;
        break;

        case 0X4:
        return CS6;
        break;

        case 0X8:
        return CS7;
        break;

        case 0Xc:
        return CS8;
        break;

        default:
        return CS8;
        break;
    }
}

int getControlOptions(tcflag_t flag)
{

#define c_ALL_ENABLED 0xd0
#define c_PAREVEN_CSTOPB 0x50
#define c_PAREVEN_NOCSTOPB 0x40
#define c_PARODD_NOCSTOPB 0xc0
#define c_NOPARENB_CSTOPB 0x10
#define c_ALL_DISABLED 0x00

    int byte = getByte(flag, 1, 0);
    return byte;
}



int WinSerial::tcsetattr(int optional_actions, const termios* termios_p)
{
    return 0;
    /*
    int ret = 0;

    ::memcpy(&(this->termios_p), termios_p, sizeof(termios));

    //Store flags into local variables
    tcflag_t iflag = termios_p->c_iflag;
    tcflag_t lflag = termios_p->c_lflag;
    tcflag_t cflag = termios_p->c_cflag;
    //tcflag_t oflag = termios_p->c_oflag;

    //std::cout << "speed=" <<(int)speed << std::endl;
    //iflag

    int IX = getIXOptions(iflag);

    if ((IX == i_IXOFF_IXON) || (IX == i_PARMRK_IXON_IXOFF)) {

        ComDCM.fOutX = TRUE;
        ComDCM.fInX = TRUE;
        ComDCM.fTXContinueOnXoff = TRUE;
    }


  //  lflag

    int EchoOpt = getEchoOptions(lflag);
    int l_opt = getLocalOptions(lflag);
    int tostop = getToStop(lflag);

    //Missing parameters...

    //    cflag


    int CharSet = getCharSet(cflag);
    int c_opt = getControlOptions(cflag);



    switch (CharSet)
    {
    case CS5:
        //std::cout << "!!!!!!!! CS5" << std::endl;
        ComDCM.ByteSize = 5;
        break;

    case CS6:
        //std::cout << "!!!!!!!! CS6" << std::endl;
        ComDCM.ByteSize = 6;
        break;

    case CS7:
        //std::cout << "!!!!!!!! CS7" << std::endl;
        ComDCM.ByteSize = 7;
        break;

    case CS8:
        //std::cout << "!!!!!!!! CS8" << std::endl;
        ComDCM.ByteSize = 8;
        break;
    }

    //std::cout << "!!!!!!!! ByteSize=" << (int)ComDCM.ByteSize << std::endl;

    switch (c_opt)
    {
    case c_ALL_ENABLED:
        ComDCM.Parity = ODDPARITY;
        ComDCM.StopBits = TWOSTOPBITS;
        break;
    case c_ALL_DISABLED:
        ComDCM.Parity = NOPARITY;
        ComDCM.StopBits = ONESTOPBIT;
        break;
    case c_PAREVEN_CSTOPB:
        ComDCM.Parity = EVENPARITY;
        ComDCM.StopBits = TWOSTOPBITS;
        break;
    case c_PAREVEN_NOCSTOPB:
        ComDCM.Parity = EVENPARITY;
        ComDCM.StopBits = ONESTOPBIT;
        break;
    case c_PARODD_NOCSTOPB:
        ComDCM.Parity = ODDPARITY;
        ComDCM.StopBits = ONESTOPBIT;
        break;
    case c_NOPARENB_CSTOPB:
        ComDCM.Parity = NOPARITY;
        ComDCM.StopBits = TWOSTOPBITS;
        break;
    }

    //   oflag

    //Missing parameters...

    //   special characters

    COMMTIMEOUTS timeouts = {};

    if (termios_p->c_cc[VEOF] != 0) ComDCM.EofChar = (char)termios_p->c_cc[VEOF];
    if (termios_p->c_cc[VINTR] != 0) ComDCM.EvtChar = (char)termios_p->c_cc[VINTR];

    if (termios_p->c_cc[VMIN] == 1)
    { //Blocking
        timeouts.ReadIntervalTimeout = 0;         // in milliseconds
        timeouts.ReadTotalTimeoutConstant = 0;    // in milliseconds
        timeouts.ReadTotalTimeoutMultiplier = 0;  // in milliseconds
        timeouts.WriteTotalTimeoutConstant = 0;   // in milliseconds
        timeouts.WriteTotalTimeoutMultiplier = 0; // in milliseconds
    }
    else
    { //Non blocking

        timeouts.ReadIntervalTimeout =termios_p->c_cc[VTIME];         // in milliseconds
        timeouts.ReadTotalTimeoutConstant =termios_p->c_cc[VTIME];    // in milliseconds
        timeouts.ReadTotalTimeoutMultiplier =termios_p->c_cc[VTIME];  // in milliseconds
        timeouts.WriteTotalTimeoutConstant = termios_p->c_cc[VTIME];   // in milliseconds
        timeouts.WriteTotalTimeoutMultiplier = 1; // in milliseconds
    }

    SetCommTimeouts(m_Handle, &timeouts);

    //  EOF

    ret = SetCommState(m_Handle, &ComDCM);

    if (ret != 0)
        return 0;
    else
        return -1;
*/
}

int WinSerial::cfsetispeed(speed_t speed)
{
    ComDCM.BaudRate = static_cast<DWORD>(speed);
    return 0;
}

int WinSerial::cfsetospeed(speed_t speed)
{
    ComDCM.BaudRate = static_cast<DWORD>(speed);
    return 0;
}
