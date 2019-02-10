#include <string.h>
#include <iostream.h>
#include <SDL2/SDL.h>
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

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x the x coordinate to draw to
 * @param y the y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
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
	SDL_Window *window = SDL_CreateWindow("Lesson 3", 0, 0, SCREEN_WIDTH, 
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
	const std::string respath = getResourcePath("Lesson4");
	SDL_Texture *background = loadTexture(respath + "background.png", renderer);
	SDL_Texture *image = loadTexture(respath + "image.png", renderer);
	/**
	 * Make sure they both loaded ok
	 */
	if (background == nullptr || image == nullptr){
		cleanup(background, image, renderer, window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	//Our event structure
	SDL_Event e;

	//For tracking if we want to quit
	bool quit = false;

	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;

	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW /2 ;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	while (!quit){	
		
		//Read any events that occured, for now we'll just quit if any
		//event occured
		while(SDL_PollEvent(&e)){
			//If the user close the window
			if (e.type == SDL_QUIT)
				quit = true; 
			//If user presses any key
			else if (e.type == SDL_KEYDOWN)
				quit = true; 
			//If user clicks the mouse
			else if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true; 
		}

		//Clear the window
		SDL_RenderClear(renderer); 

		//Determine how many tiles we need to fill the screen

		//Draw the tiles by calculating positions
		for (int i = 0; i < xTiles * yTiles; ++i){
			int x = i % xTiles;
			int y = i / xTiles;
			renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}

		/**
		 * Draw our image in the center of the window
		 * We need the foreground image's width to properly
		 * compute the position of it's top left corner so
		 * that the image will be centered 
		 */
		renderTexture(image, renderer, x, y);

		//Update the screen
		SDL_RenderPresent(renderer);

	}

	//Destroy various items
	cleanup(background, image, renderer, window);

	IMG_Quit();
	SDL_Quit(); 
}

