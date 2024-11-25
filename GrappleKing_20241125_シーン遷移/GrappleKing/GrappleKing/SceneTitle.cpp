#include "SceneTitle.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "SceneGame.h"
#include <cassert>
#include "Application.h"

namespace
{
	// スネークケースとは…
	constexpr int fade_interval = 60;
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::FadeInUpdate(Input&)
{
	update_ = &SceneTitle::NormalUpdate;
	draw_ = &SceneTitle::NormalDraw;
}

void SceneTitle::FadeOutUpdate(Input&)
{
	controller_.ChangeScene(std::make_shared<SceneGame>(controller_));
}

void SceneTitle::NormalUpdate(Input& input)
{
	// エンターキーが推されるまで何もしない
		if (input.IsTrigger("next"))
		{
			update_ = &SceneTitle::FadeOutUpdate;
			draw_ = &SceneTitle::FadeDraw;
		}
}

void SceneTitle::FadeDraw()
{
}

void SceneTitle::NormalDraw()
{
	DrawString(10, 10, "Title Scene", 0xffffff);
}

SceneTitle::SceneTitle(SceneController& cont) :
	Scene(cont),
	update_(&SceneTitle::FadeInUpdate),
	draw_(&SceneTitle::FadeDraw),
	frame_(fade_interval)
{

}

void SceneTitle::Update(Input& input)
{
	(this->*update_) (input);
}

void SceneTitle::Draw()
{

	// 画面の中心出してるだけ
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f, static_cast<float>(wsize.h) * 0.5f };
	// 背景の表示
	DrawRotaGraph(static_cast<int>(center.x), static_cast<int>(center.y), 1.0, 0.0f, backH_, true);
	// 実行
	// これは画面効果とかの描画に使ったらいいかも
	(this->*draw_) ();
}