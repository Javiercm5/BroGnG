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
	//SOUND INITS
	BASS_Init(1, 44100, 0, 0, NULL);
	lvl1Music = BASS_SampleLoad(false, "Resources/Sounds/Stage_Theme_01.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
	lvl2Music = BASS_SampleLoad(false, "Resources/Sounds/Stage_Theme_02.mp3", 0, 0, 3, BASS_SAMPLE_LOOP);
	gameStart = BASS_SampleLoad(false, "Resources/Sounds/Game_Start.mp3", 0, 0, 3, 0);
	gameOverMusic = BASS_SampleLoad(false, "Resources/Sounds/Game_Over.mp3", 0, 0, 3, 0);
	gameWinMusic = BASS_SampleLoad(false, "Resources/Sounds/Stage_Clear.mp3", 0, 0, 3, 0);
	
	shootSound = BASS_SampleLoad(false, "Resources/Sounds/shoot.wav", 0, 0, 3, 0);
	jumpSound = BASS_SampleLoad(false, "Resources/Sounds/Jump.wav", 0, 0, 3, 0);
	impactBodySound = BASS_SampleLoad(false, "Resources/Sounds/Impact_enemy.wav", 0, 0, 3, 0);
	shootEnemySound = BASS_SampleLoad(false, "Resources/Sounds/shoot_enemy.wav", 0, 0, 3, 0);
	menuMovement = BASS_SampleLoad(false, "Resources/Sounds/menuMovement.wav", 0, 0, 3, 0);


	bool res = true;
	gameEnd = false;
	gameFinished = true;
	pause = false;

	//GAMEPADS INITS (JUST 2, but extendable)
	gamepad[0].setPlayer(1);
	gamepad[1].setPlayer(2);
	keyDelay = 5;

	//Graphics initialization
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	cameraX = 0;
	cameraY = 0;

	glOrtho(cameraX, GAME_WIDTH, cameraY, GAME_HEIGHT, -1, 2);
	glMatrixMode(GL_MODELVIEW);

	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST |GLUT_DOUBLE);	

	levelZombies = MAX_ZOMBIES;
	levelTanks = MAX_TANKS;
	level = 1;


	//TEXTURES INITIALIZATIONS
	res = Data.LoadImage(IMG_BLOCKS, "Resources/Materials/tileset.png", GL_RGBA);
	if (!res) return false;
	res = Data.LoadImage(IMG_MENUS_END, "Resources/Materials/menuEnd.png", GL_RGBA);
	if (!res) return false;
	res = Data.LoadImage(IMG_MISC, "Resources/Materials/tilesheetMisc.png", GL_RGBA);
	if (!res) return false;
	res = Scene.LoadLevel(level);
	if (!res) return false;
	res = Data.LoadImage(IMG_PLAYER, "Resources/Materials/tilesheet.png", GL_RGBA);
	if (!res) return false;
	res = Data.LoadImage(IMG_MENUS, "Resources/Materials/menu.png", GL_RGBA);
	if (!res) return false;

	menu = MAIN_MENU;
	optionSelected = 0; //0 game, 1 controls, 2 credits, 3 exit
	playerselected = 0;


	levelInits(level);
	BASS_ChannelStop(hBackgroundChannel);
	hBackgroundChannel = BASS_SampleGetChannel(gameStart, false);
	BASS_ChannelPlay(hBackgroundChannel, true);


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

