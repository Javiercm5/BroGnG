#include "cZombie.h"

cZombie::cZombie()
{
	STEP_LENGTH = 1;
	FRAME_DELAY = 8;
	alive = true;
}


cZombie::~cZombie()
{
}

void cZombie::intelligence(int *map, int playerX, int playerY)
{
	//BASIC
	STEP_LENGTH = 1;
	if (canGoForward(true, map) == false) goRight = false;
	if (canGoForward(false, map) == false) goRight = true;

	int posx, posy;
	GetPosition(&posx, &posy);

	//ATTACK
	if (playerY == posy){
		int threshold = std::abs(playerX - posx);
		if (threshold <= 3 * 36){
			(playerX > posx ? goRight = true : goRight = false);
			STEP_LENGTH = 2;
		}
	}

	(goRight ? MoveRight(map) : MoveLeft(map));

	//TODO: IF Threshold, SpeedUP enemy
}

void cZombie::Draw(int tex_id)
{
	float xo, yo, xf, yf;
	float upp = 1.0f / 1024.0f;	//Units Per Pixel

	yo = upp * (facingRight ? 120.0f : 160.0f);
	switch (GetState())
	{
		case STATE_LOOK:	xo = 0.0f; break;
		case STATE_WALK:	xo = 0.0f + (GetFrame()*(upp*36.0f));
							NextFrame(4); break;
	}
	xf = xo + upp * 36.0f;
	yf = yo - upp * 40.0f;

	DrawRect(tex_id, xo, yo, xf, yf);
}

