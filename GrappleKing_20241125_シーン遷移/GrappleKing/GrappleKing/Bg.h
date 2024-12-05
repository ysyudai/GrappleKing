#pragma once

class Player;

class Bg
{
public:
	Bg(Player* pPlayer);  // コンストラクタ
	~Bg(); // デストラクタ

	void Update();  // 更新処理
	void Draw();    // 描画処理

	int m_chipNo;

	bool m_isChipHit;

	bool IsWallHit;

	int GetChipLeft()const { return m_ChipLeft; }
	int GetChipRight()const { return m_ChipRight; }
	int GetChipTop()const { return m_ChipTop; }
	int GetChipBottom()const { return m_ChipBottom; }

	int GetChipTopGround()const { return m_chipTopGround; }
	int GetChipTopWall()const { return m_chipLeftWall; }
	int GetChipTopWall()const { return m_chipRightWall; }

private:
	// グラフィックのハンドル
	int m_handle;
	int m_bgHandle;

	// グラフィックにチップがいくつ含まれているか
	int m_graphChipNumX;
	int m_graphChipNumY;

	Player* m_player;

	void Map();

	int m_ChipLeft;
	int m_ChipRight;
	int m_ChipTop;
	int m_ChipBottom;

	int m_chipTopGround;
	int m_chipLeftWall;
	int m_chipRightWall;
};