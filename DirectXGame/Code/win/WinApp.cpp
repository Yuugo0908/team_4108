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

	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}

void WinApp::CreateGameWindow()
{
	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	// ウィンドウクラスの設定
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャ
	wndClass.lpszClassName = windowClassName; // ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	RegisterClassEx(&wndClass); // ウィンドウクラスをOSに登録

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	HWND hDwnd = GetDesktopWindow();
	RECT desktop_rect;
	GetWindowRect(hDwnd, &desktop_rect);

	int desktop_width = (desktop_rect.right - (wrc.right - wrc.left)) / 2;
	int desktop_height = (desktop_rect.bottom - (wrc.bottom - wrc.top)) / 3;

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow
	(
		wndClass.lpszClassName, // クラス名
		windowClassName, // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // タイトルバーと境界線があるウィンドウ
		desktop_width, // 表示X座標（画面中央にセット）
		desktop_height, // 表示Y座標（画面中央から上よりにセット）
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		wndClass.hInstance, // 呼び出しアプリケーションハンドル
		nullptr // オプション
	);

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::TerminateGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool WinApp::ProcessMessage()
{
	MSG msg{}; // メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}