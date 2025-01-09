#pragma once

#include <cinttypes>
#include <vector>

class Protocol
{
public:
    Protocol();    
    void getMassa(std::vector<uint8_t>& buff);
    void setZero(std::vector<uint8_t>& buff);
    void setTare(std::vector<uint8_t>& buff);

private:
    void addCRC(uint8_t* data, size_t len);
};
