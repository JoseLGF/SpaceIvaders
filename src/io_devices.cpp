#include "io_devices.h"
#include <iostream>
#include <fstream>

void Io_devices::Initialize()
{
    shift_register_value = 0x0000;
}

uint8_t Io_devices::Read_device(uint8_t device_number)
{
    switch(device_number)
    {
        case 0x1: return 0x01;
        case 0x3: return Get_shift_register_result();
        default : return 0;
    }
}

void Io_devices::Write_device(uint8_t device_number, uint8_t data)
{
    switch(device_number)
    {
        case 0x2: Set_shift_register_result_offset(data); break;
        case 0x4: Fill_shift_register(data);              break;
        default : break;
    }
}

uint8_t Io_devices::Get_shift_register_result()
{
    /* std::cout << "Read shift reg " */
    /*     << "offset: " */
    /*     << (unsigned int) shift_register_offset */
    /*     << " value: " */
    /*     << std::hex << (unsigned int) shift_register_value */
    /*     << std::endl; */
    return
            /* shift_register_value >> shift_register_offset; */

        ((shift_register_value >> (8-shift_register_offset)) & 0xff);

        /* (shift_register_value & */
        /*     (0xff << (8 - shift_register_offset))) */
        /* >> (8 - shift_register_offset); */
}

void Io_devices::Set_shift_register_result_offset(uint8_t amount)
{
    /* std::cout << "Setting shift reg " */
    /*     << "offset: " */
    /*     << (unsigned int) amount */
    /*     << std::endl; */
    shift_register_offset = amount;
}

void Io_devices::Fill_shift_register(uint8_t data)
{
    shift_register_value =
        (shift_register_value >> 8) |
        (data << 8);
    /* std::cout << "New shift reg value: " */
    /*     << std::hex << (unsigned int) shift_register_value << std::endl; */
}
