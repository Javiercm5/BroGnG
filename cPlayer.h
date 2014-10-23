#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2
#define PROJECTILES_PLAYER_DELAY	15


class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	void impact(int damage) override;
	void Update(cGame& g) override;

private:
	int bulletsDelay = PROJECTILES_PLAYER_DELAY;

};
