#include <graphics.h>

#include "scene.h"
#include "scene_manager.h"

#include "menu_scene.h"
#include "game_scene.h"
#include "selector_scene.h"



Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

SceneManager scene_manager;


int main()
{
	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();

	scene_manager.setCurrentScene(menu_scene);
	

	ExMessage msg;
	const int FPS = 60;
	const int width = 1280, height = 720;

	initgraph(width, height);

	BeginBatchDraw();


	while (true)
	{
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			scene_manager.onInput(msg);
		}

		scene_manager.onUpdate();

		cleardevice();

		scene_manager.onDraw();

		FlushBatchDraw();

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / FPS)
			Sleep(1000 / FPS - delta_time);

	}
	
	EndBatchDraw();


	return 0;
}