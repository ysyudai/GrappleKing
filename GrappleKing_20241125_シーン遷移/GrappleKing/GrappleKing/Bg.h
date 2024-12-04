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
private:
	// グラフィックのハンドル
	int m_handle;
	int m_bgHandle;

	// グラフィックにチップがいくつ含まれているか
	int m_graphChipNumX;
	int m_graphChipNumY;

	Player* m_player;

	void Map();
};