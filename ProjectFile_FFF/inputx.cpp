//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "inputx.h"
#include "keyboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEADZONE			(2200)	// 各軸の25%を無効ゾーンとする
#define MAX_CONTROLLERS     (4)
#define PRESSING_COUNT		(15)
#define PAD_STICK_THUMB		(0.3f)

struct CONTROLER_STATE
{
	XINPUT_STATE		lastState;
	XINPUT_STATE		state;
	XINPUT_STATE		trigger;
	XINPUT_VIBRATION	vibration;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitializePad(void);			// パッド初期化
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CONTROLER_STATE g_Controllers[MAX_CONTROLLERS];
static float	g_LeftStickX[MAX_CONTROLLERS];
static float	g_LeftStickY[MAX_CONTROLLERS];
static float	g_RightStickX[MAX_CONTROLLERS];
static float	g_RightStickY[MAX_CONTROLLERS];

int g_PressingCount[(int)KEY_GROUP::KEY_GROUP_NUM];

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{	
	// パッドの初期化
	InitializePad();

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// パッドの終了処理
	UninitPad();
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// パッドの更新
	UpdatePad();
	for (int i = 0; i < (int)KEY_GROUP::KEY_GROUP_NUM; i++)
	{
		if (g_PressingCount[i] > 0)
		{
			g_PressingCount[i]--;
		}
	}

}

//---------------------------------------- 初期化
HRESULT InitializePad(void)			// パッド初期化
{
	//初期化
	ZeroMemory(g_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	XInputEnable(true);
		
	return true;

}

//------------------------------------------- 終了処理
void UninitPad(void)
{
	//パラメータのリセット
	ZeroMemory(g_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	//バイブレーション停止
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
		XInputSetState(i, &g_Controllers[i].vibration);

	XInputEnable(false);
}

//------------------------------------------ 更新
void UpdatePad(void)
{
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		XInputSetState(i, &g_Controllers[i].vibration);

		g_Controllers[i].lastState = g_Controllers[i].state;

		//正常：ERROR_SUCCESS　0
		//接続無し：ERROR_DEVICE_NOT_CONNECTED 1167
		DWORD result;
		result = XInputGetState(i, &g_Controllers[i].state);

		//トリガー作成
		g_Controllers[i].trigger.Gamepad.wButtons = ((g_Controllers[i].lastState.Gamepad.wButtons ^ g_Controllers[i].state.Gamepad.wButtons) & g_Controllers[i].state.Gamepad.wButtons);

		//左スティック情報の作成
		float LX = g_Controllers[i].state.Gamepad.sThumbLX;
		float LY = g_Controllers[i].state.Gamepad.sThumbLY;

		float magnitude = sqrtf((LX * LX) + (LY * LY));

		if (magnitude > 32767)
			magnitude = 32767;

		magnitude -= DEADZONE;

		if (magnitude <= 0)
		{
			g_Controllers[i].state.Gamepad.sThumbLX = 0;
			g_Controllers[i].state.Gamepad.sThumbLY = 0;
		}

		if(g_Controllers[i].state.Gamepad.sThumbLX >= 0)
			g_LeftStickX[i] = (float)g_Controllers[i].state.Gamepad.sThumbLX / 32767;
		else
			g_LeftStickX[i] = (float)g_Controllers[i].state.Gamepad.sThumbLX / 32768;

		if (g_Controllers[i].state.Gamepad.sThumbLY >= 0)
			g_LeftStickY[i] = (float)g_Controllers[i].state.Gamepad.sThumbLY / 32767;
		else
			g_LeftStickY[i] = (float)g_Controllers[i].state.Gamepad.sThumbLY / 32768;

		//右スティック情報の作成
		float RX = g_Controllers[i].state.Gamepad.sThumbRX;
		float RY = g_Controllers[i].state.Gamepad.sThumbRY;

		magnitude = sqrtf((RX * RX) + (RY * RY));

		if (magnitude > 32767)
			magnitude = 32767;

		magnitude -= DEADZONE;

		if (magnitude <= 0)
		{
			g_Controllers[i].state.Gamepad.sThumbRX = 0;
			g_Controllers[i].state.Gamepad.sThumbRY = 0;
		}

		if (g_Controllers[i].state.Gamepad.sThumbRX >= 0)
			g_RightStickX[i] = (float)g_Controllers[i].state.Gamepad.sThumbRX / 32767;
		else
			g_RightStickX[i] = (float)g_Controllers[i].state.Gamepad.sThumbRX / 32768;

		if (g_Controllers[i].state.Gamepad.sThumbLY >= 0)
			g_RightStickY[i] = (float)g_Controllers[i].state.Gamepad.sThumbRY / 32767;
		else
			g_RightStickY[i] = (float)g_Controllers[i].state.Gamepad.sThumbRY / 32768;
	}
}

//スティックの範囲
// X 左-1.0　右 1.0
// Y 上 1.0　下-1.0
float GetThumbLeftX(int padNo)
{
	return g_LeftStickX[padNo];
}

float GetThumbLeftY(int padNo)
{
	return g_LeftStickY[padNo];
}

float GetThumbRightX(int padNo)
{
	return g_RightStickX[padNo];
}

float GetThumbRightY(int padNo)
{
	return g_RightStickY[padNo];
}

//トリガーの範囲
// 0 ～ 255
int GetLeftTrigger(int padNo)
{
	return g_Controllers[padNo].state.Gamepad.bLeftTrigger;
}

int GetRightTrigger(int padNo)
{
	return g_Controllers[padNo].state.Gamepad.bRightTrigger;
}

// バイブレーションの範囲
// 0 ～ 255
void SetLeftVibration(int padNo, int speed)
{
	speed %= 256;
	g_Controllers[padNo].vibration.wLeftMotorSpeed = ((speed + 1) * 256) - 1;
}

void SetRightVibration(int padNo, int speed)
{
	speed %= 256;
	g_Controllers[padNo].vibration.wRightMotorSpeed = ((speed + 1) * 256) - 1;
}

void SetVibration(int padNo, int speed)
{
	speed %= 256;
	g_Controllers[padNo].vibration.wLeftMotorSpeed = ((speed + 1) * 256) - 1;
	g_Controllers[padNo].vibration.wRightMotorSpeed = ((speed + 1) * 256) - 1;
}

void StopVibration(int padNo)
{
	g_Controllers[padNo].vibration.wLeftMotorSpeed = 0;
	g_Controllers[padNo].vibration.wRightMotorSpeed = 0;
}

//ボタン押下状態の取得
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & g_Controllers[padNo].state.Gamepad.wButtons);
}

