#include "cDemon.h"


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
