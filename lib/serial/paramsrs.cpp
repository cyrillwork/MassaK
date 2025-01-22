#include "paramsrs.h"


ParamsRS232::ParamsRS232(ParamsRS232 &params):
    devPath(params.getDevPath()),
    parity(params.getParity()),
    speed(params.getBaudRate()),
    byteSize(params.getByteSize()),
    Is9thbit(params.Is9thbit)
{
}

ParamsRS232::ParamsRS232(string _devPath, Parity _parity, int _speed, ByteSize _byteSize,bool _Is9thbit ):
    devPath(_devPath),
    parity(_parity),

    byteSize(_byteSize),
    Is9thbit(_Is9thbit)
{
    speed = _speed;
}

std::string ParamsRS232::getBaudRateString()
{
    std::string result = {""};

    for(auto it: baudeRate)
    {
        if(it.second == speed)
        {
            return string(it.first);
        }
    }

    return string(result);
}

std::string ParamsRS232::getByteSizeString()
{
    std::string res="";
    switch(byteSize)
    {
        case ByteSize::_CS5: res= "CS5"; break;
        case ByteSize::_CS6: res= "CS6"; break;
        case ByteSize::_CS7: res= "CS7"; break;
        case ByteSize::_CS8: res= "CS8"; break;
    }
    return std::string(res);
}

void ParamsRS232::setBaudRate(string _speed)
{
    auto it = baudeRate.find(_speed);

    if(it != baudeRate.end())
    {
        speed = baudeRate[_speed];
    }
    else
    {
        std::cout << "Error find baude rate " << _speed << " set default 9600" << std::endl;
        speed   = baudeRate["9600"];
    }
}

const std::string& ParamsRS232::getDevPath() const
{
    return devPath;
}
