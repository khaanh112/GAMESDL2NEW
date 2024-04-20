
#include"SDL.h"
#include"stdafx.h"
#include"SDL_image.h"
#include"SDL_ttf.h"
#include"BaseObject.h"
#include"CommonFunc.h"
#include"Game_map.h"
#include<string>
#include"MainObject.h"
#include"ImpTimer.h"
#include"ThreatsObject.h"
#include<iostream>
#include<vector>
#include"ExplosionObject.h"
#include"TextObject.h"
#include"FlyObject.h"
#include"SDL_mixer.h"
#include<fstream>

BaseObject menu;
BaseObject g_background;
BaseObject lose_menu;
BaseObject win_menu;
TTF_Font* font_time = NULL;
TTF_Font* font_menu = NULL;

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("Game Cpp SDL2.0",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
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
		if (TTF_Init() != 0)
		{
			success = false;
		}
		font_time = TTF_OpenFont("font//dlxfont.ttf", 15);
		if (font_time == NULL)
		{
			success = false;
		}
		font_menu = TTF_OpenFont("font//dlxfont.ttf", 20);
		if (font_menu == NULL)
		{
			success = false;
		}
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			return  false;
		}
			g_sound_bullet[0] = Mix_LoadWAV("sound//playershoot.wav");
			g_sound_bullet[1] = Mix_LoadWAV("sound//playershoot.wav");
			g_sound_exp[0] = Mix_LoadWAV("sound//hitted.wav");
			g_sound_exp[1] = Mix_LoadWAV("sound//explosion.wav");
			g_sound_exp[2] = Mix_LoadWAV("sound//money.wav");
			if (g_sound_exp[0] == NULL || g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_exp[1] == NULL)
				return false;
	}
	
	return success;
}

bool LoadBackgroundMenu()
{
	bool ret = menu.LoadImg("img//menu.png", g_screen);
	if (ret == false)
		return false;

	return true;

}
bool LoadBackgroundWin()
{
	bool ret = win_menu.LoadImg("img//win_menu.png", g_screen);
	if (ret == false)
		return false;
	return true;
}
bool LoadBackgoundLose()
{
	bool ret = lose_menu.LoadImg("img//lose_menu.png", g_screen);
	if (ret == false)
		return false;
	return true;
}

bool LoadBackground()
{
	bool ret = g_background.LoadImg("img//BG.png", g_screen);
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
}

