#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "WinApp.h"

class Keyboard
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	Keyboard();

	~Keyboard();
public:
	// コピーコンストラクタを無効化
	Keyboard(const Keyboard& obj) = delete;
	// 代入演算子を無効化
	Keyboard& operator=(const Keyboard& obj) = delete;

public: //メンバ関数

	static Keyboard* GetInstance();
	//初期化
	bool Initialize(WinApp* win);
	//更新
	void Update();
	// キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

private: //メンバ変数
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
};