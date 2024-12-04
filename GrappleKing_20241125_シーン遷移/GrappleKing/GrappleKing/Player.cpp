#include "DxLib.h"
#include "game.h"
#include<cassert>

#include "Player.h"
#include "Pad.h"
#include "Bg.h"

namespace
{
	//キャラクターグラフィックの幅と高さ
	constexpr int kGraphWidth = 64;
	constexpr int kGraphHeight = 64;

	//キャラクターの初期座標
	constexpr float kDefaultX = Game::kScreenWidth * 0.2f;
	constexpr float kDefaultY = 360;

	////アニメーションのコマ数
	constexpr int kIdleAnimNum = 4;
	constexpr int kRunAnimNum = 2;
	constexpr int kUpAnimNum = 1;

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 8;

	//キャラクターの移動速度
	constexpr float kSpeed = 4.0f;
	//ロープを登る速度
	constexpr float kSpeedUp = 8.0f;
	//ロープが延びる速度
	constexpr float kSpeedRope = 10.0f;

	//重力
	constexpr float kGravity = 1.0f;
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
	m_isOnStage(true),
	m_linePos(m_pos),
	LastPos(m_pos),
	IsClear(false),
	Velocity(0.0f,0.0f)
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

void Player::Update(Bg& bg)
{
	Velocity.x = 0;
	//前回のアニメーションの状態を覚えておく
	int lastHandle = m_useHandle;

	m_useHandle = m_handleIdle;

	m_totalFrame = kIdleAnimNum * kSingleAnimFrame;

	//1フレーム前のプレイヤーの座標を覚えておく
	LastPos = m_pos;

	Action();

	Gravity(bg);
	
	//地面に当たっているかどうか
	if (bg.m_isChipHit)
	{
		m_isOnStage = true;
	}
	else
	{
		m_isOnStage = false;
	}	
	
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
	
	m_pos += Velocity;
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;
	//ロープの描画
	DrawLine(m_pos.x, m_pos.y - 1, m_pos.x, m_linePos.y, 0xffffff, 0);
	//プレイヤーの描画
	DrawRectRotaGraph(m_pos.x, m_pos.y - kGraphHeight * 0.25f,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0, 0,
		m_useHandle, true, m_isDirLeft);

//仮
	//天井
	DrawLine(0, 50, 360, 50, 0xffffff, false);
	DrawLine(Game::kScreenWidth - 120, 50, Game::kScreenWidth, 50, 0xffffff, false);

#ifdef _DEBUG
	//当たり判定のデバッグ表示
	DrawBox(GetLeft(), GetTop(),
		GetRight(), GetBottom(),
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

void Player::Action()
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = false;	//左を向く
		Velocity.x -= kSpeed;	//左に移動
		m_useHandle = m_handleRun;
		m_linePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;

	}
	if ((CheckHitKey(KEY_INPUT_RIGHT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = true;	//右を向く
		Velocity.x = kSpeed;//右に移動
		m_useHandle = m_handleRun;
		m_linePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;

	}
	if ((CheckHitKey(KEY_INPUT_UP) == 1) && !m_isRopeMove && m_isOnStage)
	{
		m_isCanMove = false;
		if (!(m_linePos.y <= 50))
		{
			m_linePos = m_pos;//ロープが伸び始める座標にプレイヤーの座標を代入する
		}

		m_isRopeMove = true;//現在ロープが伸びている		
	}

	//ロープが伸びているときの処理
	if (m_isRopeMove)
	{
		//糸を伸ばしているとき
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;
		m_linePos.y -= kSpeedRope;
	}
	else
	{
		m_linePos = m_pos;//ロープが伸び始める座標にプレイヤーの座標を代入する
	}

	//ロープが天井に着いたとき
	if (((0 <= m_pos.x && m_pos.x <= 360) ||
		(Game::kScreenWidth - 120 <= m_pos.x && m_pos.x <= Game::kScreenWidth))
		&& m_linePos.y <= 50)
	{
		//これ以上伸ばさない
		m_linePos.y = 50;
		//プレイヤーを動けるようにする
		m_isCanMove = true;
		//ボタンが押されるまではモーションを変えない
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;

		//ロープを登る
		m_pos.y -= kSpeedUp;

		//天井にめりこまないようにすこしずらす
		if (m_pos.y <= 50 + kGraphHeight * 0.4f)
		{
			m_pos.y = 50 + kGraphHeight * 0.4f;
		}
	}
	else if (360 < m_pos.x && m_pos.x < Game::kScreenWidth - 120 && m_linePos.y <= -50)
	{
		//これ以上伸ばさない
		m_linePos.y = -50;

		//ロープを登る
		m_pos.y -= kSpeedUp;

		if (m_pos.y < -50)
		{
			IsClear = true;
		}
	}

	//画面外にいかないようにする
	if (GetLeft() < 0)
	{
		m_pos.x = 0 + kGraphWidth * 0.36f;
	}
	if (Game::kScreenWidth < GetRight())
	{
		m_pos.x = Game::kScreenWidth - kGraphWidth * 0.36f;
	}
}

void Player::Gravity(Bg& bg)
{

	//重力
	if (!m_isRopeMove && !bg.m_isChipHit)
	{
		Velocity.y += kGravity;
	}
	else
	{
		Velocity.y = 0;
	}
	if (bg.IsWallHit)
	{
		Velocity.x = 0;

	}
}
