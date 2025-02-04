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
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;

	//�L�����N�^�[�̏������W
	constexpr float kDefaultX = 160;
	constexpr float kDefaultY = 640;

	////�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 4;
	constexpr int kRunAnimNum = 2;
	constexpr int kUpAnimNum = 1;
	constexpr int kFallAnimNum = 1;

	//�A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 8;

	//�L�����N�^�[�̈ړ����x
	constexpr float kSpeed = 4.0f;
	//���[�v��o�鑬�x
	constexpr float kSpeedUp = 18.0f;
	//���[�v�����т鑬�x
	constexpr float kSpeedRope = 10.0f;

	//�X�e�[�W�N���A�n���h�����ړ����鑬�x
	constexpr float kClearSpeed = 8.0f;
	
	//�d��
	constexpr float kGravity = 10.0f;
}

Player::Player() :
	m_useHandle(-1),
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleUp(-1),
	m_handleFall(-1),
	m_handleStageClear{-1,-1,-1},
	m_animFrame(0),
	m_totalFrame(0),
	m_pos(kDefaultX, kDefaultY),
	m_isDirLeft(true),
	m_isRopeMove(false),
	m_isCanMove(true),
	LinePos(kDefaultX, kDefaultY),
	LastPos(kDefaultX, kDefaultY),
	Velocity(0.0f,0.0f),
	m_isClear(false),
	left(0),
	top(0),
	right(0),
	bottom(0),
	m_posPlus{ 0.0f,0.0f },
	plusFrame(0),
	onJustGround(false),
	m_ignoreGroundCollision(true), // ���������ɒn�ʂ̔���𖳌��ɂ���
	m_ignoreGroundCollisionFrameCount(60), // 60�t���[���i1�b�ԁj�n�ʂ̔���𖳌��ɂ���
	m_landingCooldown(false), // ���n��̖����t���[����������
	m_landingCooldownFrameCount(0), // ���n��̖����t���[���J�E���g��������
	poyonnFrame(0),
	m_isClearStage1(false),
	m_isClearStage2(false),
	m_isClearStage3(false),
	groundChipTop(0),
	isGroundChipHit(false),
	LastPosBottom(0),
	stage1frame(0),
	m_clearGraphSizeX(0),
	m_clearGraphSizeY(0),
	m_clearGraphPosX(Game::kScreenWidth+10)
{
	m_handleIdle = LoadGraph("data/Flog/idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("data/Flog/run.png");
	assert(m_handleRun != -1);

	m_handleUp = LoadGraph("data/Flog/ropeMove.png");
	assert(m_handleUp != -1);

	m_handleFall = LoadGraph("data/Flog/Fall.png");
	assert(m_handleFall != -1);

	m_handleStageClear[1] = LoadGraph("data/image/Stage1Clear.png");
	assert(m_handleStageClear[1] != -1);
	m_handleStageClear[2] = LoadGraph("data/image/Stage2Clear.png");
	assert(m_handleStageClear[2] != -1);
	m_handleStageClear[3] = LoadGraph("data/image/Stage3Clear.png");
	assert(m_handleStageClear[3] != -1);

	m_useHandle = m_handleIdle;
	m_handleStageClear[0] = m_handleStageClear[1];
}

Player::~Player()
{
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleUp);
	DeleteGraph(m_handleFall);
}

