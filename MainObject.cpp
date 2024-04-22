#include"stdafx.h"
#include"MainObject.h"
#include<cmath>
#include<iostream>
#include"BulletObject.h"
#include"Game_map.h"
MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	input_type_.jump_ = 0;
	on_ground_ = false;
	map_x_ = 0;
	map_y_ = 0;
	come_back_time_ = 0;
	money_count = 0;
	health = 5;


}
MainObject::~MainObject()
{

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}
	return ret;
}

SDL_Rect MainObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}

void MainObject::set_clips() {
	if (width_frame_ > 0 && height_frame_ > 0) {
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

void MainObject::Show(SDL_Renderer* des) {
	UpdateImagePlayer(des);

	if (input_type_.left_ == 1 || input_type_.right_ == 1)
		frame_++;
	else
		frame_ = 0;

	if (frame_ >= 8) {
		frame_ = 0;
	}
	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	SDL_Rect* Current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_ , height_frame_ };

	SDL_RenderCopy(des, p_object_, Current_clip, &renderQuad);
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound[2]) {
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_d: {
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImagePlayer(screen);
			break;
		}
		case SDLK_a: {
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImagePlayer(screen);
			break;
		}
		case SDLK_w: {

			input_type_.jump_ = 1;


			break;
		}
		}
	}
	else if (events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym) {
		case SDLK_d: {
			status_ = WALK_RIGHT;
			input_type_.right_ = 0;
			break;
		}
		case SDLK_a: {
			status_ = WALK_LEFT;
			input_type_.left_ = 0;
			break;
		}
		}
	}


	if (events.type == SDL_MOUSEBUTTONDOWN)
	{
		int const stt = status_;

		// Cài đặt hướng di chuyển của viên đạn dựa trên vị trí hiện tại của chuột

		BulletObject* p_bullet = new BulletObject();

		if (events.button.button == SDL_BUTTON_LEFT) {
			p_bullet->set_type_bullet(1);
			p_bullet->LoadImg1("img//player_bullet.png", screen);
			p_bullet->Render(screen);
			Mix_PlayChannel(-1, bullet_sound[0], 0);
		}
		else if (events.button.button == SDL_BUTTON_RIGHT)
		{
			if (get_money_count() >= 3) {
				p_bullet->set_type_bullet(2);
				p_bullet->LoadImg1("img//bullet_2.png", screen);
				p_bullet->Show(screen);
				Mix_PlayChannel(-1, bullet_sound[1], 0);
				DecreaseMoney();
			}
		}


		p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.3);
		// tinh vi tri chuot de chinh huong nhan vat
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		if (mouse_x < rect_.x)
		{
			status_ = WALK_LEFT;
			UpdateImagePlayer(g_screen);


		}
		else
		{
			status_ = WALK_RIGHT;
			UpdateImagePlayer(g_screen);
		}

		p_bullet->SetDirection(mouse_x, mouse_y);
		p_bullet->set_is_move(true);

		// Thêm viên đạn mới vào danh sách viên đạn
		p_bullet_list_.push_back(p_bullet);

	}
	else if (events.type == SDL_MOUSEBUTTONUP) {

	}
}

void MainObject::HandleBullet(SDL_Renderer* des) {

	for (int i = 0; i < p_bullet_list_.size(); ) {
		BulletObject* p_bullet = p_bullet_list_.at(i);
		if (p_bullet != NULL) {
			if (p_bullet->get_is_move() == true) {
				// Cập nhật hướng di chuyển của viên đạn dựa trên vị trí hiện tại của chuột tùy theo loại đạn
				if (p_bullet->get_type_bullet() == 2) {
					int mouse_x, mouse_y;
					SDL_GetMouseState(&mouse_x, &mouse_y);
					p_bullet->SetDirection(mouse_x, mouse_y);
					p_bullet->LoadImg1("img//bullet_2.png", des);
					p_bullet->Show(des);
				}
				else if (p_bullet->get_type_bullet() == 1) {

				}
				// Xử lý di chuyển và vẽ viên đạn

				if (stt == 2)
				{

					p_bullet->HandleMove1(SCREEN_WIDTH, SCREEN_HEIGHT);
					if (p_bullet->get_type_bullet() == 2)
					{
						p_bullet->Show(des);
					}
					else if (p_bullet->get_type_bullet() == 1)
					{
						p_bullet->Render(des);
					}

				}
				else
				{

					p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
					if (p_bullet->get_type_bullet() == 2)
					{
						p_bullet->Show(des);
					}
					else if (p_bullet->get_type_bullet() == 1)
					{
						p_bullet->Render(des);
					}
					else
						p_bullet->Show(des);

				}


				// Tăng chỉ số i chỉ khi viên đạn vẫn còn di chuyển
				++i;
				if (p_bullet->get_type_bullet() == 2) {
					int mouse_x, mouse_y;
					SDL_GetMouseState(&mouse_x, &mouse_y);


					if (abs(mouse_x - p_bullet->get_rect_x()) < 10 && abs(mouse_x - p_bullet->get_rect_x()) < 10)
					{

						p_bullet->set_type_bullet(11);


					}
				}
			}
			else {
				// Xóa viên đạn khỏi danh sách và giải phóng bộ nhớ
				delete p_bullet;
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
			}
		}
	}
}




void MainObject::RemoveBullet(const int& idx)
{
	int size = p_bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = p_bullet_list_.at(idx);
		p_bullet_list_.erase(p_bullet_list_.begin() + idx);
		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}


