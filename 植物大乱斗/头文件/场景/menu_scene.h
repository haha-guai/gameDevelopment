#pragma once
#include <iostream>
#include "scene.h"
#include "scene_manager.h"

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void onEnter()
	{
		std::cout << "进入主菜单" << std::endl;
	}

	void onInput(ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
			scene_manager.switchTo(SceneManager::SceneType::Selector);
	}

	void onUpdate()
	{
		std::cout << "主菜单正在运行" << std::endl;
	}

	void onDraw()
	{
		outtextxy(0, 0, _T("主菜单正在绘制"));
	}

		void onExit()
	{
		std::cout << "退出主菜单" << std::endl;
	}
private:

};