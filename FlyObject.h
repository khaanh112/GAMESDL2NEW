#pragma once
#ifndef FLY_OBJECT_H_
#define FLY_OBJECT_H_
#define FLY_FRAME_NUM 8
#define FLY_SPEED 5
#include"CommonFunc.h"
#include"BaseObject.h"
#include"BulletObject.h"

class FlyObject : public BaseObject
{
public:
	FlyObject();
	~FlyObject();

	void set_x_val(const float xVal) { x_val_ = xVal; }
	void set_y_val(const float yVal) { y_val_ = yVal; }

	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }

	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }


	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }
	void set_clips();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }

	void InitThreats();


	void set_input_left(const int& ipLeft) { input_type_.left_ = ipLeft; }

	SDL_Rect GetRectFrame();
	std::vector<BulletObject*> get_bullet_list() const { return bullet_list_; }
	void set_bullet_list(const std::vector<BulletObject*>& bl_list) { bullet_list_ = bl_list; }

	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
	void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
	void get_player_rect(const SDL_Rect& pl) { player = pl; }
	void RemoveBullet(const int& idx);
	void SetBulletRect(const int& idx);

	void set_max_blood(const int& m_bl) { max_blood = m_bl; }
	void set_blood(const int& bl) {blood = bl;}
	int get_blood() const { return blood; }
	void Show_blood(SDL_Renderer* des);
private:
	int map_x_;
	int map_y_;
	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;
	SDL_Rect frame_clip[FLY_FRAME_NUM];
	int width_frame_;
	int height_frame_;
	int frame_;
	SDL_Rect frame_clip_[8];
	int animation_a_;
	int animation_b_;
	Input input_type_;

	std::vector<BulletObject*> bullet_list_;
	SDL_Rect player;

	int blood;
	int max_blood;

};
#endif