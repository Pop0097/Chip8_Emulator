//
// Created by Dhruv Rawat on 2020-07-08.
//

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

Chip8::Chip8():randGen(std::chrono::system_clock::now().time_since_epoch().count()) //seed random number generator into the constructor with system clock
{
    pc = START_ADD; //initialize the program counter
    for(int i = 0; i < FONTSET_SIZE; i++) { //loads fontset into memory
        memory[FONTSET_START_ADD + i] = chip8_fontset[i];
    }
    
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U); //initialize random number generator. With this we can get number between 0 and 255

    // Set up function pointer table
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;

    table8[0x0] = &Chip8::OP_8xy0;
    table8[0x1] = &Chip8::OP_8xy1;
    table8[0x2] = &Chip8::OP_8xy2;
    table8[0x3] = &Chip8::OP_8xy3;
    table8[0x4] = &Chip8::OP_8xy4;
    table8[0x5] = &Chip8::OP_8xy5;
    table8[0x6] = &Chip8::OP_8xy6;
    table8[0x7] = &Chip8::OP_8xy7;
    table8[0xE] = &Chip8::OP_8xyE;

    tableE[0x1] = &Chip8::OP_ExA1;
    tableE[0xE] = &Chip8::OP_Ex9E;

    tableF[0x07] = &Chip8::OP_Fx07;
    tableF[0x0A] = &Chip8::OP_Fx0A;
    tableF[0x15] = &Chip8::OP_Fx15;
    tableF[0x18] = &Chip8::OP_Fx18;
    tableF[0x1E] = &Chip8::OP_Fx1E;
    tableF[0x29] = &Chip8::OP_Fx29;
    tableF[0x33] = &Chip8::OP_Fx33;
    tableF[0x55] = &Chip8::OP_Fx55;
    tableF[0x65] = &Chip8::OP_Fx65;
}
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

void Chip8::Cycle() {
	// Fetch
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	// Increment the PC before we execute anything
	pc += 2;

	// Decode and Execute
	((*this).*(table[(opcode & 0xF000u) >> 12u]))();

	// Decrement the delay timer if it's been set
	if (delayTimer > 0) {
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0) {
		--soundTimer;
	}
}

void Chip8::Table0() {
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip8::Table8() {
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void Chip8::TableE() {
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void Chip8::TableF() {
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

void OP_NULL() {}

/* Chip8 instructions */
void Chip8::OP_00E0() {
    memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE() {
	--sp;
	pc = stack[sp];
}

void Chip8::OP_1nnn() {
	uint16_t address = opcode & 0x0FFFu;

	pc = address;
}

void Chip8::OP_2nnn() {
	uint16_t address = opcode & 0x0FFFu;

	stack[sp] = pc;
	++sp;
	pc = address;
}

void Chip8::OP_3xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (V[Vx] == byte) {
		pc += 2;
	}
}

void Chip8::OP_4xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (V[Vx] != byte) {
		pc += 2;
	}
}

void Chip8::OP_5xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] == V[Vy]) {
		pc += 2;
	}
}

void Chip8::OP_6xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = byte;
}

void Chip8::OP_7xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] += byte;
}

void Chip8::OP_8xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] = V[Vy];
}

void Chip8::OP_8xy1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] |= V[Vy];
}

void Chip8::OP_8xy2() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] &= V[Vy];
}

void Chip8::OP_8xy3() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] ^= V[Vy];
}

void Chip8::OP_8xy4() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = V[Vx] + V[Vy];

	if (sum > 255U) {
		V[0xF] = 1;
	}
	else {
		V[0xF] = 0;
	}

	V[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] > V[Vy]) {
		V[0xF] = 1;
	}
	else {
		V[0xF] = 0;
	}

	V[Vx] -= V[Vy];
}

void Chip8::OP_8xy6() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save LSB in VF
	V[0xF] = (V[Vx] & 0x1u);

	V[Vx] >>= 1;
}

void Chip8::OP_8xy7() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vy] > V[Vx]) {
		V[0xF] = 1;
	}
	else {
		V[0xF] = 0;
	}

	V[Vx] = V[Vy] - V[Vx];
}

void Chip8::OP_8xyE() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save MSB in VF
	V[0xF] = (V[Vx] & 0x80u) >> 7u;

	V[Vx] <<= 1;
}

void Chip8::OP_9xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] != V[Vy]) {
		pc += 2;
	}
}

void Chip8::OP_Annn() {
	uint16_t address = opcode & 0x0FFFu;

	I = address;
}

void Chip8::OP_Bnnn() {
	uint16_t address = opcode & 0x0FFFu;

	pc = V[0] + address;
}

void Chip8::OP_Cxkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = V[Vx] % VIDEO_WIDTH;
	uint8_t yPos = V[Vy] % VIDEO_HEIGHT;

	V[0xF] = 0;

	for (unsigned int row = 0; row < height; row++) {
		uint8_t spriteByte = memory[I + row];

		for (unsigned int col = 0; col < 8; col++) {
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel) {
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF) {
					V[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (keypad[key]) {
		pc += 2;
	}
}

void Chip8::OP_ExA1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (!keypad[key]) {
		pc += 2;
	}
}

void Chip8::OP_Fx07() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	V[Vx] = delayTimer;
}

void Chip8::OP_Fx0A() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypad[0]) {
		V[Vx] = 0;
	}
	else if (keypad[1]) {
		V[Vx] = 1;
	}
	else if (keypad[2]) {
		V[Vx] = 2;
	}
	else if (keypad[3]) {
		V[Vx] = 3;
	}
	else if (keypad[4]) {
		V[Vx] = 4;
	}
	else if (keypad[5]) {
		V[Vx] = 5;
	}
	else if (keypad[6]) {
		V[Vx] = 6;
	}
	else if (keypad[7]) {
		V[Vx] = 7;
	}
	else if (keypad[8]) {
		V[Vx] = 8;
	}
	else if (keypad[9]) {
		V[Vx] = 9;
	}
	else if (keypad[10]) {
		V[Vx] = 10;
	}
	else if (keypad[11]) {
		V[Vx] = 11;
	}
	else if (keypad[12]) {
		V[Vx] = 12;
	}
	else if (keypad[13]) {
		V[Vx] = 13;
	}
	else if (keypad[14]) {
		V[Vx] = 14;
	}
	else if (keypad[15]) {
		V[Vx] = 15;
	}
	else {
		pc -= 2;
	}
}

void Chip8::OP_Fx15() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = V[Vx];
}

void Chip8::OP_Fx18() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = V[Vx];
}

void Chip8::OP_Fx1E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	I += V[Vx];
}

void Chip8::OP_Fx29() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = V[Vx];

	I = FONTSET_START_ADD + (5 * digit);
}

void Chip8::OP_Fx33() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = V[Vx];

	// Ones-place
	memory[I + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[I + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[I] = value % 10;
}

void Chip8::OP_Fx55() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; i++) {
		memory[I + i] = V[i];
	}
}

void Chip8::OP_Fx65() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; i++) {
		V[i] = memory[I + i];
	}
}

