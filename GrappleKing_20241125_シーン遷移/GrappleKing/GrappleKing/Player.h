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

	//ステージをクリアしたかどうかの情報を取得
	int IsClearStage1() const { return m_isClearStage1; }
	int IsClearStage2() const { return m_isClearStage2; }
	int IsClearStage3() const { return m_isClearStage3; }

	//1フレーム前のプレイヤーの座標を覚えておく
	Vec2 LastPos;

	int LastPosBottom;

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
	int m_handleFall;

	//ステージクリアのグラフィックハンドル
	int m_handleStageClear[4];

	//フレーム数を数える
	int m_animFrame;

	//全体フレーム
	int m_totalFrame;

	void Action(Map& map);

	//現在再生中のアニメーション
	//true:走っている　false:待機
	//bool m_isRun;

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

	//ステージを全てクリアしたどうか
	bool m_isClear;

	//着地した瞬間の縦横比(倍率)
	//Vec2 m_extRate;

	int left;
	int top;
	int right;
	int bottom;

	Vec2 m_posPlus;

	int groundChipTop;
	bool isGroundChipHit;

	int plusFrame;

	bool onJustGround;

	// 地面の判定を無効にするフラグとカウンタ
	bool m_ignoreGroundCollision;
	int m_ignoreGroundCollisionFrameCount;

	// 着地後の無効フレームをカウントするフラグとカウンタ
	bool m_landingCooldown;
	int m_landingCooldownFrameCount;

	int poyonnFrame;

	bool m_isClearStage1;
	bool m_isClearStage2;
	bool m_isClearStage3;

	int stage1frame;

	int m_clearGraphSizeX;
	int m_clearGraphSizeY;

	int m_clearGraphPosX;
};