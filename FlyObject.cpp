#include"stdafx.h"
#include"FlyObject.h";
#include"BaseObject.h"
#include"MainObject.h"
#include<iostream>

FlyObject::FlyObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;

	frame_ = 0;

	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;

}

FlyObject::~FlyObject()
{


}

bool FlyObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / FLY_FRAME_NUM;
		height_frame_ = rect_.h;

	}
	return ret;
}

SDL_Rect FlyObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}

void FlyObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		frame_clip_[2].x = 2 * width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		frame_clip_[3].x = 3 * width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;

		frame_clip_[4].x = 4 * width_frame_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = width_frame_;
		frame_clip_[4].h = height_frame_;

		frame_clip_[5].x = 5 * width_frame_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = width_frame_;
		frame_clip_[5].h = height_frame_;

		frame_clip_[6].x = 6 * width_frame_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = width_frame_;
		frame_clip_[6].h = height_frame_;

		frame_clip_[7].x = 7 * width_frame_;
		frame_clip_[7].y = 0;
		frame_clip_[7].w = width_frame_;
		frame_clip_[7].h = height_frame_;

	}
}

void FlyObject::Show(SDL_Renderer* des)
{

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	frame_++;
	if (frame_ > 8)
	{
		frame_ = 0;
	}

	SDL_Rect* currentClip = &frame_clip_[frame_];
	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
	SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);


}



void FlyObject::InitThreats()
{
	x_val_ = 0;
	y_val_ = 0;
	if (x_pos_ > 256)
	{
		x_pos_ -= 256;
		animation_a_ -= 256;
		animation_b_ -= 256;
	}
	else
	{
		x_pos_ = 0;
	}
	y_pos_ = 30;


}

void FlyObject::RemoveBullet(const int& idx)
{
	int size = bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = bullet_list_.at(idx);
		bullet_list_.erase(bullet_list_.begin() + idx);
		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void FlyObject::SetBulletRect(const int& idx)
{

	BulletObject* p_bullet = bullet_list_.at(idx);
	p_bullet->SetRect(rect_.x + 20, rect_.y + 50);


}






void FlyObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
	if (p_bullet != NULL)
	{
		p_bullet->set_bullet_type(BulletObject::FIRE_BULLET);
		p_bullet->LoadImgBullet(screen);

		p_bullet->set_is_move(true);
		p_bullet->SetRect(x_pos_ + 20, y_pos_ + 10);
		//p_bullet->set_x_val(15);

		SDL_Rect main_object_position = player;

		p_bullet->SetDirection1(main_object_position);
		bullet_list_.push_back(p_bullet);


	}
}


void FlyObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move() == true)
			{

				p_bullet->HandleMove(x_limit, y_limit);
				p_bullet->Render(screen);

			}
			else
			{
				p_bullet->set_is_move(true);
				p_bullet->SetRect(rect_.x + 20, rect_.y + 20);

				SDL_Rect main_object_position = player;

				p_bullet->SetDirection1(main_object_position);
				p_bullet->HandleMove(x_limit, y_limit);
			}
		}
	}
}