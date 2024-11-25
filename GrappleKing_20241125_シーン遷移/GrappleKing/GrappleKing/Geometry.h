#pragma once
// ゲーム中で使用する幾何学情報の型を定義する場所

/// <summary>
/// ベクトル(2D)
/// </summary>
struct Vector2
{
	float x, y;
};

// 別名を作っただけ
using Position2 = Vector2;

/// <summary>
/// サイズ(2Dゲームで使用するピクセルサイズ)
/// </summary>
struct Size
{
	int w, h;
};

