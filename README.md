# Chip8_Emulator

A Chip-8 emulator built using C++. 

## Inspiration

I decided to tackle this project to learn more about how computers read, write, and process data. 

## Launching the Emulator

1. Download and extract the zip file of this repository.
2. cd into the "src" directory.
3. Type "make chip8" to activate the Makefile.
4. To start the emulator, type in this command: ./chip8 VIDEO_SCALE REFRESH_FREQUENCY_MILLISECONNDS ./roms/ROM_FILENAME
    - Example: "./chip8 10 4 ./roms/tetris"
    
I haven't included many ROM files, so if there is a game you want to play that is not included in the repository, you can find it elsewhere. A good resource for ROMS I found is [this repository](https://github.com/dmatlack/chip8). Just make sure you download the .ch8 ROMs and rename them so they're easier to type out :))

## Referencs

I found these references sources to be extremely helpful while makikng this project:
- This [blog](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/) which was a great source to understand the inner workings of Chip8.
- This [blog](https://austinmorlan.com/posts/chip8_emulator/) which I followed to understand how the classes, etc. work together to make the program work. 
- This [blog](https://medium.com/@edkins.sarah/set-up-sdl2-on-your-mac-without-xcode-6b0c33b723f7) taught me how to integrate SDL2 into my program and how to create a Makefile so my program can be built. 
- The [SDL Wiki](https://wiki.libsdl.org/SDL_Quit) which helped me understand SDL API and apply it to this program.
- And finally, [cplusplus.com](http://www.cplusplus.com/), which I used to understand different libraries such as chrono and iostream. 