//ボタンパルス状態の取得
BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & g_Controllers[padNo].trigger.Gamepad.wButtons);
}

//ボタングループ長押し
BOOL IsButtonGroupPressed(KEY_GROUP	group)
{
	bool flag = false;
	switch (group)
	{
	case KEY_GROUP::KEY_ENTER:
		if ((GetKeyboardPress(VK_SPACE)) || (IsButtonPressed(0, XINPUT_GAMEPAD_B)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_BUCK:
		if ((GetKeyboardPress('C')) || (IsButtonPressed(0, XINPUT_GAMEPAD_A)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_START:
		if ((GetKeyboardPress(VK_ESCAPE)) || (IsButtonPressed(0, XINPUT_GAMEPAD_START)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_UP:
		if ((GetKeyboardPress('W')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_UP)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_DOWN:
		if ((GetKeyboardPress('S')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_DOWN)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_LEFT:
		if ((GetKeyboardPress('A')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_LEFT)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_RIGHT:
		if ((GetKeyboardPress('D')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_RIGHT)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_TAB_LEFT:
		if ((GetKeyboardPress('Q')) || (IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_TAB_RIGHT:
		if ((GetKeyboardPress('E')) || (IsButtonPressed(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)))
		{	flag = true;	}
		break;

	default:
		break;
	}
	return flag;
}

//ボタングループトリガー
BOOL IsButtonGroupTriggered(KEY_GROUP	group)
{
	bool flag = false;
	switch (group)
	{
	case KEY_GROUP::KEY_ENTER:
		if ((GetKeyboardTrigger(VK_SPACE)) || (IsButtonTriggered(0, XINPUT_GAMEPAD_B)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_BUCK:
		if ((GetKeyboardTrigger('C')) || (IsButtonTriggered(0, XINPUT_GAMEPAD_A)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_START:
		if ((GetKeyboardTrigger(VK_ESCAPE)) || (IsButtonTriggered(0, XINPUT_GAMEPAD_START)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_UP:
		if ((GetKeyboardTrigger('W')) || (IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_UP)))
		{	flag = true;		}
		break;

	case KEY_GROUP::KEY_DOWN:
		if ((GetKeyboardTrigger('S')) || (IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_DOWN)))
		{	flag = true;		}
		break;

	case KEY_GROUP::KEY_LEFT:
		if ((GetKeyboardTrigger('A')) || (IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_LEFT)))
		{	flag = true;		}
		break;

	case KEY_GROUP::KEY_RIGHT:
		if ((GetKeyboardTrigger('D')) || (IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_RIGHT)))
		{	flag = true;		}
		break;

	case KEY_GROUP::KEY_TAB_LEFT:
		if ((GetKeyboardTrigger('Q')) || (IsButtonTriggered(0, XINPUT_GAMEPAD_LEFT_SHOULDER)))
		{	flag = true;	}
		break;

	case KEY_GROUP::KEY_TAB_RIGHT:
		if ((GetKeyboardTrigger('E')) || (IsButtonTriggered(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)))
		{	flag = true;	}
		break;

	default:
		break;
	}
	return flag;
}

//ボタングループ長押し
BOOL IsButtonGroupPressing(KEY_GROUP group,	int count)
{
	bool flag = false;
	if (g_PressingCount[(int)group] <= 0)
	{
		switch (group)
		{
		case KEY_GROUP::KEY_ENTER:
			if ((GetKeyboardPress(VK_SPACE)) || (IsButtonPressed(0, XINPUT_GAMEPAD_B)))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_BUCK:
			if ((GetKeyboardPress('C')) || (IsButtonPressed(0, XINPUT_GAMEPAD_A)))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_START:
			if ((GetKeyboardPress(VK_ESCAPE)) || (IsButtonPressed(0, XINPUT_GAMEPAD_START)))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_UP:
			if ((GetKeyboardPress('W')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_UP))|| (GetThumbLeftY(0) > PAD_STICK_THUMB))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_DOWN:
			if ((GetKeyboardPress('S')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_DOWN))|| (GetThumbLeftY(0) < -PAD_STICK_THUMB))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_LEFT:
			if ((GetKeyboardPress('A')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_LEFT))|| (GetThumbLeftX(0) < -PAD_STICK_THUMB))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_RIGHT:
			if ((GetKeyboardPress('D')) || (IsButtonPressed(0, XINPUT_GAMEPAD_DPAD_RIGHT)) || (GetThumbLeftX(0) > PAD_STICK_THUMB))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_TAB_LEFT:
			if ((GetKeyboardPress('Q')) || (IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER)))
			{
				flag = true;
			}
			break;

		case KEY_GROUP::KEY_TAB_RIGHT:
			if ((GetKeyboardPress('E')) || (IsButtonPressed(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)))
			{
				flag = true;
			}
			break;

		default:
			break;
		}
	}
	if (flag)
	{
		g_PressingCount[(int)group] = count;	
	}

	return flag;
}


