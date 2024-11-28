#include "SceneGame.h"
#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include "SceneTitle.h"
#include "Bg.h"

SceneGame::SceneGame(SceneController& cont):Scene(cont)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Update()
{
	m_pPlayer->Update();

	if (Pad::IsTrigger(PAD_INPUT_1)) {
		controller_.ChangeScene(std::make_shared<SceneTitle>(controller_));
		
		return;
	}
}

void SceneGame::Draw()
{
	DrawString(10, 10, "Game Scene", 0xffffff);

	//m_bg.Draw();
	m_pPlayer->Draw();
}
