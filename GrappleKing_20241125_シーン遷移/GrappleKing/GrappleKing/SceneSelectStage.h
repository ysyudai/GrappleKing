#pragma once
#include "Scene.h"
#include "SceneController.h"

/// <summary>
/// �X�e�[�W�Z���N�g�V�[��
/// </summary>
class SceneSelectStage : public Scene
{
public:
	SceneSelectStage(SceneController& cont);
	~SceneSelectStage();

	void Update();
	void Draw();
};

