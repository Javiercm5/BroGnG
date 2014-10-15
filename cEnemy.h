#pragma once
#include "cBicho.h"


class cEnemy :
	public cBicho
{
public:
	cEnemy();
	~cEnemy();
	void intelligence(int *map, int playerX, int playerY);
	void Draw(int tex_id);


private:
	bool goRight;
};

