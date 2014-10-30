#pragma once

#include "Globals.h"

#define TYPE_SPEAR		0
#define TYPE_FIREBALL	1
#define TYPE_AXE		3

class cProjectile
{

private:
	bool facingRight, alive;
	int posX, posY, w, h;	
	int projectileType, speed, damage;
	int seq, delay, FRAME_DELAY;
	int owner;		//-1 enemy, 0...n players

	bool CollidesMapWall(int *map, bool right);
	void NextFrame(int max);
	int  GetFrame();


public:

	cProjectile();
	~cProjectile();

	void shoot(bool shootRight, int xo, int yo, int type, int fromWho);
	int whoIsFrom();
	void impact();
	bool collideWith(AABB other);
	void logic(int *map);

	AABB getAABB();
	bool isAlive();
	int GetDamage();
	int GetType();

	void draw(int tex_id);

};

