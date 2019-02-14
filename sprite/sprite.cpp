#include <string.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_image.h>
#include "res_path.h"
#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


/**
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */

void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << "error: " << SDL_GetError() << std::endl;
}

/**
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */

SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren){
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());

	if (texture == nullptr){
		logSDLError(std::cout, "Load Texture");
	}
	return texture;
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw to
 * @param dst The destination rectangle to render the to
 * @param clip The sub-section of the texture to draw (clipping rect)
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr){
	SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of
 * the texture's
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate we want to draw to
 * @param y The y coordinate we want to draw to
 * @param clip The sub-section of the texture to draw (clipping rect)
 *             default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr) {

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	} 
	renderTexture(tex, ren, dst, clip); 
} 

int main (int argc, char **argv) {

	//Start up SDL and make sure it went ok	
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}

	//Set up our window and renderer
	SDL_Window *window = SDL_CreateWindow("Lesson 5", 0, 0, SCREEN_WIDTH, 
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow"); 
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

	if(renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window); 
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	//The textures we'll be using
	const std::string respath = getResourcePath("Lesson5");
	SDL_Texture *image = loadTexture(respath + "image.png", renderer);
	/**
	 * Make sure they both loaded ok
	 */
	if (image == nullptr){
		cleanup(image, renderer, window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}


	int iW = 100, iH = 100;
	int x = SCREEN_WIDTH / 2 - iW /2 ;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	//Setup the clips for our image
	SDL_Rect clips[4];

	//Since our clips are uniform in size, we can generate a list of their
	//positions using some math (the specificss of this are covered in the lesson)

	for (int i = 0 ; i < 4; i++){
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;

		clips[i].w = iW;
		clips[i].h = iH;
	}

	//Specify a default clip to start with
	int useClip = 0;

	SDL_Event e;
	bool quit = false;

	while (!quit){	
		
		//Read any events that occured, for now we'll just quit if any
		//event occured
		while(SDL_PollEvent(&e)){
			//here we are
			if (e.type == SDL_QUIT){
				quit = true;
			}

			if (e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
	
					case SDLK_1:
					case SDLK_KP_1:
	
						useClip = 0;
						break;
	
					case SDLK_2:
					case SDLK_KP_2:
	
						useClip = 1;
						break;
	
					case SDLK_3:
					case SDLK_KP_3:
	
						useClip = 2;
						break;
	
					case SDLK_4:
					case SDLK_KP_4:
	
						useClip = 3;
						break;
	
					case SDLK_ESCAPE:
						quit = true;
						break;	
	
					default:
						break;
				}
			}
		}

		//Clear the window
		SDL_RenderClear(renderer); 


		/**
		 * Draw our image in the center of the window
		 */
		renderTexture(image, renderer, x, y, &clips[useClip]);

		//Update the screen
		SDL_RenderPresent(renderer);

	}

	//Destroy various items
	cleanup(image, renderer, window);

	IMG_Quit();
	SDL_Quit(); 
}

