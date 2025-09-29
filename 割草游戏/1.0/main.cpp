#include <graphics.h>
#include <string>
#include <vector>
#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "winmm.lib")

int window_size[2] = { 900, 600 };
int button_size[2] = { 150, 60 };
bool is_game_started = false;
bool running = true;

inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

class Atlas {
public:
	Atlas(LPCTSTR path, int num, int nWidth = 0, int nHeight = 0) {
		TCHAR path_file[256];
		for (size_t i = 0; i < num; i++) {
			_stprintf_s(path_file, path, i);
			IMAGE* frame = new IMAGE();
			loadimage(frame, path_file, nWidth, nHeight);
			frame_list.push_back(frame);
		}
	}
	~Atlas() {
		for (size_t i = 0; i < frame_list.size(); i++)
			delete frame_list[i];
	}
	std::vector<IMAGE*> frame_list;
};

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;

class Animation {
public:
	Animation(int interval, Atlas* atlas) {
		interval_ms = interval;
		anim_atlas = atlas;
	}
	~Animation() = default;

	void play(int x, int y, int delay) {
		timer += delay;
		if (timer >= interval_ms) {
			idx_frame = (idx_frame + 1) % anim_atlas->frame_list.size();
			timer = 0;
		}
		putimage_alpha(x, y, anim_atlas->frame_list[idx_frame]);
	}

private:
	int timer = 0;
	int idx_frame = 0;
	int interval_ms = 0;
	Atlas* anim_atlas;
};

class Button {
public:
	Button(RECT rect, LPCTSTR img_idle_path, LPCTSTR img_hovered_path, LPCTSTR img_pushed_path) {
		region = rect;
		loadimage(&img_idle, img_idle_path);
		loadimage(&img_hovered, img_hovered_path);
		loadimage(&img_pushed, img_pushed_path);
	}

	~Button() = default;
	
	void processEvent(ExMessage msg) {
		switch (msg.message) {
		case WM_MOUSEMOVE:
			if (status == Status::Idle && checkCurect_btn_startorPosition(msg.x, msg.y))
				status = Status::Hovered;
			else if (status == Status::Hovered && !checkCurect_btn_startorPosition(msg.x, msg.y))
				status == Status::Idle;
			break;
		case WM_LBUTTONDOWN:
			if (checkCurect_btn_startorPosition(msg.x, msg.y))
				status = Status::Pushed; break;
		case WM_LBUTTONUP:
			if (status == Status::Pushed)
				onClick(); break;
		default:
			break;
		}
	}


	void draw() {
		switch (status) {
		case Status::Idle:
			putimage_alpha(region.left, region.top, &img_idle); break;
		case Status::Hovered:
			putimage_alpha(region.left, region.top, &img_hovered); break;
		case Status::Pushed:
			putimage_alpha(region.left, region.top, &img_pushed); break;
		}
	}

protected:
	virtual void onClick() = 0;

private:

	bool checkCurect_btn_startorPosition(int x, int y) {
		return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
	}


	enum class Status {
		Idle = 0,
		Hovered,
		Pushed
	};
	RECT region;
	IMAGE img_idle;
	IMAGE img_hovered;
	IMAGE img_pushed;
	Status status = Status::Idle;
};

class StartButton : public Button {
public:
	StartButton(RECT rect, LPCTSTR img_idle_path, LPCTSTR img_hovered_path, LPCTSTR img_pushed_path)
		:Button(rect, img_idle_path, img_hovered_path, img_pushed_path) {}
	~StartButton() = default;

protected:
	void onClick() {
		mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
		is_game_started = true;
	}
};

class QuitButton : public Button {
public:
	QuitButton(RECT rect, LPCTSTR img_idle_path, LPCTSTR img_hovered_path, LPCTSTR img_pushed_path)
		:Button(rect, img_idle_path, img_hovered_path, img_pushed_path) {}
	~QuitButton() = default;

protected:
	void onClick() {
		running = false;
	}
};

class Player {
public:
	int width = 50;
	int height = 50;
	int radius = 15;

	Player() {
		anim_left = new Animation(45, atlas_player_left);
		anim_right = new Animation(45, atlas_player_right);
	}

	~Player() {
		delete anim_left;
		delete anim_right;
	}

	void move() {
		int dir_x = is_move_right - is_move_left;
		int dir_y = is_move_down - is_move_up;
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);

		if (len_dir) {
			double normalized_x = dir_x / len_dir;
			double normalized_y = dir_y / len_dir;
			position.x += (int)(PLAYER_speed * normalized_x);
			position.y += (int)(PLAYER_speed * normalized_y);
		}