//INPUT
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//PROCESS (GAME LOGIC)
bool cGame::Process()
{
	bool res = true;

	switch (menu){
	case MAIN_MENU:
		hBackgroundChannel = BASS_SampleGetChannel(gameStart, false);
		if (keyDelay <= 0){
			if ((keys[GLUT_KEY_UP]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_DPAD_UP))) {
				optionSelected--;
				if (optionSelected == -1) optionSelected = 3;
				keyDelay = 8;
				emitSound(SOUND_MENU_MOV);
			}

			else if ((keys[GLUT_KEY_DOWN]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_DPAD_DOWN))){
				optionSelected++;
				if (optionSelected == 4) optionSelected = 0;
				keyDelay = 8;
				emitSound(SOUND_MENU_MOV);

			}
		}
		else {
			keyDelay--;
		}
		if ((keys[13]
			|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_A))){
			switch (optionSelected){
			case 0: 
				menu = PLAYERSSELECT_MENU;
				keyDelay = 8;
				break;
			case 1: menu = CONTROLS_MENU; break;
			case 2: menu = CREDITS_MENU; break;
			case 3: res = false; break;
			}
		}
		break;

	case PLAYERSSELECT_MENU:
		if (keyDelay <= 0){
			if ((keys[GLUT_KEY_UP]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_DPAD_UP))) {
				playerselected--;
				if (playerselected == -1) playerselected = 1;
				keyDelay = 8;
				emitSound(SOUND_MENU_MOV);
			}

			else if ((keys[GLUT_KEY_DOWN]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_DPAD_DOWN))){
				playerselected++;
				if (playerselected == 2) playerselected = 0;
				keyDelay = 8;
				emitSound(SOUND_MENU_MOV);
			}
			if ((keys[13]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_A))){

				if (playerselected == 0){
					nPlayers = 1;
					multiplayer = false;
				}
				else {
					nPlayers = 2;
					multiplayer = true;
				}
				menu = NO_MENU;
				score[0] = score[1] = 0;
				levelInits(1);
			}
			if ((keys[8]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_BACK))) {
				menu = MAIN_MENU;
				optionSelected = 0;
			}
		}
		
		else {
			keyDelay--;
		}
		break;
	
	case CONTROLS_MENU:
		if ((keys[8]
			|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_BACK))) {
			menu = MAIN_MENU;
			optionSelected = 0;
		}
		break;

	case CREDITS_MENU:
		if ((keys[8]
			|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_BACK))) {
			menu = MAIN_MENU;
			optionSelected = 0;
		}
		break;

	case GAMEOVER_MENU:
		if (keyDelay <= 0){
			if (keys[13]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_START)) {
				menu = NO_MENU;
				score[0] = score[1] = 0;
				levelInits(1);
				keyDelay = 8;
			}
			if ((keys[8]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_BACK))) {
				menu = MAIN_MENU;
				optionSelected = 0;
				keyDelay = 8;
			}
		}
		else {
			keyDelay--;
		}
		
		break;

	case GAMEWIN_MENU:
		if ((keys[8]
			|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_BACK))) {
			menu = MAIN_MENU;
			optionSelected = 0;
		}
		break;
	case NO_MENU:
		if (keyDelay <= 0 && (keys[27] || gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_START))){
			pause = !pause;
			keyDelay = 8;
		}
		else {
			keyDelay--;
		}

		if (!pause){
			
			//PLAYER 1 CONTROLS
			bool action = false;
			if (Players[0].isAlive()){
				if (keys[GLUT_KEY_LEFT]
					|| gamepad[0].isConnected() && gamepad[0].analogLeft()
					|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_DPAD_LEFT)) {
					Players[0].MoveLeft(Scene.GetMap());
					action = true;
				}

				else if (keys[GLUT_KEY_RIGHT]
					|| gamepad[0].isConnected() && gamepad[0].analogRight()
					|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) {
					Players[0].MoveRight(Scene.GetMap());
					action = true;
				}

				else if (keys[32]
					|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_B)) {
					Players[0].shoot();
					action = true;
				}

				if ((keys[GLUT_KEY_UP]
					|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_A))
					&& !Players[0].isInTheAir()) {
					Players[0].Jump(Scene.GetMap());
					emitSound(SOUND_JUMP_PLAYER);
					action = true;
				}

				if (action == false) Players[0].Stop();
				Players[0].Update(*this);
			}

			//PLAYER 2 CONTROLS
			if (multiplayer && Players[1].isAlive()){
				action = false;
				if (keys[52]
					|| gamepad[1].isConnected() && gamepad[1].analogLeft()
					|| gamepad[1].isConnected() && gamepad[1].isPressed(XINPUT_GAMEPAD_DPAD_LEFT)) {
					Players[1].MoveLeft(Scene.GetMap());
					action = true;
				}

				else if (keys[54]
					|| gamepad[1].isConnected() && gamepad[1].analogRight()
					|| gamepad[1].isConnected() && gamepad[1].isPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) {
					Players[1].MoveRight(Scene.GetMap());
					action = true;
				}

				else if (keys[48]
					|| gamepad[1].isConnected() && gamepad[1].isPressed(XINPUT_GAMEPAD_B)) {
					Players[1].shoot();
					action = true;
				}

				if ((keys[56]
					|| gamepad[1].isConnected() && gamepad[1].isPressed(XINPUT_GAMEPAD_A))
					&& !Players[1].isInTheAir()) {
					Players[1].Jump(Scene.GetMap());
					emitSound(SOUND_JUMP_PLAYER);
					action = true;
				}

				if (action == false) Players[1].Stop();
				Players[1].Update(*this);
			}

			//GAME LOGIC
			for (int i = 0; i < levelZombies; ++i){
				if (zombies[i].isAlive()) zombies[i].Update(*this);
			}

			for (int i = 0; i < levelTanks; ++i){
				if (tanks[i].isAlive()) tanks[i].Update(*this);
			}

			if (demon.isAlive()) demon.Update(*this);

			collisionsLogic();

			ProjectilesLogic();

		}
		
		else {
			if ((keys[8]
				|| gamepad[0].isConnected() && gamepad[0].isPressed(XINPUT_GAMEPAD_BACK))) {
				
				for (int i = 0; i < nPlayers; ++i) Players[i].setAlive(false);
				gameOver();
				pause = false;
				keyDelay = 8;

			}
			break;
		}
		
		break;
	}
	return res;
}


