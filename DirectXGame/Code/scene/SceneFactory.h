#pragma once
#include "AbstructSceneFactory.h"

// ���̃Q�[���p�̃V�[���H��
class SceneFactory : public AbstructSceneFactory
{
public:
	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

