#include "DxLib.h"
#include "game.h"
#include <cmath>
#include<cassert>

#include "Player.h"
#include "Pad.h"
#include "Map.h"

namespace
{
	//キャラクターグラフィックの幅と高さ
	constexpr int kGraphWidth = 64;
	constexpr int kGraphHeight = 64;

	//キャラクターの初期座標
	constexpr float kDefaultX = 160;
	constexpr float kDefaultY = 640;

	////アニメーションのコマ数
	constexpr int kIdleAnimNum = 4;
	constexpr int kRunAnimNum = 2;
	constexpr int kUpAnimNum = 1;

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 8;

	//キャラクターの移動速度
	constexpr float kSpeed = 4.0f;
	//ロープを登る速度
	constexpr float kSpeedUp = 18.0f;
	//ロープが延びる速度
	constexpr float kSpeedRope = 10.0f;

	//重力
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

	//前回のアニメーションの状態を覚えておく
	int lastHandle = m_useHandle;

	m_useHandle = m_handleIdle;

	m_totalFrame = kIdleAnimNum * kSingleAnimFrame;

	//1フレーム前のプレイヤーの座標を覚えておく
	LastPos = m_pos;

	Action(map);

	Gravity(map);
	
	m_animFrame++;

	//アニメーションの合計フレーム数を超えたら最初に戻す
	if (m_animFrame >= m_totalFrame)
	{
		m_animFrame = 0;
	}

	//処理を行った結果、アニメーションが変わっていた場合の処理
	if (lastHandle != m_useHandle)
	{
		m_animFrame = 0;
	}	

	//地面にいるとき
	if (map.m_isGroundHit && !m_isRopeMove && !map.IsCeilingHit)
	{
		OnGround(map);
	}
	//壁に当たった時の処理


	m_pos += Velocity;	
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;
	//ロープの描画
	DrawLine(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - 1),
		static_cast<int>(m_pos.x), static_cast<int>(LinePos.y), 0xffffff, 0);
	//プレイヤーの描画
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - kGraphHeight * 0.25f),
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0, 0,
		m_useHandle, true, m_isDirLeft);

#ifdef _DEBUG
	//当たり判定のデバッグ表示
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
	// 矢印キーを押していたらプレイヤーを移動させる
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = false;	//左を向く
		Velocity.x -= kSpeed;	//左に移動
		m_useHandle = m_handleRun;
		LinePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		map.IsCeilingHit = false;
	}
	if ((CheckHitKey(KEY_INPUT_RIGHT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = true;	//右を向く
		Velocity.x = kSpeed;//右に移動
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
			LinePos = m_pos;//ロープが伸び始める座標にプレイヤーの座標を代入する
		}

		m_isRopeMove = true;//現在ロープが伸びている	

		//map.m_isGroundHit = false;
	}

	//ロープが伸びているときの処理
	if (m_isRopeMove)
	{
		//糸を伸ばしているとき
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;
		LinePos.y -= kSpeedRope;
	}
	else
	{
		m_isCanMove = true;
		//LinePos.y -= kSpeedRope;
		LinePos = m_pos;//ロープが伸び始める座標にプレイヤーの座標を代入する
	}
	
	//ロープが天井に着いたとき
	if (map.IsCeilingHit)
	{
		RopeMove(map);
	}
	
	//クリアしたとき
	if (LinePos.y <= -50)
	{
		//これ以上伸ばさない
		LinePos.y = -50;

		//ロープを登る
		m_pos.y -= kSpeedUp;

		//画面外(上)に行くとクリア
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

	//画面外(左右)にいかないようにする
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
	//重力
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
	//これ以上伸ばさない
	LinePos.y = static_cast<float>(map.GetChipBottomCeiling());

	m_isRopeMove = false;
	
	//プレイヤーを動けるようにする
	//m_isCanMove = true;

	//ボタンが押されるまではモーションを変えない
	m_useHandle = m_handleUp;
	m_totalFrame = kUpAnimNum * kSingleAnimFrame;

	//ロープを登る
	m_pos.y -= kSpeedUp;

	//天井にくっつかせる
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