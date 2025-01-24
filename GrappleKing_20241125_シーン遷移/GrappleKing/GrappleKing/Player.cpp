#include "DxLib.h"
#include "game.h"
#include <cmath>
#include<cassert>

#include "Player.h"
#include "Pad.h"
#include "Map.h"

namespace
{
	//�L�����N�^�[�O���t�B�b�N�̕��ƍ���
	constexpr int kGraphWidth = 64;
	constexpr int kGraphHeight = 64;

	//�L�����N�^�[�̏������W
	constexpr float kDefaultX = 160;
	constexpr float kDefaultY = 640;

	////�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 4;
	constexpr int kRunAnimNum = 2;
	constexpr int kUpAnimNum = 1;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 8;

	//�L�����N�^�[�̈ړ����x
	constexpr float kSpeed = 4.0f;
	//���[�v��o�鑬�x
	constexpr float kSpeedUp = 18.0f;
	//���[�v�����т鑬�x
	constexpr float kSpeedRope = 10.0f;

	//�d��
	constexpr float kGravity = 10.0f;
}

Player::Player() :
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
	LinePos(m_pos),
	LastPos(m_pos),
	Velocity(0.0f,0.0f),
	m_isClear(false)
{
	m_handleIdle = LoadGraph("data/Flog/idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("data/Flog/run.png");
	assert(m_handleRun != -1);

	m_handleUp = LoadGraph("data/Flog/ropeMove.png");
	assert(m_handleUp != -1);

	m_useHandle = m_handleIdle;
}

Player::~Player()
{
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleUp);
}

void Player::Update(Map& map)
{	
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		m_pos = { Game::kScreenWidth - kDefaultX,Game::kScreenHeight - kDefaultY };
	}

	Velocity.x = 0;	

	//�O��̃A�j���[�V�����̏�Ԃ��o���Ă���
	int lastHandle = m_useHandle;

	m_useHandle = m_handleIdle;

	m_totalFrame = kIdleAnimNum * kSingleAnimFrame;

	//1�t���[���O�̃v���C���[�̍��W���o���Ă���
	LastPos = m_pos;

	Action(map);

	Gravity(map);
	
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

	//�n�ʂɂ���Ƃ�
	if (map.m_isGroundHit && !m_isRopeMove && !map.IsCeilingHit)
	{
		OnGround(map);
	}
	//�ǂɓ����������̏���


	m_pos += Velocity;	
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;
	//���[�v�̕`��
	DrawLine(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - 1),
		static_cast<int>(m_pos.x), static_cast<int>(LinePos.y), 0xffffff, 0);
	//�v���C���[�̕`��
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - kGraphHeight * 0.25f),
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0, 0,
		m_useHandle, true, m_isDirLeft);

#ifdef _DEBUG
	//�����蔻��̃f�o�b�O�\��
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()),
		static_cast<int>(GetRight()), static_cast<int>(GetBottom()),
		0xff0000, false);
#endif
}

float Player::GetLeft() const
{
	return (m_pos.x - kGraphWidth * 0.36f);
}

float Player::GetTop() const
{
	return (m_pos.y - kGraphHeight * 0.4f);
}

float Player::GetRight() const
{
	return (m_pos.x + kGraphWidth * 0.36f);
}

float Player::GetBottom() const
{
	return m_pos.y;
}

Vec2 Player::GetPos() const
{
	return m_pos;
}

void Player::SetPos(const Vec2 pos)
{
	m_pos = pos;
}

