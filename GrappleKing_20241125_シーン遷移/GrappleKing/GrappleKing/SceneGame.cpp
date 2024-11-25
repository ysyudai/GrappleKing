#include "SceneGame.h"
#include "Player.h"
#include "DxLib.h"

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Update()
{
	m_pPlayer->Update();
}

void SceneGame::Draw()
{
	DrawString(10, 10, "Game Scene", 0xffffff);

	m_pPlayer->Draw();
}
