#include "cDemon.h"
#include "cGame.h"




cDemon::cDemon()
{
	STEP_LENGTH = 1;
	FRAME_DELAY = 10;
	alive = true;
	health = 20;
	SetWidthHeight(64, 64);
	bichoDelay = 40;
	shootDelay = 0;
	damage = 2;

}


cDemon::~cDemon()
{
}


void cDemon::intelligence(int *map, int playerX, int playerY)
{
	int posx, posy;
	GetPosition(&posx, &posy);
	int threshold = std::abs(playerX - posx);
	if (threshold <= 3 * 36) waiting = false;
	bool attack = false;

	if (waiting) SetState(STATE_LOOK);
	else{
		bool walk;
		if (playerY == posy){
			
			//ESCAPE
			if (threshold <= 2 * 36){
				walk = true;
				if (playerX < posx) goRight = true;
				else goRight = false;

				if ((goRight && !canGoForward(true, map))) attack = true;
				else if (!goRight && !canGoForward(false, map)) attack = true;
				else (goRight ? MoveRight(map) : MoveLeft(map));
			}

			//ATTACK
			if (attack || threshold > 2 * 36){
				walk = false;
				if (playerX > posx) goRight = facingRight = true;
				else goRight = facingRight = false;
				shoot();
			}		
		}
		else waiting = true;
	}
}



void cDemon::Draw(int tex_id)
{
	if (!dying || (glutGet(GLUT_ELAPSED_TIME) / 300) % 2){

		float xo, yo, xf, yf;
		float upp = 1.0f / 1024.0f;	//Units Per Pixel

		yo = upp * (facingRight ? 384.0f : 448.0f);

		switch (GetState())
		{
		case STATE_LOOK:
			xo = 0.0f; break;
		case STATE_WALK:	xo = 64.0f * upp + (GetFrame()*(upp*64.0f));
			NextFrame(3); break;
		case STATE_SHOOT:
			xo = 4.0f * 64.0f*upp + (GetFrame()*(upp*64.0f));
			NextFrame(4); break;
		}
		xf = xo + upp * 64.0f;
		yf = yo - upp * 64.0f;

		DrawRect(tex_id, xo, yo, xf, yf);
	}
}

void cDemon::Update(cGame& g)
{
		if (dyingDelay > 0){
			dyingDelay--;
		}
		else if (dyingDelay == 0 && dying){
			alive = false;
			g.updateScore(100, deadBy);
			g.levelFinished();

		}
		else intelligence(g.getScene().GetMap(), g.getPlayer(0).GetPositionX(), g.getPlayer(0).GetPositionY());


		Logic(g.getScene().GetMap());

		if (shootDelay == bichoDelay) g.addProjectile(facingRight, GetPositionX(), GetPositionY() + 30, TYPE_FIREBALL, true);
		if (shootDelay > 0) --shootDelay;
}