void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	float upp = 1.0f / 1024.0f;
	int windowWidth, windowHeight;
	windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, windowWidth, windowHeight);


	if (menu == NO_MENU) {
		if(multiplayer) glViewport(0, windowHeight / 2, windowWidth, windowHeight / 2);
		
		CameraUpdate(Players[0].GetPositionX(), Players[0].GetPositionY(), 0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(cameraX, GAME_WIDTH + cameraX, cameraY, GAME_HEIGHT + cameraY, -1, 2);

		glMatrixMode(GL_MODELVIEW);

		Scene.Draw(Data.GetID(IMG_BLOCKS));

		if (Players[0].isAlive()) Players[0].Draw(Data.GetID(IMG_PLAYER));
		if (Players[1].isAlive()) Players[1].Draw(Data.GetID(IMG_PLAYER));

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

		drawHud(0);

		if (multiplayer){
			glViewport(0, 0, windowWidth, windowHeight / 2);
			CameraUpdate(Players[1].GetPositionX(), Players[1].GetPositionY(), 1);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(cameraX, GAME_WIDTH + cameraX, cameraY, GAME_HEIGHT + cameraY, -1, 2);

			Scene.Draw(Data.GetID(IMG_BLOCKS));

			if (Players[0].isAlive()) Players[0].Draw(Data.GetID(IMG_PLAYER));
			if (Players[1].isAlive()) Players[1].Draw(Data.GetID(IMG_PLAYER));

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
			drawHud(1);
		}
	}

	else{
		std::string s;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, 0, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		float yo, yf, aux;
		float txo, tyo, txf, tyf;


		switch (menu){
		case MAIN_MENU:
			glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MENUS));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.5f);	glVertex2i(0, 0);
				glTexCoord2f(0.5f, 0.5f);	glVertex2i(1, 0);
				glTexCoord2f(0.5f, 0.0f);	glVertex2i(1, 1);
				glTexCoord2f(0.0f, 0.0f);	glVertex2i(0, 1);
			glEnd();


			aux = 1.0f / 512.0f;
			yo = (3.5f - (float) optionSelected) * 64.0f * aux;
			yf = yo  + 64.0f * aux;

			txo = 8.0f * 64.0f * upp;
			txf = 1.0f;

			tyo = 9.0f * 64.0f * upp;
			tyf = tyo - 64.0f * upp;
			
			glBegin(GL_QUADS);
				glTexCoord2f(txo, tyf);	glVertex2f(0.0f, yo);
				glTexCoord2f(txf, tyf);	glVertex2f(1.0f, yo);
				glTexCoord2f(txf, tyo);	glVertex2f(1.0f, yf);
				glTexCoord2f(txo, tyo);	glVertex2f(0.0f, yf);
			glEnd();

			break;

		case PLAYERSSELECT_MENU:
			glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MENUS_END));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f);	glVertex2i(0, 0);
				glTexCoord2f(0.5f, 1.0f);	glVertex2i(1, 0);
				glTexCoord2f(0.5f, 0.5f);	glVertex2i(1, 1);
				glTexCoord2f(0.0f, 0.5f);	glVertex2i(0, 1);
			glEnd();

			aux = 1.0f / 512.0f;
			yo = (3.5f - (float)playerselected) * 64.0f * aux;
			yf = yo + 64.0f * aux;

			txo = 8.0f * 64.0f * upp;
			txf = 1.0f;

			tyo = 9.0f * 64.0f * upp;
			tyf = tyo - 64.0f * upp;

			glBegin(GL_QUADS);
			glTexCoord2f(txo, tyf);	glVertex2f(0.0f, yo);
			glTexCoord2f(txf, tyf);	glVertex2f(1.0f, yo);
			glTexCoord2f(txf, tyo);	glVertex2f(1.0f, yf);
			glTexCoord2f(txo, tyo);	glVertex2f(0.0f, yf);
			glEnd();

			break;

		case CONTROLS_MENU:
			glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MENUS));
			glBegin(GL_QUADS);
				glTexCoord2f(0.5f, 0.5f);	glVertex2i(0, 0);
				glTexCoord2f(1.0f, 0.5f);	glVertex2i(1, 0);
				glTexCoord2f(1.0f, 0.0f);	glVertex2i(1, 1);
				glTexCoord2f(0.5f, 0.0f);	glVertex2i(0, 1);
			glEnd();
			break;

		case CREDITS_MENU:
			glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MENUS));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f);	glVertex2i(0, 0);
				glTexCoord2f(0.5f, 1.0f);	glVertex2i(1, 0);
				glTexCoord2f(0.5f, 0.5f);	glVertex2i(1, 1);
				glTexCoord2f(0.0f, 0.5f);	glVertex2i(0, 1);
			glEnd();
			break;

		case GAMEOVER_MENU:
			glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MENUS_END));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.5f);	glVertex2i(0, 0);
				glTexCoord2f(0.5f, 0.5f);	glVertex2i(1, 0);
				glTexCoord2f(0.5f, 0.0f);	glVertex2i(1, 1);
				glTexCoord2f(0.0f, 0.0f);	glVertex2i(0, 1);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPushAttrib(GL_CURRENT_BIT);
			glColor3ub(6, 200, 229);
			s = { "Player 1 score: " };

			s = s + std::to_string(score[0]);
			glRasterPos2f(0.4f, 0.6f);
			render_string(GLUT_BITMAP_TIMES_ROMAN_24, s);
			
			if (multiplayer){
				s = { "Player 2 score: " };

				s = s + std::to_string(score[1]);
				glRasterPos2f(0.4f, 0.5f);
				render_string(GLUT_BITMAP_TIMES_ROMAN_24, s);
			}

			glPopAttrib();
			break;

		case GAMEWIN_MENU:
			glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MENUS_END));
			glBegin(GL_QUADS);
				glTexCoord2f(0.5f, 0.5f);	glVertex2i(0, 0);
				glTexCoord2f(1.0f, 0.5f);	glVertex2i(1, 0);
				glTexCoord2f(1.0f, 0.0f);	glVertex2i(1, 1);
				glTexCoord2f(0.5f, 0.0f);	glVertex2i(0, 1);
			glEnd();
			
			glDisable(GL_TEXTURE_2D);
			glPushAttrib(GL_CURRENT_BIT);
			glColor3ub(6, 200, 229);
			s = { "Player 1 score: " };

			s = s + std::to_string(score[0]);
			glRasterPos2f(0.4f, 0.6f);
			render_string(GLUT_BITMAP_TIMES_ROMAN_24, s);
			
			if (multiplayer){
				s = { "Player 2 score: " };

				s = s + std::to_string(score[1]);
				glRasterPos2f(0.4f, 0.5f);
				render_string(GLUT_BITMAP_TIMES_ROMAN_24, s);
			}

			glPopAttrib();
			break;
		}
		glDisable(GL_TEXTURE_2D);
	}

	glutSwapBuffers();

}

