#include "SceneResult.h"
#include "DxLib.h"
#include "SceneController.h"
#include "SceneTitle.h"
#include <cassert>
#include "../Application.h"
#include "../Pad.h"
#include "../game.h"
namespace
{
	//ゲームクリアの文字が表示されきるまでのフレーム数
	constexpr int fade_interval = 90;

	//クリア文字の表示位置
	constexpr int kClearY = 160;
}

SceneResult::SceneResult(SceneController& cont) :
	Scene(cont),
	m_backHandle(-1),
	m_clearHandle(-1),
	graphSizeX(0),
	graphSizeY(0),
	update_(&SceneResult::FadeInUpdate),
	draw_(&SceneResult::FadeDraw),
	frame_(fade_interval),
	isSpaceKeyPressed(false)
{
	m_backHandle = LoadGraph("data/image/blue_bg.png");
	assert(m_backHandle != -1);
	m_clearHandle = LoadGraph("data/image/GameClear.png");
	assert(m_clearHandle != -1);
}

SceneResult::~SceneResult()
{
	DeleteGraph(m_backHandle);
	DeleteGraph(m_clearHandle);
}

void SceneResult::Update()
{	
	(this->*update_) ();
}

void SceneResult::Draw()
{
	// 画面の中心出してるだけ
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f, static_cast<float>(wsize.h) * 0.5f };

	//背景
	DrawGraph(0, 0, m_backHandle, false);

	//タイトルの表示
	//int width = static_cast<int>(GetDrawStringWidth("Grapple King",
	//	static_cast<int>(strlen("Grapple King"))));

	GetGraphSize(m_clearHandle, &graphSizeX, &graphSizeY);

	/*DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width * 3.0), m_titleY,
		6, 4, 0, 0, 0, 0xffffff, 0x0000ff, false, "Grapple King", 0);*/

	DrawGraph(static_cast<int>(Game::kScreenWidth * 0.5 - graphSizeX * 0.5), kClearY, m_clearHandle, true);

	// 実行
	// これは画面効果とかの描画に使ったらいいかも
	(this->*draw_) ();
}

void SceneResult::FadeInUpdate()
{
	--frame_;

	if (frame_ <= 0)
	{
		update_ = &SceneResult::NormalUpdate;
		draw_ = &SceneResult::NormalDraw;
	}
}

void SceneResult::FadeOutUpdate()
{
	frame_ += 2;

	if (frame_ >= fade_interval)
	{
		controller_.ChangeScene(std::make_shared<SceneTitle>(controller_));
		return;
	}
}

void SceneResult::NormalUpdate()
{
	//スペースキーを押すと次へ
	//bool isSpaceKeyCurrentlyPressed = (CheckHitKey(KEY_INPUT_SPACE)!=0);

	// エンターキーが推されるまで何もしない
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		update_ = &SceneResult::FadeOutUpdate;
		draw_ = &SceneResult::FadeDraw;
	}
}

void SceneResult::FadeDraw()
{
	// 画面の中心出してるだけ
	const Size& wsize = Application::GetInstance().GetWindowSize();

	// おなじみ透過描画を超速で実装
	// まずフェード割合の計算
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	// この書き方なら引数のパラメータを0~1にできるらしい
	SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_MULA), static_cast<int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	// BlendModeを使用した後はNoblendを忘れない
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneResult::NormalDraw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Clear Scene", 0xffffff);
#endif

	int width = GetDrawStringWidth("スペースキーを押してタイトルに戻る",
		static_cast<int>(strlen("スペースキーを押してタイトルに戻る")));

	DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width),
		static_cast<int>(Game::kScreenHeight * 0.5 + 150), 2, 2,
		0, 0, 0, 0x000000, 0xffffff, false, "スペースキーを押してタイトルに戻る", 0);
}