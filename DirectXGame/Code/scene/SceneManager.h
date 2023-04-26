#pragma once
#include "BaseScene.h"
#include "AbstructSceneFactory.h"

class SceneManager
{
public:
	// �f�X�g���N�^
	~SceneManager();

	static SceneManager* GetInstance();
	// �X�V
	void Update();
	//�`��
	void Draw();
	// �V�[���t�@�N�g���[�̃Z�b�^�[
	void SetSceneFactory(AbstructSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
	// ���̃V�[����\��
	void ChangeScene(const std::string& sceneName);

private:
	// ���̃V�[��(���s���̃V�[��)
	BaseScene* scene_ = nullptr;
	// ���̃V�[��
	BaseScene* nextScene_ = nullptr;
	// �؂�Ă���V�[���t�@�N�g���[
	AbstructSceneFactory* sceneFactory_ = nullptr;
};

