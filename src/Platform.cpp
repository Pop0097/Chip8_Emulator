#include "Classes.h"

Platform::Platform(char const* t, int width, int height, int textureWidth /*Width of texture in pixels*/, int textureHeight) {
    SDL_Init(SDL_INIT_VIDEO); //Initializes SDL 
    window = SDL_CreateWindow(t, 0, 0, width, height, SDL_WINDOW_SHOWN); //creates window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //creates renderer which will accelerate 2D GPU processing
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight); //initializes variable that will render objects onto the window
    std::cerr << "Platform created\n";
}

Platform::~Platform() {
    //ends the processes and exits SDL
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 
    std::cerr << "DESTROYED\n";
}

void Platform::update(void const* buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr /*represents area to update (null = update entire texture)*/, buffer, pitch); //updates texture rectangle with new pixel data
    SDL_RenderClear(renderer); //Clears rendering target with the drawing colour
    SDL_RenderCopy(renderer, texture, nullptr /*Entire texture*/, nullptr /*Entire rendering target*/); //copy texture to current rendering target
    SDL_RenderPresent(renderer); //update the screen with rendering performed in this method
    //std::cerr << "Platform updated\n";
}

bool Platform::processInput(uint8_t* keys) {
    bool quit = false;
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type /*Reports type of event that occured on keyboard*/) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym /*Reports which key's press value has been changed*/) { //depending on the key that is pressed, the press value (1 for pressed, 0 for not pressed) is changed
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    
                    case SDLK_x:
                        keys[0] = 1;
                        break;
                    
                    case SDLK_1:
                        keys[1] = 1;
                        break;
                    
                    case SDLK_2:
                        keys[2] = 1;
                        break;
                    
                    case SDLK_3:
                        keys[3] = 1;
                        break;
                    
                    case SDLK_q:
                        keys[4] = 1;
                        break;
                    
                    case SDLK_w:
                        keys[5] = 1;
                        break;
                    
                    case SDLK_e:
                        keys[6] = 1;
                        break;
                    
                    case SDLK_a:
                        keys[7] = 1;
                        break;
                    
                    case SDLK_s:
                        keys[8] = 1;
                        break;
                    
                    case SDLK_d:
                        keys[9] = 1;
                        break;

                    case SDLK_z:
                        keys[0xA] = 1;
                        break;
                    
                    case SDLK_c:
                        keys[0xB] = 1;
                        break;
                    
                    case SDLK_4:
                        keys[0xC] = 1;
                        break;
                    
                    case SDLK_r:
                        keys[0xD] = 1;
                        break;
                    
                    case SDLK_f:
                        keys[0xE] = 1;
                        break;
                    
                    case SDLK_v:
                        keys[0xF] = 1;
                        break;
                    
                }
                break;

            case SDL_KEYUP:
                switch(event.key.keysym.sym /*Reports which key's press value has been changed*/) { //If the key is not pressed, the press value (1 for pressed, 0 for not pressed) is changed
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    
                    case SDLK_x:
                        keys[0] = 0;
                        break;
                    
                    case SDLK_1:
                        keys[1] = 0;
                        break;
                    
                    case SDLK_2:
                        keys[2] = 0;
                        break;
                    
                    case SDLK_3:
                        keys[3] = 0;
                        break;
                    
                    case SDLK_q:
                        keys[4] = 0;
                        break;
                    
                    case SDLK_w:
                        keys[5] = 0;
                        break;
                    
                    case SDLK_e:
                        keys[6] = 0;
                        break;
                    
                    case SDLK_a:
                        keys[7] = 0;
                        break;
                    
                    case SDLK_s:
                        keys[8] = 0;
                        break;
                    
                    case SDLK_d:
                        keys[9] = 0;
                        break;

                    case SDLK_z:
                        keys[0xA] = 0;
                        break;
                    
                    case SDLK_c:
                        keys[0xB] = 0;
                        break;
                    
                    case SDLK_4:
                        keys[0xC] = 0;
                        break;
                    
                    case SDLK_r:
                        keys[0xD] = 0;
                        break;
                    
                    case SDLK_f:
                        keys[0xE] = 0;
                        break;
                    
                    case SDLK_v:
                        keys[0xF] = 0;
                        break;
                }
                break;
        }
    }

    return quit;
}