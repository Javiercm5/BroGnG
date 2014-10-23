#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cData.h"
#include "cZombie.h"
#include "cProjectile.h"
#include "cTank.h"


#define GAME_WIDTH	320
#define GAME_HEIGHT 240

#define MAX_PROJECTILES 30
#define PROJECTILES_PLAYER_DELAY	15
#define PROJECTILES_TANK_DELAY		30
#define MAX_ZOMBIES					50


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
	cScene getScene();
	void addProjectile(bool right, int x, int y, int type);
	cPlayer getPlayer(int nPlayer);

private:
	unsigned char keys[256];
	int level;
	cScene Scene;
	cPlayer Player;
	cData Data;
	cTank Tank;
	cProjectile Bullets[MAX_PROJECTILES];
	
	cZombie zombies[MAX_ZOMBIES];
	cTank tanks[10];

	int bulletsDelay = PROJECTILES_PLAYER_DELAY;
	int fireballsDelay = PROJECTILES_TANK_DELAY;
	int cameraX, cameraY;
	//int GAME_WIDTH = 640;
	//int GAME_HEIGHT = 480;
	void CameraUpdate(int px, int py);
	void EnemiesLogic(int px, int py);
	void ProjectilesLogic();

	void levelInits(int lvl);

};
