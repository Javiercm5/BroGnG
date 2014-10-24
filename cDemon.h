#pragma once
#include "cBicho.h"
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

};

