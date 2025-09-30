#pragma once
#include "scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;


class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Selector,

	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	void setCurrentScene(Scene* scene)
	{
		current_scene = scene;
		current_scene->onEnter();
	}

	void switchTo(SceneType type)
	{
		current_scene->onExit();
		switch (type)
		{
		case SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneType::Game:
			current_scene = game_scene;
			break;
		case SceneType::Selector:
			current_scene = selector_scene;
			break;
		default:
			break;
		}
		current_scene->onEnter();
	}

	void onInput(ExMessage& msg)
	{
		current_scene->onInput(msg);
	}

	void onUpdate()
	{
		current_scene->onUpdate();
	}

	void onDraw()
	{
		current_scene->onDraw();
	}


private:
	Scene* current_scene = nullptr;
};