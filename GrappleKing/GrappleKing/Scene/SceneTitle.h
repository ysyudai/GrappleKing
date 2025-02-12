#pragma once
#include "Scene.h"
#include "SceneController.h"

class TitlePlayer;
/// <summary>
/// タイトルシーン
/// </summary>
class SceneTitle : public Scene
{
public:
	SceneTitle(SceneController& cont);
	virtual ~SceneTitle()override;

	virtual void Update() override;
	virtual void Draw() override;

private:
	int frame_ = 0;
	// 背景
	int m_backHandle;
	int m_titleHandle;
	int m_inductionHandle;

	int m_musicHandle;

	int graphSizeX;
	int graphSizeY;

	using UpdateFunc_t = void (SceneTitle::*) ();
	using DrawFunc_t = void (SceneTitle::*) ();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	// フェードイン時のUpdate
	void FadeInUpdate();
	// とか
	void FadeOutUpdate();
	// そうでなかったり
	void NormalUpdate();

	// 描画
	void FadeDraw();
	void NormalDraw();

	//"ボタンを押して下さい"を点滅させるためのフレームカウント
	int m_blinkFrameCount;

	//スペースキーの状態を管理する変数
	bool isSpaceKeyPressed; 

	//タイトルのY座標
	int m_titleY;

	TitlePlayer* m_pTitlePlayer[40];
};