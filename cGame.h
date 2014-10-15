#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cData.h"
#include "cZombie.h"
#include "cProjectile.h"


#define GAME_WIDTH	640
#define GAME_HEIGHT 480

#define MAX_PROJECTILES 30
class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

private:
	unsigned char keys[256];
	cScene Scene;
	cPlayer Player;
	cData Data;
	cZombie Enemy;
	cProjectile Bullets[MAX_PROJECTILES];
};
