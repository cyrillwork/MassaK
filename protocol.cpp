#include "protocol.h"
#include "messages.h"

#include <algorithm>
#include <iostream>

void Protocol::getMassa(Data& buff)
{
    size_t len_message = sizeof(GetMassa);
    buff.clear();
    buff.reserve(len_message);

    GetMassa message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::setZero(Data& buff)
{
    size_t len_message = sizeof(SetZero);
    buff.clear();
    buff.reserve(len_message);

    SetZero message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::setTare(Data& buff)
{
    size_t len_message = sizeof(SetTare);
    buff.clear();
    buff.reserve(len_message);

    SetTare message;
    uint8_t* ptr1 = (uint8_t*)&message;
    addCRC(ptr1, len_message);
    std::copy(ptr1, ptr1 + len_message, back_inserter(buff));
}

void Protocol::print(const Data& buff)
{
    for (const auto iii: buff) {
        std::cout << std::hex << (int)iii << " ";
    }
    std::cout << std::dec << "\n";
}

void Protocol::addCRC(uint8_t* data, size_t len)
{

}
