#pragma once
#include "cBicho.h"

#define STATE_IDLE2	4


class cDemon :
	public cBicho
{
public:
	cDemon();
	~cDemon();
	void intelligence(int *map, int playerX, int playerY);
	void Draw(int tex_id);
	void Update(cGame& g) override;

private: 	
	bool goRight;
	bool waiting;

};

