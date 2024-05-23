#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <Windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
static Mix_Chunk* g_sound_dead;
static Mix_Chunk* g_sound_wing;
static Mix_Chunk* g_sound_ting;
static Mix_Music* g_sound_background;




//screen		

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;

const int frame_per_second = 40;

#define BLANK_TILE 0
#define TILE_SIZE  64
#define MAX_MAP_X 126
#define MAX_MAP_Y 10
#define TREE 9
#define MONEY 8 



typedef struct Map
{
	int start_x_;
	int start_y_;

	int max_x_;
	int max_y_;

	int tile [MAX_MAP_Y][MAX_MAP_X];
	
}Map;

typedef struct Item {
	std::string name;
	int price;
}Item;


namespace SDLCommonFunc
{

	int ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
		const std::string& menu1,
		const std::string& menu2,
		const std::string& menu3,
		const std::string& img_name,
		std::string coinest = "");
	int ShowStore(SDL_Renderer* g_screen, TTF_Font* font, std::vector<Item>& items, int& coin, int& skill);
}



#endif
