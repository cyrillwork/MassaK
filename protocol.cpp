#include "protocol.h"
#include "messages.h"

#include <algorithm>

Protocol::Protocol()
{
}

void Protocol::getMassa(std::vector<uint8_t>& buff)
{
    size_t len_message = sizeof(GetMassa);
    data.clear();
    data.reserve(len_message);

    GetMassa message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::setZero(std::vector<uint8_t>& buff)
{
    size_t len_message = sizeof(SetZero);
    data.clear();
    data.reserve(len_message);

    SetZero message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::setTare(std::vector<uint8_t>& buff)
{
    size_t len_message = sizeof(SetTare);
    data.clear();
    data.reserve(len_message);

    SetTare message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::addCRC(uint8_t* data, size_t len)
{
}
