#include "SceneMain.h"
#include "../Player.h"
#include "DxLib.h"
#include "../Pad.h"
#include "SceneResult.h"
#include "../Map.h"

SceneMain::SceneMain(SceneController& cont):Scene(cont)
{
	m_pPlayer = new Player();
	m_Map = new Map(m_pPlayer);
}

SceneMain::~SceneMain()
{
}

void SceneMain::Update()
{
	m_Map->Update();
	m_pPlayer->Update(*m_Map);
	
	//if (Pad::IsTrigger(PAD_INPUT_1)) {
	if (m_pPlayer->IsClear() || Pad::IsTrigger(PAD_INPUT_1)) {
		controller_.ChangeScene(std::make_shared<SceneResult>(controller_));
		
		return;
	}
}

void SceneMain::Draw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Game Scene", 0xffffff);
#endif

	m_Map->Draw();
	m_pPlayer->Draw();
}
