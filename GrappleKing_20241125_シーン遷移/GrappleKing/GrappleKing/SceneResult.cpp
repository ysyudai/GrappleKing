#include "SceneResult.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "Pad.h"
#include "Game.h"

namespace
{
	//�Q�[���I�[�o�[�̕������\�����ꂫ��܂ł̃t���[����
	constexpr int kGameoverFadeFrame = 90;
}

SceneResult::SceneResult(SceneController& cont) :
	Scene(cont),
	m_gameoverFrameCount(0),
	m_isGameEnd(false),
	m_fadeFrameCount(0)
{
}

SceneResult::~SceneResult()
{
}

void SceneResult::Update()
{
	if (m_isGameEnd)
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

		//�Q�[���I�[�o�[�̕������\�����ꂫ������A
		//1�{�^������������^�C�g���ɖ߂�
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//return SceneManager::SceneKind::kSceneTitle;
			m_isGameEnd = true;
		}
	}
}

void SceneResult::Draw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Clear Scene", 0xffffff);
#endif

	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(160, 216, 239), true);

	//�������g�p���ĕϊ����s��
		//m_gameoverFrameCount��M�����ɕϊ�����
	float progressRate = static_cast<float>(m_gameoverFrameCount) / kGameoverFadeFrame;

	//���������ۂ̓����x�ɕϊ�����
	int alpha = static_cast<int>(255 * progressRate);

	//�����ȍ~�Ă΂��Draw�֐��̕`����@��ύX����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	int width = GetDrawStringWidth("GAME CLEAR", strlen("GAME CLEAR"), false);
	DrawRotaFormatString(Game::kScreenWidth * 0.5 - width * 2.5, Game::kScreenHeight * 0.5 - 100, 5, 3,
		0, 0, 0, 0xffffff, 0x000000, false, "GAME CLEAR", 0);

	if (m_gameoverFrameCount >= kGameoverFadeFrame)
	{
		width = GetDrawStringWidth("1�{�^���������ă^�C�g���ɖ߂�", strlen("1�{�^���������ă^�C�g���ɖ߂�"));
		DrawString(Game::kScreenWidth * 0.5 - width * 0.5, Game::kScreenHeight * 0.5 + 150,
			"1�{�^���������ă^�C�g���ɖ߂�", 0x000000);
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
	fadeAlpha = 255 * fadeRate;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}