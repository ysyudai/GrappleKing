#pragma once

#include "SceneController.h"
#include "Player.h"
#include "Map.h"
#include "Scene.h"

class SceneGame:public Scene
{
public:
	SceneGame(SceneController& cont);
	~SceneGame();

	void Init();
	void Update();
	void Draw();

private:

	Player* m_pPlayer = new Player();

	/*
	//グラフィックハンドル
	int m_lifeHandle;
	//フォントハンドル
	int m_fontHandle;
	//カウントダウンに使用するフレーム
	int m_countFrame;
	//ゲームオーバー演出に使用するフレーム
	int m_gameoverFrameCount;
	//ゲームオーバーになった後、1ボタンを押した
	bool m_isGameEnd;

	//フェード処理
	int m_fadeFrameCount;

	Player m_player;
	*/
};