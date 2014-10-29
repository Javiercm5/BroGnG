#pragma once
#include "Globals.h"
class cXboxController
{
public:
	cXboxController();
	~cXboxController();


	bool isPressed(WORD button);
	bool isConnected();
	void setPlayer(int playerNum);
	void vibrate(bool right);
	bool analogRight();
	bool analogLeft();

private:

	XINPUT_STATE state;
	int id, player;
};

