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
	// �X�l�[�N�P�[�X
	constexpr int fade_interval = 60;

	//�����̓_��
	constexpr int kBlinkCycleFrame = 60;
	constexpr int kBlinkDispFrame = 40;

	//�����̕\���ʒu
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
	//1�b�T�C�N���ŕ\���A��\����؂�ւ���
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
	// ��ʂ̒��S�o���Ă邾��
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f, static_cast<float>(wsize.h) * 0.5f };
	// �w�i�̕\��
	//DrawRotaGraph(static_cast<int>(center.x), static_cast<int>(center.y), 3.0, 0.0f, backH_, true);
	
	//�^�C�g��
	int width = GetDrawStringWidth("Grapple King", strlen("Grapple King"));

	DrawRotaFormatString(Game::kScreenWidth * 0.5 - width * 2.5, Game::kScreenHeight * 0.5 - 100, 5, 3,
		0, 0, 0, 0xffffff, 0x0000ff, false, "Grapple King", 0);

	// ���s
	// ����͉�ʌ��ʂƂ��̕`��Ɏg�����炢������
	(this->*draw_) ();
}

void SceneTitle::FadeInUpdate()
{
	--frame_;
	// ���ꂪ�g���K�[��
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
	// �G���^�[�L�[���������܂ŉ������Ȃ�
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			update_ = &SceneTitle::FadeOutUpdate;
			draw_ = &SceneTitle::FadeDraw;
		}
}

void SceneTitle::FadeDraw()
{
	// ��ʂ̒��S�o���Ă邾��
	const Size& wsize = Application::GetInstance().GetWindowSize();

	// ���Ȃ��ݓ��ߕ`��𒴑��Ŏ���
	// �܂��t�F�[�h�����̌v�Z
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	// ���̏������Ȃ�����̃p�����[�^��0~1�ɂł���炵��
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * rate);
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	// BlendMode���g�p�������Noblend��Y��Ȃ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::NormalDraw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif

	//1�{�^���������Ă�������
	if (m_blinkFrameCount < kBlinkDispFrame)
	{
		int width = GetDrawStringWidth("1�{�^���������Ă�������", strlen("1�{�^���������Ă�������"));
		DrawString(Game::kScreenWidth * 0.5 - width * 0.5, Game::kScreenHeight * 0.5 + 150, "1�{�^���������Ă�������", 0xffffff);
	}

	//int width = GetDrawStringWidth("key", strlen("key"));
	//DrawString(Game::kScreenWidth * 0.5 +200, y, "key", 0xffffff);
}