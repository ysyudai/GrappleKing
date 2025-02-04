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
	constexpr int kGraphWidth = 32;
	constexpr int kGraphHeight = 32;

	//キャラクターの初期座標
	constexpr float kDefaultX = 160;
	constexpr float kDefaultY = 640;

	////アニメーションのコマ数
	constexpr int kIdleAnimNum = 4;
	constexpr int kRunAnimNum = 2;
	constexpr int kUpAnimNum = 1;
	constexpr int kFallAnimNum = 1;

	//アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 8;

	//キャラクターの移動速度
	constexpr float kSpeed = 4.0f;
	//ロープを登る速度
	constexpr float kSpeedUp = 18.0f;
	//ロープが延びる速度
	constexpr float kSpeedRope = 10.0f;

	//ステージクリアハンドルが移動する速度
	constexpr float kClearSpeed = 8.0f;
	
	//重力
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
	m_ignoreGroundCollision(true), // 初期化時に地面の判定を無効にする
	m_ignoreGroundCollisionFrameCount(60), // 60フレーム（1秒間）地面の判定を無効にする
	m_landingCooldown(false), // 着地後の無効フレームを初期化
	m_landingCooldownFrameCount(0), // 着地後の無効フレームカウントを初期化
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
	//毎フレーム加速度を0にする
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

	//グラフサイズの変化量を初期化
	if(plusFrame)
	m_posPlus = { 0.0f,0.0f };

	//前回のアニメーションの状態を覚えておく
	int lastHandle = m_useHandle;

	m_useHandle = m_handleIdle;
	m_totalFrame = kIdleAnimNum * kSingleAnimFrame;

	//1フレーム前のプレイヤーの座標を覚えておく
	LastPos = m_pos;
	LastPosBottom = static_cast<int>(m_pos.y);

	groundChipTop = map.GetChipTopGround();
	isGroundChipHit = map.m_isGroundHit;

	// 着地後の無効フレームをカウント
	if (m_landingCooldown)
	{
		m_landingCooldownFrameCount++;
		if (m_landingCooldownFrameCount >= 10) // 10フレーム（約0.16秒）無効にする
		{
			m_landingCooldown = false;
			m_landingCooldownFrameCount = 0;
		}
	}

	//プレイヤーのアクション
	if (!m_landingCooldown)
	{
		Action(map);
	}

	//重力の処理
	Gravity(map);
	
	m_animFrame++;

	//アニメーションの合計フレーム数を超えたら最初に戻す
	if (m_animFrame >= m_totalFrame)
	{
		m_animFrame = 0;
	}

	// 地面の判定を無効にするフレームカウントを減少させる
	if (m_ignoreGroundCollisionFrameCount > 0)
	{
		m_ignoreGroundCollisionFrameCount--;
	}
	else
	{
		m_ignoreGroundCollision = false;
	}

	//処理を行った結果、アニメーションが変わっていた場合の処理
	if (lastHandle != m_useHandle)
	{
		m_animFrame = 0;
	}	

	//地面にいるとき
	if (/*!m_ignoreGroundCollision && */map.m_isGroundHit && !m_isRopeMove && !map.IsCeilingHit)
	{
		OnGround(map);
	}
	
	if (m_landingCooldown && poyonnFrame>64.0f)
	{//地面についたらポヨンとさせる処理
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

	//プレイヤーの加速度を更新
	m_pos += Velocity;	
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;
	//ロープの描画
	DrawLine(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - 1),
		static_cast<int>(m_pos.x), static_cast<int>(LinePos.y), 0xffffff, 0);
	//プレイヤーの描画
	/*DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y - kGraphHeight * 0.5f),
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		1, 0,
		m_useHandle, true, m_isDirLeft);*/

	/*DrawRectExtendGraph(static_cast<int>(m_pos.x - kGraphWidth * 0.5f), static_cast<int>(m_pos.y - kGraphHeight),
		static_cast<int>(m_pos.x + kGraphWidth * 0.5f), static_cast<int>(m_pos.y),
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		m_useHandle, m_isDirLeft);

	 //プレイヤーの描画
	int left = static_cast<int>(m_pos.x - kGraphWidth * 0.5f);
	int top = static_cast<int>(m_pos.y - kGraphHeight);
	int right = static_cast<int>(m_pos.x + kGraphWidth * 0.5f);
	int bottom = static_cast<int>(m_pos.y);*/

	//if (m_isDirLeft)
	//{
	//	// 右向きの場合、手動で反転
	//	DrawRectExtendGraph(right, top, left, bottom,
	//		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
	//		m_useHandle, true);
	//}
	//else
	//{
	//	// 左向きの場合
	//	DrawRectExtendGraph(left, top, right, bottom,
	//		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
	//		m_useHandle, true);		
	//}

	//プレイヤーの描画
	left = static_cast<int>(GetRight());
	top = static_cast<int>(m_pos.y - kGraphHeight * 2.0f);
	right = static_cast<int>(GetLeft());
	bottom = static_cast<int>(GetBottom()); 
	
	if (m_isDirLeft)
	{
		// 右向きの場合、手動で反転
		DrawRectExtendGraph(static_cast<int>(left + m_posPlus.x), static_cast<int>(top + m_posPlus.y),
			static_cast<int>(right - m_posPlus.x), bottom,
			animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
			m_useHandle, true);
	}
	else
	{
		// 左向きの場合
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
	//当たり判定のデバッグ表示
	DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop()),
		static_cast<int>(GetRight()), static_cast<int>(GetBottom()),
		0x0000ff, false);

	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 5, 0xff0000, true);
