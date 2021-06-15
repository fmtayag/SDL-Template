#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <iterator>

SDL_Texture* loadTexture(SDL_Renderer* renderer, auto path) {
    // TODO: Change path's type to something not auto. But it's fine for now.
    SDL_Surface* img = SDL_LoadBMP(path);
    SDL_Texture* tImg = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    return tImg;
}

int main(int argv, char **args) {

    // Init
	SDL_Init(SDL_INIT_EVERYTHING);

    // Create window and renderer
	SDL_Window *window = SDL_CreateWindow("Star Fighter C++!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 960, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	// Load textures
	SDL_Texture *tImg1 = loadTexture(renderer, "images/player.bmp");
	SDL_Texture *tImg2 = loadTexture(renderer, "images/player1.bmp");
	SDL_Texture *tBackground = loadTexture(renderer, "images/background.bmp");

	// Create texture's rect
	SDL_Rect myRect;
	myRect.x = 0;
	myRect.y = 0;
	myRect.w = 64;
	myRect.h = 64;
	float posX = 0;
	float posY = 0;
	float speed = 300;

	// Loop thingamajigs
	bool isRunning = true;
	SDL_Event event;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	Uint32 lastUpdate = SDL_GetTicks();
	Uint32 current = 0;
	float dt;

	// Animation thingamajigs
	std::vector<SDL_Texture*> imgList = {tImg1, tImg2};
	std::vector<SDL_Texture*>::const_iterator iter = imgList.begin();
    SDL_Texture* currentImg = *iter;
	int animationTimer = SDL_GetTicks();
	int animationDelay = 100;
	int now;

    while (isRunning)
	{
	    // Calculate dt
        current = SDL_GetTicks();
		dt = (current - lastUpdate) / 1000.0f;
		lastUpdate = current;

        // Poll events
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // Get input
        SDL_PumpEvents();
        if(keystate[SDL_SCANCODE_W]) {
            posY -= speed * dt;
		}
		if(keystate[SDL_SCANCODE_S]) {
			posY += speed * dt;
		}
		if(keystate[SDL_SCANCODE_A]) {
            posX -= speed * dt;
		}
		if(keystate[SDL_SCANCODE_D]) {
            posX += speed * dt;
		}

        // Update
        myRect.x = posX;
        myRect.y = posY;

        now = SDL_GetTicks();
        if(now - animationTimer > animationDelay) {
            animationTimer = now;
            if(iter == imgList.end() - 1) {
                iter = imgList.begin();
            }
            else {
                iter++;
            }
            currentImg = *iter;
        }

		// Draw to screen
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderCopy(renderer, tBackground, NULL, NULL);
		SDL_RenderCopy(renderer, currentImg, NULL, &myRect);
		SDL_RenderPresent(renderer);
	}

    // Clean up stuff
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}

