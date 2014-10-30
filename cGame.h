#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cData.h"
#include "cZombie.h"
#include "cProjectile.h"
#include "cTank.h"
#include "cDemon.h"
#include "cXboxController.h"

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
#define SOUND_MENU_MOV			6

#define MAIN_MENU			0
#define CONTROLS_MENU		1
#define CREDITS_MENU		2
#define GAMEOVER_MENU		3
#define GAMEWIN_MENU		4
#define PLAYERSSELECT_MENU	5
#define NO_MENU				-1

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
	void addProjectile(bool right, int x, int y, int type, int shooter);
	cPlayer getPlayer(int nPlayer);
	void gameOver();
	void levelFinished();
	void updateScore(int points, int player);
	bool isOver();
	void emitSound(int type);
	int getNumPlayers();


private:
	//CONTROLS
	unsigned char keys[256];
	cXboxController gamepad[2];

	//SOUND
	HWND handle;
	HSAMPLE lvl1Music, gameOverMusic, lvl2Music, gameWinMusic, shootSound, jumpSound, impactBodySound, impactWallSound, shootEnemySound, gameStart, menuMovement;
	HCHANNEL hBackgroundChannel, hPlayerChannel, hZombieChannel, hTankChannel, hDemonChannel, hImpactsChannel;
	
	//GAME
	int level;
	cData Data;
	bool multiplayer;
	int score[2];
	int nPlayers;
	int menu, optionSelected, playerselected;
	bool pause;
	int keyDelay;

	cScene Scene;
	int cameraX, cameraY;
	cPlayer Players[2];
	int levelZombies, levelTanks;
	cZombie zombies[MAX_ZOMBIES];
	cTank tanks[MAX_TANKS];
	cDemon demon;
	cProjectile Bullets[MAX_PROJECTILES];



	void CameraUpdate(int px, int py, int playerNum);
	void ProjectilesLogic();
	void collisionsLogic();
	void levelInits(int lvl);
	void render_string(void* font, const std::string s);
	bool gameEnd;
	bool gameFinished;

	void loadResources();
	void drawHud(int playerNum);

};
