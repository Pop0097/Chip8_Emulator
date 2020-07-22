
#include "Classes.h"

int main(int argc, char** argv) {
    /* 
        This function will: 
            - Call Chip8::Cycle() continuously until program is terminated 
            - Handle inputs
            - Render with SDL
    */

	if (argc != 4) {
		//std::cerr standared output stream for errors 
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <Rom>\n";
		std::exit(EXIT_FAILURE);
	}

    //std::stoi --> interprets a signed integer value in the string argv[x]
    int videoScale = std::stoi(argv[1]); // amount video needs to be scaled by (integer scale factor)
	int cycleDelay = std::stoi(argv[2]); //Time we want to wait between calling Chip8::Cycle()
	const char* romFilename = argv[3]; //romfile that continas instructions for Chip8 emulator 
   
    Platform platform("Chip8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT); //creates platform object

	Chip8 emulator; //creates Chip8 object 
	emulator.loadROM(romFilename); //loads the ROM files so instructions are in memory

	int videoPitch = sizeof(emulator.video[0]) * VIDEO_WIDTH; //resizes the video

	auto lastCycleTime = std::chrono::high_resolution_clock::now(); //gets current time (this is also the reference time)
	bool quit = false;

	std::cerr << "Starting Loop\n";

	while (!quit) {
		quit = platform.processInput(emulator.keypad); //calls method to get input from keypad (passes through Chip8 keyboard)

		auto currentTime = std::chrono::high_resolution_clock::now(); //gets current time
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count(); // gets time elapsed between currentTime and lastCycleTime
		// std::cerr << "Delay: " << dt << "\n";
		if (dt > cycleDelay) { //if time between calling Chip8::Cycle() is adequate, then the method can be called again
			//std::cerr << "Refreshing screen\n";
			lastCycleTime = currentTime; //current time becomes the reference time 

			emulator.Cycle(); //runs cycle of Chip8 to execute instruction from the keypad

			platform.update(emulator.video, videoPitch); //updates the window
		}
	}
	
	std::cerr << "Program terminated!\n";

	return 0;
}