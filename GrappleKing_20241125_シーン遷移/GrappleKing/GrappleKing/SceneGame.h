#pragma once

#include "SceneController.h"
#include "Player.h"
#include "Bg.h"
#include "Scene.h"

class SceneGame:public Scene
{
public:
	SceneGame(SceneController& cont);
	~SceneGame();

	void Update();
	void Draw();

private:

	Player* m_pPlayer;
	Bg* m_bg;
};