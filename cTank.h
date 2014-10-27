#pragma once
#include "cBicho.h"


#define PROJECTILES_TANK_DELAY		30

class cTank :
	public cBicho
{
public:
	cTank();
	~cTank();
	void intelligence(int *map, int playerX, int playerY);
	void Draw(int tex_id);
	void Update(cGame& g) override;
	void init() override;



private:
	bool goRight;
};

