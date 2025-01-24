#pragma once
#include "Scene.h"
#include "SceneController.h"

/// <summary>
/// ステージセレクトシーン
/// </summary>
class SceneResult : public Scene
{
public:
	SceneResult(SceneController& cont);
	~SceneResult();

	void Update()override;
	void Draw()override;

private:
	//背景
	int m_backHandle;
	int m_clearHandle;

	int graphSizeX;
	int graphSizeY;

	//ゲームオーバー演出に使用するフレーム
	int m_gameoverFrameCount;
	//ゲームオーバーになった後、1ボタンを押した
	bool m_isGameEnd;
	//フェード処理
	int m_fadeFrameCount;

};