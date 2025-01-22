#include "rsinterface.h"

#include <thread>

RSInterface::RSInterface(TypeParamsRS _params, PtrSerial _serial):
    IInterface(_params),
    _channelId(-1),
    serial(_serial)
{
    params = _params;
    if(params)
    {
        //std::cout << "constructor RSInterface devPath=" << params->getDevPath() << std::endl;
    }
    else
    {
        std::cout << "Fatal Error get TypeParamsRS. Exit program" << std::endl;
        exit(0);
    }

    //samlpe using log
    //log = std::make_unique<LogMessage>(REC_TO_FILE|REC_ADD_TIME/*|REC_SHOW_DISPLAY*/);
    //communication_timer= std::make_unique<timer_chrono>(ERROR_COMMUNICATION_TIME);
}

RSInterface::~RSInterface()
{

}

bool RSInterface::open()
{
    //communication_timer->resetWatch(false);
    std::cout << "RSInterface::open() " << params->getDevPath() << std::endl;

#ifdef MASSAK_WINDOWS
    _channelId = serial->open(params->getDevPath().c_str(),O_RDWR /* | O_NOCTTY | O_NONBLOCK  O_RDWR | O_NOCTTY | O_NDELAY*/);
#else
    _channelId = serial->open(params->getDevPath().c_str(),O_RDWR | O_NOCTTY /*| O_NONBLOCK  O_RDWR | O_NOCTTY | O_NDELAY*/);
#endif

    if (_channelId < 0) {
        std::cout << "Can't open device" << params->getDevPath() << std::endl;
        perror(params->getDevPath().c_str());
        return false;
    }

    memset (&newtio0, 0, sizeof (newtio0));

    if(params->get9thBit())
    {
        std::cout<<"Set 9thBit mode"<<std::endl;
        memset(&newtio0, 0x00, sizeof(newtio0));
        newtio0.c_cflag = params->getByteSize() | CLOCAL | CREAD | (int)params->getParity();
        newtio0.c_oflag = 0;
        newtio0.c_lflag = 0;
        newtio0.c_cc[VTIME] = 1;    // inter-character timer unused
        newtio0.c_cc[VMIN]  = 0;    // blocking read until  chars received
        newtio0.c_cflag &= ~CRTSCTS;                                                   // disable hardware flow control
        newtio0.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);                            // raw mode!
#ifdef MASSAK_WINDOWS
        newtio0.c_iflag &= ~(INPCK | ISTRIP /*| IUCLC*/ | IGNCR | ICRNL | INLCR | PARMRK); // raw mode!
        newtio0.c_iflag &= ~(IXON | IXOFF /*| IXANY*/);                                    // disable software flow control
#else
        newtio0.c_iflag &= ~(INPCK | ISTRIP | IUCLC | IGNCR | ICRNL | INLCR | PARMRK); // raw mode!
        newtio0.c_iflag &= ~(IXON | IXOFF | IXANY);                                    // disable software flow control
#endif
        newtio0.c_oflag &= ~OPOST;
        newtio0.c_iflag |= IGNBRK;
        newtio0.c_iflag &= ~IGNPAR;
        newtio0.c_iflag &= ~ISTRIP;
        newtio0.c_iflag |= INPCK;
        newtio0.c_iflag |= PARMRK; //Mark all bytes received with 9th bit set by "ff 0"
#ifndef MASSAK_WINDOWS
        newtio0.c_cflag |= CMSPAR;
#endif
        newtio0.c_cflag &= ~PARODD;	//normal state - space parity

        serial->cfsetospeed( params->getBaudRate() );
        serial->cfsetispeed( params->getBaudRate() );

        if (serial->tcsetattr(TCSANOW, &newtio0)!=0)
        {
            printf("[Qcom serial] newtio0 write error!\n");
            return -1;
        }
    }
    else
    {
        std::cout<<"Set 8thBit mode\n"<<std::endl;
        newtio0.c_cflag = params->getByteSize() | CLOCAL | CREAD | (int)params->getParity();
        newtio0.c_oflag = 0;
        newtio0.c_lflag = 0;
        newtio0.c_cc[VTIME] = 1;    // inter-character timer unused
        newtio0.c_cc[VMIN]  = 0;    // blocking read until  chars received

        newtio0.c_cflag &= ~CRTSCTS;                                                   // disable hardware flow control
        newtio0.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);                            // raw mode!
        newtio0.c_iflag &= ~(INPCK | ISTRIP /*| IUCLC*/ | IGNCR | ICRNL | INLCR | PARMRK); // raw mode!
        newtio0.c_iflag &= ~(IXON | IXOFF /*| IXANY*/);                                    // disable software flow control
        newtio0.c_oflag &= ~OPOST;
        newtio0.c_iflag &=~PARMRK; //Mark all bytes received with 9th bit set by "ff 0"
