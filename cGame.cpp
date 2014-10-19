#include "cGame.h"
#include "Globals.h"
#include "iostream"

cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cameraX = 0;
	cameraY = 0;
	glOrtho(cameraX, GAME_WIDTH, cameraY, GAME_HEIGHT, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS,"tileset.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_MISC, "tilesheetMisc.png", GL_RGBA);
	if (!res) return false;
	res = Scene.LoadLevel(1);
	if(!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"tilesheet.png",GL_RGBA);
	if(!res) return false;
	Player.SetWidthHeight(36, 40);
	Player.SetTile(4, 1);
	Player.SetWidthHeight(36, 40);
	Player.SetState(STATE_LOOK);
	Player.SetDirection(1);

	
	
	//Enemies inicialitzations
	Enemy.SetWidthHeight(36, 40);
	Enemy.SetTile(10, 10);
	Enemy.SetWidthHeight(36, 40);
	Enemy.SetState(STATE_WALK);
	Enemy.SetDirection(1);



	return res;
}

bool cGame::Loop()
{
	bool res=true;

	res = Process();
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	bool res=true;
	
	//Process Input
	if(keys[27])	res=false;
	
	if(keys[GLUT_KEY_UP])			Player.Jump(Scene.GetMap());
	if(keys[GLUT_KEY_LEFT])			Player.MoveLeft(Scene.GetMap());
	else if(keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
	//else if(keys[GLUT_KEY_DOWN])	Player.crouch();
	else if (keys[32])				Player.shoot();
	else Player.Stop();
	
	
	//--GAME LOGIC

	//Player logic
	Player.Logic(Scene.GetMap());

	int px, py;
	Player.GetPosition(&px, &py);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//cameraX = px - 1 / 3 * GAME_WIDTH;
	//cameraY = py - 1/3 * GAME_HEIGHT;
	
	
	int cameraVx = 2;
	int cameraVy = 2;
	int factor = 5;
	if (Player.GetState() == STATE_WALK){
		if ((px - cameraX) > (GAME_WIDTH / 3)) cameraVx = factor * 2;
		else cameraVx = 0;

		if ((py - cameraY) > (GAME_HEIGHT / 3)) cameraVy = factor * 2;
		else cameraVy = 0;
	}
	else{
		if ((px - cameraX) < (2 * GAME_WIDTH / 3)) cameraVx = factor * 2;
		else cameraVx = 0;
		if ((py - cameraY) < (2 * GAME_HEIGHT / 3)) cameraVy = factor * 2;
		else cameraVy = 0;
	}

	cameraX += 1000/50 * cameraVx;
	cameraX = min(cameraX, px - 2*GAME_WIDTH / 3);
	cameraX = max(cameraX, px - GAME_WIDTH / 3);
	cameraX = max(cameraX, SCENE_Xo);
	cameraX = min(cameraX, GAME_WIDTH - SCENE_Xo);

	cameraY += 1000 / 50 * cameraVy;
	cameraY = min(cameraY, py - 2 * GAME_HEIGHT / 3);
	cameraY = max(cameraY, py - GAME_HEIGHT / 3);
	cameraY = max(cameraY, SCENE_Yo);
	cameraY = min(cameraY, GAME_HEIGHT - SCENE_Yo);


	glOrtho(cameraX, GAME_WIDTH + cameraX, cameraY, GAME_HEIGHT + cameraY, -1, 3);

	glMatrixMode(GL_MODELVIEW);
	

	if (bulletsDelay == 0) bulletsDelay = PROJECTILES_DELAY;
	if (bulletsDelay < PROJECTILES_DELAY)	--bulletsDelay;

	if (Player.GetState() == STATE_SHOOT){
		for (int i = 0; i < MAX_PROJECTILES; ++i){
			if (!Bullets[i].isAlive() && bulletsDelay == PROJECTILES_DELAY) {
				Bullets[i].shoot(Player.isFacingRight(), px, py);
				--bulletsDelay;
				break;
			}
		}
	}

	//Projectiles logic
	for (int i = 0; i < MAX_PROJECTILES; ++i){
		if (Bullets[i].isAlive()) {
			if (Enemy.isAlive() && Enemy.collidesWith(Bullets[i].getAABB())){
				Enemy.die();
				Bullets[i].impact();
			}
			Bullets[i].logic(Scene.GetMap());
		}
	}


	//Enemies logic
	if (Enemy.isAlive()){
		Enemy.intelligence(Scene.GetMap(), px, py);
		Enemy.Logic(Scene.GetMap());
	}

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Player.Draw(Data.GetID(IMG_PLAYER));

	for (int i = 0; i < MAX_PROJECTILES; ++i) {
		if (Bullets[i].isAlive()) Bullets[i].draw(Data.GetID(IMG_MISC), Player.isFacingRight());
	}

	if (Enemy.isAlive()) Enemy.Draw(Data.GetID(IMG_PLAYER));

	glutSwapBuffers();
}
