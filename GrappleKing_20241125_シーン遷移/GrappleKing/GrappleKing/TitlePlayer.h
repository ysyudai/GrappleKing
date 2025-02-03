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

	//�v���C���[�̍��W
	Vec2 m_pos;
	//���C���̏I�_���W
	Vec2 m_linePos;
};