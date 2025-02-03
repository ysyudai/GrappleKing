#pragma once
#include "SceneController.h"
#include "Scene.h"

class SceneClearStage1:public Scene
{
public:
	SceneClearStage1(SceneController& cont);
	~SceneClearStage1();

	void Update();
	void Draw();
};

