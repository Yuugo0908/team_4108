#include "Mouse.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")

Mouse* Mouse::GetInstance()
{
	static Mouse instance;

	return &instance;
}

bool Mouse::Initialize(WinApp* win_app)
{
	HRESULT result = S_FALSE;

	assert(!dinput);
	assert(win_app);

	this->winApp = win_app;

	// DirectInput�I�u�W�F�N�g�̐���	
	result = DirectInput8Create(win_app->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(win_app->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

void Mouse::Update()
{
	HRESULT result;
	// �}�E�X����J�n
	result = devMouse->Acquire();

	// �O��̓��͂�ۑ�
	mouseStatePre = mouseState;

	// �}�E�X�̓���
	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
}

void Mouse::CursorLimit()
{
	hwnd = winApp->GetHwnd();
	GetClientRect(hwnd, &rcClient);

	ptClientUL.x = rcClient.left + 100;
	ptClientUL.y = rcClient.top + 100;
	ptClientLR.x = rcClient.right - 100;
	ptClientLR.y = rcClient.bottom - 100;
	ClientToScreen(hwnd, &ptClientUL);
	ClientToScreen(hwnd, &ptClientLR);

	SetRect(&rcClient, ptClientUL.x, ptClientUL.y,
		ptClientLR.x, ptClientLR.y);

	ClipCursor(&rcClient);
}

bool Mouse::PushMouseLeft()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[0])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Mouse::PushMouseMiddle()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[2])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Mouse::PushMouseRight()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[1])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Mouse::TriggerMouseLeft()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Mouse::TriggerMouseMiddle()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Mouse::TriggerMouseRight()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

Mouse::MouseMove Mouse::GetMouseMove()
{
	MouseMove tmp = {};
	tmp.MouseX = mouseState.lX;
	tmp.MouseY = mouseState.lY;
	tmp.MouseZ = mouseState.lZ;
	return tmp;
}
