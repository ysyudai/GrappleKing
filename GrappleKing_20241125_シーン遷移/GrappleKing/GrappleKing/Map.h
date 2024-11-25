#pragma once
class Bg
{
public:
	Bg();  // コンストラクタ
	~Bg(); // デストラクタ

	void Init();    // 初期化
	void Update();  // 更新処理
	void Draw();    // 描画処理
	void End();    // 描画処理

private:
	// グラフィックのハンドル
	int m_handle;

	// グラフィックにチップがいくつ含まれているか
	int m_graphChipNumX;
	int m_graphChipNumY;

};