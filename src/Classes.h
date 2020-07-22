//
// Created by Dhruv Rawat on 2020-07-08.
//
#include <iostream>
#include <cstdint>
#include <chrono>
#include <random>
#include <cstring>
#include <SDL2/SDL.h>
#include <fstream> //input output stream class to operate on files

/* other constants */
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;
const unsigned int  MEMSIZE = 0x1000;

#ifndef CHIP_8_H
#define CHIP_8_H

class Chip8 {
private:
    uint16_t opcode = {}; //Current op code (needs to store two bytes)

    uint8_t V[16] = {}; //CPU registers (V0 to VF) (8-bit general purpose registers)
    uint8_t memory[4096] = {}; //Memory (Chip 8 has 4K memory in total)
    uint16_t I = {}; //index register (value ranges from 0x000 to 0xFFF)
    uint16_t pc = {};//program counter (value ranges from 0x000 to 0xFFF)

    //Stack used to remember the current location before a jump to an address or subroutine is performed
    uint16_t stack[16] = {};
    uint8_t sp = {}; //used to remember which level of the stack is used to store the current location

    //timer registers that count at 60 Hz.
    uint8_t delayTimer = {};
    uint8_t soundTimer = {}; //system buzzer sounds when this timer reaches 0

    //Chip-8 instructions are emulated in these methods (note variables such as Vx and Vy are defined within the methods)
    void OP_00E0(); //clear the display
    void OP_00EE(); //return from a subroutine
    void OP_1nnn(); //jump to location "nnn" (interpreter sets program counter to "nnn")
    void OP_2nnn(); //call subroutine at "nnn"
    void OP_3xkk(); //skips instruction if Vx = kk
    void OP_4xkk(); //skip next instruction if Vx != kk
    void OP_5xy0(); //skip next instruction if Vx != Vy
    void OP_6xkk(); //set Vx == kk
    void OP_7xkk(); //set Vx = Vx + kk
    void OP_8xy0(); //set Vx == Vy
    void OP_8xy1(); //set Vx == Vy OR Vx
    void OP_8xy2(); //set Vx == Vy AND Vx
    void OP_8xy3(); //set Vx == Vy XOR Vx
    void OP_8xy4(); //set Vx = Vx + Vy, set VF = carry
    void OP_8xy5(); //set Vx = Vx + Vy, set VF = NOT borrow
    void OP_8xy6(); //set Vx = Vx SHR 1
    void OP_8xy7(); //set Vx = Vy - Vx, seet VF = NOT borrow
    void OP_8xyE(); //set Vx = Vx SHL 1
    void OP_9xy0(); //skip next instruction if Vx != Vy]
    void OP_Annn(); //set I = nnn
    void OP_Bnnn(); //jump to location nnn + V0
    void OP_Cxkk(); //set Vx = random byte AND kk
    void OP_Dxyn(); // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
    void OP_Ex9E(); //skip next instruction if key with the value of Vx is pressed
    void OP_ExA1(); //skip next instruction if key with the value of Vx is not pressed
    void OP_Fx07(); //set Vx = delay timer value
    void OP_Fx0A(); //wait for a key press, store the value of the key in Vx
    void OP_Fx15(); //set delay timer = Vx
    void OP_Fx18(); //set sound timer = Vx
    void OP_Fx1E(); //set I = I + Vx
    void OP_Fx29(); //Set I = location of sprite for digit Vx
    void OP_Fx33(); //Store BCD representation of Vx in memory locations I, I+1, and I+2
    void OP_Fx55(); //store registers V0 through Vx in memory starting at location I
    void OP_Fx65(); //read registers V0 through Vx from memory starting at location I

    //defines member variables that will be used to put random numbers into registers
    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

public:
    Chip8();
    void loadROM(char const*); 
    void Cycle();

    uint8_t keypad[16] = {}; //Hex based keypad (0x0 to 0xF)
    uint32_t video[64 * 32] = {}; //Black and white graphics with a total of 2048 pixels with a state of either 0 or 1)
};

#endif


#ifndef PLATFORM_H
#define PLATFORM_H

class SDL_Window;
class SDL_Renderer; //gives program 2D GPU acceleration
class SDL_Texture; //makes it easy to render a 2D image

class Platform {
private:
    SDL_Window* window = {};
    SDL_Renderer* renderer = {};
    SDL_Texture* texture = {};

public:
    Platform(char const*, int, int, int, int);
    ~Platform();
    void update(void const*, int);
    bool processInput(uint8_t*);
};

#endif

