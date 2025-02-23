#pragma once

class Input;
class SceneController;

class Scene
{
protected:
	// 各状態が切り替えるために
	SceneController& controller_;

public:
	Scene(SceneController& cont);
	virtual ~Scene() {};

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	/// <param name="input">入力状態</param>
	virtual void Update() = 0;

	/// <summary>
	///  描画
	/// </summary>
	virtual void Draw() = 0;
};

