#include "Application.h"
#include "DxLib.h"
#include <cassert>
#include "game.h"
#include <string>
#include "SceneController.h"
#include "Input.h"

Application::Application() :
	windowSize_({ 640, 480 })
{

}

Application& Application::GetInstance()
{
	// ���̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
	static Application app;
	return app;
}

bool Application::Init()
{
	// �t���X�N���[���łȂ��A�E�B���h�E���[�h�ŊJ���悤�ɂ���
	// �����������֐��̓E�B���h�E���J���O(DXLib_Init()�̑O)�ɏ������Ă����K�v������
	ChangeWindowMode(Game::kDefaultWindowMode);

	// ��ʂ̃T�C�Y��ύX����
	// ��O�����͐F�Ɏg������(�r�b�g)
	SetGraphMode(windowSize_.w, windowSize_.h, Game::kScreenBitNum);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return false;			// �G���[���N�����璼���ɏI��
	}

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);
	
	return true;
}

void Application::Run()
{
	SceneController sceneController;
	Input input;
	// �Q�[�����[�v
	constexpr uint64_t frame_milliseconds = 16;
	auto lastTime = 0;
	while (ProcessMessage() == 0) // Windows���s���������󂯎��֐���While�̃��[�v���Ƃɑ��点�Ă���B
		// ����While���̓t���[���̏������s���Ă��邩�疈�t���[�����点�Ă��邱�ƂɂȂ�B
		// ���̖߂�l��While�̔���B�ُ�(���E�B���h�E������ꂽ��)���o���烋�[�v�𔲂��ăv���O�����I���B
	{
		// ��ʑS�̂��N���A����
		ClearDrawScreen();

		input.Update();
		

		// �����ɃQ�[���̏���������

		// ����input���V�[���ԂŎ󂯓n�������Ă��銴����
		sceneController.Update(input);
		sceneController.Draw();

		// ��ʂ̐؂�ւ���҂�
		ScreenFlip();

		// �҂����s��
		auto nowTime = GetTickCount64();
		if (nowTime < frame_milliseconds + lastTime)
		{
			Sleep(frame_milliseconds + lastTime - nowTime);
		}
		lastTime = GetTickCount64();
	}
}

void Application::Terminate()
{
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

const Size& Application::GetWindowSize() const
{
	return windowSize_;
}