void MainObject::DoPlayer(Map& map_data, Mix_Chunk* exp_sound[3])
{
	
	if (come_back_time_ == 0)
	{

		x_val_ = 0;
		y_val_ += 0.8;

		if (y_val_ >= MAX_FALL_SPEED)
		{
			y_val_ = MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1)
		{
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += PLAYER_SPEED;
		}

		if (input_type_.jump_ == 1)
		{
			if (on_ground_ == true)
			{
				y_val_ = -PLAYER_JUMP_VAL;
				input_type_.jump_ = 0;
				on_ground_ = false;
			}
		}

		CheckToMap(map_data,exp_sound);
		CenterEntityOnMap(map_data);
	}
	static bool hasFallen = false;
	if (come_back_time_ > 0)
	{ 
		
		if (!hasFallen)
		{
			DecreaseHealth();
			
		}
		come_back_time_--;
		if (come_back_time_ == 0)
		{
			if (x_pos_ > 256)
			{
				x_pos_ -= 256;
				map_x_ -= 256;
			}
			else
			{
				x_pos_ = 0;
			}

			y_pos_ = 0;
			x_val_ = 0;
			y_val_ = 0;

		}
		hasFallen = true;

		
	}
	else
	{
		hasFallen = false;
	}

}

void MainObject::CenterEntityOnMap(Map& map_data)
{

	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.start_x_ - SCREEN_WIDTH;
	}

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT);
	if (map_data.start_y_ < 0)
	{
		map_data.start_y_ = 0;
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.start_y_ - SCREEN_HEIGHT;
	}
}

void MainObject::CheckToMap(Map& map_data, Mix_Chunk* exp_sound[2])
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	//check horizonal
	int  height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (x_val_ > 0)// moving right
		{

			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				Mix_PlayChannel(-1, exp_sound[2], 0);
				IncreaseMoney();
			}
			else if (val1 == TRAP || val2 == TRAP)
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				Mix_PlayChannel(-1, exp_sound[0], 0);
				DecreaseHealth();
			}

			else if (val1 == HP || val2 == HP)
			{
				if (get_health() < 5)
				{
					map_data.tile[y1][x2] = 0;
					map_data.tile[y2][x2] = 0;
					IncreaseHealth();
				}
			}
			else
			{


				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0;
				}
			}
		}
		else if (x_val_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				Mix_PlayChannel(-1, exp_sound[2], 0);
				IncreaseMoney();
			}

			else if (val1 == TRAP || val2 == TRAP)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				Mix_PlayChannel(-1, exp_sound[0], 0);
				DecreaseHealth();
				
			
			}

			else if (val1 == HP || val2 == HP)
			{
				if (get_health() < 5)
				{
					map_data.tile[y1][x1] = 0;
					map_data.tile[y2][x1] = 0;
					IncreaseHealth();
				}
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					x_pos_ = (x1 + 1) * TILE_SIZE;

					x_val_ = 0;
				}
			}
		}
	}

	//check  vertical
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0) {

			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];
			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;
				Mix_PlayChannel(-1, exp_sound[2], 0);
				IncreaseMoney();

			}
			else if (val1 == TRAP || val2 == TRAP)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x2] = 0;
				Mix_PlayChannel(-1, exp_sound[0], 0);
				if(x_val_ >=0)
				DecreaseHealth();
			}

			else if (val1 == HP || val2 == HP)
			{
				if (get_health() < 5)
				{
					map_data.tile[y1][x1] = 0;
					map_data.tile[y2][x2] = 0;
					IncreaseHealth();
				}
			}
			else
			{
				if (val1 != BLANK_TILE || val2 != BLANK_TILE)
				{
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= height_frame_ + 1;
					y_val_ = 0;
					on_ground_ = true;
					if (status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
				}
			}
		}
		else if (y_val_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				Mix_PlayChannel(-1, exp_sound[2], 0);
				IncreaseMoney();

			}
			else if (val1 == TRAP || val2 == TRAP)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				Mix_PlayChannel(-1, exp_sound[0], 0);
				DecreaseHealth();
			}
			else if (val1 == HP || val2 == HP)
			{
				if (get_health() < 5)
				{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				IncreaseHealth();
			}
			}
			else
			{
				if (val1 != BLANK_TILE || val1 != BLANK_TILE)
				{
					y_pos_ = (y1 + 1) * TILE_SIZE;
					y_val_ = 0;
				}
			}
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 100;
	}

	if (y_pos_ + height_frame_ > map_data.max_y_)
	{
		y_pos_ = map_data.max_y_ - height_frame_ + 60;
		come_back_time_ = 60;
	}
}

void MainObject::DecreaseMoney()
{
	if(money_count >=3)
	money_count = money_count - 3;
}
void MainObject::IncreaseMoney()
{
	money_count++;

}
void MainObject::IncreaseHealth()
{
	if(health < 5) 
	health++;
}
void MainObject::DecreaseHealth()
{
	health--;
}

void MainObject::UpdateImagePlayer(SDL_Renderer* des)
{
	if (on_ground_ == true)
	{
		if (status_ == WALK_RIGHT)
		{
			LoadImg("img//player_right.png", des);
		}
		else if (status_ == WALK_LEFT)
		{
			LoadImg("img//player_left.png", des);
		}
	}
	else
	{
		if (status_ == WALK_LEFT)
		{
			LoadImg("img//jum_left.png", des);
		}
		else if (status_ == WALK_RIGHT)
		{
			LoadImg("img//jum_right.png", des);
		}
	}

}