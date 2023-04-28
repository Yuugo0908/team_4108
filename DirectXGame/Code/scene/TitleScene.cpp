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

	jumpEffect.reset(Particle::Create(L"Resources/effectCircle.png"));
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
		fadeIn = true;
	}

	if (fadeIn)
	{
		alpha += 0.02f;
		fadeTex->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
		if (alpha >= 1.0f)
		{
			fadeIn = false;
			backGround->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
			{
				SceneManager::GetInstance()->ChangeScene("Game");
			}
		}
	}

	if (keyboard->PushKey(DIK_RETURN))
	{
		for (int i = 0; i < 6; i++)
		{
			XMFLOAT3 pos = { 0, 0, 0 };
			XMFLOAT3 vel = { 0, 0, 0 };
			XMFLOAT3 acc = { static_cast<float>(Random::GetRanNum(0, 100) - 50) / 1000, static_cast<float>(Random::GetRanNum(0, 10)) / 1000, 0 };
			float startScale = 0.0f;
			float endScale = 0.8f;
			XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 0.1f };
			XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			jumpEffect->Add(15, pos, vel, acc, startScale, endScale, startColor, endColor);
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
	jumpEffect->Draw(DirectXCommon::GetInstance()->GetCommandList());

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
#pragma endregion 3D�I�u�W�F�N�g�`��

#pragma region �O�i�摜�`��
	// �O�i�摜�`��O����
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// �O�i�摜�̕`��
	
	fadeTex->Draw();

	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(DirectXCommon::GetInstance()->GetCommandList());
	// �摜�`��㏈��
	Image2d::PostDraw();
#pragma endregion �O�i�摜�`��
}