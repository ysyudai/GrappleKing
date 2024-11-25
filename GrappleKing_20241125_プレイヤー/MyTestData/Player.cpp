#include "DxLib.h"
#include "game.h"
#include<cassert>

#include "Player.h"
#include "Pad.h"

namespace
{
	//�L�����N�^�[�O���t�B�b�N�̕��ƍ���
	constexpr int kGraphWidth = 64;
	constexpr int kGraphHeight = 64;
	
	//�L�����N�^�[�̏������W
	constexpr float kDefaultX = 64;
	constexpr float kDefaultY = 360;

	////�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 4;
	constexpr int kRunAnimNum = 2;
	constexpr int kUpAnimNum = 1;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 8;

	//�L�����N�^�[�̈ړ����x
	constexpr float kSpeed = 4.0f;
	//���[�v��o�鑬�x
	constexpr float kSpeedUp = 12.0f;
	//���[�v�����т鑬�x
	constexpr float kSpeedRope = 10.0f;

	//�d��
	constexpr float kGravity = 8.0f;
}

Player::Player():
	m_useHandle(-1),
	m_handleIdle(-1),
	m_handleRun(-1),
	m_animFrame(0),
	m_totalFrame(0),
	m_isRun(false),
	m_pos(kDefaultX, kDefaultY),
	m_isDirLeft(true),
	m_isRopeMove(false),
	m_isCanMove(true),
	m_isOnStage(true),
	m_linePos(m_pos)
{
	m_handleIdle = LoadGraph("image/Flog/idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("image/Flog/run.png");
	assert(m_handleRun != -1);

	m_handleUp = LoadGraph("image/Flog/ropeMove.png");
	assert(m_handleUp != -1);

	m_useHandle = m_handleIdle;
}

Player::~Player()
{
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleUp);
}

void Player::Update()
{
	//�O��̃A�j���[�V�����̏�Ԃ��o���Ă���
	int lastHandle = m_useHandle;

	m_useHandle = m_handleIdle;

	m_totalFrame = kIdleAnimNum * kSingleAnimFrame;

	//�p�b�h�Ή�����������
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ���L�[�������Ă�����v���C���[���ړ�������
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = false;	//��������
		m_pos.x -= kSpeed;	//���Ɉړ�
		m_useHandle = m_handleRun;
		m_linePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		
	}
	if ((CheckHitKey(KEY_INPUT_RIGHT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = true;	//�E������
		m_pos.x += kSpeed;//�E�Ɉړ�
		m_useHandle = m_handleRun;
		m_linePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		
	}
	if ((CheckHitKey(KEY_INPUT_UP) == 1) && !m_isRopeMove && m_isOnStage)
	{
		m_isCanMove = false;
		if (!(m_linePos.y <= 50))
		{
			m_linePos = m_pos;//���[�v���L�юn�߂���W�Ƀv���C���[�̍��W��������
		}		

		m_isRopeMove = true;//���݃��[�v���L�тĂ���		
	}	
	//��������Ă���Ƃ��̏���
	if (m_isRopeMove)
	{
		//����L�΂��Ă���Ƃ�
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;
		m_linePos.y -= kSpeedRope;
	}
	else
	{
		m_linePos = m_pos;//���[�v���L�юn�߂���W�Ƀv���C���[�̍��W��������
	}

	//���[�v���V��ɒ�������
	if (m_linePos.y <= 50)
	{
		//����ȏ�L�΂��Ȃ�
		m_linePos.y = 50;
		//m_isRopeMove = false;
		//�v���C���[�𓮂���悤�ɂ���
		m_isCanMove = true;
		//�{�^�����������܂ł̓��[�V������ς��Ȃ�
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;

		//���[�v��o��
		m_pos.y -= kSpeedUp;
	}
	//�V��ɂ߂肱�܂Ȃ��悤�ɂ��������炷
	if (m_pos.y <= 50 + kGraphHeight * 0.3f)
	{
		m_pos.y = 50 + kGraphHeight * 0.3f;
	}

	m_animFrame++;

	//�A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (m_animFrame >= m_totalFrame)
	{
		m_animFrame = 0;
	}

	//�������s�������ʁA�A�j���[�V�������ς���Ă����ꍇ�̏���
	if (lastHandle != m_useHandle)
	{
		m_animFrame = 0;
	}

	//�d��
	if (!m_isRopeMove)
	{
		m_pos.y += kGravity;
	}	
	
	if (m_pos.y >= kDefaultY)
	{
		//�n�ʂ̔���
		m_pos.y = kDefaultY;
	}

	if (m_pos.y >= kDefaultY)
	{
		m_isOnStage = true;
	}
	else
	{
		m_isOnStage = false;
	}
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;
	//�v���C���[�̕`��
	DrawRectRotaGraph(m_pos.x, m_pos.y-kGraphHeight*0.25f,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0, 0,
		m_useHandle, true, m_isDirLeft);
	//���[�v�̕`��
//	if (m_isCanMove && (m_useHandle == m_handleUp))
	{
		DrawLine(m_pos.x, m_pos.y, m_pos.x, m_linePos.y, 0xffffff, 0);
	}
}