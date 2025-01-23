#include "SceneResult.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "../Pad.h"
#include "../Game.h"
#include <cassert>

namespace
{
	//�Q�[���I�[�o�[�̕������\�����ꂫ��܂ł̃t���[����
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
	//�Q�[���I�[�o�[�ɂȂ�����P�{�^������������t�F�[�h�A�E�g
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
		//�t�F�[�h�C������
		m_fadeFrameCount++;
		if (m_fadeFrameCount > 30)
		{
			m_fadeFrameCount = 30;
		}
	}

	//�Q�[���I�[�o�[���o
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

	//�w�i
	DrawGraph(0, 0, m_backHandle, false);

	//�������g�p���ĕϊ����s��
		//m_gameoverFrameCount��M�����ɕϊ�����
	float progressRate = static_cast<float>(m_gameoverFrameCount) / kGameoverFadeFrame;

	//���������ۂ̓����x�ɕϊ�����
	int alpha = static_cast<int>(255 * progressRate);

	//�����ȍ~�Ă΂��Draw�֐��̕`����@��ύX����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	int width = GetDrawStringWidth("GAME CLEAR", static_cast<int>(strlen("GAME CLEAR")), false);
	DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width * 2.5),
		static_cast<int>(Game::kScreenHeight * 0.5 - 100), 5, 3,
		0, 0, 0, 0xffffff, 0x000000, false, "GAME CLEAR", 0);

	if (m_gameoverFrameCount >= kGameoverFadeFrame)
	{
		width = GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g���ɖ߂�",
			static_cast<int>(strlen("�X�y�[�X�L�[�������ă^�C�g���ɖ߂�")));
		DrawString(static_cast<int>(Game::kScreenWidth * 0.5 - width * 0.5),
			static_cast<int>(Game::kScreenHeight * 0.5 + 150),
			"�X�y�[�X�L�[�������ă^�C�g���ɖ߂�", 0x000000);
	}

	//�ȍ~�̕\�������������Ȃ�Ȃ��悤�Ɍ��̐ݒ�ɖ߂��Ă���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�t�F�[�h����
	int fadeAlpha = 0;

	// m_fadeFrameCount = 0 �̎�fadeAlpha = 255 �^����
	// m_fadeFrameCount = 30�̎�fadeAlpha = 0   ���S�ɉ�ʂ�������

	//�����𗘗p����m_fadeFrameCount��fadeAlpha�ɕϊ�����

	float fadeRate = static_cast<float>(m_fadeFrameCount) / 30;
	fadeRate = 1.0f - fadeRate;
	fadeAlpha = static_cast<int>(255 * fadeRate);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}