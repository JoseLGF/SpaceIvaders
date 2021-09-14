/*
 * Implementation for instructions that fall under the IO and special
 * categories
 */

#include "i8080.h"
#include <iostream>

void CPU_8080::OUT(uint8_t device_number)
{
    /* std::cout << "Writing " << std::hex << std::to_string((int) a) << " to device number " */
    /*     << std::to_string((int) device_number) << std::endl; */
    /* std::cout << std::flush; */
    /* if(a!=0 && (device_number == 4)) */
    /* { */
    /*     std::cout << "Stap!" << std::endl; */
    /* } */
    devices->Write_device(device_number, a);
    pc += 2;
    cycles += 10;
}

void CPU_8080::IN(uint8_t device_number)
{
    /* std::cout << "Read " << " from device number " */
    /*     << std::to_string((int) device_number) << std::endl; */
    /* std::cout << std::flush; */
    a = devices->Read_device(device_number);
    /* if(device_number == 4) */
    /* { */
    /*     std::cout << "Stap!" << std::endl; */
    /* } */
    /* std::cout << std::hex << std::to_string((int) a) << std::endl; */
    pc += 2;
    cycles += 10;
}

void CPU_8080::EI()
{
    int_enable = true;
    pc += 1;
    cycles += 4;
}

// Set carry
void CPU_8080::STC()
{
    cc.cy = true;
    pc += 1;
    cycles += 4;
}
