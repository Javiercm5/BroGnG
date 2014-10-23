#include "cProjectile.h"
#include "cData.h"
#include "cScene.h"
#include "iostream"

cProjectile::cProjectile()
{
	alive = false;
	seq = 0;
	delay = 0;
	FRAME_DELAY = 0;
	speed = 3;
}

cProjectile::~cProjectile(){}

void cProjectile::shoot(bool shootRight, int xo, int yo, int type, bool enemy)	
{
	facingRight = shootRight;
	posX = xo;
	posY = yo;
	alive = true;
	isFromEnemy = enemy;
	
	switch (type){
		case TYPE_SPEAR: 
			w = 36; h = 12; speed = 3;  damage = 1; break;
		case TYPE_FIREBALL:
			w = 16; h = 16; speed = 2; damage = 2;
			FRAME_DELAY = 20;
			break;
		case TYPE_AXE:
			w = 23; h = 23; speed = 3; damage = 1; FRAME_DELAY = 3; break;
	}
	
	projectileType = type;

}

void cProjectile::impact(){
	alive = false;
}





void cProjectile::logic(int *map)
{
	int xaux;

	(facingRight ? posX += speed : posX -= speed);
	if (CollidesMapWall(map, facingRight)){
		impact();
	}


	/*
	//////////////
	if ((posX % TILE_SIZE) == 0){
		xaux = posX;
		(facingRight ? posX += speed : posX -= speed);

		if (CollidesMapWall(map, facingRight)){
			impact();
		}
		//impactar enemic ese if ...
	}
	else (facingRight ? posX += speed : posX -= speed);
	*/

}


bool cProjectile::CollidesMapWall(int *map, bool right)
{
	int tile_x, tile_y;
	int j;
	int width_tiles, height_tiles;

	tile_x = posX / TILE_SIZE;
	tile_y = posY / TILE_SIZE;
	width_tiles = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if (right)	tile_x += width_tiles;

	if (height_tiles == 0) if (map[tile_x + ((tile_y)*SCENE_WIDTH)] != -1)	return true;

	for (j = 0; j<height_tiles; j++)
	{
		if (map[tile_x + ((tile_y + j)*SCENE_WIDTH)] != -1)	return true;
	}

	return false;
	
}




void cProjectile::draw(int tex_id)
{
	float upp = 1.0f / 256.0f;	//Units Per Pixel
	float xo, yo;
	xo = 0.0f;

	switch (projectileType){
		case TYPE_SPEAR:
			xo = upp *(facingRight ? 0.0f : 36.0f);
			yo = 12.0f * upp;
			break;
		case TYPE_FIREBALL:
			yo = upp * 32.0f;
			xo = 0.0f + (GetFrame()*(upp*16.0f));
			NextFrame(3);
			break;
		case TYPE_AXE:
			yo = upp * 69.0f;
			xo = 0.0f + (GetFrame()*(upp*23.0f));
			NextFrame(8);
			break;
	}
	
	
	float xf = xo + float(w) * upp;
	float yf = yo - upp * float(h);

	int screen_x = posX + SCENE_Xo;
	int screen_y = posY + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glBegin(GL_QUADS);
			glTexCoord2f(xo, yo);	glVertex2i(screen_x, screen_y);
			glTexCoord2f(xf, yo);	glVertex2i(screen_x + w, screen_y);
			glTexCoord2f(xf, yf);	glVertex2i(screen_x + w, screen_y + h);
			glTexCoord2f(xo, yf);	glVertex2i(screen_x, screen_y + h);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	
}


bool cProjectile::isAlive()
{
	return alive;
}

AABB cProjectile::getAABB(){
	AABB aabb;
	aabb.minX = posX;
	aabb.minY = posY;
	aabb.maxX = posX + w;
	aabb.maxY = posY + h;
	return aabb;
}


void cProjectile::NextFrame(int max)
{
	delay++;
	if (delay == FRAME_DELAY)
	{
		if (++seq >= max) {
			seq %= max;
		}
		delay = 0;
	}

}
int cProjectile::GetFrame()
{
	return seq;
}

int cProjectile::GetDamage()
{
	return damage;
}

int cProjectile::GetType()
{
	return projectileType;
}

bool cProjectile::isEnemy()
{
	return isFromEnemy;
}

bool cProjectile::collideWith(AABB other)
{
	if (posX + w < other.minX || posX > other.maxX) return false;
	if (posY + h < other.minY || posY > other.maxY) return false;
	return true;
}