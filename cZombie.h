#pragma once
#include "cBicho.h"


class cZombie :
	public cBicho
{
public:
	cZombie();
	~cZombie();
	void intelligence(int *map, int playerX, int playerY);
	void Draw(int tex_id);


private:
	bool goRight;
	bool alive;
};
