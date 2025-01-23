#include "SceneResult.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "../Game.h"
#include <cassert>

namespace
{
	//ゲームオーバーの文字が表示されきるまでのフレーム数
	constexpr int kGameoverFadeFrame = 90;
}

SceneResult::SceneResult(SceneController& cont) :
	Scene(cont),
	m_backHandle(-1),
	m_gameoverFrameCount(0),
	m_isGameEnd(false),
	m_fadeFrameCount(0)
{
	m_backHandle = LoadGraph("data/image/blue_bg.png");
	assert(m_backHandle != -1);
}

SceneResult::~SceneResult()
{
}

void SceneResult::Update()
{
	//ゲームオーバーになった後１ボタンを押したらフェードアウト
	m_fadeFrameCount--;
	if (m_fadeFrameCount < 0)
	{
		m_fadeFrameCount = 0;

		if (Pad::IsTrigger(PAD_INPUT_1)) {
			controller_.ChangeScene(std::make_shared<SceneTitle>(controller_));

			return;
		}
	}

	else
	{
		//フェードイン処理
		m_fadeFrameCount++;
		if (m_fadeFrameCount > 30)
		{
			m_fadeFrameCount = 30;
		}
	}

	//ゲームオーバー演出
	m_gameoverFrameCount++;
	if (m_gameoverFrameCount >= kGameoverFadeFrame)
	{
		m_gameoverFrameCount = kGameoverFadeFrame;
	}
}

void SceneResult::Draw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Clear Scene", 0xffffff);
#endif

	//背景
	DrawGraph(0, 0, m_backHandle, false);

	//割合を使用して変換を行う
		//m_gameoverFrameCountを信仰割合に変換する
	float progressRate = static_cast<float>(m_gameoverFrameCount) / kGameoverFadeFrame;

	//割合を実際の透明度に変換する
	int alpha = static_cast<int>(255 * progressRate);

	//ここ以降呼ばれるDraw関数の描画方法を変更する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	int width = GetDrawStringWidth("GAME CLEAR", static_cast<int>(strlen("GAME CLEAR")), false);
	DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width * 2.5),
		static_cast<int>(Game::kScreenHeight * 0.5 - 100), 5, 3,
		0, 0, 0, 0xffffff, 0x000000, false, "GAME CLEAR", 0);

	if (m_gameoverFrameCount >= kGameoverFadeFrame)
	{
		width = GetDrawStringWidth("スペースキーを押してタイトルに戻る",
			static_cast<int>(strlen("スペースキーを押してタイトルに戻る")));
		DrawString(static_cast<int>(Game::kScreenWidth * 0.5 - width * 0.5),
			static_cast<int>(Game::kScreenHeight * 0.5 + 150),
			"スペースキーを押してタイトルに戻る", 0x000000);
	}

	//以降の表示がおかしくならないように元の設定に戻しておく
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//フェード処理
	int fadeAlpha = 0;

	// m_fadeFrameCount = 0 の時fadeAlpha = 255 真っ黒
	// m_fadeFrameCount = 30の時fadeAlpha = 0   完全に画面が見える

	//割合を利用してm_fadeFrameCountをfadeAlphaに変換する

	float fadeRate = static_cast<float>(m_fadeFrameCount) / 30;
	fadeRate = 1.0f - fadeRate;
	fadeAlpha = static_cast<int>(255 * fadeRate);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}