		if (position.x < 0) position.x = 0;
		if (position.x > 850) position.x = 850;
		if (position.y < 0) position.y = 0;
		if (position.y > 550) position.y = 550;

	}

	void draw(int delay) {
		static bool facing_left = true;
		int dir_x = is_move_right - is_move_left;
		if (dir_x > 0)
			facing_left = false;
		else if (dir_x < 0)
			facing_left = true;

		if (facing_left)
			anim_left->play(position.x, position.y, delay);
		else
			anim_right->play(position.x, position.y, delay);
	}

	void processEvent(ExMessage msg) {//const
		if (msg.message == WM_KEYDOWN) {
			switch (msg.vkcode) {
			case 'W':
				is_move_up = true; break;
			case 'S':
				is_move_down = true; break;
			case 'A':
				is_move_left = true; break;
			case 'D':
				is_move_right = true; break;
			}
		}
		else if (msg.message == WM_KEYUP) {
			switch (msg.vkcode) {
			case 'W':
				is_move_up = false; break;
			case 'S':
				is_move_down = false; break;
			case 'A':
				is_move_left = false; break;
			case 'D':
				is_move_right = false; break;
			}
		}
	}

	POINT& getPosition() {
		return position;
	}

private:
	Animation* anim_left;
	Animation* anim_right;
	const int PLAYER_speed = 5;
	POINT position = { 450, 300 };
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;
};


class Bullet {
public:
	POINT position = { 0, 0 };
	Bullet() = default;
	~Bullet() = default;

	void draw() {
		setlinecolor(BLACK);
		setfillcolor(RED);
		fillcircle(position.x, position.y, radius);
	}
private:
	int radius = 5;
};

class Enemy {
public:
	Enemy() {
		anim_left = new Animation(45, atlas_enemy_left);
		anim_right = new Animation(45, atlas_enemy_right);

		enum SpawnEdge {
			Up = 0,
			Down,
			Left,
			Right
		};
		SpawnEdge edge = (SpawnEdge)(rand() % 4);
		switch (edge) {
		case SpawnEdge::Up:
			position.x = rand() % window_size[0];
			position.y = -height; break;
		case SpawnEdge::Down:
			position.x = rand() % window_size[0];
			position.y = window_size[1]; break;
		case SpawnEdge::Left:
			position.x = -height;
			position.y = rand() % window_size[1]; break;
		case SpawnEdge::Right:
			position.x = window_size[0];
			position.y = rand() % window_size[1]; break;
		}
	}
	~Enemy() {
		delete anim_left;
		delete anim_right;
	}

	bool checkBulletCollision(const Bullet& bullet) {
		bool is_overlap_x = bullet.position.x > position.x && bullet.position.x < position.x + width;
		bool is_overlap_y = bullet.position.y > position.y && bullet.position.y < position.y + height;
		return is_overlap_x && is_overlap_y;
	}

	bool checkPlayerCollision(Player& player) {
		POINT& player_position = player.getPosition();

		int dir_x2 = (player_position.x - position.x) * (player_position.x - position.x);
		int dir_y2 = (player_position.y - position.y) * (player_position.y - position.y);
		return dir_x2 + dir_y2 < (radius + player.radius) * (radius + player.radius);
	}

	void move(Player& player) {
		const POINT& player_position = player.getPosition();
		int dir_x = player_position.x - position.x;
		int dir_y = player_position.y - position.y;
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir) {
			double normalized_x = dir_x / len_dir;
			double normalized_y = dir_y / len_dir;
			position.x += (int)(speed * normalized_x);
			position.y += (int)(speed * normalized_y);
		}
		if (dir_x < 0) facing_left = true;
		else if (dir_x > 0) facing_left = false;
	}

	void draw(int delay) {
		if (facing_left)
			anim_left->play(position.x, position.y, delay);
		else
			anim_right->play(position.x, position.y, delay);
	}

	void hurt() {
		alive = false;
	}

	bool checkAlive() {
		return alive;
	}

private:
	int width = 48;
	int height = 48;
	int interval_ms;
	int speed = 4;
	int radius = 15;
	Animation* anim_left;
	Animation* anim_right;
	POINT position = { 0, 0 };
	bool facing_left = true;
	bool alive = true;
};

void tryGenerateEnemy(std::vector<Enemy*>& enemy_list) {
	int interval = 100;
	static int count = 0;
	if (++count % interval == 0)
		enemy_list.push_back(new Enemy());
}

void updateBullets(std::vector<Bullet>& bullet_list, Player& player) {
	double radial_speed = 0.001;
	double tangent_speed = 0.001;
	double radian_interval = 3.14159 * 2 / bullet_list.size();
	POINT player_position = player.getPosition();
	double radius = 80 + 25 * sin(GetTickCount() * radial_speed);

	for (size_t i = 0; i < bullet_list.size(); i++) {
		double radian = GetTickCount() * tangent_speed + radian_interval * i;
		bullet_list[i].position.x = player_position.x + player.width / 2 + (int)(radius * sin(radian));
		bullet_list[i].position.y = player_position.y + player.height / 2 + (int)(radius * cos(radian));
	}
}

