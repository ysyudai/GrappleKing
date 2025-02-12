#pragma once
#include "Vec2.h"

class Map;

class Player
{
public:
	Player();
	~Player();

	void Update(Map& map);
	void Draw();

	//�v���C���[�̏����擾����
	float GetLeft() const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom() const;

	//�v���C���[�̍��W���擾
	Vec2 GetPos() const;

	//�v���C���[�̏���ύX����
	void SetPos(const Vec2 pos);

	//�X�e�[�W���N���A�������ǂ����̏����擾
	int IsClearStage1() const { return m_isClearStage1; }
	int IsClearStage2() const { return m_isClearStage2; }
	int IsClearStage3() const { return m_isClearStage3; }

	//1�t���[���O�̃v���C���[�̍��W���o���Ă���
	Vec2 LastPos;

	int LastPosBottom;

	//���[�v���L�юn�߂���W
	Vec2 LinePos;

	Vec2 Velocity;
	
	void OnGround(Map& map);

	bool IsClear() const{ return m_isClear; }

private:
	//���݂̃n���h��
	int m_useHandle;
	//�O���t�B�b�N�n���h��
	int m_handleIdle;
	int m_handleRun;
	int m_handleUp;
	int m_handleFall;

	//�X�e�[�W�N���A�̃O���t�B�b�N�n���h��
	int m_handleStageClear[4];

	//�t���[�����𐔂���
	int m_animFrame;

	//�S�̃t���[��
	int m_totalFrame;

	void Action(Map& map);

	//���ݍĐ����̃A�j���[�V����
	//true:�����Ă���@false:�ҋ@
	//bool m_isRun;

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

	//�d�͂̏���
	void Gravity(Map& map);

	//���[�v�̏���
	void RopeMove(Map& map);

	//�X�e�[�W��S�ăN���A�����ǂ���
	bool m_isClear;

	//���n�����u�Ԃ̏c����(�{��)
	//Vec2 m_extRate;

	int left;
	int top;
	int right;
	int bottom;

	Vec2 m_posPlus;

	int groundChipTop;
	bool isGroundChipHit;

	int plusFrame;

	bool onJustGround;

	// �n�ʂ̔���𖳌��ɂ���t���O�ƃJ�E���^
	bool m_ignoreGroundCollision;
	int m_ignoreGroundCollisionFrameCount;

	// ���n��̖����t���[�����J�E���g����t���O�ƃJ�E���^
	bool m_landingCooldown;
	int m_landingCooldownFrameCount;

	int poyonnFrame;

	bool m_isClearStage1;
	bool m_isClearStage2;
	bool m_isClearStage3;

	int stage1frame;

	int m_clearGraphSizeX;
	int m_clearGraphSizeY;

	int m_clearGraphPosX;
};