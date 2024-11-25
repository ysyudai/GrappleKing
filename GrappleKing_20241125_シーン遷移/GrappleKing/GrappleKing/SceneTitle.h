#pragma once
#include "Scene.h"

/// <summary>
/// タイトルシーン
/// </summary>
class SceneTitle : public Scene
{
private:
	int frame_ = 0;
	// 背景
	int backH_ = -1;

	using UpdateFunc_t = void (SceneTitle::*) (Input&);
	using DrawFunc_t = void (SceneTitle::*) ();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	// フェードイン時のUpdate
	void FadeInUpdate(Input&);
	// とか
	void FadeOutUpdate(Input&);
	// そうでなかったり
	void NormalUpdate(Input&);

	// 描画
	void FadeDraw();
	void NormalDraw();



public:
	SceneTitle(SceneController& cont);
	virtual ~SceneTitle()override;
	/// <summary>
	/// 派生クラスで実装を実装
	/// virtual はこれを継承するかもしれないから、
	/// overrideは明示的にオーバーライドをエディタに示すため
	/// </summary>
	virtual void Update(Input& input) override;

	virtual void Draw() override;
};

