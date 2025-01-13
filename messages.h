#pragma once

#include <cinttypes>

enum ProtocolCommands: uint8_t
{
    CMD_GET_SCALE_PAR   = 0x75,
    CMD_GET_MASSA       = 0x23,
    CMD_SET_TARE        = 0xA3,
    CMD_SET_ZERO        = 0x72,
    CMD_GET_NAME        = 0x20,
    CMD_SET_NAME        = 0x22
};

struct CommonMessage
{
    CommonMessage(uint8_t _command):
        command(_command)
    { }
    uint8_t header_1 = 0xF8;
    uint8_t header_2 = 0x55;
    uint8_t header_3 = 0xCE;
    uint16_t length;
    uint8_t command;
} __attribute__ ((packed));

struct GetMassa: public CommonMessage
{
    GetMassa(): CommonMessage(CMD_GET_MASSA)
    { length = 0x0001; }
    uint16_t crc;
} __attribute__ ((packed));

struct SetZero: public CommonMessage
{
    SetZero(): CommonMessage(CMD_SET_ZERO)
    { length = 0x0001; }
    uint16_t crc;
} __attribute__ ((packed));

struct SetTare: public CommonMessage
{
    SetTare(): CommonMessage(CMD_SET_TARE)
    { length = 0x0001; }
    uint16_t crc;
} __attribute__ ((packed));

struct GetScalePar: public CommonMessage
{
    GetScalePar(): CommonMessage(CMD_GET_SCALE_PAR)
    { length = 0x0001; }
    uint16_t crc;
} __attribute__ ((packed));

