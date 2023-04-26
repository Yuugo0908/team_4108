#include "Keyboard.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Keyboard::Keyboard()
{

}

Keyboard::~Keyboard()
{

}

Keyboard* Keyboard::GetInstance()
{
	static Keyboard instance;

	return &instance;
}

bool Keyboard::Initialize(WinApp* win)
{
	HRESULT result = S_FALSE;

	// �������i��x�����s�������j
	result = DirectInput8Create
	(
		win->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel
	(
		win->GetHwnd() , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

void Keyboard::Update()
{
	HRESULT result;

	// �L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	// �O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));
	// �S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool Keyboard::PushKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0�łȂ���Ή����Ă���
	if (key[keyNumber])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Keyboard::TriggerKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}