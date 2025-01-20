#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneController.h"
#include "SceneGame.h"
#include <cassert>
#include "Application.h"
#include "Pad.h"
#include "game.h"

namespace
{
	// スネークケース
	constexpr int fade_interval = 60;

	//文字の点滅
	constexpr int kBlinkCycleFrame = 60;
	constexpr int kBlinkDispFrame = 40;

	//文字の表示位置
	constexpr int kTitleY = 200;
	constexpr int kButtonTextY = 280;
}

SceneTitle::SceneTitle(SceneController& cont) :
	Scene(cont),
	update_(&SceneTitle::FadeInUpdate),
	draw_(&SceneTitle::FadeDraw),
	frame_(fade_interval),
	m_blinkFrameCount(0),
	y(Game::kScreenHeight + 50)
{
	backH_ = LoadGraph("data/Flog/idle.png");
	assert(backH_ != -1);
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(backH_);
}

void SceneTitle::Update()
{
	//1秒サイクルで表示、非表示を切り替える
	m_blinkFrameCount++;
	if (m_blinkFrameCount >= kBlinkCycleFrame)
	{
		m_blinkFrameCount = 0;
	}

	y--;
	if (y <= Game::kScreenHeight * 0.5)
	{
		y = Game::kScreenHeight * 0.5;
	}

	(this->*update_) ();
}

void SceneTitle::Draw()
{
	// 画面の中心出してるだけ
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f, static_cast<float>(wsize.h) * 0.5f };
	// 背景の表示
	//DrawRotaGraph(static_cast<int>(center.x), static_cast<int>(center.y), 3.0, 0.0f, backH_, true);
	
	//タイトル
	int width = GetDrawStringWidth("Grapple King", strlen("Grapple King"));

	DrawRotaFormatString(Game::kScreenWidth * 0.5 - width * 2.5, Game::kScreenHeight * 0.5 - 100, 5, 3,
		0, 0, 0, 0xffffff, 0x0000ff, false, "Grapple King", 0);

	// 実行
	// これは画面効果とかの描画に使ったらいいかも
	(this->*draw_) ();
}

void SceneTitle::FadeInUpdate()
{
	--frame_;
	// これがトリガーか
	if (frame_ <= 0)
	{
		update_ = &SceneTitle::NormalUpdate;
		draw_ = &SceneTitle::NormalDraw;
	}
}

void SceneTitle::FadeOutUpdate()
{
	++frame_;

	if (frame_ >= 60)
	{
		controller_.ChangeScene(std::make_shared<SceneGame>(controller_));
		return;
	}
}

void SceneTitle::NormalUpdate()
{
	// エンターキーが推されるまで何もしない
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			update_ = &SceneTitle::FadeOutUpdate;
			draw_ = &SceneTitle::FadeDraw;
		}
}

void SceneTitle::FadeDraw()
{
	// 画面の中心出してるだけ
	const Size& wsize = Application::GetInstance().GetWindowSize();

	// おなじみ透過描画を超速で実装
	// まずフェード割合の計算
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	// この書き方なら引数のパラメータを0~1にできるらしい
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * rate);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	// BlendModeを使用した後はNoblendを忘れない
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::NormalDraw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif

	//1ボタンを押してください
	if (m_blinkFrameCount < kBlinkDispFrame)
	{
		int width = GetDrawStringWidth("1ボタンを押してください", strlen("1ボタンを押してください"));
		DrawString(Game::kScreenWidth * 0.5 - width * 0.5, Game::kScreenHeight * 0.5 + 150, "1ボタンを押してください", 0xffffff);
	}

	//int width = GetDrawStringWidth("key", strlen("key"));
	//DrawString(Game::kScreenWidth * 0.5 +200, y, "key", 0xffffff);
}