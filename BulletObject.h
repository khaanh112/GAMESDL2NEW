#pragma once

#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_



#include"BaseObject.h"
#include"CommonFunc.h"

class BulletObject : public BaseObject
{
public:
	BulletObject();
	~BulletObject();

	enum BulletType
	{
		SPHERE_BULLET = 20,
		ARROW_BULLET = 21,
		FIRE_BULLET = 22,
	};

	void set_x_val(const int& xVal) { x_val_ = xVal; }
	void set_y_val(const int& yVal) { y_val_ = yVal; }
	int get_x_val() const { return x_val_; }
	int get_y_val() const { return y_val_; }
	int get_rect_x() const { return rect_.x; }
	int get_rect_y() const { return rect_.y; }

	void set_type_bullet(const int& typeBullet) { type_bullet_ = typeBullet; }
	int get_type_bullet() const { return type_bullet_; }
	void SetDirection(const int& xVal, const int& yVal); // them
	void SetDirection1(const SDL_Rect& target_position);
	void set_is_move(const bool& isMove) { is_move_ = isMove; }
	bool get_is_move() const { return is_move_; }

	void CheckToMap(Map& map_data);

	void set_bullet_type(const unsigned int& bulletType) { bullet_type_ = bulletType; }
	unsigned int get_bullet_type() const { return bullet_type_; }

	void HandleMove(const int& x_boder, const int& y_border);
	void HandleMove1(const int& x_boder, const int& y_border);

	void LoadImgBullet(SDL_Renderer* des);
	virtual bool LoadImg1(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* renderer);

private:
	int x_val_;
	int y_val_;
	int  initial_x_val_;
	int  initial_y_val_;

	float x_pos_;
	float y_pos_;

	bool is_move_;
	int type_bullet_;
	double angle_of_bullet_;


	unsigned int bullet_type_;




};
#endif 