std::vector<FlyObject*> MakeFlyThreatsList()
{
	std::vector<FlyObject*> list_flythreats;

	FlyObject* dynamic_threats = new FlyObject[8];
	for (int i = 0; i < 8; i++)
	{
		FlyObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//fly_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(i * 1500 + 1600);
			p_threat->set_y_pos(50);
			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			list_flythreats.push_back(p_threat);
		}
	}

	return list_flythreats;
}
std::vector<ThreatsObject*> MakeThreatsList()
{
	std::vector<ThreatsObject*> list_threats;

	ThreatsObject* dynamic_threats = new ThreatsObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_left1.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_SPACE_THREAT);
			p_threat->set_x_pos(i * 505 + 500);
			p_threat->set_y_pos(200);

			int pos1 = p_threat->get_x_pos() - 100;
			int pos2 = p_threat->get_x_pos() + 100;
			p_threat->set_input_left(1);
			p_threat->SetAnimationPos(pos1, pos2);
			list_threats.push_back(p_threat);
		}
	}


	ThreatsObject* threats_objs = new ThreatsObject[30];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(i * 1200 + 1500);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);


			list_threats.push_back(p_threat);
		}

	}
	return list_threats;
}
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath) {
	// Load ảnh từ file
	SDL_Surface* surface = IMG_Load(filePath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}
int getHighScore() {
	std::ifstream file("highscore.txt");
	int highScore = 0;
	if (file) {
		file >> highScore;
		file.close();
	}
	return highScore;
}

void updateHighScore(int score) {
	int highScore = getHighScore();
	if (score > highScore) {
		std::ofstream file("highscore.txt");
		if (file) {
			file << score;
			file.close();
		}
	}
}


int main(int argc, char* argv[])
{

	ImpTimer fps_timer;

	if (InitData() == false)
		return -1;

	if (LoadBackground() == false)
		return -1;

	if (LoadBackgroundMenu() == false)
		return -1;
	if (LoadBackgoundLose() == false)
		return -1;
	if (LoadBackgroundWin() == false)
		return -1;
	Mix_Music* g_music = Mix_LoadMUS("sound//music.mp3");
	if (g_music == NULL)
		return -1;
	Mix_PlayMusic(g_music, -1);

	GameMap game_map;
	game_map.LoadMap("map//map01.dat");
	game_map.LoadTiles(g_screen);

	MainObject p_player;
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clips();

	Uint32  time_bit = 0;
	Uint32 time_bit1 = 0;
	Uint32 time_bit2 = 0;
	bool vong1 = true;
	bool can_lien_tuc = false;

	std::vector<ThreatsObject*> threats_list = MakeThreatsList();
	std::vector<FlyObject*> fly_list = MakeFlyThreatsList();

	ExplosionObject exp_threat;
	bool tRet = exp_threat.LoadImg("img//explosion.png", g_screen);
	if (!tRet) return -1;
	exp_threat.set_clip();

	ExplosionObject exp_player;
	bool pRet = exp_player.LoadImg("img//blood (2).png", g_screen);
	if (!pRet) return -1;
	exp_player.set_clip();


	//time
	TextObject time_game;
	time_game.SetColor(TextObject::BLACK_TEXT);

	TextObject money_countt;
	money_countt.SetColor(TextObject::YELLOW_TEXT);

	TextObject high_score;
	TextObject start_button;
	TextObject quit_button;
	TextObject continue_button;
	TextObject your_score;

	bool is_menu = true;
	bool start = true;
	bool quit = true;
	bool time = false;
	bool tmp_time = false;
	bool is_quit = false;
	bool pause_menu = false;
	bool l_menu = false;
	bool w_menu = false;
	Uint32 time_menu = 0;
	Uint32 time_pause = 0;
	SDL_Texture* gBackgroundTexture = menu.GetObject();
	SDL_Texture* Backgoundlose = lose_menu.GetObject();
	SDL_Texture* Backgoundwin = win_menu.GetObject();
	SDL_Texture* tien = loadTexture(g_screen, "img//tien.png");
	SDL_Texture* mau = loadTexture(g_screen, "img//mau1.png");
	
	while (!is_quit)
	{
		int score = 0;
		int heal = p_player.get_health();
		fps_timer.start();

		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			p_player.HandleInputAction(g_event, g_screen,g_sound_bullet);
			
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);
		while (is_menu)
		{

			while (SDL_PollEvent(&g_event) != 0)
			{
				if (g_event.type == SDL_QUIT)
				{
					is_quit = true;
				}
			}

			SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			//SDL_SetTextureBlendMode(gBackgroundTexture, SDL_BLENDMODE_MOD);
			SDL_RenderCopy(g_screen, gBackgroundTexture, NULL, &backgroundRect);


			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			high_score.SetText("HIGHEST SCORE: " + std::to_string(getHighScore()));
			start_button.SetText("START");
			quit_button.SetText("QUIT");

			start_button.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 50 && mouseX <= 1280 / 2 - 40 + 130 && mouseY >= 480 / 2 + 40 && mouseY <= 480 / 2 + 37 + 40) {
				start_button.SetColor(TextObject::YELLOW_TEXT);
				if (g_event.type == SDL_MOUSEBUTTONDOWN) {
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						start = true;
						is_menu = false;
						time = true;
						if (tmp_time == false)
						{
							time_menu = SDL_GetTicks() / 1000;
							tmp_time = true;
						}
					}
				}
			}
			else {

				start_button.SetColor(TextObject::RED_TEXT);
			}
			start_button.RenderText(g_screen, 1280 / 2 - 50, 480 / 2 + 50);
			high_score.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 -100 && mouseX <= 1280 / 2 - 40 + 300 && mouseY >= 480 / 2  && mouseY <= 480 / 2 + 37 ) {
				high_score.SetColor(TextObject::YELLOW_TEXT);
			}
			else {

				high_score.SetColor(TextObject::RED_TEXT);
			}

			high_score.RenderText(g_screen, 1280 / 2 - 100, 480 / 2 + 10);

		
			quit_button.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 120 && mouseY <= 480 / 2 + 37 - 15 + 120) {
				quit_button.SetColor(TextObject::YELLOW_TEXT);
				if (g_event.type == SDL_MOUSEBUTTONDOWN) {
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						is_quit = true;
						is_menu = false;
					}
				}
			}
			else {
				quit_button.SetColor(TextObject::RED_TEXT);
			}
			quit_button.RenderText(g_screen, 1280 / 2 - 40 + 5, 480 / 2 + 114);

			SDL_RenderPresent(g_screen);
			SDL_Delay(100);
		}
		if (g_event.type == SDL_KEYDOWN) {
			if (g_event.key.keysym.sym == SDLK_ESCAPE) {
				time_pause = SDL_GetTicks() / 1000;
				pause_menu = true;
				
			}
		}
		while (pause_menu)
		{

			while (SDL_PollEvent(&g_event) != 0)
			{
				if (g_event.type == SDL_QUIT)
				{
					is_quit = true;
				}
			}

			SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			SDL_RenderCopy(g_screen, gBackgroundTexture, NULL, &backgroundRect);

			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			continue_button.SetText("CONTINUE");
			quit_button.SetText("QUIT");

			continue_button.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 50 && mouseX <= 1280 / 2 - 40 + 130 && mouseY >= 480 / 2 +40 && mouseY <= 480 / 2 + 37 + 40) {
				continue_button.SetColor(TextObject::YELLOW_TEXT);
				if (g_event.type == SDL_MOUSEBUTTONDOWN) {
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						pause_menu = false;
						time_pause = SDL_GetTicks() / 1000 - time_pause;
						
					}
				}
			}
			else {

				continue_button.SetColor(TextObject::RED_TEXT);
			}
			continue_button.RenderText(g_screen, 1280 / 2 - 60, 480 /2 + 50);
			high_score.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 100 && mouseX <= 1280 / 2 - 40 + 300 && mouseY >= 480 / 2 && mouseY <= 480 / 2 + 37) {
				high_score.SetColor(TextObject::YELLOW_TEXT);
			}
			else {

				high_score.SetColor(TextObject::RED_TEXT);
			}

			high_score.RenderText(g_screen, 1280 / 2 - 100, 480 / 2 + 10);


			quit_button.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 120 && mouseY <= 480 / 2 + 37 - 15 + 120) {
				quit_button.SetColor(TextObject::YELLOW_TEXT);
				if (g_event.type == SDL_MOUSEBUTTONDOWN) {
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						is_quit = true;
						pause_menu = false;
					}
				}
			}
			else {
				quit_button.SetColor(TextObject::RED_TEXT);
			}
			quit_button.RenderText(g_screen, 1280 / 2 - 40 + 5, 480 / 2 + 114);

			SDL_RenderPresent(g_screen);
			SDL_Delay(100);
		}

		g_background.Render(g_screen, NULL);

		Map map_data = game_map.getMap();

		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data, g_sound_exp);
		p_player.Show(g_screen);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);

		// quai thuong
		for (int i = 0; i < threats_list.size(); i++)
		{
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->DoPlayer(map_data);
				p_threat->get_player_rect(p_player.GetRect());
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);

				SDL_Rect rect_player = p_player.GetRectFrame();
				bool bCol1 = false;

				int frame_exp_wid = exp_player.get_frame_width();
				int frame_exp_hei = exp_player.get_frame_height();

				std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
				for (int jj = 0; jj < tBullet_list.size(); jj++)
				{
					BulletObject* pt_bullet = tBullet_list.at(jj);

					if (pt_bullet) {
						pt_bullet->CheckToMap(map_data);
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if (bCol1)
						{
							for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
							{
								int x_pos = pt_bullet->GetRect().x - frame_exp_wid * 0.5;
								int y_pos = pt_bullet->GetRect().y - frame_exp_hei * 0.5;

								exp_player.set_frame(ex);
								exp_player.SetRect(x_pos, y_pos);
								exp_player.Show(g_screen);
								SDL_RenderPresent(g_screen);

							}
							p_threat->SetBulletRect(jj);
							Mix_PlayChannel(-1, g_sound_exp[0], 0);
							p_player.DecreaseHealth();

						}
					}
				}


				//break;

				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
				if (bCol2)   //sua them
				{

					for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
					{
						//	int x_pos = pt_bullet->GetRect().x - frame_exp_wid * 0.5;
						//	int y_pos = pt_bullet->GetRect().y - frame_exp_hei * 0.5;
						int x_pos = p_player.GetRect().x;
						int y_pos = p_player.GetRect().y;
						exp_player.set_frame(ex);
						exp_player.SetRect(x_pos, y_pos);
						exp_player.Show(g_screen);
					}
					if (vong1)
					{
						time_bit = SDL_GetTicks();
						vong1 = false;
					}
					if (!can_lien_tuc) {
						time_bit1 = SDL_GetTicks();
						can_lien_tuc = true;
					}
					time_bit2 = SDL_GetTicks();

					if (time_bit2 - time_bit1 > 100)
					{
						can_lien_tuc = false;

					}
					if ((time_bit1 - time_bit > 400) || ((time_bit2 - time_bit > 300) && (time_bit2 - time_bit1) > 200))
					{
						time_bit = time_bit2;
						vong1 = true;
					}
					if ((time_bit2 - time_bit) > 400)
					{

						p_player.DecreaseHealth();
						Mix_PlayChannel(-1, g_sound_exp[0], 0);
						time_bit = time_bit2;

					}
					
				}
			}
		}
		// quai bay
		for (int i = 0; i < fly_list.size(); i++)
		{
			FlyObject* p_threat = fly_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->get_player_rect(p_player.GetRect());
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);

				SDL_Rect rect_player = p_player.GetRectFrame();
				bool bCol1 = false;

				int frame_exp_wid = exp_player.get_frame_width();
				int frame_exp_hei = exp_player.get_frame_height();

				std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
				for (int jj = 0; jj < tBullet_list.size(); jj++)
				{
					BulletObject* pt_bullet = tBullet_list.at(jj);
					
					if (pt_bullet) {
						pt_bullet->CheckToMap(map_data);
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if (bCol1)
						{
							for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
							{
								int x_pos = pt_bullet->GetRect().x - frame_exp_wid * 0.5;
								int y_pos = pt_bullet->GetRect().y - frame_exp_hei * 0.5;

								exp_player.set_frame(ex);
								exp_player.SetRect(x_pos, y_pos);
								exp_player.Show(g_screen);
								SDL_RenderPresent(g_screen);

							}
							p_threat->SetBulletRect(jj);
							Mix_PlayChannel(-1, g_sound_exp[0], 0);
							p_player.DecreaseHealth();

						}
					}
				}

			}

		}


		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height = exp_threat.get_frame_height();
		//quai bay chet:
		std::vector<BulletObject*> bullet_ar = p_player.get_bullet_list();
		for (int r = 0; r < bullet_ar.size(); r++) {
			BulletObject* p_bullet = bullet_ar.at(r);
			if (p_bullet != NULL)
			{
				p_bullet->CheckToMap(map_data);
				if (p_bullet->get_is_move() == false)
				{
					p_player.RemoveBullet(r);


				}
				for (int t = 0; t < fly_list.size(); t++)
				{
					FlyObject* obj_threat = fly_list.at(t);
					if (obj_threat != NULL)
					{
						SDL_Rect tRect;
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();

						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

						if (bCol)
						{
							for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
							{
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
							}
							p_player.RemoveBullet(r);
							obj_threat->Free();
							fly_list.erase(fly_list.begin() + t);
							Mix_PlayChannel(-1, g_sound_exp[1], 0);

						}
					}
				}
			}
		}

		std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
		for (int r = 0; r < bullet_arr.size(); r++) {
			BulletObject* p_bullet = bullet_arr.at(r);
			if (p_bullet != NULL)
			{

				p_bullet->CheckToMap(map_data);
				if (p_bullet->get_is_move() == false)
				{
					p_player.RemoveBullet(r);

				}
				for (int t = 0; t < threats_list.size(); t++)
				{
					ThreatsObject* obj_threat = threats_list.at(t);
					if (obj_threat != NULL)
					{
						SDL_Rect tRect;
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();

						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

						if (bCol)
						{
							for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
							{
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
							}
							p_player.RemoveBullet(r);
							obj_threat->Free();
							threats_list.erase(threats_list.begin() + t);
							Mix_PlayChannel(-1, g_sound_exp[1], 0);

						}
					}
				}
			}
		}
		
		//show game time
		std::string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks() / 1000;
		Uint32 val_time = 200 - time_val + time_menu + time_pause;

		if ( val_time<= 0 || heal < 1)
		{
			l_menu = true;
			while (l_menu)
			{

				while (SDL_PollEvent(&g_event) != 0)
				{
					if (g_event.type == SDL_QUIT)
					{
						is_quit = true;
					}
				}

				SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				SDL_RenderCopy(g_screen, Backgoundlose, NULL, &backgroundRect);

				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);

			
				quit_button.SetText("QUIT");

				quit_button.LoadFromRenderText(font_menu, g_screen);
				if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 120 && mouseY <= 480 / 2 + 37 - 15 + 120) {
					quit_button.SetColor(TextObject::YELLOW_TEXT);
					if (g_event.type == SDL_MOUSEBUTTONDOWN) {
						if (g_event.button.button == SDL_BUTTON_LEFT)
						{
							is_quit = true;
							l_menu = false;
						}
					}
				}
				else {
					quit_button.SetColor(TextObject::RED_TEXT);
				}
				quit_button.RenderText(g_screen, 1280 / 2 - 40 + 5, 480 / 2 + 114);
				high_score.LoadFromRenderText(font_menu, g_screen);
				if (mouseX >= 1280 / 2 - 100 && mouseX <= 1280 / 2 - 40 + 300 && mouseY >= 480 / 2 && mouseY <= 480 / 2 + 37) {
					high_score.SetColor(TextObject::YELLOW_TEXT);
				}
				else {

					high_score.SetColor(TextObject::BLACK_TEXT);
				}

				high_score.RenderText(g_screen, 1280 / 2 - 100, 480 / 2 + 10);

				SDL_RenderPresent(g_screen);
				SDL_Delay(100);
			}
		}
		else
		{
			std::string str_val = std::to_string(val_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.LoadFromRenderText(font_time, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);

			for (int i = 1; i <= heal; i++)
			{
				
				int x = 10 + i * 30; // Vị trí x trên cửa sổ
				int y = 30; // Vị trí y trên cửa sổ
				int width = 30; // Chiều rộng của texture
				int height = 30; // Chiều cao của texture
				SDL_Rect dstrect = { x, y, width, height };
				SDL_RenderCopy(g_screen, mau, NULL, &dstrect);
			}
		}

		//tiền
		
		int x = 40; // Vị trí x trên cửa sổ
		int y = 7; // Vị trí y trên cửa sổ
		int width = 30; // Chiều rộng của texture
		int height = 30; // Chiều cao của texture
		SDL_Rect dstrect = { x, y, width, height };
		SDL_RenderCopy(g_screen, tien, NULL, &dstrect);

		std::string money = "$: ";
		int money_count = p_player.get_money_count();
		std::string moneyy = std::to_string(money_count);
		money += moneyy;
		money_countt.SetText(money);
		money_countt.LoadFromRenderText(font_time, g_screen);
		money_countt.RenderText(g_screen, SCREEN_WIDTH - 1200, 15);
		

		SDL_RenderPresent(g_screen);


		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0)
				SDL_Delay(delay_time);
		}
		
		if (p_player.get_x_pos() >= game_map.get_max_map()-600)
		{
			w_menu = true;
			score = money_count + 200 - time_val + heal * 10;
			std::cout << score << " ";
			updateHighScore(score);
			while (w_menu)
			{

				while (SDL_PollEvent(&g_event) != 0)
				{
					if (g_event.type == SDL_QUIT)
					{
						is_quit = true;
					}
				}

				SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				SDL_RenderCopy(g_screen, Backgoundwin, NULL, &backgroundRect);

				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);


				quit_button.SetText("QUIT");

				quit_button.LoadFromRenderText(font_menu, g_screen);
				if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 120 && mouseY <= 480 / 2 + 37 - 15 + 120) {
					quit_button.SetColor(TextObject::YELLOW_TEXT);
					if (g_event.type == SDL_MOUSEBUTTONDOWN) {
						if (g_event.button.button == SDL_BUTTON_LEFT)
						{
							is_quit = true;
							w_menu = false;
						}
					}
				}
				else {
					quit_button.SetColor(TextObject::RED_TEXT);
				}
				quit_button.RenderText(g_screen, 1280 / 2 - 40 + 5, 480 / 2 + 114);
				high_score.LoadFromRenderText(font_menu, g_screen);
				if (mouseX >= 1280 / 2 - 100 && mouseX <= 1280 / 2 - 40 + 300 && mouseY >= 480 / 2 && mouseY <= 480 / 2 + 37) {
					high_score.SetColor(TextObject::YELLOW_TEXT);
				}
				else {

					high_score.SetColor(TextObject::RED_TEXT);
				}

				high_score.RenderText(g_screen, 1280 / 2 - 100, 480 / 2 + 10);
				your_score.SetText("YOUR SCORE: " + std::to_string(score));
				your_score.LoadFromRenderText(font_menu, g_screen);
				if (mouseX >= 1280 / 2 - 100 && mouseX <= 1280 / 2 - 40 + 230 && mouseY >= 480 / 2 +40 && mouseY <= 480 / 2 + 37 +40) {
					your_score.SetColor(TextObject::YELLOW_TEXT);
				}
				else {

					your_score.SetColor(TextObject::RED_TEXT);
				}

				your_score.RenderText(g_screen, 1280 / 2 - 100, 480 / 2 + 50);

				SDL_RenderPresent(g_screen);
				SDL_Delay(100);
			}
		}
	}
	Mix_HaltMusic();
	Mix_FreeMusic(g_music);
	Mix_CloseAudio();

	close();
	return 0;

}
