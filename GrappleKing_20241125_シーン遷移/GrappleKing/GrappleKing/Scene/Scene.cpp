#include "Scene.h"

// 基底クラスでシーンコントローラーを持たせておく
Scene::Scene(SceneController& cont) : controller_(cont)
{
}