void Player::Action(Map& map)
{
	// ���L�[�������Ă�����v���C���[���ړ�������
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = false;	//��������
		Velocity.x -= kSpeed;	//���Ɉړ�
		m_useHandle = m_handleRun;
		LinePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		map.IsCeilingHit = false;
	}
	if ((CheckHitKey(KEY_INPUT_RIGHT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = true;	//�E������
		Velocity.x = kSpeed;//�E�Ɉړ�
		m_useHandle = m_handleRun;
		LinePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		map.IsCeilingHit = false;
	}
	if ((CheckHitKey(KEY_INPUT_UP) == 1) && !m_isRopeMove && map.m_isGroundHit)
	{
		m_isCanMove = false;
		if (!(LinePos.y <= map.GetChipBottomCeiling()))
		{
			LinePos = m_pos;//���[�v���L�юn�߂���W�Ƀv���C���[�̍��W��������
		}

		m_isRopeMove = true;//���݃��[�v���L�тĂ���	

		//map.m_isGroundHit = false;
	}

	//���[�v���L�тĂ���Ƃ��̏���
	if (m_isRopeMove)
	{
		//����L�΂��Ă���Ƃ�
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;
		LinePos.y -= kSpeedRope;
	}
	else
	{
		m_isCanMove = true;
		//LinePos.y -= kSpeedRope;
		LinePos = m_pos;//���[�v���L�юn�߂���W�Ƀv���C���[�̍��W��������
	}
	
	//���[�v���V��ɒ������Ƃ�
	if (map.IsCeilingHit)
	{
		RopeMove(map);
	}
	
	//�N���A�����Ƃ�
	if (LinePos.y <= -50)
	{
		//����ȏ�L�΂��Ȃ�
		LinePos.y = -50;

		//���[�v��o��
		m_pos.y -= kSpeedUp;

		//��ʊO(��)�ɍs���ƃN���A
		if (m_pos.y < -50)
		{
			m_isRun = false;
			m_isDirLeft = true;
			m_isRopeMove = false;
			m_isCanMove = false;
			LinePos = m_pos;
			map.m_isGroundHit = false;
			map.IsWallHit = false;
			map.IsCeilingHit = false;
			
			if (map.IsStage1)
			{
				map.IsStage1 = false;
				map.IsStage2 = true;

				m_pos = { Game::kScreenWidth - kDefaultX,kDefaultY };
			}
			else if (map.IsStage2)
			{
				map.IsStage2 = false;
				map.IsStage3 = true;

				m_pos = { Game::kScreenWidth * 0.5f ,kDefaultY };
			}
			else if (map.IsStage3)
			{
				m_isClear = true;
			}
		}
	}		

	//��ʊO(���E)�ɂ����Ȃ��悤�ɂ���
	if (GetLeft() < 0)
	{
		m_pos.x = 0 + kGraphWidth * 0.36f;
	}
	if (Game::kScreenWidth < GetRight())
	{
		m_pos.x = Game::kScreenWidth - kGraphWidth * 0.36f;
	}
}

void Player::Gravity(Map& map)
{
	//�d��
	if (!m_isRopeMove && !map.m_isGroundHit)
	{
		Velocity.y = kGravity;
	}
	else
	{
		Velocity.y = 0;
	}
	if (map.IsWallHit)
	{
		Velocity.x = 0;
	}
}

void Player::RopeMove(Map& map)
{
	//����ȏ�L�΂��Ȃ�
	LinePos.y = static_cast<float>(map.GetChipBottomCeiling());

	m_isRopeMove = false;
	
	//�v���C���[�𓮂���悤�ɂ���
	//m_isCanMove = true;

	//�{�^�����������܂ł̓��[�V������ς��Ȃ�
	m_useHandle = m_handleUp;
	m_totalFrame = kUpAnimNum * kSingleAnimFrame;

	//���[�v��o��
	m_pos.y -= kSpeedUp;

	//�V��ɂ���������
	if (m_pos.y <= map.GetChipBottomCeiling() + kGraphHeight * 0.2f)
	{
		m_pos.y = map.GetChipBottomCeiling() + kGraphHeight * 0.2f;
	}
}

void Player::OnGround(Map& map)
{
	Vec2 NextPos = m_pos;
	NextPos.y = static_cast<float>(map.GetChipTopGround());
	m_pos=NextPos;
	Velocity.y = 0;
}