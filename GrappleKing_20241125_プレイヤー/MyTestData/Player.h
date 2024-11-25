#pragma once
#include "Vec2.h"

class Player
{
public:
	Player();
	~Player();

	void Update();
	void Draw();

private:
	//���݂̃n���h��
	int m_useHandle;
	//�O���t�B�b�N�n���h��
	int m_handleIdle;
	int m_handleRun;
	int m_handleUp;

	//�t���[�����𐔂���
	int m_animFrame;

	//�S�̃t���[��
	int m_totalFrame;

	//���ݍĐ����̃A�j���[�V����
	//true:�����Ă���@false:�ҋ@
	bool m_isRun;

	//�v���C���[�̈ʒu
	Vec2 m_pos;

	//�v���C���[���������������Ă��邩
	//true:�E�����@false:������
	bool m_isDirLeft;

	//���݃��[�v���L�тĂ��邩�ǂ���
	//true:�L�тĂ���@false:�L�тĂ��Ȃ�
	bool m_isRopeMove;

	//���[�v���V��ɒ��������ǂ���
	//true:�����@false:�܂�
	bool m_isCanMove;

	//�n�ʂɂ��Ă��邩�ǂ���
	bool m_isOnStage;

	//���[�v���L�юn�߂���W
	Vec2 m_linePos;
};