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
	
	
	bool res = true;
	gameEnd = false;
	player1Score = 0;
	//Graphics initialization
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cameraX = 0;
	cameraY = 0;
	glOrtho(cameraX, GAME_WIDTH, cameraY, GAME_HEIGHT, 0, 1);
	glMatrixMode(GL_MODELVIEW);

	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	levelZombies = MAX_ZOMBIES;
	levelTanks = MAX_TANKS;

	level = 1;
	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS, "tileset.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_GAME_OVER, "gameOver.png", GL_RGB);
	if (!res) return false;


	res = Data.LoadImage(IMG_MISC, "tilesheetMisc.png", GL_RGBA);
	if (!res) return false;

	res = Scene.LoadLevel(level);
	if (!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER, "tilesheet.png", GL_RGBA);
	if (!res) return false;

	levelInits(level);



	return res;
}


bool cGame::Loop()
{
	bool res = true;

	res = Process();
	if (res) Render();

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
	bool res = true;
	if (keys[27])	res = false;

	if (gameEnd){
		CameraUpdate(0, 0);


		if (keys[32]) {
			gameEnd = false;
			levelInits(level);
		}
	}
	else {
		//Process Input

		if (keys[GLUT_KEY_UP])			Player.Jump(Scene.GetMap());
		if (keys[GLUT_KEY_LEFT])			Player.MoveLeft(Scene.GetMap());
		else if (keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
		//else if(keys[GLUT_KEY_DOWN])	Player.crouch();
		else if (keys[32])				Player.shoot();
		else Player.Stop();

		//GAME LOGIC
		//if (Player.isAlive()) 
		Player.Update(*this);

		for (int i = 0; i < levelZombies; ++i){
			if (zombies[i].isAlive()) zombies[i].Update(*this);
		}
		for (int i = 0; i < levelTanks; ++i){
			if (tanks[i].isAlive()) tanks[i].Update(*this);
		}
		if (demon.isAlive()) demon.Update(*this);

		collisionsLogic();
		ProjectilesLogic();

		CameraUpdate(Player.GetPositionX(), Player.GetPositionY());
	}
	return res;
}

//Output

//falta bosses
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	if (gameEnd) {
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_GAME_OVER));
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);	glVertex2i(SCENE_Xo, SCENE_Yo);
		glTexCoord2f(1.0f, 1.0f);	glVertex2i(SCENE_Xo + SCENE_WIDTH * TILE_SIZE, SCENE_Yo);
		glTexCoord2f(1.0f, 0.0f);	glVertex2i(SCENE_Xo + SCENE_WIDTH * TILE_SIZE, SCENE_Yo + GAME_HEIGHT * TILE_SIZE);
		glTexCoord2f(0.0f, 0.0f);	glVertex2i(SCENE_Xo, SCENE_Yo + GAME_HEIGHT * TILE_SIZE);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}


	else{

		Scene.Draw(Data.GetID(IMG_BLOCKS));

		if (Player.isAlive()) Player.Draw(Data.GetID(IMG_PLAYER));

		for (int i = 0; i < levelZombies; ++i){
			if (zombies[i].isAlive()) zombies[i].Draw(Data.GetID(IMG_PLAYER));
		}

		for (int i = 0; i < levelTanks; ++i){
			if (tanks[i].isAlive()) tanks[i].Draw(Data.GetID(IMG_PLAYER));
		}

		for (int i = 0; i < MAX_PROJECTILES; ++i) {
			if (Bullets[i].isAlive()) Bullets[i].draw(Data.GetID(IMG_MISC));
		}

		if (demon.isAlive()) demon.Draw(Data.GetID(IMG_PLAYER));

		glDisable(GL_TEXTURE_2D);

		std::string s = { "Player 1 score: " };
		s = s + std::to_string(player1Score);
		glRasterPos2f(cameraX, cameraY + GAME_HEIGHT - 18);
		render_string(GLUT_BITMAP_HELVETICA_18, s);

		s = { "Health: " };
		s = s + std::to_string(Player.getHealth());
		glRasterPos2f(cameraX, cameraY);
		render_string(GLUT_BITMAP_HELVETICA_18, s);


		//glEnable(GL_TEXTURE_2D);
		glutSwapBuffers();
	}
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



void cGame::addProjectile(bool right, int x, int y, int type, bool enemy)
{
	for (int i = 0; i < MAX_PROJECTILES; ++i){
		if (!Bullets[i].isAlive()){
			Bullets[i].shoot(right, x, y, type, enemy);
			break;
		}
	}
}

void cGame::ProjectilesLogic()
{
	for (int i = 0; i < MAX_PROJECTILES; ++i){

		if (Bullets[i].isAlive()) {

			Bullets[i].logic(Scene.GetMap());

			//Enemy bullets vs player
			if (Bullets[i].isEnemy()){
				if (Player.isAlive() && Player.collidesWith(Bullets[i].getAABB())){
					Player.impact(Bullets[i].GetDamage(), 0);
					Bullets[i].impact();
				}
			}
			else{

				for (int j = 0; j < levelZombies; ++j){

					if (zombies[j].isAlive() &&  !zombies[j].isDying() & zombies[j].collidesWith(Bullets[i].getAABB())){
						zombies[j].impact(Bullets[i].GetDamage(), 1);
						Bullets[i].impact();
					}
				}
				for (int j = 0; j < levelTanks; ++j){
					if (tanks[j].isAlive() && tanks[j].collidesWith(Bullets[i].getAABB())){
						tanks[j].impact(Bullets[i].GetDamage(), 1);
						Bullets[i].impact();
					}
				}

				if (demon.isAlive() && demon.collidesWith(Bullets[i].getAABB())){
					demon.impact(Bullets[i].GetDamage(), 1);
					Bullets[i].impact();
				}

				for (int j = 0; j < MAX_PROJECTILES; ++j){
					if (Bullets[i].isAlive() && Bullets[j].isAlive() && Bullets[j].isEnemy() && Bullets[j].collideWith(Bullets[i].getAABB())){
						Bullets[j].impact();
						Bullets[i].impact();
						player1Score += 1;
					}
				}
			}


		}
	}
	

}