void cGame::drawHud(int playerNum)
{
	float xo, xf, yo, yf;
	float rate = 1.0f / 256.0f;
	glDisable(GL_TEXTURE_2D);
	glPushAttrib(GL_CURRENT_BIT);

	glColor3i(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex3i(cameraX, cameraY + GAME_HEIGHT - 19, 0);
		glVertex3i(cameraX + GAME_WIDTH, cameraY + GAME_HEIGHT - 19, 0);
		glVertex3i(cameraX + GAME_WIDTH, cameraY + GAME_HEIGHT, 0);
		glVertex3i(cameraX, cameraY + GAME_HEIGHT, 0);
	glEnd();
	glPopAttrib();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MISC));
	xo = 0.0f;
	xf = 9.0f*13.0f*rate;
	yo = 13.0f*12.0f*rate;
	yf = yo - (3.0f * 13.0f * rate);
	
	glBegin(GL_QUADS);
		glTexCoord2f(xo, yo);			glVertex2i(cameraX + (GAME_WIDTH / 3), cameraY + GAME_HEIGHT - 18);
		glTexCoord2f(xf, yo);			glVertex2i(cameraX + 2*(GAME_WIDTH / 3), cameraY + GAME_HEIGHT - 18);
		glTexCoord2f(xf, yf);			glVertex2i(cameraX + 2*(GAME_WIDTH / 3), cameraY + GAME_HEIGHT -2);
		glTexCoord2f(xo, yf);			glVertex2i(cameraX + (GAME_WIDTH / 3), cameraY + GAME_HEIGHT -2);
	glEnd();



	xo = 0.0f;
	xf = 3.0f*13.0f*rate;
	yo = 13.0f*18.0f*rate;
	yf = yo - (3.0f * 13.0f * rate);
	int xHeart = cameraX + GAME_WIDTH;
	int heartSize = GAME_WIDTH / 12;
	for (int i = 0; i < Players[playerNum].getHealth(); ++i){
		glBegin(GL_QUADS);
			glTexCoord2f(xo, yo);			glVertex2i(xHeart - heartSize + 5, cameraY + GAME_HEIGHT - 13);
			glTexCoord2f(xf, yo);			glVertex2i(xHeart - 5, cameraY + GAME_HEIGHT - 13);
			glTexCoord2f(xf, yf);			glVertex2i(xHeart - 5, cameraY + GAME_HEIGHT-5);
			glTexCoord2f(xo, yf);			glVertex2i(xHeart - heartSize + 5, cameraY + GAME_HEIGHT-5);
		glEnd();
		xHeart -= heartSize;
	}

	glDisable(GL_TEXTURE_2D);


	if (multiplayer){
		for (int i = 0; i < nPlayers; ++i){
			if (Players[i].isAlive()){
				std::string s = { "Player " };
				s = s + std::to_string(i + 1);
				glRasterPos3f(Players[i].GetPositionX() + SCENE_Xo + 8, Players[i].GetPositionY() + SCENE_Yo + 50, -1.0f);
				render_string(GLUT_BITMAP_TIMES_ROMAN_10, s);
			}
		}
	}

	if (pause){
		float upp = 1.0f / 1024.0f;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Data.GetID(IMG_MENUS_END));
		glBegin(GL_QUADS);
		glTexCoord2f(0.5f, 0.5f + 4.0f * 64.0f * upp);		glVertex2f(cameraX, cameraY + (GAME_HEIGHT / 3) -20);
		glTexCoord2f(1.0f, 0.5f + 4.0f * 64.0f * upp);		glVertex2f(cameraX + GAME_WIDTH, cameraY + (GAME_HEIGHT / 3) -20);
		glTexCoord2f(1.0f, 0.5f + 64.0f * upp);				glVertex2f(cameraX + GAME_WIDTH, cameraY + 2 * (GAME_HEIGHT / 3) +20);
		glTexCoord2f(0.5f, 0.5f + 64.0f * upp);				glVertex2f(cameraX, cameraY + 2 * (GAME_HEIGHT / 3) + 20);
		glEnd();


	}

	glDisable(GL_TEXTURE_2D);

	std::string s = { "Score: " };
	s = s + std::to_string(score[playerNum]);
	glRasterPos3f((float)cameraX + 5.0f, (float)cameraY + (float)GAME_HEIGHT - 16.0f, -1.0f);
	render_string(GLUT_BITMAP_HELVETICA_18, s);

}



