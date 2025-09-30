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
		std::cout << "����ѡ��ҳ��" << std::endl;
	}

	void onInput(ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
			scene_manager.switchTo(SceneManager::SceneType::Game);
	}

	void onUpdate()
	{
		std::cout << "����ѡ��" << std::endl;
	}

	void onDraw()
	{
		outtextxy(0, 0, _T("ѡ��ҳ��"));
	}

	void onExit()
	{
		std::cout << "�˳�ѡ��ҳ��" << std::endl;
	}

private:

};