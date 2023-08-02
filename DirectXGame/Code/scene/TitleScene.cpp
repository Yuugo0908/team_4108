#include "TitleScene.h"
#include <cassert>
#include "SceneManager.h"
#include <Random.h>

void TitleScene::Initialize() {

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

	if (!Image2d::LoadTexture(3, L"Resources/titlename.png"))
	{
		assert(0);
	}
	
	titleName = Image2d::Create(3, { 640.0f,360.0f }, {1.0f,1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	titleName->SetSize({ 693.0f,117.0f });
	titleName->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	takeOBJ.reset(Particle::Create(L"Resources/effectCircle.png"));
}

void TitleScene::Finalize()
{
	safe_delete(fadeTex);
	safe_delete(backGround);
}

void TitleScene::Update()
{
	if (!fadeIn && keyboard->TriggerKey(DIK_SPACE) || controller->GetPadState(Controller::State::A, Controller::Type::TRIGGER))
	{
		// �^�C�g���J��
		Audio::GetInstance()->PlayWave("Resources/SE/se1.wav", 0, 0.1f);
		fadeIn = true;
	}

	if (fadeIn)
	{
		alpha += 0.02f;
		fadeTex->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
		titleName->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		if (alpha >= 1.0f)
		{
			fadeIn = false;
			backGround->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			{
				SceneManager::GetInstance()->ChangeScene("Game");
			}
		}
	}
}

void TitleScene::Draw()
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
	takeOBJ->Draw(DirectXCommon::GetInstance()->GetCommandList());

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��

#pragma region �O�i�摜�`��
	// �O�i�摜�`��O����
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// �O�i�摜�̕`��
	
	fadeTex->Draw();
	titleName->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(DirectXCommon::GetInstance()->GetCommandList());
	// �摜�`��㏈��
	Image2d::PostDraw();
#pragma endregion �O�i�摜�`��
}