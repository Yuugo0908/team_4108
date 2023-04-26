#include "GameClearScene.h"
#include "SceneManager.h"

void GameClearScene::Initialize()
{
	// �Q�[���N���A�摜�ǂݍ���
	if (!Image2d::LoadTexture(GameClearNum, L"Resources/GameClear.png"))
	{
		assert(0);
	}
	GameClear = Image2d::Create(GameClearNum, { 0.0f,0.0f });
	GameClear->SetSize({ 1280.0f,720.0f });

	if (!Image2d::LoadTexture(fadeNum, L"Resources/fade.png"))
	{
		assert(0);
	}
	fadeTex = Image2d::Create(fadeNum, { 0.0f,0.0f });
	fadeTex->SetSize({ 1280.0f,720.0f });
	fadeTex->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	if (!Image2d::LoadTexture(backNum, L"Resources/backGround.png"))
	{
		assert(0);
	}
	backGround = Image2d::Create(backNum, { 0.0f,0.0f });
	backGround->SetSize({ 1280.0f,720.0f });
	backGround->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

void GameClearScene::Finalize()
{
	safe_delete(GameClear);
	safe_delete(fadeTex);
	safe_delete(backGround);
}

void GameClearScene::Update()
{
	ClipCursor(NULL);
	if (fadeFlag == false && alpha > 0.0f)
	{
		alpha -= 0.02f;
	}
	else
	{
		if (keyboard->TriggerKey(DIK_SPACE) || controller->GetPadState(Controller::State::A, Controller::Type::TRIGGER))
		{
			SceneManager::GetInstance()->ChangeScene("Title");
		}
	}
	fadeTex->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
}

void GameClearScene::Draw()
{
#pragma region �w�i�摜�`��
	// �w�i�摜�`��O����
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	backGround->Draw();

	// �摜�`��㏈��
	Image2d::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion �w�i�摜�`��

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// 3D�I�u�N�W�F�N�g�̕`��

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��

#pragma region �O�i�摜�`��
	// �O�i�摜�`��O����
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// �O�i�摜�̕`��
	GameClear->Draw();
	fadeTex->Draw();

	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(DirectXCommon::GetInstance()->GetCommandList());
	// �摜�`��㏈��
	Image2d::PostDraw();
#pragma endregion �O�i�摜�`��
}
