#pragma once
#include <iostream>
#include "scene.h"
#include "scene_manager.h"

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void onEnter()
	{
		std::cout << "进入游戏界面" << std::endl;
	}

	void onInput(ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
			scene_manager.switchTo(SceneManager::SceneType::Menu);
	}

	void onUpdate()
	{
		std::cout << "游戏正在运行" << std::endl;
	}

	void onDraw()
	{
		outtextxy(0, 0, _T("游戏内容正在绘制"));
	}

	void onExit()
	{
		std::cout << "退出游戏" << std::endl;
	}
private:

};