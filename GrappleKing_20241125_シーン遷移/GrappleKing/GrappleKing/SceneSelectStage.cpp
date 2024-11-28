#include "SceneSelectStage.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "Pad.h"

SceneSelectStage::SceneSelectStage(SceneController& cont) :
	Scene(cont)
{
}

SceneSelectStage::~SceneSelectStage()
{
}

void SceneSelectStage::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_1)) {
		controller_.ChangeScene(std::make_shared<SceneTitle>(controller_));

		return;
	}
}

void SceneSelectStage::Draw()
{
	DrawString(10, 10, "SelectStage Scene", 0xffffff);
}