#pragma once

#include "BaseScene.h"
#include <string>

// �V�[���H��(�T�O)
class AbstructSceneFactory
{
public:
	// ���z�f�X�g���N�^
	virtual ~AbstructSceneFactory() = default;

	// �V�[������
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};