void cGame::collisionsLogic()
{
	for (int i = 0; i < levelZombies; i++){
		if (zombies[i].isAlive() && !zombies[i].isDying() && Player.collidesWith(zombies[i].getAABB()) ){
			Player.impact(zombies[i].getDamage(), 0);
		}
	}
	for (int i = 0; i < levelTanks; ++i){
		if (tanks[i].isAlive() && !tanks[i].isDying() && Player.collidesWith(tanks[i].getAABB())){
			Player.impact(tanks[i].getDamage(), 0);
		}
	}
	if (demon.isAlive() && Player.collidesWith(demon.getAABB())) Player.impact(demon.getDamage(), 0);
}

cScene cGame::getScene()
{
	return Scene;
}

cPlayer cGame::getPlayer(int nPlayer)
{
	return Player;
}


struct position{
	int x, y;
};


//falta ficar bosses
void cGame::levelInits(int lvl)
{
	level = lvl;
	bool res = true;
	gameEnd = false;
	player1Score = 0;
	//Graphics initialization
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cameraX = 0;
	cameraY = 0;
	glOrtho(cameraX, GAME_WIDTH, cameraY, GAME_HEIGHT, 0, 1);
	glMatrixMode(GL_MODELVIEW);

	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	levelZombies = MAX_ZOMBIES;
	levelTanks = MAX_TANKS;

	res = Scene.LoadLevel(level);
	//if (!res) return false;

	if (lvl == 1){
		levelZombies = 15;
		levelTanks = 3;
		int ZpositionsX[15] = { 10, 15, 4, 10, 15, 47, 53, 56, 72, 77, 80, 49, 55, 60, 65 };
		int ZpositionsY[15] = { 19, 19, 14, 2, 2, 16, 23, 23, 20, 20, 14, 2, 2, 2, 2 };

		int TPositionsX[3] = { 6, 35, 86 };
		int TPositionsY[3] = { 4, 21, 26 };

		Player.SetWidthHeight(36, 40);
		Player.SetTile(4, 25);
		Player.SetWidthHeight(36, 40);
		Player.SetState(STATE_LOOK);
		Player.SetDirection(1);


		//Enemies inicialitzations
		for (int i = 0; i < levelZombies; ++i){
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetTile(ZpositionsX[i], ZpositionsY[i]);
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetState(STATE_WALK);
			zombies[i].SetDirection(1);
		}
		for (int i = 0; i < levelTanks; ++i){
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetTile(TPositionsX[i], TPositionsY[i]);
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetState(STATE_WALK);
			tanks[i].SetDirection(1);
		}

		demon.SetWidthHeight(64, 64);
		demon.SetTile(90, 4);
		demon.SetWidthHeight(64, 64);
		demon.SetState(STATE_WALK);
		demon.SetDirection(1);
	}

	if (lvl == 2){
		levelZombies = 20;
		levelTanks = 4;
		int ZpositionsX[20] = { 9, 24, 37, 49, 60, 75, 88, 82, 81, 83, 60, 57, 25, 32, 30, 28, 54, 42, 15, 80 };
		int ZpositionsY[20] = { 2, 2, 2, 2, 2, 2, 2, 9, 15, 21, 21, 15, 9, 9, 15, 15, 2, 2, 2, 2 };

		int TPositionsX[4] = { 89, 30, 64, 30 };
		int TPositionsY[4] = { 26, 26, 9, 15 };

		Player.SetWidthHeight(36, 40);
		Player.SetTile(2, 2);
		Player.SetWidthHeight(36, 40);
		Player.SetState(STATE_LOOK);
		Player.SetDirection(1);


		//Enemies inicialitzations
		for (int i = 0; i < levelZombies; ++i){
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetTile(ZpositionsX[i], ZpositionsY[i]);
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetState(STATE_WALK);
			zombies[i].SetDirection(1);
		}
		for (int i = 0; i < levelTanks; ++i){
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetTile(TPositionsX[i], TPositionsY[i]);
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetState(STATE_WALK);
			tanks[i].SetDirection(1);
		}
		demon.SetWidthHeight(64, 64);
		demon.SetTile(90, 4);
		demon.SetWidthHeight(64, 64);
		demon.SetState(STATE_WALK);
		demon.SetDirection(1);
	}

} 


void cGame::gameOver()
{
	gameEnd = true;

}



void cGame::render_string(void* font, const std::string s)
{
	for (int i = 0; i<s.length(); i++)
		glutBitmapCharacter(font, s[i]);
}

void cGame::levelFinished()
{
	if (level < 2){
		level++;
		levelInits(level);
		
		//cargar nou lvl, etc
	}
	else{
		//mostrar pantalla final blabalba
	}

}

void cGame::updateScore(int points, int player)
{
	switch (player){
	case 1: player1Score += points; break;
	}
}
