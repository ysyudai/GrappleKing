#pragma once
#include "Scene.h"
#include "SceneController.h"

/// <summary>
/// タイトルシーン
/// </summary>
class SceneTitle : public Scene
{
public:
	SceneTitle(SceneController& cont);
	virtual ~SceneTitle()override;
	/// <summary>
	/// 派生クラスで実装を実装
	/// virtual はこれを継承するかもしれないから、
	/// overrideは明示的にオーバーライドをエディタに示すため
	/// </summary>
	virtual void Update() override;

	virtual void Draw() override;

private:
	int frame_ = 0;
	// 背景
	int m_backHandle = -1;
	int m_titleHandle = -1;

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

	
};

