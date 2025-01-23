#include "SceneController.h"
#include <memory>
#include "SceneTitle.h"
#include "SceneMain.h"

SceneController::SceneController()
{
	// 一番最初のシーンだけは割り当てる
	// 自分自身のインスタンスを渡してあげる
	//ChangeScene(std::make_shared<SceneTitle>(*this));
	ChangeScene(std::make_shared<SceneTitle>(*this));
}

void SceneController::Update()
{
	// 一番上にあるやつだけUpdate
	scenes_.back()->Update();
}

void SceneController::Draw()
{
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<Scene> scene)
{
	// スマポなのでこれでオッケー
	// 一番乗りなら
	if (scenes_.empty())
	{
		scenes_.push_back(scene);
	}
	// 先客がいた場合、どいてもらう
	else
	{
		// 末尾の要素の参照そのものに
		scenes_.back() = scene;
	}
	// ↑前のオブジェクトは参照を失うので消去される
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	scenes_.push_back(scene);
}

void SceneController::PopScene()
{
	// 実行すべきシーンがなくなるので許可しない
	if (scenes_.size() == 1) return;

	// 末尾を取り除く
	scenes_.pop_back();
}