void cGame::CameraUpdate(int px, int py, int playerNum)
{
	int cameraVx = 2;
	int cameraVy = 2;
	int factor = 2;
	if (Players[playerNum].GetState() == STATE_WALK){
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
}



void cGame::addProjectile(bool right, int x, int y, int type, int shooter)
{
	for (int i = 0; i < MAX_PROJECTILES; ++i){
		if (!Bullets[i].isAlive()){
			Bullets[i].shoot(right, x, y, type, shooter);
			break;
		}
	}
}

void cGame::ProjectilesLogic()
{
	for (int i = 0; i < MAX_PROJECTILES; ++i){

		if (Bullets[i].isAlive()) {

			Bullets[i].logic(Scene.GetMap());

			//Enemy bullets vs players
			if (Bullets[i].whoIsFrom() == -1){
				for (int j = 0; j < nPlayers; ++j){
					if (Players[j].isAlive() && Players[j].collidesWith(Bullets[i].getAABB())){
						Players[j].impact(Bullets[i].GetDamage(), -1);
						Bullets[i].impact();
						gamepad[j].vibrate(1);
					}
				}
			}

			else{
				//Players bullets vs enemies
				for (int j = 0; j < levelZombies; ++j){

					if (zombies[j].isAlive() &&  !zombies[j].isDying() & zombies[j].collidesWith(Bullets[i].getAABB())){
						zombies[j].impact(Bullets[i].GetDamage(), Bullets[i].whoIsFrom());
						Bullets[i].impact();
						emitSound(SOUND_BODY_IMPACT);
					}
				}
				for (int j = 0; j < levelTanks; ++j){
					if (tanks[j].isAlive() && tanks[j].collidesWith(Bullets[i].getAABB())){
						tanks[j].impact(Bullets[i].GetDamage(), Bullets[i].whoIsFrom());
						Bullets[i].impact();
						emitSound(SOUND_BODY_IMPACT);

					}
				}

				if (demon.isAlive() && demon.collidesWith(Bullets[i].getAABB())){
					demon.impact(Bullets[i].GetDamage(), Bullets[i].whoIsFrom());
					Bullets[i].impact();
					emitSound(SOUND_BODY_IMPACT);

				}

				for (int j = 0; j < MAX_PROJECTILES; ++j){
					if (Bullets[i].isAlive() && Bullets[j].isAlive() && (Bullets[j].whoIsFrom() == -1) && Bullets[j].collideWith(Bullets[i].getAABB())){
						Bullets[j].impact();
						Bullets[i].impact();
						score[0] += 1;
						emitSound(SOUND_BODY_IMPACT);

					}
				}
			}


		}
	}
	

}

void cGame::collisionsLogic()
{
	for (int p = 0; p < nPlayers; ++p){

		for (int i = 0; i < levelZombies; i++){
			if (zombies[i].isAlive() && !zombies[i].isDying() && Players[p].collidesWith(zombies[i].getAABB())){
				Players[p].impact(zombies[i].getDamage(), -1);
				gamepad[p].vibrate(1);
			}
		}
		for (int i = 0; i < levelTanks; ++i){
			if (tanks[i].isAlive() && !tanks[i].isDying() && Players[p].collidesWith(tanks[i].getAABB())){
				Players[p].impact(tanks[i].getDamage(), -1);
				gamepad[p].vibrate(1);

			}
		}
		if (demon.isAlive() && Players[p].collidesWith(demon.getAABB())){
			Players[p].impact(demon.getDamage(), -1);
			gamepad[p].vibrate(1);
		}
	}
}

cScene cGame::getScene()
{
	return Scene;
}

cPlayer cGame::getPlayer(int nPlayer)
{
	return Players[nPlayer];
}


struct position{
	int x, y;
};


void cGame::levelInits(int lvl)
{
	/*
	HARDCODED, It would be better to put all those data in a file, but due to time constraints its in the code
	Sorry :(
	*/

	level = lvl;
	bool res = true;
	gameEnd = false;
	
	//GRAPHICS INITIALIZATIONS
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
	BASS_ChannelStop(hBackgroundChannel);

	if (lvl == 1){
		hBackgroundChannel = BASS_SampleGetChannel(lvl1Music, false);

		gameFinished = false;
		levelZombies = 15;
		levelTanks = 3;
		int ZpositionsX[15] = { 10, 15, 4, 10, 15, 47, 53, 56, 72, 77, 80, 49, 55, 60, 65 };
		int ZpositionsY[15] = { 19, 19, 14, 2, 2, 16, 23, 23, 20, 20, 14, 2, 2, 2, 2 };

		int TPositionsX[3] = { 6, 35, 86 };
		int TPositionsY[3] = { 4, 21, 26 };

		//PLAYERS INITIALIZATIONS
		Players[0].init();
		Players[0].SetWidthHeight(36, 40);
		Players[0].SetTile(4, 26);
		Players[0].SetState(STATE_LOOK);
		Players[0].SetDirection(1);
		Players[0].setPlayerNumber(0);

		if (multiplayer){
			Players[1].init();
			Players[1].SetWidthHeight(36, 40);
			Players[1].SetTile(6, 26);
			Players[1].SetState(STATE_LOOK);
			Players[1].SetDirection(1);
			Players[1].setPlayerNumber(1);
		}
		else Players[1].die(-1);

		//ENEMIES INITIALIZATIONS
		for (int i = 0; i < levelZombies; ++i){
			zombies[i].init();
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetTile(ZpositionsX[i], ZpositionsY[i]);
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetState(STATE_WALK);
			zombies[i].SetDirection(1);
		}
		for (int i = 0; i < levelTanks; ++i){
			tanks[i].init();
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetTile(TPositionsX[i], TPositionsY[i]);
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetState(STATE_WALK);
			tanks[i].SetDirection(1);
		}
		demon.init();
		demon.SetWidthHeight(64, 64);
		demon.SetTile(90, 4);
		demon.SetWidthHeight(64, 64);
		demon.SetState(STATE_WALK);
		demon.SetDirection(1);
	}

	if (lvl == 2){
		hBackgroundChannel = BASS_SampleGetChannel(lvl2Music, false);
		
		levelZombies = 20;
		levelTanks = 4;
		int ZpositionsX[20] = { 9, 24, 37, 49, 60, 75, 88, 82, 81, 83, 60, 57, 25, 32, 30, 28, 54, 42, 15, 80 };
		int ZpositionsY[20] = { 2, 2, 2, 2, 2, 2, 2, 9, 15, 21, 21, 15, 9, 9, 15, 15, 2, 2, 2, 2 };

		int TPositionsX[4] = { 89, 30, 64, 30 };
		int TPositionsY[4] = { 26, 26, 9, 15 };

		Players[0].init();
		Players[0].SetWidthHeight(36, 40);
		Players[0].SetTile(2, 2);
		Players[0].SetWidthHeight(36, 40);
		Players[0].SetState(STATE_LOOK);
		Players[0].SetDirection(1);
		Players[0].setPlayerNumber(0);

		
		if (multiplayer){
			Players[1].init();
			Players[1].SetWidthHeight(36, 40);
			Players[1].SetTile(4, 2);
			//Player.SetWidthHeight(36, 40);
			Players[1].SetState(STATE_LOOK);
			Players[1].SetDirection(1);
			Players[1].setPlayerNumber(1);
		}
		else Players[1].die(-1);

		//Enemies inicialitzations
		for (int i = 0; i < levelZombies; ++i){
			zombies[i].init();
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetTile(ZpositionsX[i], ZpositionsY[i]);
			zombies[i].SetWidthHeight(36, 40);
			zombies[i].SetState(STATE_WALK);
			zombies[i].SetDirection(1);
		}
		for (int i = 0; i < levelTanks; ++i){
			tanks[i].init();
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetTile(TPositionsX[i], TPositionsY[i]);
			tanks[i].SetWidthHeight(64, 64);
			tanks[i].SetState(STATE_WALK);
			tanks[i].SetDirection(1);
		}
		demon.init();
		demon.SetWidthHeight(64, 64);
		demon.SetTile(14, 20);
		demon.SetWidthHeight(64, 64);
		demon.SetState(STATE_WALK);
		demon.SetDirection(1);
	}
	BASS_ChannelPlay(hBackgroundChannel, true);

} 


void cGame::gameOver()
{
	bool playersDead = true;
	for (int i = 0; i < nPlayers; ++i) if (Players[i].isAlive()) playersDead = false;
	if (playersDead){
		menu = GAMEOVER_MENU;
		gameEnd = true;
		BASS_ChannelStop(hBackgroundChannel);
		hBackgroundChannel = BASS_SampleGetChannel(gameOverMusic, false);
		BASS_ChannelPlay(hBackgroundChannel, true);
	}
}

bool cGame::isOver()
{
	return gameEnd;
}


void cGame::render_string(void* font, const std::string s)
{
	for (int i = 0; i < s.length(); i++)
		glutBitmapCharacter(font, s[i]);
}

void cGame::levelFinished()
{
	if (level < 2){
		level++;
		levelInits(level);	
	}
	else{
		gameFinished = true;
		menu = GAMEWIN_MENU;
		BASS_ChannelStop(hBackgroundChannel);
		hBackgroundChannel = BASS_SampleGetChannel(gameWinMusic, false);
		BASS_ChannelPlay(hBackgroundChannel, true);

	}

}

void cGame::updateScore(int points, int player)
{
	score[player] += points;

}

void cGame::emitSound(int type){
	switch (type){
	case SOUND_JUMP_PLAYER:
		hPlayerChannel = BASS_SampleGetChannel(jumpSound, false);
		BASS_ChannelPlay(hPlayerChannel, false);
		break; 

	case SOUND_SHOOT_PLAYER:
		hPlayerChannel = BASS_SampleGetChannel(shootSound, false);
		BASS_ChannelPlay(hPlayerChannel, false);
		break;
	case SOUND_BODY_IMPACT:
		hImpactsChannel = BASS_SampleGetChannel(impactBodySound, false);
		BASS_ChannelPlay(hImpactsChannel, false);
		break;
	case SOUND_WALL_IMPACT:
		hImpactsChannel = BASS_SampleGetChannel(impactWallSound, false);
		BASS_ChannelPlay(hImpactsChannel, false);
		break;
	case SOUND_SHOOT_ENEMY:
		hTankChannel = BASS_SampleGetChannel(shootEnemySound, false);
		BASS_ChannelPlay(hTankChannel, false);
		break;
	case SOUND_MENU_MOV:
		hPlayerChannel = BASS_SampleGetChannel(menuMovement, false);
		BASS_ChannelPlay(hPlayerChannel, false);
		break;
	}
}

int cGame::getNumPlayers()
{
	return nPlayers;
}
