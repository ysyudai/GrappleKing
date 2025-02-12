#pragma once
#include "Geometry.h"

class Application
{
private:
	Application();
	// 関数宣言の後に = deleteでその関数の使用を禁止できる
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	Size windowSize_;

public:
	/// <summary>
	/// シングルトンオブジェクトを返す関数
	/// 参照で返すことで、複製を防ぐ
	/// 間違えて値を返した場合、コピーコンストラクタが走るので、
	/// それをprivateにしておくことで弾くことができる
	/// </summary>
	/// <returns>Applicatoin.SingletonObject</returns>
	static Application& GetInstance();

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>true:初期化成功, false:失敗</returns>
	bool Init();

	void Run();

	void Terminate();

	const Size& GetWindowSize() const;
};

