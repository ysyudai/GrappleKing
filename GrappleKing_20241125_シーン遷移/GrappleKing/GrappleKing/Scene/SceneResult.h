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
	virtual ~SceneResult()override;

	void Update()override;
	void Draw()override;

private:
	int frame_ = 0;
	//背景
	int m_backHandle;
	int m_clearHandle;

	int graphSizeX;
	int graphSizeY;

	using UpdateFunc_t = void (SceneResult::*) ();
	using DrawFunc_t = void (SceneResult::*) ();

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

	//スペースキーの状態を管理する変数
	bool isSpaceKeyPressed;

};