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

	int m_musicHandle;

	Player* m_pPlayer;
	Map* m_pMap;
};