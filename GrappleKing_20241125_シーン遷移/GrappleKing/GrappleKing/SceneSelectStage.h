#pragma once
#include "Scene.h"
#include "SceneController.h"

/// <summary>
/// ステージセレクトシーン
/// </summary>
class SceneSelectStage : public Scene
{
public:
	SceneSelectStage(SceneController& cont);
	~SceneSelectStage();

	void Update();
	void Draw();
};

