#pragma once

#include <iostream>
#include <map>
#include <cstring>

#include "iserial.h"

#include "paramsrs.h"
#include "iinterface.h"

#ifdef _WIN32
    #include "serial/winserial.h"
    using namespace win_serial;
#else
    #include "serial/linuxserial.h"
    #include <sys/time.h>
#endif

//----------------------------------------------
#define ERROR_COMMUNICATION_TIME 1000  //milli sec.

class RSInterface : public IInterface
{
public:
    RSInterface(TypeParamsRS _params, PtrSerial _serial);
    ~RSInterface() override;

    bool open() override;
    bool close() override;
    bool set9thBit(bool _is9thbit);
    /* timeout - micro seconds*/
    int read(char *data, int size, int timeout) override;    
    int write(const char *data, int size) override;    

    //get statistic pool
    unsigned long GetTotalsPolsCounter(void)         {return TotalsPolls;}
    unsigned long GetIncompleteMessagesCounter(void)  {return IncompleteMessages;}
    unsigned long GetCRC_ErrorsPollsCounter(void)     {return CRC_ErrorsPolls;}
    unsigned long GetB_CRC_ErrorsPollsCounter(void)   {return B_CRC_ErrorsPolls;}

    //void logEnable(void){log->log_Enable();}
    //void logDisable(void){log->log_Disable();}
    //void SettingsLogFile(const char* file, size_t _max_size_file);

    //bool getCommunicationStatus(void){return communication_timer->isAlarm();}
    //void resetTimer() { communication_timer->resetWatch(); }

private:
    //std::unique_ptr<LogMessage> log;
    //statistic pool
    unsigned long TotalsPolls           =0;
    unsigned long IncompleteMessages    =0;
    unsigned long CRC_ErrorsPolls       =0;
    unsigned long B_CRC_ErrorsPolls     =0;

    // error communication line
    //std::unique_ptr<timer_chrono> communication_timer;
    //bool isFirstByte = false;
    int _channelId;
    //bool p9thBit=false;

    termios newtio0;
    TypeParamsRS params;    

    PtrSerial serial;
};
