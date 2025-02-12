#pragma once
#include "Vec2.h"

class TitlePlayer
{
public:
	TitlePlayer();
	~TitlePlayer();
	void Update();
	void Draw();
private:
	int m_playerHandle;

	//プレイヤーの座標
	Vec2 m_pos;

	//プレイヤーの出現位置
	int m_posOver;

	//ラインの終点座標
	Vec2 m_linePos;
};