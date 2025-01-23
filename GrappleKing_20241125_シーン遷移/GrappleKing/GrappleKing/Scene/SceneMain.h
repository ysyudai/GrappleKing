#pragma once

#include "SceneController.h"
#include "Scene.h"
class Player;
class Map;
class SceneMain:public Scene
{
public:
	SceneMain(SceneController& cont);
	~SceneMain();

	void Update() override;
	void Draw() override;

private:

	Player* m_pPlayer;
	Map* m_Map;
};