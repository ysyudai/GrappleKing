#include "Bg.h"
#include "DxLib.h"
#include "game.h"
#include"Player.h"

namespace
{
	// マップチップの1つのサイズ
	constexpr int kChipWidth = 16;
	constexpr int kChipHeight = 16;

	// マップに敷き詰めるチップ数
	constexpr int kChipNumX = Game::kScreenWidth / kChipWidth;
	constexpr int kChipNumY = Game::kScreenHeight / kChipHeight;

	//マップの配置データ　2次元配列で持つ
	//マップ番号に-1が指定されている場合は何も表示しない
	constexpr int kChipSetData[kChipNumY][kChipNumX] =
	{
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,/**/-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
		{ 7, 7, 7, 7, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
		{29,29,29,29,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
		{29,29,29,29,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
		{29,29,29,29,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
		{29,29,29,29,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
		{29,29,29,29,29,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
		{29,29,29,29,29, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29},
		{28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30},
		{28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30},
		{28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30},
		{28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30},
		{28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30},
		{28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30},
		{50,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,52},
	};
}

Bg::Bg(Player* pPlayer) :
	m_handle(-1),
	m_bgHandle(-1),
	m_graphChipNumX(0),
	m_graphChipNumY(0),
	m_chipNo(0),
	m_isChipHit(false)
{
	m_player = pPlayer;
	m_handle = LoadGraph("data/image/map.png");
	m_bgHandle = LoadGraph("data/image/background.png");
	
	// 読み込んだグラフィックにチップが何個あるかを教えておく
	int graphWidth = 0;
	int graphHeight = 0;
	GetGraphSize(m_handle, &graphWidth, &graphHeight);

	m_graphChipNumX = graphWidth / kChipWidth;
	m_graphChipNumY = graphWidth / kChipHeight;
}

Bg::~Bg()
{
	DeleteGraph(m_handle);
}

void Bg::Update()
{
	m_isChipHit = false;

	IsWallHit = false;

	Map();	
}

void Bg::Draw()
{
	// マップチップを表示するテスト
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			//データから配置するチップを決定する
			int chipNo = kChipSetData[y][x];

			if (chipNo < 0)
			{
				//continueは繰り返し分(for,while)の中で使用する命令
				//continueが呼ばれたら以降の繰り返し処理は行わず、次のループに移行する
				continue;
			}

			// 0から始まる通し番号を
			// 上から何個目、左から何個目なのか、という情報に変換する必要がある
			// グラフィックに何個チップが含まれているか、という情報を使用して
			// 計算で求める
			int indexX = chipNo % m_graphChipNumX; // 左から何個目のチップか
			int indexY = chipNo / m_graphChipNumX; // 上から何個目のチップか

			// チップ番号から切り出し位置を計算する
			int cutX = indexX * kChipWidth;
			int cutY = indexY * kChipHeight;

			DrawRectGraph(x * kChipWidth, y * kChipHeight,
				cutX, cutY, kChipWidth, kChipHeight,
				m_handle, true);
		}
	}
	//画面全体を空色で埋め尽くす
	//DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(160, 216, 239), true);
}

void Bg::Map()
{
	// マップチップを表示するテスト
	for (int y = 0; y < kChipNumY; y++)
	{
		for (int x = 0; x < kChipNumX; x++)
		{
			//データから配置するチップを決定する
			int chipNo = kChipSetData[y][x];

			if (chipNo < 0)
			{
				//continueは繰り返し分(for,while)の中で使用する命令
				//continueが呼ばれたら以降の繰り返し処理は行わず、次のループに移行する
				continue;
			}

			int GetChipLeft = x * kChipWidth;
			int GetChipTop = y * kChipHeight;
			int GetChipRight = x * kChipWidth + kChipWidth;
			int GetChipBottom = y * kChipHeight + kChipHeight;

			//フラグ判定
			bool isPlayerLeft = (m_player->GetRight() < GetChipLeft);//Playerがチップの左側にいる
			bool isPlayerRight = (GetChipRight < m_player->GetLeft());//Playerがチップの右側にいる。
			bool isPlayerTop = (m_player->GetBottom() < GetChipTop);//Playerがチップの上にいる。
			bool isPlayerBottom = (GetChipBottom < m_player->GetTop());//Playerがチップの下にいる。		

			//地面に当たった時(7は地面)
			if (chipNo == 7)
			{
				if (!(isPlayerLeft || isPlayerRight || isPlayerTop || isPlayerBottom))
				{
					m_isChipHit = true;

					Vec2 NextPos = m_player->GetPos();
					NextPos.y = GetChipTop + (m_player->GetBottom() - m_player->GetPos().y);
					m_player->SetPos(NextPos);
				}

				/*if (x * kChipWidth < m_player->GetLeft() && m_player->GetLeft() < x * kChipWidth + kChipWidth &&
					y * kChipHeight < m_player->GetTop() && m_player->GetTop() < y * kChipHeight + kChipHeight)
				{
					DrawString(10,160,"hit\n",0xffffff);
				}
				if (x * kChipWidth < m_player->GetRight() && m_player->GetRight() < x * kChipWidth + kChipWidth &&
					y * kChipHeight < m_player->GetTop() && m_player->GetTop() < y * kChipHeight + kChipHeight)
				{
					DrawString(10, 160, "hit\n", 0xffffff);
				}
				if (x * kChipWidth < m_player->GetLeft() && m_player->GetLeft() < x * kChipWidth + kChipWidth&&
					y * kChipHeight + kChipHeight < m_player->GetBottom() && m_player->GetBottom() < y * kChipHeight)
				{

				}*/
			}
			//壁に当たった時(29は壁)
			if (chipNo == 29)
			{
				//右に壁がある場合
				if ((m_player->LastPos.x < GetChipLeft) && !(isPlayerLeft || isPlayerRight || isPlayerTop || isPlayerBottom))
				{
					IsWallHit = true;
					Vec2 NextPos = m_player->GetPos();
					NextPos.x = GetChipLeft - (m_player->GetRight() - m_player->GetPos().x) - 0.001f;
					m_player->SetPos(NextPos);
				}
				//左に壁がある場合
				if ((GetChipRight < m_player->LastPos.x) && !(isPlayerLeft || isPlayerRight || isPlayerTop || isPlayerBottom))
				{
					IsWallHit = true;
					Vec2 NextPos = m_player->GetPos();
					NextPos.x = GetChipRight + (m_player->GetRight() - m_player->GetPos().x) + 0.001f;
					m_player->SetPos(NextPos);
				}
			}
		}
	}
}
