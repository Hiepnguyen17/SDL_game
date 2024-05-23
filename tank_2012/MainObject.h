		
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include  <vector>
#include "CommonFunc.h"
#include "BaseObject.h"



class MainObject : public BaseObject
{
public: 
	MainObject ();
	~MainObject();

	

	bool LoadImg(std::string path, SDL_Renderer* screen = NULL );
	void Show (SDL_Renderer* des, std::string skill);
	void HandelInputAction(SDL_Event events, Mix_Chunk * g_sound_dead);
	void set_clips() ;
	void DoPlayer(Map& map_data,SDL_Renderer* des, Mix_Chunk* g_sound_ting);
	void CheckToMap( Map& map_data, Mix_Chunk* g_sound_ting);
	void SetMapXY(const int map_x,const int  map_y){map_x_ = map_x; map_y_ = map_y;};
	void CenterEntitOnMap (Map& map_data);
	bool GetLose  ()const {return lose ;}

	void CountCoin() { coin++; }
	int GetCoin() { return coin; }
	
	bool get_space() { return is_space_pressed; }


	
private:
	
	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[3];
	
	int frame_;
	
	

	int map_x_;
	int map_y_;
	bool lose ;

	int coin;

	bool is_space_pressed;
	
};


#endif