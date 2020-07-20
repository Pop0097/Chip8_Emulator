//
// Created by Dhruv Rawat on 2020-07-08.
//
#include <iostream>
#include <cstdint>

#ifndef CHIP_8_H
#define CHIP_8_H

class Chip8 {
public:
    uint8_t V[16]{}; //CPU registers (V0 to VF) (8-bit general purpose registers)
    uint8_t memory[4096]{}; //Memory (Chip 8 has 4K memory in total)
    uint16_t I{}; //index register (value ranges from 0x000 to 0xFFF)
    uint16_t pc{};//program counter (value ranges from 0x000 to 0xFFF)

    //Stack used to remember the current location before a jump to an address or subroutine is performed
    uint16_t stack[16]{};
    uint8_t sp{}; //used to remember which level of the stack is used to store the current location

    //timer registers that count at 60 Hz.
    uint8_t delayTimer{};
    uint8_t soundTimer{}; //system buzzer sounds when this timer reaches 0

    uint8_t keypad[16]{}; //Hex based keypad (0x0 to 0xF)
    uint32_t video[64 * 32]{}; //Black and white graphics with a total of 2048 pixels with a state of either 0 or 1)
    uint16_t opcode; //Current op code (needs to store two bytes)

    Chip8();
    ~Chip8();
    void loadROM(char const*);


};

#endif
