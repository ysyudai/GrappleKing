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

	/*���Z�q�I�[�o�[���[�h*/
	//�P�����Z�q
	Vec2 operator+(const Vec2& right)
	{
		Vec2 ans;
		ans.x = this->x + right.x;
		ans.y = this->y + right.y;		
		return ans;
	}
	Vec2 operator-(const Vec2& right)
	{
		Vec2 ans;
		ans.x = this->x - right.x;
		ans.y = this->y - right.y;
		return ans;
	}
	Vec2 operator*(const float& right)
	{
		Vec2 ans;
		ans.x = this->x * right;
		ans.y = this->y * right;
		return ans;
	}
	Vec2 operator/(const float& right)
	{
		Vec2 ans;
		ans.x = this->x / right;
		ans.y = this->y / right;
		return ans;
	}
	void operator+=(const Vec2& right)
	{
		this->x += right.x;
		this->y += right.y;
	}
	void operator-=(const Vec2& right)
	{
		this->x -= right.x;
		this->y -= right.y;
	}
};