#include "cEnemy.h"

cEnemy::cEnemy()
{

	STEP_LENGTH = 1;
	FRAME_DELAY = 8;
}


cEnemy::~cEnemy()
{
}

void cEnemy::intelligence(int *map, int playerX, int playerY)
{
	//BASIC
	if (canGoForward(true, map) == false) goRight = false;
	if (canGoForward(false, map) == false) goRight = true;

	int posx, posy;
	GetPosition(&posx, &posy);
	
	if (playerY == posy){
		int threshold = std::abs(playerX - posx);
		if (threshold <= 2 * 36){
			//Follow
			(playerX > posx ? goRight = true : goRight = false);
		}
	}

	(goRight ? MoveRight(map) : MoveLeft(map));

	//TODO: IF Threshold, SpeedUP enemy
}

void cEnemy::Draw(int tex_id)
{
	float xo, yo, xf, yf;
	float upp = 1.0f / 1024.0f;	//Units Per Pixel

	yo = upp * (facingRight ? 120.0f : 160.0f);
	if (jumping) {
		switch (GetState())
		{
		case STATE_LOOK:xo = 0.0f + upp*36.0f*7.0f; break;
		case STATE_WALK:xo = 0.0f + upp*36.0f*8.0f; break;
		case STATE_CROUCH:xo = 0.0f + upp*36.0f*16.0f; break;
		case STATE_SHOOT:	xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f));
			NextFrame(2); break;
		}
	}
	else{
		switch (GetState())
		{
		case STATE_LOOK:	xo = 0.0f; break;
		case STATE_WALK:	xo = 0.0f + (GetFrame()*(upp*36.0f));
			NextFrame(4); break;
		case STATE_CROUCH:	xo = 0.0f + upp*36.0f*11.0f; break;

		case STATE_SHOOT:	xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f));
			NextFrame(2); break;
		}
	}

	xf = xo + upp * 36.0f;
	yf = yo - upp * 40.0f;

	DrawRect(tex_id, xo, yo, xf, yf);
}

