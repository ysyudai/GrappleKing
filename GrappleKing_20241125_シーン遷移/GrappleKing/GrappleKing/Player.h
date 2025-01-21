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

	//プレイヤーの情報を取得する
	float GetLeft() const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom() const;

	//プレイヤーの座標を取得
	Vec2 GetPos() const;

	//プレイヤーの情報を変更する
	void SetPos(const Vec2 pos);

	//1フレーム前のプレイヤーの座標を覚えておく
	Vec2 LastPos;

	//ロープが伸び始める座標
	Vec2 LinePos;

	Vec2 Velocity;

	void OnGround(Map& map);

	bool IsClear() const{ return m_isClear; }

private:
	//現在のハンドル
	int m_useHandle;
	//グラフィックハンドル
	int m_handleIdle;
	int m_handleRun;
	int m_handleUp;

	//フレーム数を数える
	int m_animFrame;

	//全体フレーム
	int m_totalFrame;

	void Action(Map& map);

	//現在再生中のアニメーション
	//true:走っている　false:待機
	bool m_isRun;

	//プレイヤーの位置
	Vec2 m_pos;

	//プレイヤーが左方向を向いているか
	//true:右向き　false:左向き
	bool m_isDirLeft;

	//現在ロープが伸びているかどうか
	//true:伸びている　false:伸びていない
	bool m_isRopeMove;

	//ロープが天井に着いたかどうか
	//true:ついた　false:まだ
	bool m_isCanMove;

	//重力の処理
	void Gravity(Map& map);

	//ロープの処理
	void RopeMove(Map& map);

	bool m_isClear;
};