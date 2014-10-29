#include "cXboxController.h"


cXboxController::cXboxController()
{
}


cXboxController::~cXboxController()
{
}

bool cXboxController::isPressed(WORD button)
{
	return (state.Gamepad.wButtons & button) != 0;
}

bool cXboxController::isConnected()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	return (XInputGetState(player - 1, &state) == ERROR_SUCCESS);
}

void cXboxController::setPlayer(int playerNum)
{
	player = playerNum;
}

bool cXboxController::analogRight()
{
	
	//Normalize (-1.0f...1.0f)
	float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);

	//deadzone 0.30f
	float leftStickX = (abs(normLX) < 0.30f ? 0 : normLX);

	return (leftStickX > 0);


}

bool cXboxController::analogLeft()
{

	//Normalize (-1.0f...1.0f)
	float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);

	//deadzone 0.30f
	float leftStickX = (abs(normLX) < 0.30f ? 0 : normLX);

	return (leftStickX < 0);


}

void cXboxController::vibrate(bool right)
{
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	Vibration.wRightMotorSpeed = 33333;
	Vibration.wLeftMotorSpeed = 33333;

	XInputSetState(player-1, &Vibration);

	Vibration.wRightMotorSpeed = 0;
	Vibration.wLeftMotorSpeed = 0;
	XInputSetState(player - 1, &Vibration);

}