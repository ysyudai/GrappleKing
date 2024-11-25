#pragma once
#include "Vec2.h"

class Player
{
public:
	Player();
	~Player();

	void Update();
	void Draw();

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
};