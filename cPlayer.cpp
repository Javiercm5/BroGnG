
#include "cPlayer.h"
#include "cGame.h"
#include "iostream"

cPlayer::cPlayer()
{
	bichoDelay = 15;
	shootDelay = 0;
	health = 4;
}
cPlayer::~cPlayer()
{}

void cPlayer::Draw(int tex_id)
{
	if (!godMode || (glutGet(GLUT_ELAPSED_TIME) / 500) % 2){
		float xo, yo, xf, yf;
		float upp = 1.0f / 1024.0f;	//Units Per Pixel

		yo = upp * (facingRight ? 40.0f : 80.0f);
		if (health <= 2) yo = upp * (facingRight ? 520.0f : 560.0f);
		if (jumping) {
			xo = 0.0f + upp*36.0f*7.0f;
			int state = GetState();
			if (state == STATE_WALK) xo = 0.0f + upp*36.0f*8.0f;
			else if (state == STATE_SHOOT){
				xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f));
				NextFrame(2);
			}
		}

		else if (falling){
			xo = 0.0f + upp*36.0f*7.0f;
			if (GetState() == STATE_SHOOT) {
				xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f));
				NextFrame(2);
			}
		}
		else{
			switch (GetState())
			{
			case STATE_LOOK:	xo = 0.0f; break;
			case STATE_WALK:	xo = 0.0f + (GetFrame()*(upp*36.0f));
				NextFrame(7); break;
				//case STATE_CROUCH:	xo = 0.0f + upp*36.0f*11.0f; break;

			case STATE_SHOOT:	xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f));
				NextFrame(2); break;
			}
		}

		xf = xo + upp * 36.0f;
		yf = yo - upp * 40.0f;

		DrawRect(tex_id, xo, yo, xf, yf);
	}
}

void cPlayer::impact(int damage, int player) {
	
	if (!godMode){
		health -= damage;
		if (health <= 0) alive = false;
		
		//godmode 3 sec
		else{
			godMode = true;
			invulnerableDelay = 100;
		}
	}
}


void cPlayer::Update(cGame& g)
{
	if (health <= 0){
		g.gameOver();
		alive = false;
	}
	else{
		if (godMode){
			invulnerableDelay--;
			if (invulnerableDelay == 0) godMode = false;
		}


		Logic(g.getScene().GetMap());

		if (shootDelay == bichoDelay){
			if (facingRight) g.addProjectile(facingRight, GetPositionX(), GetPositionY() + 30, TYPE_SPEAR, false);
			else g.addProjectile(facingRight, GetPositionX(), GetPositionY() + 30, TYPE_SPEAR, false);
		}
		if (shootDelay > 0) --shootDelay;
	}



}


bool cPlayer::isGod()
{
	return godMode;
}

int cPlayer::getHealth()
{
	return health;
}