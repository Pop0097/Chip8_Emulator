//
// Created by Dhruv Rawat on 2020-07-08.
//

#include <fstream> //input output stream class to operate on files
#include "Chip8.h"

/*Fonts and text*/
//16 characters at 5 bytes each (5*16 = 80 array elements)
const unsigned int FONTSET_SIZE = 80;

unsigned char chip8_fontset[FONTSET_SIZE] = { //defines all fonts. Each character is 4 px wide and 5px tall
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

/*Start addresses*/
const unsigned int START_ADD = 0x200; //start address for the program counter

const unsigned int FONTSET_START_ADD = 0x50; 




Chip8::Chip8() {
    pc = START_ADD; //initialize the program counter
    for(int i = 0; i < FONTSET_SIZE; i++) { //loads fontset into memory
        memory[FONTSET_START_ADD + i] = chip8_fontset[i];
    }
}

Chip8::~Chip8() {}

//loads contents from ROM file into memory so we can execute instructions
void Chip8::loadROM(char const* romfile) {

    std::ifstream file(romfile, std::ios::binary | std::ios::ate);  //open file as a stream of binary. File pointer moved to the end

    if(file.is_open()) {
        //get file size and make a buffer to hold file contents
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg); //go to beining of file
        file.read(buffer, size); //read the file and fill buffer
        file.close();

        for (long i = 0; i < size; i++) {
            memory[START_ADD + i] = buffer[i]; //loads ROM contents into Chip8 memory
        }

        delete[] buffer; //empty buffer
    }

}