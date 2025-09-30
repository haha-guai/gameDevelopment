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
		std::cout << "������Ϸ����" << std::endl;
	}

	void onInput(ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
			scene_manager.switchTo(SceneManager::SceneType::Menu);
	}

	void onUpdate()
	{
		std::cout << "��Ϸ��������" << std::endl;
	}

	void onDraw()
	{
		outtextxy(0, 0, _T("��Ϸ�������ڻ���"));
	}

	void onExit()
	{
		std::cout << "�˳���Ϸ" << std::endl;
	}
private:

};