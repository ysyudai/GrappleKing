#include "SceneController.h"
#include <memory>
#include "SceneTitle.h"
#include "SceneMain.h"

SceneController::SceneController()
{
	// ��ԍŏ��̃V�[�������͊��蓖�Ă�
	// �������g�̃C���X�^���X��n���Ă�����
	//ChangeScene(std::make_shared<SceneTitle>(*this));
	ChangeScene(std::make_shared<SceneTitle>(*this));
}

void SceneController::Update()
{
	// ��ԏ�ɂ�������Update
	scenes_.back()->Update();
}

void SceneController::Draw()
{
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<Scene> scene)
{
	// �X�}�|�Ȃ̂ł���ŃI�b�P�[
	// ��ԏ��Ȃ�
	if (scenes_.empty())
	{
		scenes_.push_back(scene);
	}
	// ��q�������ꍇ�A�ǂ��Ă��炤
	else
	{
		// �����̗v�f�̎Q�Ƃ��̂��̂�
		scenes_.back() = scene;
	}
	// ���O�̃I�u�W�F�N�g�͎Q�Ƃ������̂ŏ��������
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	scenes_.push_back(scene);
}

void SceneController::PopScene()
{
	// ���s���ׂ��V�[�����Ȃ��Ȃ�̂ŋ����Ȃ�
	if (scenes_.size() == 1) return;

	// ��������菜��
	scenes_.pop_back();
}
