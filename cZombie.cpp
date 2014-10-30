#include "cZombie.h"
#include "cGame.h"

cZombie::cZombie()
{
	stateMaxFrames[STATE_LOOK] = 1;
	stateMaxFrames[STATE_WALK] = 4;
	stateMaxFrames[STATE_SHOOT] = 0;
	STEP_LENGTH = 1;
	FRAME_DELAY = 8;
}
cZombie::~cZombie()
{


}
void cZombie::init()
{
	alive = true;
	health = 2;
	damage = 1;
	dying = false;

}

void cZombie::intelligence(cGame& g)
{
	//BASIC
	STEP_LENGTH = 1;
	int *map = g.getScene().GetMap();
	if (canGoForward(true, map) == false) goRight = false;
	if (canGoForward(false, map) == false) goRight = true;

	int posx, posy;
	GetPosition(&posx, &posy);

	//ATTACK
	int nearestDist = -1;
	for (int i = 0; i < g.getNumPlayers(); ++i){
		
		if (g.getPlayer(i).GetPositionY() == posy && g.getPlayer(i).isAlive()){
			int threshold = std::abs(g.getPlayer(i).GetPositionX() - posx);
			
			if (threshold <= 3 * 36){
				if (nearestDist == -1){
					nearestDist = threshold;
					(g.getPlayer(i).GetPositionX() > posx ? goRight = true : goRight = false);
				}
				else if (threshold < nearestDist){
					nearestDist = threshold;
					(g.getPlayer(i).GetPositionX() > posx ? goRight = true : goRight = false);

				}
				STEP_LENGTH = 2;
			}

		}
	}
	(goRight ? MoveRight(map) : MoveLeft(map));
}

void cZombie::Draw(int tex_id)
{

	if (!dying || (glutGet(GLUT_ELAPSED_TIME) / 150) % 2){

		float xo, yo, xf, yf;
		float upp = 1.0f / 1024.0f;	//Units Per Pixel

		yo = upp * (facingRight ? 120.0f : 160.0f);
		switch (GetState())
		{
		case STATE_LOOK:	xo = 4.0f*upp*36.0f; break;
		case STATE_WALK:	xo = 0.0f + (GetFrame()*(upp*36.0f));
			break;
		}
		xf = xo + upp * 36.0f;
		yf = yo - upp * 40.0f;

		DrawRect(tex_id, xo, yo, xf, yf);
	}
}

void cZombie::Update(cGame& g)
{
	if (dyingDelay > 0){
		dyingDelay--;
	}
	else if (dyingDelay == 0 && dying){
		g.updateScore(10, deadBy);
		alive = false;
	}
	else intelligence(g);

	Logic(g.getScene().GetMap());

	if (GetState() != STATE_LOOK){
		NextFrame(stateMaxFrames[GetState()]);
	}
}
