#include "TitlePlayer.h"
#include "DxLib.h"
#include <cassert>
#include "game.h"

namespace
{
	constexpr int kSpeedY = 4;
}

TitlePlayer::TitlePlayer():
	m_playerHandle(-1),
	m_pos{128,Game::kScreenHeight + 100 },
	m_linePos(m_pos)
{
	m_playerHandle = LoadGraph("data/Flog/ropeMove.png");
	assert(m_playerHandle != -1);
}

TitlePlayer::~TitlePlayer()
{
	DeleteGraph(m_playerHandle);
}

void TitlePlayer::Update()
{
	if (m_linePos.y < -20)
	{
		m_pos.y -= kSpeedY;
	}
	else
	{
		m_linePos.y -= kSpeedY * 1.5f;
	}

	if (m_pos.y < -20)
	{
		int x = GetRand(10);
		m_pos.x = 100 * x;
		m_pos.y = Game::kScreenHeight + 100;
		m_linePos = m_pos;
	}
}

void TitlePlayer::Draw()
{
	DrawRotaGraph(m_pos.x, m_pos.y, 2, 0, m_playerHandle, true);
	DrawLine(m_pos.x, m_pos.y, m_linePos.x, m_linePos.y, 0xffffff, 0);
}
