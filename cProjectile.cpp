#include "cProjectile.h"
#include "cData.h"
#include "cScene.h"
#include "iostream"

cProjectile::cProjectile()
{
	alive = false;
}

cProjectile::~cProjectile(){}

void cProjectile::shoot(bool shootRight, int xo, int yo)	//shoot (join setpos & aim
{
	facingRight = shootRight;
	posX = xo;
	posY = yo+35;
	alive = true;
}

void cProjectile::impact(){
	alive = false;
}

void cProjectile::getAABB(int *minX, int *minY, int *maxX, int *maxY)
{
	*minX = posX;
	*minY = posY;
	*maxX = posX + w;
	*maxY = posY + h;
}




void cProjectile::logic(int *map)
{
	int xaux;

	if ((posX % TILE_SIZE) == 0){
		xaux = posX;
		(facingRight ? posX += 3 : posX -= 3);

		if (CollidesMapWall(map, facingRight)){
			impact();
		}
		//impactar enemic ese if ...
	}
	else (facingRight ? posX += 3 : posX -= 3);
}


bool cProjectile::CollidesMapWall(int *map, bool right)
{
	int tile_x, tile_y;
	int width_tiles, height_tiles;

	tile_x = posX / TILE_SIZE;
	tile_y = posY / TILE_SIZE;
	width_tiles = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if (right)	tile_x += width_tiles;

	//for (j = 0; j<height_tiles; j++)
	//{
	if (map[tile_x + ((tile_y)*SCENE_WIDTH) ] != -1)	return true;
	//}

	return false;
}




void cProjectile::draw(int tex_id, bool facingRight)
{
	float upp = 1.0f / 256.0f;	//Units Per Pixel
	float yo = upp *(facingRight ? 8.0f : 16.0f);
	float xo = 0.0f;
	float xf = 30.0f * upp;
	float yf = yo - upp * 8.0f;
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