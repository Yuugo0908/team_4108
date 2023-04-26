#pragma once
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
class Mouse
{
public:
	// �}�E�X�̍��W
	struct MouseMove
	{
		long MouseX;
		long MouseY;
		long MouseZ;
	};

private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // �ÓI�����o�ϐ�
	static Mouse* GetInstance();

public: // �����o�֐�

	// ������
	bool Initialize(WinApp* win_app);

	// �X�V
	void Update();

	// �}�E�X�̈ړ�����
	void CursorLimit();

	// �L�[�̍��N���b�N���`�F�b�N
	bool PushMouseLeft();

	// �L�[�̒��{�^���������`�F�b�N
	bool PushMouseMiddle();

	// �L�[�̉E�N���b�N���`�F�b�N
	bool PushMouseRight();

	// �L�[�̍��N���b�N�g���K�[���`�F�b�N
	bool TriggerMouseLeft();

	// �L�[�̒��{�^���g���K�[���`�F�b�N
	bool TriggerMouseMiddle();

	// �L�[�̉E�N���b�N�g���K�[���`�F�b�N
	bool TriggerMouseRight();

	// �}�E�X�ړ��ʂ��擾
	MouseMove GetMouseMove();

private: // �����o�ϐ�

	WinApp* winApp = nullptr;

	// �f�o�C�X�֘A
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	// �E�B���h�E���W�擾�p
	HWND hwnd = NULL;
	RECT rcClient;
	POINT ptClientUL;
	POINT ptClientLR;
};

