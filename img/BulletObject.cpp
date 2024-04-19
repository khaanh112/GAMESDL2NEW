#include"stdafx.h"
#include"BulletObject.h"
#include<cmath>
#include<iostream>
BulletObject::BulletObject()
{
	x_val_ = 0; 
	y_val_ = 0;
	is_move_ = false;
    bullet_type_ = SPHERE_BULLET;
	 
}

BulletObject::~BulletObject()
{
}

void BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    if(bullet_type_ == ARROW_BULLET)
    { 
        LoadImg("img//arrow_bullet.png", des);
    }
    else {
        LoadImg("img//sphere_bullet.png", des);
    }
}
/*
void BulletObject::CheckToMap(Map& map_data)
{
    // Tính toán vùng chiếm của viên đạn trên bản đồ dựa trên rect
    int x1 = (rect_.x + x_val_); // TILE_SIZE;
    int x2 = (rect_.x + x_val_ + rect_.w - 1); // TILE_SIZE;

    int y1 = (rect_.y + y_val_); // TILE_SIZE;
    int y2 = (rect_.y + y_val_ + rect_.h - 1); // TILE_SIZE;

    // Kiểm tra va chạm với bản đồ
    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        // Kiểm tra từng ô lưới
        if ((map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] != STATE_MONEY) ||
            (map_data.tile[y1][x2] != BLANK_TILE && map_data.tile[y1][x2] != STATE_MONEY) ||
            (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] != STATE_MONEY) ||
            (map_data.tile[y2][x2] != BLANK_TILE && map_data.tile[y2][x2] != STATE_MONEY))
        {
            // Nếu đạn va chạm với bản đồ và không phải là STATE_MONEY hoặc BLANK_TILE, đặt trạng thái di chuyển của viên đạn thành false để biến mất
            is_move_ = false;
        }
    }
}
*/
void BulletObject::CheckToMap(Map& map_data)
{
    int x = (rect_.x + x_val_ -1) / TILE_SIZE;
    int y = (rect_.y + + y_val_ -1)/ TILE_SIZE;
    if (x > 0 && x < MAX_MAP_X && y > 0 && y < MAX_MAP_Y)
    {
        if (map_data.tile[y][x] != BLANK_TILE && map_data.tile[y][x] != STATE_MONEY)
        {
            is_move_ = false;
        }
    }
}



void BulletObject::SetDirection(const int& mouse_x, const int& mouse_y) {
    // Tính toán vector hướng từ vị trí hiện tại của viên đạn đến vị trí đã bấm chuột
    int dx = mouse_x - rect_.x;
    int dy =   mouse_y - rect_.y;

    // Tính toán góc giữa vector hướng và trục x bằng arctan
    double angle = atan2(dy, dx) * (180.0 / M_PI);

    // Đảm bảo góc nằm trong khoảng từ 0 đến 360 độ
    if (angle < 0) {
        angle += 360;
    }

    // Chuyển đổi góc 
    double angle_radians = angle * (M_PI / 180.0);

    // Thiết lập hướng di chuyển cho viên đạn
    const double bullet_speed = 15.0; // Tốc độ của viên đạn
    x_val_ = cos(angle_radians) * bullet_speed;
    y_val_ = sin(angle_radians) * bullet_speed*1.15;
}

void BulletObject::SetDirection1(const SDL_Rect& target_position) {
    // Tính toán vector hướng từ vị trí hiện tại của viên đạn đến vị trí của MainObject
    int dx = target_position.x - rect_.x;
    int dy = target_position.y - rect_.y;
  //  std::cout << target_position.x << "  " << rect_.x;
    // Tính toán góc giữa vector hướng và trục x bằng arctan
    double angle = atan2(dy, dx) * (180.0 / M_PI);

    // Đảm bảo góc nằm trong khoảng từ 0 đến 360 độ
    if (angle < 0) {
        angle += 360;
    }

    // Chuyển đổi góc 
    double angle_radians = angle * (M_PI / 180.0);

    // Thiết lập hướng di chuyển cho viên đạn
    const double bullet_speed = 15.0; // Tốc độ của viên đạn
    x_val_ = cos(angle_radians) * bullet_speed;
    y_val_ = sin(angle_radians) * bullet_speed;
}





void BulletObject::HandleMove(const int& x_border, const int& y_border) {
    // Di chuyển viên đạn theo hướng ban đầu
    
    rect_.x += x_val_;
    rect_.y += y_val_;

    // Kiểm tra nếu viên đạn ra khỏi màn hình
    if (rect_.x > x_border || rect_.x < 0 || rect_.y > y_border || rect_.y < 0) {
        is_move_ = false; // Đặt trạng thái di chuyển của viên đạn thành false nếu ra khỏi biên giới
    }
}


void BulletObject:: HandleMove1(const int& x_border, const int& y_border) {
    // Di chuyển viên đạn theo hướng ban đầu

    rect_.x -= x_val_;
    rect_.y += y_val_;

    // Kiểm tra nếu viên đạn ra khỏi màn hình
    if (rect_.x > x_border || rect_.x < 0 || rect_.y > y_border || rect_.y < 0) {
        is_move_ = false; // Đặt trạng thái di chuyển của viên đạn thành false nếu ra khỏi biên giới
    }
}

