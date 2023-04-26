#pragma once
#include "AbstructSceneFactory.h"

// このゲーム用のシーン工場
class SceneFactory : public AbstructSceneFactory
{
public:
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

