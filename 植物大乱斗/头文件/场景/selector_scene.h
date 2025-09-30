#pragma once
#include <iostream>
#include "scene.h"
#include "scene_manager.h"

extern SceneManager scene_manager;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void onEnter()
	{
		std::cout << "进入选角页面" << std::endl;
	}

	void onInput(ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
			scene_manager.switchTo(SceneManager::SceneType::Game);
	}

	void onUpdate()
	{
		std::cout << "正在选角" << std::endl;
	}

	void onDraw()
	{
		outtextxy(0, 0, _T("选角页面"));
	}

	void onExit()
	{
		std::cout << "退出选角页面" << std::endl;
	}

private:

};