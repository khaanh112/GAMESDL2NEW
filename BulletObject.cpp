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
    if (bullet_type_ == ARROW_BULLET)
    {
        LoadImg("img//arrow_bullet.png", des);
    }
    else if (bullet_type_ == SPHERE_BULLET) {
        LoadImg("img//sphere_bullet.png", des);
    }
    else
    {
        LoadImg("img//fire_bullet.png", des);
    }
}
bool BulletObject::LoadImg1(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if (load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if (new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }
    p_object_ = new_texture;

    return p_object_ != NULL;
}
void BulletObject::Show(SDL_Renderer* renderer) {
    // Xoay ảnh viên đạn theo góc đã tính toán
   // SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
    SDL_Rect renderquad = { rect_.x, rect_.y , rect_.w , rect_.h };
    SDL_RenderCopyEx(renderer, p_object_, NULL, &renderquad, angle_of_bullet_, NULL, SDL_FLIP_NONE);
}



void BulletObject::CheckToMap(Map& map_data)
{
    int x = (rect_.x + map_data.start_x_) / TILE_SIZE;
    int y = (rect_.y + map_data.start_y_) / TILE_SIZE;
    if (x > 0 && x < MAX_MAP_X && y > 0 && y < MAX_MAP_Y)
    {
        if (map_data.tile[y][x] != BLANK_TILE && map_data.tile[y][x] != STATE_MONEY && map_data.tile[y][x] != HP && map_data.tile[y][x] != TRAP)
        {
            is_move_ = false;

        }
    }
}



void BulletObject::SetDirection(const int& mouse_x, const int& mouse_y) {
    // Tính toán vector hướng từ vị trí hiện tại của viên đạn đến vị trí đã bấm chuột
    int dx = mouse_x - rect_.x;
    int dy = mouse_y - rect_.y;

    // Tính toán góc giữa vector hướng và trục x bằng arctan
    double angle = atan2(dy, dx) * (180.0 / M_PI);

    // Đảm bảo góc nằm trong khoảng từ 0 đến 360 độ
    if (angle < 0) {
        angle += 360;
    }

    // Chuyển đổi góc 
    double angle_radians = angle * (M_PI / 180.0);
    angle_of_bullet_ = angle;

    // Thiết lập hướng di chuyển cho viên đạn
    const double bullet_speed = 10.0; // Tốc độ của viên đạn
    x_val_ = cos(angle_radians) * bullet_speed;
    y_val_ = sin(angle_radians) * bullet_speed * 1.15;
}

void BulletObject::SetDirection1(const SDL_Rect& target_position) {
    
    int dx = target_position.x - rect_.x;
    int dy = target_position.y - rect_.y;
    
    double angle = atan2(dy, dx) * (180.0 / M_PI);

    // Đảm bảo góc nằm trong khoảng từ 0 đến 360 độ
    if (angle < 0) {
        angle += 360;
    }

    double angle_radians = angle * (M_PI / 180.0);

    const double bullet_speed = 15.0; 
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


void BulletObject::HandleMove1(const int& x_border, const int& y_border) {
    // Di chuyển viên đạn theo hướng ban đầu

    rect_.x -= x_val_;
    rect_.y += y_val_;

    // Kiểm tra nếu viên đạn ra khỏi màn hình
    if (rect_.x > x_border || rect_.x < 0 || rect_.y > y_border || rect_.y < 0) {
        is_move_ = false; // Đặt trạng thái di chuyển của viên đạn thành false nếu ra khỏi biên giới
    }
}