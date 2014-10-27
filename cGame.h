#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cData.h"
#include "cZombie.h"
#include "cProjectile.h"
#include "cTank.h"
#include "cDemon.h"

#define GAME_WIDTH	320
#define GAME_HEIGHT 240

#define MAX_PROJECTILES 30
#define MAX_ZOMBIES		50
#define MAX_TANKS		15

#define SOUND_JUMP_PLAYER		0
#define SOUND_SHOOT_PLAYER		1
#define SOUND_BODY_IMPACT		3
#define SOUND_WALL_IMPACT		4
#define SOUND_SHOOT_ENEMY		5

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
	void addProjectile(bool right, int x, int y, int type, bool enemy);
	cPlayer getPlayer(int nPlayer);
	void gameOver();
	void levelFinished();
	void updateScore(int points, int player);
	bool isOver();
	void emitSound(int type);


private:
	unsigned char keys[256];
	int level;
	cScene Scene;
	cPlayer Player;
	cData Data;
	cProjectile Bullets[MAX_PROJECTILES];

	cZombie zombies[MAX_ZOMBIES];
	cTank tanks[MAX_TANKS];
	cDemon demon;

	int levelZombies;
	int levelTanks;
	int cameraX, cameraY;

	int player1Score;

	//int GAME_WIDTH = 640;
	//int GAME_HEIGHT = 480;

	void CameraUpdate(int px, int py);
	void EnemiesLogic(int px, int py);
	void ProjectilesLogic();
	void collisionsLogic();
	void levelInits(int lvl);
	void render_string(void* font, const std::string s);
	bool gameEnd;
	bool gameFinished;

	void loadResources();

	HWND handle;
	HSAMPLE lvl1Music, gameOverMusic, lvl2Music, gameWinMusic, shootSound, jumpSound, impactBodySound, impactWallSound, shootEnemySound;
	HCHANNEL hBackgroundChannel, hPlayerChannel, hZombieChannel, hTankChannel, hDemonChannel, hImpactsChannel;



};
