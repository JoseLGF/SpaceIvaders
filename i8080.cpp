#include "i8080.h"
#include <iostream>

void CPU_8080::Initialize()
{
    // Initialize registers, stacks, pc and flags
    a  = 0;
    b  = 0;
    c  = 0;
    d  = 0;
    e  = 0;
    h  = 0;
    l  = 0;
    sp = 0;
    pc = 0;
    cc = {0,0,0,0,0,0};
    int_enable = 0;
}


void CPU_8080::LoadRom()
{
    std::cout << "Loading file invaders..." << std::endl;

    FILE * pFile;
    long lSize;
    unsigned char * buffer;
    size_t result;

    pFile = fopen ( "invaders" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (unsigned char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    /* the whole file is now loaded in the memory buffer. */


    // transfer the contents of buffer to the memory starting
    // at address 0x0
    for(int i = 0; i < lSize; ++i)
    {
        if(i<10){
            printf("%02x \n", buffer[i]);
        }
        memory[i] = buffer[i];
    }

    // terminate
    fclose (pFile);
    free(buffer);
}

void CPU_8080::PrintState()
{
    std::cout << "======= CPU STATUS =======" << std::endl;
    std::cout << "CPU Registers:" << std::endl;
    std::cout << "A : " << std::hex << (unsigned int) a << std::endl;
    std::cout << "B : " << std::hex << (unsigned int) b << std::endl;
    std::cout << "C : " << std::hex << (unsigned int) c << std::endl;
    std::cout << "D : " << std::hex << (unsigned int) d << std::endl;
    std::cout << "E : " << std::hex << (unsigned int) e << std::endl;
    std::cout << "H : " << std::hex << (unsigned int) h << std::endl;
    std::cout << "L : " << std::hex << (unsigned int) l << std::endl;
    std::cout << "SP: " << std::hex << (unsigned int) sp << std::endl;
    std::cout << "PC: " << std::hex << (unsigned int) pc << std::endl;
    std::cout << "Processor Flags:" << std::endl;
    std::cout << "Z : " << std::hex << (unsigned int) cc.z << std::endl;
    std::cout << "S : " << std::hex << (unsigned int) cc.s << std::endl;
    std::cout << "P : " << std::hex << (unsigned int) cc.p << std::endl;
    std::cout << "CY: " << std::hex << (unsigned int) cc.cy << std::endl;
    std::cout << "AC: " << std::hex << (unsigned int) cc.ac << std::endl;
    std::cout << "==========================" << std::endl;
}