void drawPlayerScore(int score) {
	static TCHAR text[64];
	_stprintf_s(text, _T("得分：%d"), score);
	setbkcolor(BLACK);
	settextcolor(WHITE);
	outtextxy(0, 0, text);
}

int main() {
	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open mus/hit1.wav alias hit1"), NULL, 0, NULL);
	mciSendString(_T("open mus/hit2.wav alias hit2"), NULL, 0, NULL);

	atlas_player_left = new Atlas(_T("img/player_left_%d.png"), 5, 50, 50);
	atlas_player_right = new Atlas(_T("img/player_right_%d.png"), 5, 50, 50);
	atlas_enemy_left = new Atlas(_T("img/enemy_left_%d.png"), 2);
	atlas_enemy_right = new Atlas(_T("img/enemy_right_%d.png"), 2);

	RECT rect_btn_start, rect_btn_quit;
	rect_btn_start.left = (window_size[0] - button_size[0]) / 2;
	rect_btn_start.right = (window_size[0] + button_size[0]) / 2;
	rect_btn_start.top = 350;
	rect_btn_start.bottom = rect_btn_start.top + button_size[1];

	rect_btn_quit.left = (window_size[0] - button_size[0]) / 2;
	rect_btn_quit.right = (window_size[0] + button_size[0]) / 2;
	rect_btn_quit.top = rect_btn_start.bottom + 20;
	rect_btn_quit.bottom = rect_btn_quit.top + button_size[1];

	StartButton btn_start = StartButton(rect_btn_start, _T("img/btn_start_idle.png"), _T("img/btn_start_hovered.png"), _T("img/btn_start_pushed.png"));
	QuitButton btn_quit = QuitButton(rect_btn_quit, _T("img/btn_quit_idle.png"), _T("img/btn_quit_hovered.png"), _T("img/btn_quit_pushed.png"));

	IMAGE background_image;
	IMAGE menu_image;
	Player player = Player();
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list(3);
	int score = 0;

	initgraph(window_size[0], window_size[1]);
	loadimage(&menu_image, _T("img/menu.png"));
	loadimage(&background_image, _T("img/background.png"));
	BeginBatchDraw();

	while (running) {
		DWORD start_time = GetTickCount();
		ExMessage msg;
		while (peekmessage(&msg)) {
			if (is_game_started) {
				player.processEvent(msg);
			}
			else {
				btn_start.processEvent(msg);
				btn_quit.processEvent(msg);
			}
		}
		if (is_game_started) {
			tryGenerateEnemy(enemy_list);
			player.move();
			updateBullets(bullet_list, player);
			for (Enemy* enemy : enemy_list) enemy->move(player);

			for (Enemy* enemy : enemy_list)
				for (Bullet bullet : bullet_list) {
					if (enemy->checkBulletCollision(bullet)) {
						enemy->hurt();
						mciSendString(_T("play hit1 from 0"), NULL, 0, 0);
						score++;
					}
				}

			for (Enemy* enemy : enemy_list)
				if (enemy->checkPlayerCollision(player)) {
					running = false;
					TCHAR text[120];
					_stprintf_s(text, _T("最终得分：%d"), score);
					mciSendString(_T("play hit2 from 0"), NULL, 0, NULL);
					MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);
					break;
				}

			for (size_t i = 0; i < enemy_list.size(); i++) {
				Enemy* enemy = enemy_list[i];
				if (!enemy->checkAlive()) {
					std::swap(enemy_list[i], enemy_list.back());
					enemy_list.pop_back();
					delete enemy;
				}
			}
		}
		
		cleardevice();
		if (is_game_started) {
			putimage(0, 0, &background_image);
			player.draw(1000 / 60);
			for (Bullet bullet : bullet_list) bullet.draw();
			for (Enemy* enemy : enemy_list) enemy->draw(1000 / 60);
			drawPlayerScore(score);
		}
		else {
			putimage(0, 0, &menu_image);
			btn_start.draw();
			btn_quit.draw();
		}
		
		FlushBatchDraw();



		DWORD end_time = GetTickCount();
		DWORD delay_time = end_time - start_time;

		if (delay_time < 1000 / 60)
			Sleep(1000 / 60 - delay_time);
	}
	EndBatchDraw();
	delete atlas_player_left;
	delete atlas_player_right;
	delete atlas_enemy_left;
	delete atlas_enemy_right;

	return 0;
}