void Player::Update(Map& map)
{	
	//���t���[�������x��0�ɂ���
	Velocity.x = 0;	
	
	if (plusFrame > 10)
	{
		onJustGround = false;
		plusFrame = 0;
	}
	if(onJustGround)
	{
	plusFrame++;
	}

	//�O���t�T�C�Y�̕ω��ʂ�������
	if(plusFrame)
	m_posPlus = { 0.0f,0.0f };

	//�O��̃A�j���[�V�����̏�Ԃ��o���Ă���
	int lastHandle = m_useHandle;

	m_useHandle = m_handleIdle;
	m_totalFrame = kIdleAnimNum * kSingleAnimFrame;

	//1�t���[���O�̃v���C���[�̍��W���o���Ă���
	LastPos = m_pos;
	LastPosBottom = static_cast<int>(m_pos.y);

	groundChipTop = map.GetChipTopGround();
	isGroundChipHit = map.m_isGroundHit;

	// ���n��̖����t���[�����J�E���g
	if (m_landingCooldown)
	{
		m_landingCooldownFrameCount++;
		if (m_landingCooldownFrameCount >= 10) // 10�t���[���i��0.16�b�j�����ɂ���
		{
			m_landingCooldown = false;
			m_landingCooldownFrameCount = 0;
		}
	}

	//�v���C���[�̃A�N�V����
	if (!m_landingCooldown)
	{
		Action(map);
	}

	//�d�͂̏���
	Gravity(map);
	
	m_animFrame++;

	//�A�j���[�V�����̍��v�t���[�����𒴂�����ŏ��ɖ߂�
	if (m_animFrame >= m_totalFrame)
	{
		m_animFrame = 0;
	}

	// �n�ʂ̔���𖳌��ɂ���t���[���J�E���g������������
	if (m_ignoreGroundCollisionFrameCount > 0)
	{
		m_ignoreGroundCollisionFrameCount--;
	}
	else
	{
		m_ignoreGroundCollision = false;
	}

	//�������s�������ʁA�A�j���[�V�������ς���Ă����ꍇ�̏���
	if (lastHandle != m_useHandle)
	{
		m_animFrame = 0;
	}	

	//�n�ʂɂ���Ƃ�
	if (/*!m_ignoreGroundCollision && */map.m_isGroundHit && !m_isRopeMove && !map.IsCeilingHit)
	{
		OnGround(map);
	}
	
	if (m_landingCooldown && poyonnFrame>64.0f)
	{//�n�ʂɂ�����|�����Ƃ����鏈��
	/*	if (m_posPlus.x < kGraphWidth * 2.0f)
		{
			m_posPlus.x += 2.0f;
		}
		
		if (m_posPlus.y < kGraphWidth * 1.0f)
		{
			m_posPlus.y += 1.0f;
		}*/

		m_posPlus.x = kGraphWidth * 1.0f;
		m_posPlus.y = kGraphWidth * 0.5f;

		
	}
	else if(m_landingCooldown)
	{
		poyonnFrame -= 1;
		if (poyonnFrame < 0)
		{
			poyonnFrame = 0;
		}

		if (m_posPlus.x > 0)
		{
			m_posPlus.x -= 3.0f;
		}
		if (m_posPlus.y > 0)
		{
			m_posPlus.y -= 1.5f;
		}

		if (m_posPlus.x < 0) {
			m_posPlus.x = 0.0f;
		}
		if (m_posPlus.y < 0) {
			m_posPlus.y = 0.0f;
		}
	}

	if (stage1frame > 0)
	{
		if (map.IsStage1)
		{
			m_handleStageClear[0] = m_handleStageClear[2];

			GetGraphSize(m_handleStageClear[0], &m_clearGraphSizeX, &m_clearGraphSizeY);
			
			m_clearGraphPosX -= static_cast<int>(kClearSpeed);
		}
		if (map.IsStage2)
		{
			m_handleStageClear[0] = m_handleStageClear[3];

			GetGraphSize(m_handleStageClear[0], &m_clearGraphSizeX, &m_clearGraphSizeY);
			
			m_clearGraphPosX += static_cast<int>(kClearSpeed);
		}
		if (map.IsStage3)
		{
			GetGraphSize(m_handleStageClear[0], &m_clearGraphSizeX, &m_clearGraphSizeY);
			
			m_clearGraphPosX -= static_cast<int>(kClearSpeed);
		}
	}

	//�v���C���[�̉����x���X�V
	m_pos += Velocity;	
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;
	//���[�v�̕`��
	DrawLine(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - 1),
		static_cast<int>(m_pos.x), static_cast<int>(LinePos.y), 0xffffff, 0);
	//�v���C���[�̕`��
	/*DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - kGraphHeight * 0.5f),
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		1, 0,
		m_useHandle, true, m_isDirLeft);*/

	/*DrawRectExtendGraph(static_cast<int>(m_pos.x - kGraphWidth * 0.5f), static_cast<int>(m_pos.y - kGraphHeight),
		static_cast<int>(m_pos.x + kGraphWidth * 0.5f), static_cast<int>(m_pos.y),
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		m_useHandle, m_isDirLeft);

	 //�v���C���[�̕`��
	int left = static_cast<int>(m_pos.x - kGraphWidth * 0.5f);
	int top = static_cast<int>(m_pos.y - kGraphHeight);
	int right = static_cast<int>(m_pos.x + kGraphWidth * 0.5f);
	int bottom = static_cast<int>(m_pos.y);*/

	//if (m_isDirLeft)
	//{
	//	// �E�����̏ꍇ�A�蓮�Ŕ��]
	//	DrawRectExtendGraph(right, top, left, bottom,
	//		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
	//		m_useHandle, true);
	//}
	//else
	//{
	//	// �������̏ꍇ
	//	DrawRectExtendGraph(left, top, right, bottom,
	//		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
	//		m_useHandle, true);		
	//}

	//�v���C���[�̕`��
	left = static_cast<int>(GetRight());
	top = static_cast<int>(m_pos.y - kGraphHeight * 2.0f);
	right = static_cast<int>(GetLeft());
	bottom = static_cast<int>(GetBottom()); 
	
	if (m_isDirLeft)
	{
		// �E�����̏ꍇ�A�蓮�Ŕ��]
		DrawRectExtendGraph(static_cast<int>(left + m_posPlus.x), static_cast<int>(top + m_posPlus.y),
			static_cast<int>(right - m_posPlus.x), bottom,
			animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
			m_useHandle, true);
	}
	else
	{
		// �������̏ꍇ
		DrawRectExtendGraph(static_cast<int>(right - m_posPlus.x), static_cast<int>(top + m_posPlus.y),
			static_cast<int>(left + m_posPlus.x), bottom,
			animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
			m_useHandle, true);
	}
	
	if (stage1frame > 0)
	{
		DrawGraph(static_cast<int>(m_clearGraphPosX),
			static_cast<int>(Game::kScreenHeight*0.5f) - static_cast<int>(m_clearGraphSizeY) * 0.5,
			m_handleStageClear[0],true);
	}

