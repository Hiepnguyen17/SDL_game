

#include "MainObject.h"


MainObject::MainObject()
{
	frame_ = 0 ;
	x_pos_ = 0 ;
	y_pos_ = 100 ;
	x_val_ = 0 ;
	y_val_ = 0 ;
	
	width_frame_ = 0 ;
	height_frame_ = 0 ;
	
	map_x_ = 0 ;
	map_y_ = 0 ;
	lose = false  ;
	
	coin = 0;
	
	is_space_pressed = false;

}

MainObject::~MainObject()
{
	
}

bool MainObject::LoadImg(std:: string path, SDL_Renderer* screen ) 
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true)
	{
		width_frame_ = rect_.w / 3;
		height_frame_ = rect_.h; 

	}
	return ret;
}

void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0 )
	{
		//hoat anh
	frame_clip_[ 0 ].x = 0;
    frame_clip_[ 0 ].y = 0;
    frame_clip_[ 0 ].w = width_frame_;
    frame_clip_[ 0 ].h = height_frame_;

    frame_clip_[ 1 ].x = width_frame_;
    frame_clip_[ 1 ].y = 0;
    frame_clip_[ 1 ].w = width_frame_;
    frame_clip_[ 1 ].h = height_frame_;

    frame_clip_[ 2 ].x = width_frame_ * 2;
    frame_clip_[ 2 ].y = 0;
    frame_clip_[ 2 ].w = width_frame_;
    frame_clip_[ 2 ].h = height_frame_;

    
	}
}
void MainObject::Show(SDL_Renderer* des , std::string skill)
{
	LoadImg(skill, des);
	
		frame_ ++ ;

	
	
	if (frame_ >= 3)
	{
		frame_ = 0;
	}
	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	SDL_Rect* current_clip = &frame_clip_[frame_];
	SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}


void MainObject::HandelInputAction(SDL_Event events, Mix_Chunk* g_sound_wing)
{
	if (events.type == SDL_MOUSEBUTTONDOWN)
	{

		if (events.button.button == SDL_BUTTON_LEFT) 
		{

			y_val_ = -15;
			x_val_ = 5;
			Mix_PlayChannel(-1,g_sound_wing, 0);

		}

		if (events.button.button == SDL_BUTTON_RIGHT)
		{


		}
		
	}
	else if (events.type == SDL_MOUSEBUTTONUP)

	{
		x_val_ = 5;
		y_val_ = 7;
		

	}
	else if(events.type == SDL_KEYDOWN) 
	{
		if (events.key.keysym.sym == SDLK_SPACE) 
		{
			x_val_ = 35 ;
			y_val_ = 0;
			is_space_pressed = true;
			
		}
		 

	}
	
	else if ( events.type == SDL_KEYUP)

	{
		x_val_ = 5;
		y_val_ = 7;
		
		is_space_pressed = false;               
		
	}
	
}

void MainObject::DoPlayer(Map& map_data, SDL_Renderer* des, Mix_Chunk* g_sound_ting)
{
	
	CheckToMap(map_data, g_sound_ting);   
	CenterEntitOnMap(map_data);
}

void MainObject:: CenterEntitOnMap(Map& map_data){
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH/2) ;
	if (map_data.start_x_ < 0 ){
		map_data.start_x_ = 0 ;
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}
	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT/2);	
	if (map_data.start_y_ < 0 )
	{
		map_data.start_y_ = 0 ;

		
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >=  map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}

void MainObject::CheckToMap(Map& map_data, Mix_Chunk* g_sound_ting)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0 ;
	int y2 = 0;

	// check chieu ngang 
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
	 
	x1 = (x_pos_ + x_val_)/ TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;

	y1 = (y_pos_)/TILE_SIZE;
	y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

	int w_num1 = map_data.tile[y1][x2];
	int w_num2 = map_data.tile[y2][x2];		

	if (x1  >= 0 && x2 <MAX_MAP_X && y1 >=0 && y2 < MAX_MAP_Y)
	{
		if (x_val_ > 0) // di chuyen ve ben phai
		{
			if (is_space_pressed && (w_num1 == TREE || w_num2 == TREE) )
				
			{
				    
			}
			else if (w_num1 == MONEY || w_num2 == MONEY) {
				if (is_space_pressed) {
			

					lose = true;	
					
				}
				else {

					Mix_PlayChannel(-1, g_sound_ting, 0);

					if (w_num1 == MONEY) map_data.tile[y1][x2] = 0;
					if (w_num2 == MONEY) map_data.tile[y2][x2] = 0;
					
					CountCoin();
				}

			}

			else if (w_num1 != BLANK_TILE || w_num2 != BLANK_TILE)

			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -=(width_frame_) + 1;
				x_val_ = 0;

				lose = true;

			}

		}
		
	}


	// chieu doc 
	if (y_pos_ < 0 || y_pos_ > 640)
	{
		y_val_ = 0;
		x_val_ = 0;
		lose = true;
	}

	else
	{
		w_num1 = map_data.tile[y2][x1];
		w_num2 = map_data.tile[y2][x2];
		int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
		x1 = (x_pos_) / TILE_SIZE;
		x2 = (x_pos_ + width_min - 1) / TILE_SIZE;

		y1 = (y_pos_ + y_val_) / TILE_SIZE;
		y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

		if (x1 >= 0 && x2 < MAX_MAP_X && y1  > 0 && y2 < MAX_MAP_Y)
		{
			if (y_val_ >= 0)
			{
				if (is_space_pressed && (w_num1 == TREE || w_num2 == TREE))
				{


				}
				else if (w_num1 == MONEY || w_num2 == MONEY) {
					if (is_space_pressed) {

						lose = true;

					}
					else {

						Mix_PlayChannel(-1, g_sound_ting, 0);

						if (w_num1 == MONEY) map_data.tile[y2][x1] = 0;
						if (w_num2 == MONEY) map_data.tile[y2][x2] = 0;

						CountCoin();
					}

				}

				else if (w_num1 != BLANK_TILE || w_num2 != BLANK_TILE)

				{
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= (height_frame_);

					y_val_ = 0;


					lose = true;

				}



			}
			else if (y_val_ < 0)
			{

				if (w_num1 == MONEY || w_num2 == MONEY) {

					Mix_PlayChannel(-1, g_sound_ting, 0);

					if (w_num1 == MONEY) map_data.tile[y2][x1] = 0;
					if (w_num2 == MONEY) map_data.tile[y2][x2] = 0;


					CountCoin();
				}
				else if (w_num1 != BLANK_TILE || w_num2 != BLANK_TILE)
				{
					y_pos_ = (y1 + 1) * TILE_SIZE;
					y_val_ = 0;

					lose = true;
				}
			}
		}
	}

	if (x_pos_ < 0 || x_pos_ + width_frame_ > map_data.max_x_ || y_pos_ < 0 || y_pos_ + height_frame_ > map_data.max_y_)
	{
		lose = true;
	}

	// Đảm bảo nhân vật không vượt ra ngoài bản đồ
	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}

	if (y_pos_ < 0)
	{
		y_pos_ = 0;
	}
	else if (y_pos_ + height_frame_ > map_data.max_y_)
	{
		y_pos_ = map_data.max_y_ - height_frame_ - 1;
	}


	x_pos_ += x_val_;
	y_pos_ += y_val_;
	
}

