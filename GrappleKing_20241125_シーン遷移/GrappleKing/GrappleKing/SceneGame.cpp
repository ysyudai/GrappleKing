#include "SceneGame.h"
#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include "SceneSelectStage.h"
#include "Bg.h"

SceneGame::SceneGame(SceneController& cont):Scene(cont)
{
	m_pPlayer = new Player();
	m_bg = new Bg(m_pPlayer);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Update()
{
	m_bg->Update();
	m_pPlayer->Update(*m_bg);

	if (Pad::IsTrigger(PAD_INPUT_1)) {
		controller_.ChangeScene(std::make_shared<SceneSelectStage>(controller_));
		
		return;
	}
}

void SceneGame::Draw()
{
	DrawString(10, 10, "Game Scene", 0xffffff);

	m_bg->Draw();
	m_pPlayer->Draw();
}
