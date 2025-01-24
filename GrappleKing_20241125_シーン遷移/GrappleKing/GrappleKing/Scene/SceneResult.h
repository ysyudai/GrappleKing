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
	~SceneResult();

	void Update()override;
	void Draw()override;

private:
	//�w�i
	int m_backHandle;
	int m_clearHandle;

	int graphSizeX;
	int graphSizeY;

	//�Q�[���I�[�o�[���o�Ɏg�p����t���[��
	int m_gameoverFrameCount;
	//�Q�[���I�[�o�[�ɂȂ�����A1�{�^����������
	bool m_isGameEnd;
	//�t�F�[�h����
	int m_fadeFrameCount;

};