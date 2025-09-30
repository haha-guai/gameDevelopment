#pragma once
#include <graphics.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void onEnter() {}
	virtual void onInput(ExMessage& msg) {}
	virtual void onUpdate() {}
	virtual void onDraw() {}
	virtual void onExit() {}

private:

};