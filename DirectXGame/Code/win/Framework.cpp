#include "Framework.h"

void Framework::Initialize()
{
	// �Q�[���E�B���h�E�̍쐬
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX����������
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

	//���͂̏�����
	keyboard = Keyboard::GetInstance();
	keyboard->Initialize(win);
	controller = Controller::GetInstance();
	mouse = Mouse::GetInstance();
	mouse->Initialize(win);

	// �I�[�f�B�I�̏�����
	audio = Audio::GetInstance();
	audio->Initialize();

	// �J����������
	camera = Camera::GetInstance();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	// �摜�ÓI������
	if (!Image2d::StaticInitialize(dxCommon->GetDevice()))
	{
		assert(0);
	}

	// 3D�I�u�W�F�N�g�ÓI������
	if (!Object3d::StaticInitialize(dxCommon->GetDevice(), camera))
	{
		assert(0);
	}
	// ���C�g�ÓI������
	if (!Light::StaticInitialize(dxCommon->GetDevice()))
	{
		assert(0);
	}


	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Image2d::LoadTexture(debugTextNum, L"Resources/debugfont.png"))
	{
		assert(0);
	}

	// �f�o�b�O�e�L�X�g������
	DebugText::GetInstance()->Initialize(debugTextNum);
}

void Framework::Finalize()
{
	//���
	safe_delete(image2d);
	safe_delete(light);
	safe_delete(sceneFactory_);
	firstBootFlag = false;

	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();
	safe_delete(win);
}

void Framework::Update()
{
	// ���b�Z�[�W����
	if (win->ProcessMessage() || keyboard->TriggerKey(DIK_ESCAPE))
	{
		// �Q�[�����[�v�𔲂���
		endRequest_ = true;
		return;
	}

	// ��A�N�e�B�u��ԂȂ�X�V���Ȃ�(����N���͏���)
	if (firstBootFlag && win->GetHwnd() != GetForegroundWindow())
	{
		ClipCursor(NULL);
		return;
	}

	// ���͊֘A�̖��t���[������
	keyboard->Update();
	controller->Update();
	mouse->Update();
	// �Q�[���V�[���̖��t���[������
	SceneManager::GetInstance()->Update();
}

void Framework::Draw()
{
	// �`��J�n
	dxCommon->PreDraw();
	// �Q�[���V�[���̕`��
	SceneManager::GetInstance()->Draw();
	// �`��I��
	dxCommon->PostDraw();
}

void Framework::Run()
{
	// �Q�[���̏�����
	Initialize();

	while (true)
	{
		// ���t���[���X�V
		Update();

		if (IsEndRequest())
		{
			break;
		}

		Draw();
		firstBootFlag = true;
	}

	Finalize();
}
