#pragma once

//2�����x�N�g���N���X
class Vec2
{
public:
	float x;
	float y;

public:
	Vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	Vec2(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}
};