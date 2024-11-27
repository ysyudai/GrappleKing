#pragma once
#include "Scene.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class SceneTitle : public Scene
{
private:
	int frame_ = 0;
	// �w�i
	int backH_ = -1;

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

public:
	SceneTitle(SceneController& cont);
	virtual ~SceneTitle()override;
	/// <summary>
	/// �h���N���X�Ŏ���������
	/// virtual �͂�����p�����邩������Ȃ�����A
	/// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
	/// </summary>
	virtual void Update() override;

	virtual void Draw() override;
};

