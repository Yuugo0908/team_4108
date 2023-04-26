#include "Controller.h"
#include <DebugText.h>
#include <WinApp.h>

Controller::Controller()
{

}

Controller::~Controller()
{

}

Controller* Controller::GetInstance()
{
	static Controller instance;

	return &instance;
}

bool Controller::Update()
{
	statePre = state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	// 情報取得
	dwResult = XInputGetState(controllerNum, &state);

	if (vCount <= 0)
	{
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		XInputSetState(controllerNum, &vibration);
	}
	else
	{
		vCount--;
	}
	
	if (dwResult == ERROR_SUCCESS)
	{
		return true;
	}
	return false;
}


bool Controller::GetPadState(Controller::State p_state, Type p_type)
{
	// 情報取得
	dwResult = XInputGetState(controllerNum, &state);

	if (dwResult == ERROR_SUCCESS)
	{
		CheckMode(p_state);
		switch (mode)
		{
		case Controller::Mode::BUTTON:
			if (GetButtonState(state, p_state, p_type))
			{
				return true;
			}
			break;
		case Controller::Mode::STICK:
			if (GetStickState(state, p_state))
			{
				return true;
			}
			break;
		}
	}
	return false;
}

bool Controller::GetButtonState(XINPUT_STATE state, Controller::State p_state, Type p_type)
{
	switch (p_type)
	{
	case Controller::Type::NONE:
	case Controller::Type::PUSH:
		if (PushButton(p_state) && state.Gamepad.wButtons & buttonNum)
		{
			return true;
		}
		break;
	case Controller::Type::TRIGGER:
		if (TriggerButton(state, p_state) && state.Gamepad.wButtons & buttonNum)
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool Controller::GetStickState(XINPUT_STATE state, Controller::State p_state)
{
	// 範囲指定用
	int TRIGGER_DEADZONE = 100;
	int L_STICK_THUMB_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	int R_STICK_THUMB_DEAD = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	switch (p_state)
	{
	case Controller::State::LT:
		if (statePre.Gamepad.bLeftTrigger < TRIGGER_DEADZONE && state.Gamepad.bLeftTrigger > TRIGGER_DEADZONE)
		{
			return true;
		}
		break;
	case Controller::State::RT:
		if (statePre.Gamepad.bRightTrigger < TRIGGER_DEADZONE && state.Gamepad.bRightTrigger > TRIGGER_DEADZONE)
		{
			return true;
		}
		break;
	case Controller::State::LEFT_U_STICK:
		if (state.Gamepad.sThumbLY > L_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	case Controller::State::LEFT_D_STICK:
		if (state.Gamepad.sThumbLY < -L_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	case Controller::State::LEFT_R_STICK:
		if (state.Gamepad.sThumbLX > L_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	case Controller::State::LEFT_L_STICK:
		if (state.Gamepad.sThumbLX < -L_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	case Controller::State::RIGHT_U_STICK:
		if (state.Gamepad.sThumbRY > R_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	case Controller::State::RIGHT_D_STICK:
		if (state.Gamepad.sThumbRY < -R_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	case Controller::State::RIGHT_R_STICK:
		if (state.Gamepad.sThumbRX > R_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	case Controller::State::RIGHT_L_STICK:
		if (state.Gamepad.sThumbRX < -R_STICK_THUMB_DEAD)
		{
			return true;
		}
		break;
	}
	return false;
}

bool Controller::PushButton(Controller::State& p_state)
{
	switch (p_state)
	{
	case Controller::State::A:
		buttonNum = XINPUT_GAMEPAD_A;
		break;
	case Controller::State::B:
		buttonNum = XINPUT_GAMEPAD_B;
		break;
	case Controller::State::X:
		buttonNum = XINPUT_GAMEPAD_X;
		break;
	case Controller::State::Y:
		buttonNum = XINPUT_GAMEPAD_Y;
		break;
	case Controller::State::LB:
		buttonNum = XINPUT_GAMEPAD_LEFT_SHOULDER;
		break;
	case Controller::State::RB:
		buttonNum = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		break;
	case Controller::State::BACK:
		buttonNum = XINPUT_GAMEPAD_BACK;
		break;
	case Controller::State::START:
		buttonNum = XINPUT_GAMEPAD_START;
		break;
	case Controller::State::UP:
		buttonNum = XINPUT_GAMEPAD_DPAD_UP;
		break;
	case Controller::State::DOWN:
		buttonNum = XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case Controller::State::LEFT:
		buttonNum = XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case Controller::State::RIGHT:
		buttonNum = XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case Controller::State::LEFT_THUMB:
		buttonNum = XINPUT_GAMEPAD_LEFT_THUMB;
		break;
	case Controller::State::RIGHT_THUMB:
		buttonNum = XINPUT_GAMEPAD_RIGHT_THUMB;
		break;
	}

	return true;
}

bool Controller::TriggerButton(XINPUT_STATE state, Controller::State& p_state)
{
	switch (p_state)
	{
	case Controller::State::A:
		buttonNum = XINPUT_GAMEPAD_A;
		break;
	case Controller::State::B:
		buttonNum = XINPUT_GAMEPAD_B;
		break;
	case Controller::State::X:
		buttonNum = XINPUT_GAMEPAD_X;
		break;
	case Controller::State::Y:
		buttonNum = XINPUT_GAMEPAD_Y;
		break;
	case Controller::State::LB:
		buttonNum = XINPUT_GAMEPAD_LEFT_SHOULDER;
		break;
	case Controller::State::RB:
		buttonNum = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		break;
	case Controller::State::BACK:
		buttonNum = XINPUT_GAMEPAD_BACK;
		break;
	case Controller::State::START:
		buttonNum = XINPUT_GAMEPAD_START;
		break;
	case Controller::State::UP:
		buttonNum = XINPUT_GAMEPAD_DPAD_UP;
		break;
	case Controller::State::DOWN:
		buttonNum = XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case Controller::State::LEFT:
		buttonNum = XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case Controller::State::RIGHT:
		buttonNum = XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case Controller::State::LEFT_THUMB:
		buttonNum = XINPUT_GAMEPAD_LEFT_THUMB;
		break;
	case Controller::State::RIGHT_THUMB:
		buttonNum = XINPUT_GAMEPAD_RIGHT_THUMB;
		break;
	}

	if (!statePre.Gamepad.wButtons && state.Gamepad.wButtons)
	{
		return true;
	}
	return false;
}

void Controller::CheckMode(Controller::State p_state)
{
	stateNum = p_state;

	// デジタルかアナログかの判別
	if (stateNum <= Controller::State::RIGHT_THUMB)
	{
		mode = Controller::Mode::BUTTON;
	}
	else
	{
		mode = Controller::Mode::STICK;
	}
}

void Controller::Vibration()
{
	vibration.wLeftMotorSpeed = USHRT_MAX; // 数値は0～65535
	vibration.wRightMotorSpeed = USHRT_MAX; // 数値は0～65535
	XInputSetState(controllerNum, &vibration);
	vCount = 20;
}

bool Controller::Detection()
{
	if (dwResult == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}
