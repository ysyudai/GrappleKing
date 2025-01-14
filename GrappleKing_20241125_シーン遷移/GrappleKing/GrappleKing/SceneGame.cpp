#include "SceneGame.h"
#include "Player.h"
#include "DxLib.h"
#include "Pad.h"
#include "SceneSelectStage.h"
#include "Map.h"

SceneGame::SceneGame(SceneController& cont):Scene(cont)
{
	m_pPlayer = new Player();
	m_Map = new Map(m_pPlayer);
}

SceneGame::~SceneGame()
{
}

void SceneGame::Update()
{
	m_Map->Update();
	m_pPlayer->Update(*m_Map);
	
	if (Pad::IsTrigger(PAD_INPUT_1)) {
	//if (m_pPlayer->IsClearStage1) {
		controller_.ChangeScene(std::make_shared<SceneSelectStage>(controller_));
		
		return;
	}
}

void SceneGame::Draw()
{
	DrawString(10, 10, "Game Scene", 0xffffff);

	m_Map->Draw();
	m_pPlayer->Draw();
}
