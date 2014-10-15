#include "cGame.h"
#include "Globals.h"


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
	glOrtho(0,GAME_WIDTH,0,GAME_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS,"blocks.png",GL_RGBA);
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
	Enemy.SetTile(10, 1);
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
	else if(keys[GLUT_KEY_DOWN])	Player.crouch();
	else if (keys[32])				Player.shoot();
	else Player.Stop();
	
	
	//Game Logic
	Player.Logic(Scene.GetMap());
	Bullet.logic(Scene.GetMap());

	int px, py;
	Player.GetPosition(&px, &py);

	Enemy.intelligence(Scene.GetMap(), px, py);
	Enemy.Logic(Scene.GetMap());
	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));
	Player.Draw(Data.GetID(IMG_PLAYER));
	if (Player.GetState() == STATE_SHOOT){

		int xb;
		int yb;
		Player.GetPosition(&xb, &yb);
		Bullet.setPosition(xb, yb);
		Bullet.aim(Player.isFacingRight());
	}
	Enemy.Draw(Data.GetID(IMG_PLAYER));

	Bullet.draw(Data.GetID(IMG_MISC));

	glutSwapBuffers();
}
