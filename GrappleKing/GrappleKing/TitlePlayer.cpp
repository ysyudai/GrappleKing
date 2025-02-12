#include "TitlePlayer.h"
#include "DxLib.h"
#include <cassert>
#include "game.h"

namespace
{
	constexpr int kGraphWidth = 64;

	constexpr int kSpeedY = 4;
}

TitlePlayer::TitlePlayer():
	m_playerHandle(-1),
	m_pos{0,-64},
	m_posOver(0),
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
	//m_posOver = GetRand(720*6);
	m_posOver = 0;

	if (m_linePos.y < -(m_posOver + 15))
	{
		m_pos.y -= kSpeedY;
	}
	else
	{
		m_linePos.y -= kSpeedY * 1.5f;
	}

	if (m_pos.y < -(m_posOver + 15))
	{
		int x = GetRand(1240);
		int y = GetRand(720);
		m_pos.x = static_cast<float>(x + 5);
		m_pos.y = static_cast<float>(y + Game::kScreenHeight);
		m_linePos.x = static_cast<float>(m_pos.x);
		m_linePos.y = m_pos.y;
	}
}

void TitlePlayer::Draw()
{
	DrawLine(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(m_linePos.x),
		static_cast<int>(m_linePos.y), 0xffffff, 0);
	DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		2, 0, m_playerHandle, true);
}
