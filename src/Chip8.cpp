//
// Created by Dhruv Rawat on 2020-07-08.
//

#include "Classes.h"

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

	std::cerr << "Chip8 constructed\n";
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
	std::cerr << "ROM loaded\n";
}

void Chip8::Cycle() {
	/*
		A cycle includes three things:
			1. fetch next instructino in the form of an opcode
			2. decode the instruction to determine what operation needs to occur (done using "table" function pointer)
			3. execute the instruction (done in instruction methods below)
	*/

	// Fetchches instruction 
	opcode = (memory[pc] << 8u) | memory[pc + 1]; //Get first digit of OP code with a bitmask and shift so it becmes a single digit from $0 to $F

	std::cerr << "Binary: " <<  0xF000 << " | Mem: " << (memory[pc] << 8u) << " | Mem2: " << memory[pc + 1] <<  " | OP Code: " << opcode << " | Both: " << ((opcode & 0xF000)) << "\n";

	// Increment the PC before we execute anything (prevents a continuous loop from occuring)
	pc += 2; 

	// Tables decode the opcode and then call one instruction accordingly
	// ((*this).*(table[(opcode & 0xF000u) >> 12u]))();
	
	switch(opcode & 0xF000) {
		case 0x0000: 
			switch(opcode & 0x00FF) {
				case 0x00E0: 
					this->OP_00E0();
					break;

				case 0x00EE: 
					this->OP_00EE();
					break;

				default:
					std::cerr << "0x0000 --> invalid op code\n";
					exit(0);
			}
			break;
		
		case 0x1000: 
			this->OP_1nnn();
			break;
		
		case 0x2000: 
			this->OP_2nnn();
			break;
		
		case 0x3000:
			this->OP_3xkk();
			break;
		
		case 0x4000: 
			this->OP_4xkk();
			break;
		
		case 0x5000: 
			this->OP_5xy0();
			break;
		
		case 0x6000:
			this->OP_6xkk();
			break;
		
		case 0x7000: 
			this->OP_7xkk();
			break;
		
		case 0x8000: 
			switch (opcode & 0x000F) {
				case 0x0000: 
					this->OP_8xy0();
					break;
				
				case 0x0001: 
					this->OP_8xy1();
					break;
				
				case 0x0002: 
					this->OP_8xy2();
					break;
				
				case 0x0003: 
					this->OP_8xy3();
					break;
				
				case 0x0004: 
					this->OP_8xy4();
					break;
				
				case 0x0005: 
					this->OP_8xy5();
					break;
				
				case 0x0006: 
					this->OP_8xy6();
					break;
				
				case 0x0007: 
					this->OP_8xy7();
					break;
				
				case 0x000E: 
					this->OP_8xyE();
					break;
				
				default:
					std::cerr << "0x8000 -> invalid opcode\n";
					exit(0);
			}
			break;
		
		case 0x9000: 
			this->OP_9xy0();
			break;
		
		case 0xA000: 
			this->OP_Annn();
			break;
		
		case 0xB000:
			this->OP_Bnnn();
			break;
		
		case 0xC000:
			this->OP_Cxkk(); 
			break;
		
		case 0xD000:
			this->OP_Dxyn();
			break;
		
		case 0xE000:
			switch (opcode & 0x00FF) {
				case 0x009E:
					this->OP_Ex9E();
					break;
				
				case 0x00A1:
					this->OP_ExA1();
					break;

				default:
					std::cerr << "0xE000 -> invalid opcode\n";
					exit(0);
			}
			break;
		
		case 0xF000: 
			switch (opcode & 0x00FF) {
				case 0x0007:
					this->OP_Fx07();
					break;
				
				case 0x000A:
					this->OP_Fx0A();
					break;
					
				case 0x0015:
					this->OP_Fx15();
					break;
					
				case 0x001E:
					this->OP_Fx1E();
					break;
				
				case 0x0029:
					this->OP_Fx29();
					break;

				case 0x0033:
					this->OP_Fx33();
					break;

				case 0x0055:
					this->OP_Fx55();
					break;
					
				case 0x0065:
					this->OP_Fx65();
					break;
				
				default:
					std::cerr << "0xF000 -> invalid opcode\n";
					exit(0);
			}
			break;

		default:
			std::cerr << "General -> invalid opcode \n";
			exit(0);
	}	

	// Decrement the delay timer if it's been set
	if (delayTimer > 0) {
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0) {
		--soundTimer;
	}
	//std::cerr << "Ran through cycle\n";
}

