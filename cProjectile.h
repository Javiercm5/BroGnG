#pragma once

#include "Globals.h"

#define TYPE_SPEAR		0
#define TYPE_FIREBALL	1

class cProjectile
{

private:
	bool facingRight, alive;
	int posX, posY, w, h;	
	int projectileType, speed, damage;
	int seq, delay, FRAME_DELAY;
	
	bool CollidesMapWall(int *map, bool right);
	void NextFrame(int max);
	int  GetFrame();


public:

	cProjectile();
	~cProjectile();

	void shoot(bool shootRight, int xo, int yo, int type);
	void impact();
	bool collideWith();
	void logic(int *map);

	AABB getAABB();
	bool isAlive();
	int GetDamage();
	int GetType();

	void draw(int tex_id);

};

