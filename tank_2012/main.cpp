
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "TextObject.h"
#include "Timer.h"
#include <string>
#include <fstream>	

BaseObject g_background;

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;
std::vector <Item> items;
int skill = 0;

void saveCoin(const int coinest, const int coin) {
	std::fstream file("info//save_coin.txt");
	if (file.is_open()) {

		file << std::to_string(coinest) << " ";
		file << std::to_string(coin);

	}
	file.close();
}
void readCoin(int& coinest, int& coin) {
	std::fstream file("info//save_coin.txt");
	if (file.is_open()) {
		std::string a, b;
		file >> a >> b;
		coinest = std::stoi(a);
		coin = std::stoi(b);
	}
	file.close();
}

int loadSkill(std::vector <Item>& items) {
	std::fstream file("info//loadSkill.txt");
	int count = 0;
	int info = 0; // vi tri skill dang dung
	if (file.is_open()) {
		while (!file.eof()) {
			std::string a, b;
			file >> a >> b;
			if (!a.empty() && !b.empty()) 
			{
				if (a == "Equipped") info = count;

				Item item;
				item.name = a;

				char* c = new char[b.length() + 1];// tao char* de dung atoi (tra ve so dau tien gap) vi stoi bi loi 
				strcpy_s(c, b.length() + 1, (char*)b.c_str());
				item.price = std::atoi(c);

				items.push_back(item);

				delete[]c;
				count++;
			}

			
		}	
	}
	file.close();
	return info;
}

void saveSkill(std::vector <Item> items)
{
	std::fstream file("info//loadSkill.txt");
	if (file.is_open()) {

		for (int i = 0; i < items.size(); i++)
		{
			file << items[i].name << " ";
			file << std::to_string(items[i].price) << std::endl;
		}

	}
	file.close();
}

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_window = SDL_CreateWindow("game sdl cpp ",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (g_window == NULL)
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
		{
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		success = false;
	}
	else
	{
		g_sound_dead = Mix_LoadWAV("sound//punch.wav");
		g_sound_wing = Mix_LoadWAV("sound//wing.wav");
		g_sound_ting = Mix_LoadWAV("sound//ting.wav");
		g_sound_background = Mix_LoadMUS("sound//soud_background.mp3");
		if (g_sound_dead == NULL && g_sound_wing == NULL && g_sound_background == NULL) success = false;
	}

	if (TTF_Init() == -1) return false;
	g_font_text = TTF_OpenFont("font//ARCADE.ttf", 40);
	if (g_font_text == NULL)
	{

		success = false;
	}
	g_font_MENU = TTF_OpenFont("font//ARCADE.ttf", 40);
	if (g_font_MENU == NULL)
	{
		return false;
	}
	skill = loadSkill(items);
	return success;
}

bool LoadBackground()
{
	bool ret = g_background.LoadImg("img//background.png", g_screen);
	if (ret == false)
		return false;
	return true;
}

void close()
{


	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();

	SDL_Quit();
	Mix_Quit();
}




int main(int argc, char* argv[])
{
	ImpTimer fps_timer;
	// danh sach skill dang co
	std::vector <std::string> skill_list = {
		"img\\bird.png",
		"img\\fire_bird.png",
		"img\\fl_bird.png"
	};
	int coin = 0;
	int coinest = 0;
	

	readCoin(coinest, coin);

	bool is_quit = false;
	if (InitData() == false)
		return -1;
	if (LoadBackground() == false)
		return -1;

	Mix_PlayMusic(g_sound_background, -1);
again_menu:
	int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, 
											"PLAY", 
											"SHOP", 
											"EXIT", 
											"img//MENU.png", 
											std::to_string(coinest));
	
	
	if (ret_menu == 1) {
		int ret_store = SDLCommonFunc::ShowStore(g_screen, g_font_MENU, items, coin, skill);
		while (ret_store != -1) {
			ret_store = SDLCommonFunc::ShowStore(g_screen, g_font_MENU, items, coin, skill);
		}

		goto again_menu;

	}
	else if (ret_menu == 2)
		is_quit = true;
	else {
		is_quit = false;
		goto again_label;
	}
	
again_label:
	GameMap game_map;
	game_map.LoadMap();
	game_map.LoadTiles(g_screen);


	MainObject p_player;
	p_player.LoadImg(skill_list[skill], g_screen);
	p_player.set_clips();

	TextObject text_count_[1];
	text_count_[0].setColor(1);
	


	while (!is_quit)
	{

		fps_timer.start();

		Mix_ResumeMusic();
		while (SDL_PollEvent(&g_event) != 0)
		{

			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			else if (g_event.type == SDL_MOUSEBUTTONDOWN && g_event.button.button == SDL_BUTTON_RIGHT)
			{
				
				int ret_pause = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "RESUME", "MENU", "EXIT", "", " ");
				if (ret_pause == 2)
				{
					is_quit = true;
					continue;
				}

				else if (ret_pause == 1) {
					coin += p_player.GetCoin();
					if (p_player.GetCoin() > coinest) coinest = p_player.GetCoin();
					goto again_menu;

				}
			}
			p_player.HandelInputAction(g_event, g_sound_wing);
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);


		g_background.Render(g_screen, NULL);

		game_map.DrawMap(g_screen);
		Map map_data = game_map.getMap();

		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);

		p_player.CheckToMap(map_data, g_sound_ting);

		// check game over de khung hinh khong chuyen dong (CenterEntitOnMap)
		bool game_over = p_player.GetLose();
		if (game_over == true)
		{

			Mix_PlayChannel(-1, g_sound_dead, 0);
			Sleep(500);

			
			int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,
				"PLAY AGAIN", "MENU", "EXIT",
				"img//MENU END.png"," ");
			if (ret_menu == 2)
			{
				is_quit = true;
				continue;
			}

			else if (ret_menu == 1) {
				is_quit = true;
				coin += p_player.GetCoin();
				if (p_player.GetCoin() > coinest) coinest = p_player.GetCoin();
				goto again_menu;

			}

			else {
				is_quit = false;
				coin += p_player.GetCoin();
				if (p_player.GetCoin() > coinest) coinest = p_player.GetCoin();
				goto again_label;
			}

		}


		p_player.CenterEntitOnMap(map_data);
		p_player.Show(g_screen, skill_list[skill]);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);

		text_count_[0].SetText("COIN: " + std::to_string(p_player.GetCoin()));
		text_count_[0].loadFromRenderedText(g_font_text, g_screen);
		text_count_[0].RenderText(g_screen, 0, 2);


		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / frame_per_second;

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if(delay_time >= 0 )
				SDL_Delay(delay_time);
		}

		

	}
	if (p_player.GetCoin() > coinest) coinest = p_player.GetCoin();
	coin += p_player.GetCoin();
	saveCoin(coinest, coin);
	saveSkill(items);
	close();
	return 0;
}