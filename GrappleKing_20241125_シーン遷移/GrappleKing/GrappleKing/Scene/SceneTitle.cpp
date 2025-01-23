#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneController.h"
#include "SceneMain.h"
#include <cassert>
#include "../Application.h"
#include "../Pad.h"
#include "../game.h"

namespace
{
	// �X�l�[�N�P�[�X
	constexpr int fade_interval = 240;

	//�����̓_��
	constexpr int kBlinkCycleFrame = 60;
	constexpr int kBlinkDispFrame = 40;

	//�����̕\���ʒu
	constexpr int kTitleY = 200;
	constexpr int kButtonTextY = 280;

	//�^�C�g���̕\�����x
	constexpr int kTitleSpeed = 4;
}

SceneTitle::SceneTitle(SceneController& cont) :
	Scene(cont),
	update_(&SceneTitle::FadeInUpdate),
	draw_(&SceneTitle::FadeDraw),
	frame_(fade_interval),
	m_blinkFrameCount(0),
	isSpaceKeyPressed(false),
	m_titleY(Game::kScreenHeight + 100)	
{
	m_titleHandle = LoadGraph("data/image/GameTitle.png");
	assert(m_titleHandle != -1);
	m_backHandle = LoadGraph("data/image/blue_bg.png");
	assert(m_backHandle != -1);
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_backHandle);
	DeleteGraph(m_titleHandle);
}

void SceneTitle::Update()
{
	//1�b�T�C�N���ŕ\���A��\����؂�ւ���
	m_blinkFrameCount++;
	if (m_blinkFrameCount >= kBlinkCycleFrame)
	{
		m_blinkFrameCount = 0;
	}

	(this->*update_) ();
}

void SceneTitle::Draw()
{
	// ��ʂ̒��S�o���Ă邾��
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f, static_cast<float>(wsize.h) * 0.5f };
	
	//�w�i
	DrawGraph(0, 0, m_backHandle, false);

	//�^�C�g���̕\��
	//int width = static_cast<int>(GetDrawStringWidth("Grapple King",
	//	static_cast<int>(strlen("Grapple King"))));
	
	GetGraphSize(m_titleHandle, &graphSizeX, &graphSizeY);

	if (m_titleY > kTitleY)
	{
		m_titleY -= kTitleSpeed;
	}
	/*DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width * 3.0), m_titleY,
		6, 4, 0, 0, 0, 0xffffff, 0x0000ff, false, "Grapple King", 0);*/

	DrawGraph(static_cast<int>(Game::kScreenWidth * 0.5 - graphSizeX * 0.5), m_titleY, m_titleHandle, true);

	// ���s
	// ����͉�ʌ��ʂƂ��̕`��Ɏg�����炢������
	(this->*draw_) ();
}

void SceneTitle::FadeInUpdate()
{
	--frame_;
	
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
		controller_.ChangeScene(std::make_shared<SceneMain>(controller_));
		return;
	}
}

void SceneTitle::NormalUpdate()
{
	// �X�y�[�X�L�[�̏�Ԃ��擾
	bool isSpaceKeyCurrentlyPressed = (CheckHitKey(KEY_INPUT_SPACE) != 0);

	// �G���^�[�L�[���������܂ŉ������Ȃ�
		if (isSpaceKeyCurrentlyPressed && !isSpaceKeyPressed)
		{
			update_ = &SceneTitle::FadeOutUpdate;
			draw_ = &SceneTitle::FadeDraw;
		}

		// �X�y�[�X�L�[�̏�Ԃ��X�V
		isSpaceKeyPressed = isSpaceKeyCurrentlyPressed;
}

void SceneTitle::FadeDraw()
{
	// ��ʂ̒��S�o���Ă邾��
	const Size& wsize = Application::GetInstance().GetWindowSize();

	// ���Ȃ��ݓ��ߕ`��𒴑��Ŏ���
	// �܂��t�F�[�h�����̌v�Z
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	// ���̏������Ȃ�����̃p�����[�^��0~1�ɂł���炵��
	SetDrawBlendMode(static_cast<int>(DX_BLENDMODE_MULA), static_cast<int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	// BlendMode���g�p�������Noblend��Y��Ȃ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::NormalDraw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Title Scene", 0xffffff);
#endif

	//�X�y�[�X�L�[�������Ă�������
	if (m_blinkFrameCount < kBlinkDispFrame)
	{
		int width = GetDrawStringWidth("PRESS SPACE KEY",
			static_cast<int>(strlen("PRESS SPACE KEY")));

		DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width),
			static_cast<int>(Game::kScreenHeight * 0.5 + 150), 2, 2,
			0, 0, 0, 0x000000, 0xffffff, false, "PRESS SPACE KEY", 0);
	}
}