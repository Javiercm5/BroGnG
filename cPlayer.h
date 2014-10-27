#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2
#define PROJECTILES_PLAYER_DELAY	15
#define PLAYER_MAX_HEALTH		4


class cPlayer : public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void Draw(int tex_id);
	void impact(int damage, int player) override;
	void Update(cGame& g) override;
	int getHealth();
	void heal(int amount);
	void init() override;



private:
	int bulletsDelay = PROJECTILES_PLAYER_DELAY;
	bool godMode;
	int invulnerableDelay;
	bool isGod();

};
