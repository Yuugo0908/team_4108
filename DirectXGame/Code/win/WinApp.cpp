#include "WinApp.h"
#include <imgui_impl_win32.h>

#pragma comment(lib, "winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const wchar_t WinApp::windowClassName[] = L"VoxelSpider";

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return 1;
	}

	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // �W���̏������s��
}

void WinApp::CreateGameWindow()
{
	// �V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);

	// �E�B���h�E�N���X�̐ݒ�
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W��
	wndClass.lpszClassName = windowClassName; // �E�B���h�E�N���X��
	wndClass.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	RegisterClassEx(&wndClass); // �E�B���h�E�N���X��OS�ɓo�^

	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

	HWND hDwnd = GetDesktopWindow();
	RECT desktop_rect;
	GetWindowRect(hDwnd, &desktop_rect);

	int desktop_width = (desktop_rect.right - (wrc.right - wrc.left)) / 2;
	int desktop_height = (desktop_rect.bottom - (wrc.bottom - wrc.top)) / 3;

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow
	(
		wndClass.lpszClassName, // �N���X��
		windowClassName, // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW, // �^�C�g���o�[�Ƌ��E��������E�B���h�E
		desktop_width, // �\��X���W�i��ʒ����ɃZ�b�g�j
		desktop_height, // �\��Y���W�i��ʒ����������ɃZ�b�g�j
		wrc.right - wrc.left, // �E�B���h�E����
		wrc.bottom - wrc.top, // �E�B���h�E�c��
		nullptr, // �e�E�B���h�E�n���h��
		nullptr, // ���j���[�n���h��
		wndClass.hInstance, // �Ăяo���A�v���P�[�V�����n���h��
		nullptr // �I�v�V����
	);

	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::TerminateGameWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool WinApp::ProcessMessage()
{
	MSG msg{}; // ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}

	return false;
}