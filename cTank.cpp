#include "cTank.h"
#include "cGame.h"



cTank::cTank()
{
	STEP_LENGTH = 1;
	FRAME_DELAY = 13;
	bichoDelay = 30;
	damage = 2;

}
cTank::~cTank()
{
}

void cTank::init()
{
	alive = true;
	health = 6;
	shootDelay = 0;
	dying = false;
}

void cTank::intelligence(int *map, int playerX, int playerY)
{
	//BASIC
	bool walk = true;
	if (canGoForward(true, map) == false) goRight = false;
	if (canGoForward(false, map) == false) goRight = true;

	int posx, posy;
	GetPosition(&posx, &posy);

	//ATTACK
	if (playerY == posy){
		int threshold = std::abs(playerX - posx);
		if (playerX > posx) goRight = facingRight = true;
		else goRight = facingRight = false;

		if (threshold > 3 * 36 && threshold < 6*36){
			walk = false;
			shoot();
		}
	}
	if (walk) (goRight ? MoveRight(map) : MoveLeft(map));

}

void cTank::Draw(int tex_id)
{
	if (!dying || (glutGet(GLUT_ELAPSED_TIME) / 300) % 2){

		float xo, yo, xf, yf;
		float upp = 1.0f / 1024.0f;	//Units Per Pixel

		yo = upp * (facingRight ? 256.0f : 320.0f);
		switch (GetState())
		{
		case STATE_LOOK:	xo = 0.0f; break;
		case STATE_WALK:	xo = (GetFrame()*(upp*64.0f));
			NextFrame(3); break;
		case STATE_SHOOT:
			xo = 3.0f * 64.0f*upp + (GetFrame()*(upp*64.0f));
			NextFrame(3); break;
		}
		xf = xo + upp * 64.0f;
		yf = yo - upp * 64.0f;

		DrawRect(tex_id, xo, yo, xf, yf);
	}
}

void cTank::Update(cGame& g)
{
	if (dyingDelay > 0){
		dyingDelay--;
	}
	else if (dyingDelay == 0 && dying){
		g.updateScore(30, deadBy);

		alive = false;
	}
	else 	intelligence(g.getScene().GetMap(), g.getPlayer(0).GetPositionX(), g.getPlayer(0).GetPositionY());


	Logic(g.getScene().GetMap());

	if (shootDelay == bichoDelay){
		g.addProjectile(facingRight, GetPositionX(), GetPositionY() + 30, TYPE_FIREBALL, true);
		g.emitSound(SOUND_SHOOT_ENEMY);
	}
	if (shootDelay > 0) --shootDelay;
}
