#pragma once

#include "Globals.h"

class cProjectile
{

private:
	bool facingRight;
	int posX, posY;
	bool alive;
	int w = 30;
	int h = 8;
	bool CollidesMapWall(int *map, bool right);

public:
	cProjectile();
	~cProjectile();

	void shoot(bool shootRight, int xo, int yo);
	void impact();

	void getAABB(int *minX, int *minY, int *maxX, int *maxY);

	bool isAlive();
	void logic(int *map);

	void draw(int tex_id, bool facingRight);

	bool collideWith();

	AABB getAABB();
};

