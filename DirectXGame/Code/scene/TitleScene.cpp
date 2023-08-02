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
		// タイトル遷移
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
#pragma region 背景画像描画
	// 背景画像描画前処理
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	backGround->Draw();

	// 画像描画後処理
	Image2d::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion 背景画像描画

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// 3Dオブクジェクトの描画
	takeOBJ->Draw(DirectXCommon::GetInstance()->GetCommandList());

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画

#pragma region 前景画像描画
	// 前景画像描画前処理
	Image2d::PreDraw(DirectXCommon::GetInstance()->GetCommandList());

	// 前景画像の描画
	
	fadeTex->Draw();
	titleName->Draw();
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(DirectXCommon::GetInstance()->GetCommandList());
	// 画像描画後処理
	Image2d::PostDraw();
#pragma endregion 前景画像描画
}