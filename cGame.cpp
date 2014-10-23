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

	level = 1;
	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS,"tileset.png",GL_RGBA);
	if(!res) return false;

	res = Data.LoadImage(IMG_MISC, "tilesheetMisc.png", GL_RGBA);
	if (!res) return false;

	res = Scene.LoadLevel(level);
	if(!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"tilesheet.png",GL_RGBA);
	if(!res) return false;
	
	levelInits(level);
	


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

	Player.Update(*this);
	Tank.Update(*this);
	
	for (int i = 0; i < MAX_ZOMBIES; ++i){
		zombies[i].Update(*this);
	}


	ProjectilesLogic();
	
	CameraUpdate(Player.GetPositionX(), Player.GetPositionY());

	return res;
}

//Output

void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_BLOCKS));

	Player.Draw(Data.GetID(IMG_PLAYER));
	for (int i = 0; i < MAX_ZOMBIES; ++i){
		if (zombies[i].isAlive()) zombies[i].Draw(Data.GetID(IMG_PLAYER));
	}
	

	if (Tank.isAlive()) Tank.Draw(Data.GetID(IMG_PLAYER));

	for (int i = 0; i < MAX_PROJECTILES; ++i) {
		if (Bullets[i].isAlive()) Bullets[i].draw(Data.GetID(IMG_MISC));
	}

	glutSwapBuffers();
}

void cGame::CameraUpdate(int px, int py)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int cameraVx = 2;
	int cameraVy = 2;
	int factor = 2;
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

	cameraX += 1000 / 50 * cameraVx;
	cameraX = min(cameraX, px - 2 * GAME_WIDTH / 3);
	cameraX = max(cameraX, px - GAME_WIDTH / 3);
	cameraX = max(cameraX, SCENE_Xo);
	cameraX = min(cameraX, SCENE_WIDTH*BLOCK_SIZE - SCENE_Xo*BLOCK_SIZE);

	cameraY += 1000 / 50 * cameraVy;
	cameraY = min(cameraY, py - 2 * GAME_HEIGHT / 3);
	cameraY = max(cameraY, py - GAME_HEIGHT / 3);
	cameraY = max(cameraY, SCENE_Yo);
	cameraY = min(cameraY, (SCENE_HEIGHT * BLOCK_SIZE - SCENE_Yo*BLOCK_SIZE) - SCENE_Yo);

	glOrtho(cameraX, GAME_WIDTH + cameraX, cameraY, GAME_HEIGHT + cameraY, -1, 3);
	glMatrixMode(GL_MODELVIEW);
}



void cGame::addProjectile(bool right, int x, int y, int type)
{
	for (int i = 0; i < MAX_PROJECTILES; ++i){
		if (!Bullets[i].isAlive()){
			Bullets[i].shoot(right, x, y, type);
			break;
		}
	}
}

void cGame::ProjectilesLogic()
{
	for (int i = 0; i < MAX_PROJECTILES; ++i){
		if (Bullets[i].isAlive()) {
			for (int i = 0; i < MAX_ZOMBIES; ++i){
				if (zombies[i].isAlive() && zombies[i].collidesWith(Bullets[i].getAABB()) && Bullets[i].GetType() != TYPE_FIREBALL){
					zombies[i].impact(Bullets[i].GetDamage());
					Bullets[i].impact();
				}
			}
			
			if (Player.isAlive() && Player.collidesWith(Bullets[i].getAABB()) && Bullets[i].GetType() != TYPE_SPEAR){
				Player.impact(Bullets[i].GetDamage());
				Bullets[i].impact();
			}
			
			else Bullets[i].logic(Scene.GetMap());
		}
	}
}

cScene cGame::getScene()
{
	return Scene;
}

cPlayer cGame::getPlayer(int nPlayer)
{
	return Player;
}

void cGame::levelInits(int lvl)
{
	if (lvl == 1){
			Player.SetWidthHeight(36, 40);
			Player.SetTile(4, 25);
			Player.SetWidthHeight(36, 40);
			Player.SetState(STATE_LOOK);
			Player.SetDirection(1);


			//Enemies inicialitzations
			for (int i = 0; i < 14; ++i){
				zombies[i].SetWidthHeight(36, 40);
			}

			for (int i = 14; i < MAX_ZOMBIES; ++i){
				zombies[i].die();
			}

			zombies[0].SetTile(10, 19);
			zombies[0].SetWidthHeight(36, 40);
			zombies[0].SetState(STATE_WALK);
			zombies[0].SetDirection(1);

			zombies[1].SetTile(15, 19);
			zombies[1].SetWidthHeight(36, 40);
			zombies[1].SetState(STATE_WALK);
			zombies[1].SetDirection(1);

			zombies[2].SetTile(8, 14);
			zombies[2].SetWidthHeight(36, 40);
			zombies[2].SetState(STATE_WALK);
			zombies[2].SetDirection(1);

			zombies[3].SetTile(47, 23);
			zombies[3].SetWidthHeight(36, 40);
			zombies[3].SetState(STATE_WALK);
			zombies[3].SetDirection(1);

			zombies[4].SetTile(15, 4);
			zombies[4].SetWidthHeight(36, 40);
			zombies[4].SetState(STATE_WALK);
			zombies[4].SetDirection(1);

			zombies[5].SetTile(50, 18);
			zombies[5].SetWidthHeight(36, 40);
			zombies[6].SetState(STATE_WALK);
			zombies[6].SetDirection(1);

			zombies[7].SetTile(54, 23);
			zombies[7].SetWidthHeight(36, 40);
			zombies[7].SetState(STATE_WALK);
			zombies[7].SetDirection(1);

			//zombies[8].SetTile(, );
			zombies[8].SetWidthHeight(36, 40);
			zombies[8].SetState(STATE_WALK);
			zombies[8].SetDirection(1);

			//zombies[9].SetTile(, );
			zombies[9].SetWidthHeight(36, 40);
			zombies[9].SetState(STATE_WALK);
			zombies[9].SetDirection(1);

			//zombies[10].SetTile(, );
			zombies[10].SetWidthHeight(36, 40);
			zombies[10].SetState(STATE_WALK);
			zombies[10].SetDirection(1);

			//	zombies[11].SetTile(, );
			zombies[11].SetWidthHeight(36, 40);
			zombies[11].SetState(STATE_WALK);
			zombies[11].SetDirection(1);

			//zombies[12].SetTile(, );
			zombies[12].SetWidthHeight(36, 40);
			zombies[12].SetState(STATE_WALK);
			zombies[12].SetDirection(1);

			//zombies[13].SetTile(, );
			zombies[13].SetWidthHeight(36, 40);
			zombies[13].SetState(STATE_WALK);
			zombies[13].SetDirection(1);

			//zombies[13].SetTile(, );
			zombies[13].SetWidthHeight(36, 40);
			zombies[13].SetState(STATE_WALK);
			zombies[13].SetDirection(1);



			Tank.SetWidthHeight(64, 64);
			Tank.SetTile(6, 4);
			Tank.SetWidthHeight(64, 64);
			Tank.SetState(STATE_WALK);
			Tank.SetDirection(1);
		}
	
}


