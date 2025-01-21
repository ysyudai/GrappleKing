#pragma once

#include "SceneController.h"
#include "Scene.h"
class Player;
class Map;
class SceneGame:public Scene
{
public:
	SceneGame(SceneController& cont);
	~SceneGame();

	void Update() override;
	void Draw() override;

private:

	Player* m_pPlayer;
	Map* m_Map;
};