#pragma once
#include "Vec2.h"

class Bg;

class Player
{
public:
	Player();
	~Player();

	void Update(Bg& bg);
	void Draw();

	//�v���C���[�̏����擾����
	float GetLeft() const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom() const;

	Vec2 GetPos() const;

	//�v���C���[�̏���ύX����
	void SetPos(const Vec2 pos);

	//1�t���[���O�̃v���C���[�̍��W���o���Ă���
	Vec2 LastPos;

	bool IsClear;

	Vec2 Velocity;

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

	void Action();

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

	void Gravity(Bg& bg);
};