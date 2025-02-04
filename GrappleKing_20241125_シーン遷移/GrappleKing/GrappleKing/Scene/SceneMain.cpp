#include "SceneMain.h"
#include "../Player.h"
#include "DxLib.h"
#include "../Pad.h"
#include "SceneResult.h"
#include "../Map.h"
#include <cassert>

SceneMain::SceneMain(SceneController& cont):Scene(cont),
	m_musicHandle(-1)
{
	m_pPlayer = new Player();
	m_pMap = new Map(m_pPlayer);

	m_musicHandle = LoadSoundMem("data/music/mainMusic.mp3");
	assert(m_musicHandle != -1);

	PlaySoundMem(m_musicHandle, DX_PLAYTYPE_LOOP);
}

SceneMain::~SceneMain()
{
	StopSoundMem(m_musicHandle);
	DeleteSoundMem(m_musicHandle);
}

void SceneMain::Update()
{
	ChangeVolumeSoundMem(128, m_musicHandle);

	m_pMap->Update();
	m_pPlayer->Update(*m_pMap);
	
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

	m_pMap->Draw();
	m_pPlayer->Draw();
}
