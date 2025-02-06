#pragma once

#include <cinttypes>

enum ProtocolCommands: uint8_t
{
    CMD_NONE            = 0x0,
    CMD_GET_SCALE_PAR   = 0x75,
    CMD_GET_MASSA       = 0x23,
    CMD_SET_TARE        = 0xA3,
    CMD_SET_ZERO        = 0x72,
    CMD_GET_NAME        = 0x20,
    CMD_SET_NAME        = 0x22,

    CMD_ACK_MASSA       = 0x24,
    CMD_ACK_SCALE_PAR   = 0x76,
    CMD_ACK_SET_ZERO    = 0x27,
    CMD_ACK_SET_TARE    = 0x12,
    CMD_NACK_SET_TARE   = 0x15,
    CMD_ERROR           = 0x28
};

enum ErrorCodes: uint8_t
{
    NONE                = 0x0,
    UNSUPPORTED         = 0x7,
    OVER_WEIRGHT        = 0x8,
    OUT_WEIRGHT         = 0x9,
    INPUT_DATA          = 0xA,
    SAVE_DATA           = 0xB,
    WIFI_UNSUPPORTED    = 0x10,
    ETH_UNSUPPORTED     = 0x11,
    NONE_SET_ZERO       = 0x15,
    NONE_CONNECTION     = 0x17,
    SET_WEIGHT_OFF      = 0x18,
    SCALE_FAULTY        = 0x19,
    UNKNOWN             = 0xF0
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

struct ErrorMessage: public CommonMessage
{
    ErrorMessage(): CommonMessage(CMD_ERROR)
    { length = 0x0002; }
    uint8_t     errorCode;
    uint16_t    crc;
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
    { length = 0x0005; }
    int32_t tare;
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

//struct AckScalePar: public CommonMessage
//{
//    AckScalePar(): CommonMessage(CMD_ACK_SCALE_PAR)
//    { length = 0x0064; }
//    char P_Max[21]  = {};   // Максимальная нагрузка
//    char P_Min[21]  = {};   // Минимальная нагрузка
//    char P_e[11]    = {};   // Поверочный интервал весов, e,
//    char P_T[11]    = {};   // Поверочный интервал весов, e,
//    char Fix[7]     = {};   // Параметр фиксации веса
//    char Calcode[12]= {};   // Код юстировки
//    char PO_Ver[9]  = {};   // Версия ПО датчика взвешивания,
//    char PO_Summ[8] = {};   // Контрольная сумма ПО датчика взвешивания
//} __attribute__ ((packed));
