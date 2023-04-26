#pragma once
#pragma comment(lib,"XInput.lib")
#include <windows.h>
#include <Xinput.h>


class Controller
{
private:
	Controller();

	~Controller();
public:
	enum class State
	{
		// デジタル
		A = 1,	//	Aボタン
		B,	//	Bボタン
		X,	//	Xボタン
		Y,	//	Yボタン
		LB,  //	LB
		RB,	//	RB
		BACK,	//	Back
		START,	//	Start
		UP,		//	十字キーの上
		DOWN,	//	十字キーの下
		LEFT,	//	十字キーの左
		RIGHT,	//	十字キーの右
		LEFT_THUMB,		//	左スティック
		RIGHT_THUMB,	//	右スティック

		// アナログ
		LT,	//	左トリガー
		RT,	//	右トリガー
		LEFT_U_STICK,	//	左スティックの上
		LEFT_D_STICK,	//	左スティックの下
		LEFT_L_STICK,	//	左スティックの左
		LEFT_R_STICK,	//	左スティックの右
		RIGHT_U_STICK,	//	右スティックの上
		RIGHT_D_STICK,	//	右スティックの下
		RIGHT_L_STICK,	//	右スティックの左
		RIGHT_R_STICK,	//	右スティックの右
	};
	enum class Mode
	{
		BUTTON,
		STICK,
	};

	enum class Type
	{
		NONE,
		PUSH,
		TRIGGER,
	};

public:
	static Controller* GetInstance();
	bool Update();
	bool GetPadState(State p_state, Type p_type);
	void Vibration();
	bool Detection();

private:
	bool GetButtonState(XINPUT_STATE state, State p_state, Type p_type);
	bool GetStickState(XINPUT_STATE state, State p_state);
	bool PushButton(State& p_state);
	bool TriggerButton(XINPUT_STATE state, State& p_state);
	void CheckMode(State p_state);

	// ボタンを押したか、スティックを操作したかの判別
	Mode mode{};
	// コントローラーの状態
	XINPUT_STATE state{};
	XINPUT_STATE statePre{};
	// コントローラーの検出
	DWORD dwResult = {};

	State stateNum{};
	WORD buttonNum{};
	// 振動
	XINPUT_VIBRATION vibration{};
	// 振動するフレーム数
	int vCount = 0;
	// コントローラーの接続番号
	const int controllerNum = 0;
};