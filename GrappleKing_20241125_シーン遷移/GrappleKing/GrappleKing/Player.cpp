#include "Player.h"
#include "DxLib.h"
#include "game.h"

namespace
{
	// キャラクターグラフィックの幅と高さ
	constexpr int kGraphSize = 32;

	constexpr float kDefaultX = 64;
	constexpr float kDefaultY = 360;

	// スピード
	constexpr float kSpeed = 4.0f;
}

Player::Player():
	x(0),y(0)
{
}

Player::~Player()
{
}

void Player::Update()
{
	// 矢印キーを押していたらプレイヤーを移動させる
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		x -= kSpeed;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		x += kSpeed;
	}
	if (CheckHitKey(KEY_INPUT_UP) == 1)
	{
		y -= kSpeed;
	}
	if (CheckHitKey(KEY_INPUT_DOWN) == 1)
	{
		y += kSpeed;
	}
}

void Player::Draw()
{
	DrawBox(kDefaultX + x, kDefaultY + y,
		kDefaultX + kGraphSize + x, kDefaultY + kGraphSize + y, 0xffffff, true);
}
