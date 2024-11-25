#pragma once
class Player
{
public:
	Player();
	~Player();

	void Update();
	void Draw();
private:
	int x;
	int y;
};