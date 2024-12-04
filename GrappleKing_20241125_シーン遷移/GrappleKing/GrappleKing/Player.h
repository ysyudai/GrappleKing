#pragma once
#include "Vec2.h"

class Bg;

class Player
{
public:
	Player();
	~Player();

	void Update(Bg& bg);
	void Draw();

	//プレイヤーの情報を取得する
	float GetLeft() const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom() const;

	Vec2 GetPos() const;

	//プレイヤーの情報を変更する
	void SetPos(const Vec2 pos);

	//1フレーム前のプレイヤーの座標を覚えておく
	Vec2 LastPos;

	bool IsClear;

	Vec2 Velocity;

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

	void Action();

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

	//地面についているかどうか
	bool m_isOnStage;

	//ロープが伸び始める座標
	Vec2 m_linePos;

	void Gravity(Bg& bg);
};