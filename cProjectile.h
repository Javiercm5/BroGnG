#pragma once
class cProjectile
{

private:
	bool facingRight;
	int posX, posY;
	bool alive;


	int w = 30;
	int h = 7;


	bool CollidesMapWall(int *map, bool right);

public:
	cProjectile();
	~cProjectile();
	cProjectile(bool shootRight, int xo, int yo);

	void setPosition(int xo, int yo);
	void aim(bool aimRight);
	bool isAlive();
	void logic(int *map);

	void draw(int tex_id);

};

