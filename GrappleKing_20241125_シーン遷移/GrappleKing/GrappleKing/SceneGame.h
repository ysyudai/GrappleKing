#pragma once

#include "SceneController.h"
#include "Player.h"
#include "Map.h"
#include "Scene.h"

class SceneGame:public Scene
{
public:
	SceneGame(SceneController& cont);
	~SceneGame();

	void Init();
	void Update();
	void Draw();

private:

	Player* m_pPlayer = new Player();

	/*
	//�O���t�B�b�N�n���h��
	int m_lifeHandle;
	//�t�H���g�n���h��
	int m_fontHandle;
	//�J�E���g�_�E���Ɏg�p����t���[��
	int m_countFrame;
	//�Q�[���I�[�o�[���o�Ɏg�p����t���[��
	int m_gameoverFrameCount;
	//�Q�[���I�[�o�[�ɂȂ�����A1�{�^����������
	bool m_isGameEnd;

	//�t�F�[�h����
	int m_fadeFrameCount;

	Player m_player;
	*/
};