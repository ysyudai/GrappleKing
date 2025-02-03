#pragma once
#include "Scene.h"
#include "SceneController.h"

/// <summary>
/// �X�e�[�W�Z���N�g�V�[��
/// </summary>
class SceneResult : public Scene
{
public:
	SceneResult(SceneController& cont);
	virtual ~SceneResult()override;

	void Update()override;
	void Draw()override;

private:
	int frame_ = 0;
	//�w�i
	int m_backHandle;
	int m_clearHandle;

	int graphSizeX;
	int graphSizeY;

	using UpdateFunc_t = void (SceneResult::*) ();
	using DrawFunc_t = void (SceneResult::*) ();

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

	//�X�y�[�X�L�[�̏�Ԃ��Ǘ�����ϐ�
	bool isSpaceKeyPressed;

};