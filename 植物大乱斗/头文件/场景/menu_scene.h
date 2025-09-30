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
		std::cout << "�������˵�" << std::endl;
	}

	void onInput(ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
			scene_manager.switchTo(SceneManager::SceneType::Selector);
	}

	void onUpdate()
	{
		std::cout << "���˵���������" << std::endl;
	}

	void onDraw()
	{
		outtextxy(0, 0, _T("���˵����ڻ���"));
	}

		void onExit()
	{
		std::cout << "�˳����˵�" << std::endl;
	}
private:

};