/* Chip8 instructions */
void Chip8::OP_00E0() {

	std::cerr << "00E0\n";

    memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE() {

	std::cerr << "00EE\n";

	/* 
		CPUs use a stack to keep track of the order of execution when it calls functions/routines. 
		Like stack navigation in React JS, when you call a sub routine, the sub routine will run on the next level of the CPU stack, meaning if you want to 
		go back to the task that called the subroutine, you will have to move down one level in the stack. 
		This is what this function does, it goes down one level in the stack to exit the sub routine and go to the routine that called it. 
	 */

	--sp; //reduces stack level by 1 to access calling process
	pc = stack[sp]; //resets program counter to reflect new stack position
}

void Chip8::OP_1nnn() {

	std::cerr << "1nnn\n";
	/*
		We want to jump to a different location or register. 
		This jump does not remember the origin, so there is no stack interaction (notice we did not change "sp")
	*/

	//NOTE the "u" is added to be 100% sure the number is unsigned
	uint16_t address = opcode & 0x0FFFu; //conducts binary AND operator to get new address

	pc = address; //program counter is set to new address
}

void Chip8::OP_2nnn() {

	std::cerr << "2nnn\n";
	/*
		When we call a sub routine, we want to return to the original "calling" routine. 
	*/
	uint16_t address = opcode & 0x0FFFu; //defines address of where sub routine will take place

	stack[sp] = pc; //stores current routine at the top of the stack
	++sp; //increments stack pointer to point at the sub routine
	pc = address; //program counter is set to the address of the sub routine
}

void Chip8::OP_3xkk() {

	std::cerr << "3xkk\n";

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (V[Vx] == byte) {
		pc += 2;
	}
}

void Chip8::OP_4xkk() {
	std::cerr << "4xkk\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (V[Vx] != byte) {
		pc += 2;
	}
}

void Chip8::OP_5xy0() {
	std::cerr << "5xy0\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] == V[Vy]) {
		pc += 2;
	}
}

void Chip8::OP_6xkk() {
	std::cerr << "6xkk\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = byte;
}

void Chip8::OP_7xkk() {
	std::cerr << "7xkk\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] += byte;
}

void Chip8::OP_8xy0() {
	std::cerr << "8xy0\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] = V[Vy];
}

void Chip8::OP_8xy1() {
	std::cerr << "8xy1\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] |= V[Vy];
}

void Chip8::OP_8xy2() {
	std::cerr << "8xy2\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] &= V[Vy];
}

void Chip8::OP_8xy3() {
	std::cerr << "8xy3\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] ^= V[Vy];
}

void Chip8::OP_8xy4() {
	std::cerr << "8xy4\n";
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
	std::cerr << "8xy5\n";
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
	std::cerr << "8xy6\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save LSB in VF
	V[0xF] = (V[Vx] & 0x1u);

	V[Vx] >>= 1;
}

void Chip8::OP_8xy7() {
	std::cerr << "8xy7\n";
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
	std::cerr << "8xyE\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save MSB in VF
	V[0xF] = (V[Vx] & 0x80u) >> 7u;

	V[Vx] <<= 1;
}

void Chip8::OP_9xy0() {
	std::cerr << "9xy0\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] != V[Vy]) {
		pc += 2;
	}
}

void Chip8::OP_Annn() {
	std::cerr << "Annn\n";
	uint16_t address = opcode & 0x0FFFu;

	I = address;
}

void Chip8::OP_Bnnn() {
	std::cerr << "Bnnn\n";
	uint16_t address = opcode & 0x0FFFu;

	pc = V[0] + address;
}

void Chip8::OP_Cxkk() {
	std::cerr << "Cxkk\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = randByte(randGen) & byte;
}

void Chip8::OP_Dxyn() {
	std::cerr << "Dxyn\n";
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
	std::cerr << "Ex9E\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (keypad[key]) {
		pc += 2;
	}
}

void Chip8::OP_ExA1() {
	std::cerr << "ExA1\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = V[Vx];

	if (!keypad[key]) {
		pc += 2;
	}
}

void Chip8::OP_Fx07() {
	std::cerr << "Fx07\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	V[Vx] = delayTimer;
}

void Chip8::OP_Fx0A() {
	std::cerr << "Fx0A\n";
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
	std::cerr << "Fx15\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = V[Vx];
}

void Chip8::OP_Fx18() {
	std::cerr << "Fx18\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = V[Vx];
}

void Chip8::OP_Fx1E() {
	std::cerr << "Fx1E\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	I += V[Vx];
}

void Chip8::OP_Fx29() {
	std::cerr << "Fx29\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = V[Vx];

	I = FONTSET_START_ADD + (5 * digit);
}

void Chip8::OP_Fx33() {
	std::cerr << "Fx33\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; //the >> operator means shift along the binary
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
	std::cerr << "Fx55\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; i++) {
		memory[I + i] = V[i];
	}
}

void Chip8::OP_Fx65() {
	std::cerr << "Fx65\n";
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; i++) {
		V[i] = memory[I + i];
	}
}