#ifndef MASSAK_WINDOWS
        newtio0.c_cflag |= CMSPAR;
#endif
        serial->cfsetospeed( params->getBaudRate() );
        serial->cfsetispeed( params->getBaudRate() );

        if (serial->tcsetattr(TCSANOW, &newtio0)!=0)
        {
            printf("[Qcom serial] newtio0 write error!\n");
            return -1;
        }
    }


    return true;
}

bool RSInterface::close()
{
    return (serial->close() == 0)?true:false;
}
//---------------------------------------------------------------------------
bool RSInterface::set9thBit(bool _is9thbit)
{
    std::cout<<"Set 9thBit "<<((_is9thbit)?"enable":"disable")<<std::endl;
    params->set9thBit(_is9thbit);

    std::cout<<"RS Interface close"<<std::endl;
    close();
    open();
    std::cout<<"RS Interface run"<<std::endl;

    return 0;
}

//---------------------------------------------------------------------------

int RSInterface::read(char *data, int /*size*/, int timeout)
{
    int result=0;

    //clear timer communication error
    if(serial->select(static_cast<size_t>(timeout)) > 0)
    {
        /*
        int size_pack=0;

        POLL_STATUS status = GetPoll((unsigned char*)data,size_pack);

        if(status == POLL_STATUS::NONE_FRAME)
            return result;

        if(status == POLL_STATUS::STARTBIT1_ERROR || status == POLL_STATUS::STARTBIT1_ERROR)
        {
            ++IncompleteMessages;
            return result;
        }

        if(status == POLL_STATUS::RECIVE_FRAME)
        {
            if(PollAddress==POOL_ADDRES::ADDRESS_OUR)
            {
                ++TotalsPolls;
                communication_timer->resetWatch();
            }
            log->insertQCOMlog(HOST,data,size_pack);
            result = size_pack;
        }

        if(PollAddress==POOL_ADDRES::ADDRESS_INCORECT)
            return result;

        if(status==POLL_STATUS::CRC_ERROR)
        {
            if (PollAddress==POOL_ADDRES::ADDRESS_OUR)
            {
                ++CRC_ErrorsPolls;
                //        log.insertlog("from [%d] poll [0x%02X] CRC error\n",data[0],data[3]);
            }
            else
            {
                //   log.insertlog("from [%d] poll [0x%02X] BCRC error\n",data[0],data[3]);
                ++B_CRC_ErrorsPolls;
            }
        }
        if(status==POLL_STATUS::LEN_ERROR)
        {
            ++IncompleteMessages;
            //log.insertlog("IncompleteMessages\n");
        }

        //   return static_cast<int>(serial->read(data, static_cast<size_t>(size)));
        */
    }

    return result;
}

int RSInterface::write(const char *data, int size)
{
    int res = 0;
    int offset = 0;

    if(!data) {
        return -1;
    }

    res += serial->write(data + offset, size);

    //log->insertQCOMlog(EGM,(char *)data,size);
    //std::cout << "write res=" << res << std::endl;

    return res;
}

//void RSInterface::SettingsLogFile(const char*file, size_t _max_size_file) {
//    log->SetLogSettings(file, _max_size_file);
//}

