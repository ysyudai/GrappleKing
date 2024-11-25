#include "DxLib.h"
#include "game.h"
#include<cassert>

#include "Player.h"
#include "Pad.h"

namespace
{
	//キャラクターグラフィックの幅と高さ
	constexpr int kGraphWidth = 64;
	constexpr int kGraphHeight = 64;
	
	//キャラクターの初期座標
	constexpr float kDefaultX = 64;
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
	constexpr float kSpeedUp = 12.0f;
	//ロープが延びる速度
	constexpr float kSpeedRope = 10.0f;

	//重力
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
	//前回のアニメーションの状態を覚えておく
	int lastHandle = m_useHandle;

	m_useHandle = m_handleIdle;

	m_totalFrame = kIdleAnimNum * kSingleAnimFrame;

	//パッド対応させたいよ
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 矢印キーを押していたらプレイヤーを移動させる
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = false;	//左を向く
		m_pos.x -= kSpeed;	//左に移動
		m_useHandle = m_handleRun;
		m_linePos.y = m_pos.y;
		m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		
	}
	if ((CheckHitKey(KEY_INPUT_RIGHT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;
		m_isDirLeft = true;	//右を向く
		m_pos.x += kSpeed;//右に移動
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
	//上を押しているときの処理
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

	//ロープが天井に着いたら
	if (m_linePos.y <= 50)
	{
		//これ以上伸ばさない
		m_linePos.y = 50;
		//m_isRopeMove = false;
		//プレイヤーを動けるようにする
		m_isCanMove = true;
		//ボタンが押されるまではモーションを変えない
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;

		//ロープを登る
		m_pos.y -= kSpeedUp;
	}
	//天井にめりこまないようにすこしずらす
	if (m_pos.y <= 50 + kGraphHeight * 0.3f)
	{
		m_pos.y = 50 + kGraphHeight * 0.3f;
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

	//重力
	if (!m_isRopeMove)
	{
		m_pos.y += kGravity;
	}	
	
	if (m_pos.y >= kDefaultY)
	{
		//地面の判定
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
	//プレイヤーの描画
	DrawRectRotaGraph(m_pos.x, m_pos.y-kGraphHeight*0.25f,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0, 0,
		m_useHandle, true, m_isDirLeft);
	//ロープの描画
//	if (m_isCanMove && (m_useHandle == m_handleUp))
	{
		DrawLine(m_pos.x, m_pos.y, m_pos.x, m_linePos.y, 0xffffff, 0);
	}
}