#include "cTank.h"
#include "cGame.h"



cTank::cTank()
{
	STEP_LENGTH = 1;
	FRAME_DELAY = 8;
	bichoDelay = 30;
	damage = 2;
	stateMaxFrames[STATE_LOOK] = 1;
	stateMaxFrames[STATE_WALK] = 4;
	stateMaxFrames[STATE_SHOOT] = 3;

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

void cTank::intelligence(cGame& g)
{
	//BASIC
	bool walk = true;
	int *map = g.getScene().GetMap();
	if (canGoForward(true, map) == false) goRight = false;
	if (canGoForward(false, map) == false) goRight = true;

	int posx, posy;
	GetPosition(&posx, &posy);

	//ATTACK

	int nearestDist = -1;
	for (int i = 0; i < g.getNumPlayers(); ++i){

		if (g.getPlayer(i).GetPositionY() == posy){
			int threshold = std::abs(g.getPlayer(i).GetPositionX() - posx);

			if (threshold > 3 * 36 && threshold < 6 * 36){
				walk = false;
				if (nearestDist == -1){
					nearestDist = threshold;
					(g.getPlayer(i).GetPositionX() > posx ? goRight = facingRight = true : goRight = facingRight = false);
				}
				else if (threshold < nearestDist){
					nearestDist = threshold;
					(g.getPlayer(i).GetPositionX() > posx ? goRight = facingRight = true : goRight = facingRight = false);

				}
			}

		}

	}
	if (walk) (goRight ? MoveRight(map) : MoveLeft(map));
	else shoot();
}

void cTank::Draw(int tex_id)
{
	if (!dying || (glutGet(GLUT_ELAPSED_TIME) / 150) % 2){

		float xo, yo, xf, yf;
		float upp = 1.0f / 1024.0f;	//Units Per Pixel

		yo = upp * (facingRight ? 256.0f : 320.0f);
		switch (GetState())
		{
		case STATE_LOOK:	xo = 0.0f; break;
		case STATE_WALK:	xo = (GetFrame()*(upp*64.0f));
			break;
		case STATE_SHOOT:
			xo = 3.0f * 64.0f*upp + (GetFrame()*(upp*64.0f));
			break;
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
	else 	intelligence(g);


	Logic(g.getScene().GetMap());

	if (shootDelay == bichoDelay){
		if(facingRight) g.addProjectile(facingRight, GetPositionX() + 64, GetPositionY() + 30, TYPE_FIREBALL, -1);
		else g.addProjectile(facingRight, GetPositionX(), GetPositionY() + 30, TYPE_FIREBALL, -1);

		g.emitSound(SOUND_SHOOT_ENEMY);
	}
	if (shootDelay > 0) --shootDelay;

	if (GetState() != STATE_LOOK){
		NextFrame(stateMaxFrames[GetState()]);
	}
}
