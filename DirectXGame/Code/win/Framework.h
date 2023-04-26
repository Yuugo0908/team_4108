#pragma once
#include "DirectXCommon.h"
#include "Audio.h"
#include "Keyboard.h"
#include "Camera.h"
#include "Object3d.h"
#include "SceneManager.h"
#include "Controller.h"
#include "Light.h"
#include "Mouse.h"
#include "AbstructSceneFactory.h"
#include "SafeDelete.h"

class Framework
{
public: // �����o�֐�

	virtual ~Framework() = default;

	// ������
	virtual void Initialize();

	// �I��
	virtual void Finalize();

	// ���t���[���X�V
	virtual void Update();

	// �`��
	virtual void Draw();

	// �I���`�F�b�N
	virtual bool IsEndRequest() { return endRequest_; }

	// ���s
	void Run();

protected: // �����o�ϐ�
	bool endRequest_ = false;
	//�|�C���^�u����
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Keyboard* keyboard = nullptr;
	Mouse* mouse = nullptr;
	Controller* controller = nullptr;
	Audio* audio = nullptr;
	Camera* camera = nullptr;
	Image2d* image2d = nullptr;
	Light* light = nullptr;
	SceneManager* sceneManager = nullptr;
	AbstructSceneFactory* sceneFactory_ = nullptr;

	const int debugTextNum = 0;
	bool firstBootFlag = false;
};

