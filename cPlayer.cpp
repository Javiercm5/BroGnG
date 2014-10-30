
#include "cPlayer.h"
#include "cGame.h"

cPlayer::cPlayer()
{
	bichoDelay = 15;
	stateMaxFrames[STATE_LOOK] = 1;
	stateMaxFrames[STATE_WALK] = 7;
	stateMaxFrames[STATE_SHOOT] = 2;
	FRAME_DELAY = 4;
	bulletsDelay = PROJECTILES_PLAYER_DELAY;
}
cPlayer::~cPlayer()
{}

void cPlayer::init()
{
	health = 4;
	alive = true;
	falling = false;
	SetState(STATE_LOOK);
	jumping = false;
	crouching = false;
	facingRight = true;
	shootDelay = 0;
	dyingDelay = 0;

}


void cPlayer::setPlayerNumber(int number)
{
	playerNum = number;
}

void cPlayer::Draw(int tex_id)
{
	if (!godMode || (glutGet(GLUT_ELAPSED_TIME) / 500) % 2){
		float xo, yo, xf, yf;
		float upp = 1.0f / 1024.0f;	//Units Per Pixel

		yo = upp * (facingRight ? 40.0f : 80.0f);
		if (health <= 2) yo = upp * (facingRight ? 520.0f : 560.0f);
		switch (GetState())
		{
			case STATE_LOOK:
				if (jumping)xo = 0.0f + upp*36.0f*7.0f;
				else xo = 0.0f; 
				break;
			case STATE_WALK:	
				if (jumping) xo = 0.0f + upp*36.0f*8.0f;
				else xo = 0.0f + (GetFrame()*(upp*36.0f));
				break;

			case STATE_SHOOT:	
				xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f)); 
				break;
		}

		xf = xo + upp * 36.0f;
		yf = yo - upp * 40.0f;

		DrawRect(tex_id, xo, yo, xf, yf);
	}
}

void cPlayer::impact(int damage, int player) {
	
	if (!godMode){
		health -= damage;
		
		if(health > 0){
			godMode = true;
			invulnerableDelay = 100;
		}
	}
}


void cPlayer::Update(cGame& g)
{
	if (health <= 0 && !g.isOver()){
		alive = false;
		g.gameOver();

	}
	else{
		if (godMode){
			invulnerableDelay--;
			if (invulnerableDelay == 0) godMode = false;
		}


		Logic(g.getScene().GetMap());

		if (GetState() != STATE_LOOK){
			NextFrame(stateMaxFrames[GetState()]);
		}

		if (shootDelay == bichoDelay){
			g.addProjectile(facingRight, GetPositionX(), GetPositionY() + 30, TYPE_SPEAR, playerNum);
			g.emitSound(SOUND_SHOOT_PLAYER);
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

void cPlayer::heal(int amount)
{
	health += amount;
}

