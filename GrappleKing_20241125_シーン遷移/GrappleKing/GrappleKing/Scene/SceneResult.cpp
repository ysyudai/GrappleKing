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
	//�Q�[���N���A�̕������\�����ꂫ��܂ł̃t���[����
	constexpr int fade_interval = 90;

	//�N���A�����̕\���ʒu
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
	// ��ʂ̒��S�o���Ă邾��
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f, static_cast<float>(wsize.h) * 0.5f };

	//�w�i
	DrawGraph(0, 0, m_backHandle, false);

	//�^�C�g���̕\��
	//int width = static_cast<int>(GetDrawStringWidth("Grapple King",
	//	static_cast<int>(strlen("Grapple King"))));

	GetGraphSize(m_clearHandle, &graphSizeX, &graphSizeY);

	/*DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width * 3.0), m_titleY,
		6, 4, 0, 0, 0, 0xffffff, 0x0000ff, false, "Grapple King", 0);*/

	DrawGraph(static_cast<int>(Game::kScreenWidth * 0.5 - graphSizeX * 0.5), kClearY, m_clearHandle, true);

	// ���s
	// ����͉�ʌ��ʂƂ��̕`��Ɏg�����炢������
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
	//�X�y�[�X�L�[�������Ǝ���
	//bool isSpaceKeyCurrentlyPressed = (CheckHitKey(KEY_INPUT_SPACE)!=0);

	// �G���^�[�L�[���������܂ŉ������Ȃ�
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		update_ = &SceneResult::FadeOutUpdate;
		draw_ = &SceneResult::FadeDraw;
	}
}

void SceneResult::FadeDraw()
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

void SceneResult::NormalDraw()
{
#ifdef _DEBUG
	DrawString(10, 10, "Clear Scene", 0xffffff);
#endif

	int width = GetDrawStringWidth("�X�y�[�X�L�[�������ă^�C�g���ɖ߂�",
		static_cast<int>(strlen("�X�y�[�X�L�[�������ă^�C�g���ɖ߂�")));

	DrawRotaFormatString(static_cast<int>(Game::kScreenWidth * 0.5 - width),
		static_cast<int>(Game::kScreenHeight * 0.5 + 150), 2, 2,
		0, 0, 0, 0x000000, 0xffffff, false, "�X�y�[�X�L�[�������ă^�C�g���ɖ߂�", 0);
}