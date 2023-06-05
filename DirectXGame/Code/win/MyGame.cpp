#include "MyGame.h"
#include "SceneFactory.h"

void MyGame::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();

	// �V�[���t�@�N�g���[�𐶐����A�}�l�[�W���[�ɃZ�b�g
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);

	SceneManager::GetInstance()->ChangeScene("Title");
}

void MyGame::Finalize()
{
	// ���N���X�̏I��
	Framework::Finalize();
}

void MyGame::Update()
{
	//���N���X�̃A�b�v�f�[�g
	Framework::Update();
}

void MyGame::Draw()
{
	//���N���X�̃A�b�v�f�[�g
	Framework::Draw();
}
