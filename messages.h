#pragma once

#include <cinttypes>

enum ProtocolCommands: uint8_t
{
    CMD_GET_SCALE_PAR   = 0x75,
    CMD_GET_MASSA       = 0x23,
    CMD_SET_TARE        = 0xA3,
    CMD_SET_ZERO        = 0x72,
    CMD_GET_NAME        = 0x20,
    CMD_SET_NAME        = 0x22,

    CMD_ACK_MASSA       = 0x24,
    CMD_ACK_SCALE_PAR   = 0x76,
    CMD_ERROR           = 0x28
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

struct AckMassa: public CommonMessage
{
    AckMassa(): CommonMessage(CMD_ACK_MASSA)
    { length = 0x0009; }
    int32_t weight;
    uint8_t division; //Цена деления в значении массы нетто и массы тары:
                          //0 – 100 мг, 1 – 1 г, 2 – 10 г, 3 – 100 г, 4 – 1 кг
    uint8_t stable;   //Признак стабилизации массы: 0 – нестабильна, 1 – стабильна
    uint8_t net;      //Признак индикации <NET>: 0 – нет индикации, 1 – есть индикация
    uint8_t zero;     //Признак индикации >0< : 0 – нет индикации, 1 – есть индикация
} __attribute__ ((packed));

struct AckMassaTare: public AckMassa
{
    AckMassaTare(): AckMassa()
    { length = 0x000d; }
    int32_t tare;
} __attribute__ ((packed));

