#pragma once

#include "cTexture.h"
#include "Globals.h"

#define JUMP_HEIGHT		96
#define JUMP_STEP		4

#define STATE_LOOK			0
#define STATE_WALK			1
#define	STATE_SHOOT			2

class cRect
{
public:
	int left, top,
		right, bottom;
};

class cGame;

class cBicho
{
public:
	cBicho(void);
	cBicho(int x, int y, int w, int h);
	~cBicho(void);

	void SetPosition(int x, int y);
	void GetPosition(int *x, int *y);
	int GetPositionX();
	int GetPositionY();
	void SetTile(int tx, int ty);
	void GetTile(int *tx, int *ty);
	void SetWidthHeight(int w, int h);
	void GetWidthHeight(int *w, int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map, bool right);
	bool CollidesMapFloor(int *map);
	bool CollidesMapCeiling(int *map);

	void GetArea(cRect *rc);
	void DrawRect(int tex_id, float xo, float yo, float xf, float yf);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	bool Jump(int *map);
	void Stop();
	void Logic(int *map);

	void shoot();	

	int  GetState();
	void SetState(int s);
	void SetDirection(bool r);	
	bool isFacingRight();

	void NextFrame(int max);
	int  GetFrame();

	AABB getAABB();

	bool collidesWith(AABB other);
	void die(int player);
	virtual void impact(int damage, int player);
	bool isAlive();
	bool isDying();
	virtual void init();


	virtual void Update(cGame& g);
	int getDamage();

	bool isInTheAir();

	void setAlive(bool sAlive);

private:
	int x, y;
	int w, h;
	int state;

	int jump_alfa;
	int jump_y;

	int seq, delay;


protected:
	bool facingRight;
	bool jumping;
	bool falling;
	bool crouching;
	bool dying;
	int deadBy;
	int stateMaxFrames[3];


	bool canGoForward(bool goRight, int *map);

	int STEP_LENGTH;
	int FRAME_DELAY;

	int health;
	bool alive;
	int shootDelay, bichoDelay, dyingDelay;
	int damage;


};
