#pragma once
#include "Scene.h"
#include "SceneController.h"

class TitlePlayer;
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class SceneTitle : public Scene
{
public:
	SceneTitle(SceneController& cont);
	virtual ~SceneTitle()override;

	virtual void Update() override;
	virtual void Draw() override;

private:
	int frame_ = 0;
	// �w�i
	int m_backHandle;
	int m_titleHandle;
	int m_inductionHandle;

	int m_musicHandle;

	int graphSizeX;
	int graphSizeY;

	using UpdateFunc_t = void (SceneTitle::*) ();
	using DrawFunc_t = void (SceneTitle::*) ();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	// �t�F�[�h�C������Update
	void FadeInUpdate();
	// �Ƃ�
	void FadeOutUpdate();
	// �����łȂ�������
	void NormalUpdate();

	// �`��
	void FadeDraw();
	void NormalDraw();

	//"�{�^���������ĉ�����"��_�ł����邽�߂̃t���[���J�E���g
	int m_blinkFrameCount;

	//�X�y�[�X�L�[�̏�Ԃ��Ǘ�����ϐ�
	bool isSpaceKeyPressed; 

	//�^�C�g����Y���W
	int m_titleY;

	TitlePlayer* m_pTitlePlayer[40];
};