#ifdef _DEBUG
	//�����蔻��̃f�o�b�O�\��
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()),
		static_cast<int>(GetRight()), static_cast<int>(GetBottom()),
		0x0000ff, false);

	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 5, 0xff0000, true);
#endif
}

//�v���C���[�̏����擾����
float Player::GetLeft() const
{
	return (m_pos.x - kGraphWidth * 1.0f);
}
float Player::GetTop() const
{
	return (m_pos.y - kGraphHeight * 1.2f);
}
float Player::GetRight() const
{
	return (m_pos.x + kGraphWidth * 1.0f);
}
float Player::GetBottom() const
{
	return m_pos.y;
}
Vec2 Player::GetPos() const
{
	return m_pos;
}
//�v���C���[�̏���ύX����
void Player::SetPos(const Vec2 pos)
{
	m_pos = pos;
}

void Player::Action(Map& map)
{
	// ���L�[�������Ă�����v���C���[���ړ�������
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) && m_isCanMove)
	{		
		m_isRopeMove = false;	//���[�v���オ�铮����L�����Z��
		m_isDirLeft = false;	//��������
		Velocity.x -= kSpeed;	//���Ɉړ�
		if (map.m_isGroundHit)
		{//�n�ʂɂ���Ƃ�run�A�j���[�V����
			m_useHandle = m_handleRun;
			m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		}
		LinePos.y = m_pos.y;	//���[�v���L�юn�߂���W�Ƀv���C���[�̍��W��������
		map.IsCeilingHit = false;	//�V��ɓ������Ă��Ȃ�
	}
	else if ((CheckHitKey(KEY_INPUT_RIGHT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;	//���[�v���オ�铮����L�����Z��
		m_isDirLeft = true;		//�E������
		Velocity.x = kSpeed;	//�E�Ɉړ�
		if (map.m_isGroundHit)
		{//�n�ʂɂ���Ƃ�run�A�j���[�V����
			m_useHandle = m_handleRun;
			m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		}
		LinePos.y = m_pos.y;	//���[�v���L�юn�߂���W�Ƀv���C���[�̍��W��������
		map.IsCeilingHit = false;	//�V��ɓ������Ă��Ȃ�
	}
	else if ((CheckHitKey(KEY_INPUT_UP) == 1) && !m_isRopeMove && map.m_isGroundHit)
	{
		m_isCanMove = false;	//���[�v���L�тĂ���Ԃ͑���s��
		if (!(LinePos.y <= map.GetChipBottomCeiling()))
		{
			LinePos = m_pos;//���[�v���L�юn�߂���W�Ƀv���C���[�̍��W��������
		}

		m_isRopeMove = true;//���݃��[�v���L�тĂ���
	}

	if (!map.m_isGroundHit)
	{//�����Ă���Œ��̃n���h��
		m_useHandle = m_handleFall;
		m_totalFrame = kFallAnimNum * kSingleAnimFrame;

		poyonnFrame++;
	}

	//���[�v���L�тĂ���Ƃ��̏���
	if (m_isRopeMove)
	{//����L�΂��Ă���Ƃ�
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;
		LinePos.y -= kSpeedRope;
	}
	else
	{//�L�т�������
		m_isCanMove = true;	//������悤�ɂ���
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
			stage1frame++;
			if (map.IsStage1)
			{//�}�b�v���X�e�[�W1�̂Ƃ�
				m_isDirLeft = false;
				m_isCanMove = false;
				LinePos = m_pos;

				m_isClearStage1 = true;

				map.m_isGroundHit = false;
				map.IsWallHit = false;
				map.IsCeilingHit = false;

				if (stage1frame > 300)
				{
					m_clearGraphPosX = - m_clearGraphSizeX - 10;					

					m_isRopeMove = false;

					map.IsStage1 = false;
					map.IsStage2 = true;

					m_pos = { Game::kScreenWidth - kDefaultX,kDefaultY };
				}
				else
				{
					m_pos.y = -50;
				}
			}
			else if (map.IsStage2)
			{
				m_isDirLeft = true;
				
				m_isCanMove = false;
				LinePos = m_pos;
				map.m_isGroundHit = false;
				map.IsWallHit = false;
				map.IsCeilingHit = false;

				if (stage1frame > 300)
				{
					m_clearGraphPosX = Game::kScreenWidth + 10;					

					m_isRopeMove = false;

					map.IsStage2 = false;
					map.IsStage3 = true;

					m_pos = { Game::kScreenWidth * 0.5f ,kDefaultY };
				}
				else
				{
					m_pos.y = -50;
				}
			}
			else if (map.IsStage3)
			{
				if (stage1frame > 300)
				{
					m_isClear = true;
				}
				else
				{
					m_pos.y = -50;
				}
			}
		}
	}
	else
	{
		stage1frame = 0;
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

	//�n�ʂɒ��n�����Ƃ�
	if (!m_ignoreGroundCollision && LastPosBottom > groundChipTop && isGroundChipHit)
	{
		onJustGround = true;
		
		m_landingCooldown = true;
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
	if (GetTop() <= map.GetChipBottomCeiling())
	{
		m_pos.y = map.GetChipBottomCeiling() + kGraphWidth * 0.9f;
	}
}

void Player::OnGround(Map& map)
{
	Vec2 NextPos = m_pos;
	NextPos.y = static_cast<float>(map.GetChipTopGround());
	m_pos=NextPos;
	Velocity.y = 0;
}