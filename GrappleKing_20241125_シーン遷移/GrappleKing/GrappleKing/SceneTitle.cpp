#include "SceneTitle.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "SceneGame.h"
#include <cassert>
#include "Application.h"

namespace
{
	// �X�l�[�N�P�[�X�Ƃ́c
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
	// �G���^�[�L�[���������܂ŉ������Ȃ�
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

	// ��ʂ̒��S�o���Ă邾��
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f, static_cast<float>(wsize.h) * 0.5f };
	// �w�i�̕\��
	DrawRotaGraph(static_cast<int>(center.x), static_cast<int>(center.y), 1.0, 0.0f, backH_, true);
	// ���s
	// ����͉�ʌ��ʂƂ��̕`��Ɏg�����炢������
	(this->*draw_) ();
}