#endif
}

//プレイヤーの情報を取得する
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
//プレイヤーの情報を変更する
void Player::SetPos(const Vec2 pos)
{
	m_pos = pos;
}

void Player::Action(Map& map)
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if ((CheckHitKey(KEY_INPUT_LEFT) == 1) && m_isCanMove)
	{		
		m_isRopeMove = false;	//ロープを上がる動作をキャンセル
		m_isDirLeft = false;	//左を向く
		Velocity.x -= kSpeed;	//左に移動
		if (map.m_isGroundHit)
		{//地面にいるときrunアニメーション
			m_useHandle = m_handleRun;
			m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		}
		LinePos.y = m_pos.y;	//ロープが伸び始める座標にプレイヤーの座標を代入する
		map.IsCeilingHit = false;	//天井に当たっていない
	}
	else if ((CheckHitKey(KEY_INPUT_RIGHT) == 1) && m_isCanMove)
	{
		m_isRopeMove = false;	//ロープを上がる動作をキャンセル
		m_isDirLeft = true;		//右を向く
		Velocity.x = kSpeed;	//右に移動
		if (map.m_isGroundHit)
		{//地面にいるときrunアニメーション
			m_useHandle = m_handleRun;
			m_totalFrame = kRunAnimNum * kSingleAnimFrame;
		}
		LinePos.y = m_pos.y;	//ロープが伸び始める座標にプレイヤーの座標を代入する
		map.IsCeilingHit = false;	//天井に当たっていない
	}
	else if ((CheckHitKey(KEY_INPUT_UP) == 1) && !m_isRopeMove && map.m_isGroundHit)
	{
		m_isCanMove = false;	//ロープが伸びている間は操作不可
		if (!(LinePos.y <= map.GetChipBottomCeiling()))
		{
			LinePos = m_pos;//ロープが伸び始める座標にプレイヤーの座標を代入する
		}

		m_isRopeMove = true;//現在ロープが伸びている
	}

	if (!map.m_isGroundHit)
	{//落ちている最中のハンドル
		m_useHandle = m_handleFall;
		m_totalFrame = kFallAnimNum * kSingleAnimFrame;

		poyonnFrame++;
	}

	//ロープが伸びているときの処理
	if (m_isRopeMove)
	{//糸を伸ばしているとき
		m_useHandle = m_handleUp;
		m_totalFrame = kUpAnimNum * kSingleAnimFrame;
		LinePos.y -= kSpeedRope;
	}
	else
	{//伸びきった後
		m_isCanMove = true;	//動けるようにする
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
			stage1frame++;
			if (map.IsStage1)
			{//マップがステージ1のとき
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

	//画面外(左右)にいかないようにする
	if (GetLeft() < 0)
	{
		m_pos.x = 0 + kGraphWidth * 0.36f;
	}
	if (Game::kScreenWidth < GetRight())
	{
		m_pos.x = Game::kScreenWidth - kGraphWidth * 0.36f;
	}

	//地面に着地したとき
	if (!m_ignoreGroundCollision && LastPosBottom > groundChipTop && isGroundChipHit)
	{
		onJustGround = true;
		
		m_landingCooldown = true;
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