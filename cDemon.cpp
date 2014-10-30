#include "cDemon.h"
#include "cGame.h"




cDemon::cDemon()
{
	STEP_LENGTH = 1;
	FRAME_DELAY = 10;
	bichoDelay = 40;
	damage = 2;

}


cDemon::~cDemon()
{

}

void cDemon::init()
{
	alive = true;
	health = 10;
	shootDelay = 0;
	dying = false;
}


void cDemon::intelligence(cGame& g)
{
	int posx, posy;
	GetPosition(&posx, &posy);
	int *map = g.getScene().GetMap();
	bool attack = false;
	bool walk = true;

	for (int i = 0; i < g.getNumPlayers(); ++i){
		int threshold = std::abs(g.getPlayer(i).GetPositionX() - posx);
		if (g.getPlayer(i).isAlive() && (posy >= (g.getPlayer(i).GetPositionY() - 36) && posy <= (g.getPlayer(i).GetPositionY() + 36))){
			if (threshold <= 5 * 36){
				walk = false;
				(g.getPlayer(i).GetPositionX() > posx ? goRight = facingRight = true : goRight = facingRight = false);
				shoot();
			}
			else {
				if ((g.getPlayer(i).GetPositionX() > posx) && (canGoForward(false, map) == false)) goRight = true;
				else if ((g.getPlayer(i).GetPositionX() < posx) && (canGoForward(true, map) == false)) goRight = false;
				(goRight ? MoveRight(map) : MoveLeft(map));       
			}
		}
	}
}



void cDemon::Draw(int tex_id)
{
	if (!dying || (glutGet(GLUT_ELAPSED_TIME) / 150) % 2){

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
		else intelligence(g);


		Logic(g.getScene().GetMap());

		if (shootDelay == bichoDelay){
			if (facingRight)g.addProjectile(facingRight, GetPositionX() + 64, GetPositionY() + 30, TYPE_FIREBALL, -1);
			else g.addProjectile(facingRight, GetPositionX() , GetPositionY() + 30, TYPE_FIREBALL, -1);
			g.emitSound(SOUND_SHOOT_ENEMY);
		}
		if (shootDelay > 0) --shootDelay;
}