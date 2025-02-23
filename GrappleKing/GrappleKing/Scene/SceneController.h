#pragma once
// スマポ使う
#include <memory>
#include <list>

class Scene;

/// <summary>
/// 各シーンを管理するクラス
/// 管理はするが、遷移を管理はしない
/// インターフェイスを提供するクラス
/// </summary>
class SceneController
{
private:
	std::list<std::shared_ptr<Scene>> scenes_; // 現在実行中のシーン スタック構造

public:
	SceneController();

	/// <summary>
	/// Applicationから呼ばれる関数。現在のシーンのUpdateをそのまま実行。
	/// </summary>
	/// <param name="input"></param>
	void Update();
	/// <summary>
	/// Applicationから呼ばれる関数。現在のシーンのDrawをそのまま実行。
	/// </summary>
	void Draw();
	/// <summary>
	/// 次の状態をセットする(セットするのは各状態の役割)
	/// 各シーンがコントローラーを見て切り替えさせる？
	/// </summary>
	/// <param name="">次の状態のポインタ（スマポ）</param>
	void ChangeScene(std::shared_ptr<Scene>);

	/// <summary>
	/// 現在実行中のシーンの上に別のシーンを乗っける
	/// </summary>
	/// <param mane="scene">乗っけたいシーン</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// 現在、一番上にある実行中のシーンを取り除く
	/// </summary>
	void PopScene();
};

