#pragma once

#include "cTexture.h"
#include "Globals.h"

//#define FRAME_DELAY		4
//#define STEP_LENGTH		2
#define JUMP_HEIGHT		96
#define JUMP_STEP		4

#define STATE_LOOK			0
#define STATE_WALK			1
#define	STATE_CROUCH		2
#define	STATE_SHOOT			3

class cRect
{
public:
	int left,top,
		right,bottom;
};



class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right);
	bool CollidesMapFloor(int *map);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void Stop();
	void Logic(int *map);
	
	void crouch();	//NEW
	void shoot();	//NEW

	int  GetState();
	void SetState(int s);
	void SetDirection(int r);	//NEW
	bool isFacingRight();

	void NextFrame(int max);
	int  GetFrame();

	void getAABB(int *minX, int *minY, int *maxX, int *maxY);

	bool collidesWith(AABB other);
	void die();
	bool isAlive();

	
private:
	int x,y;
	int w,h;
	int state;

	int jump_alfa;
	int jump_y;

	int seq,delay;

	int alive;

protected:
	bool facingRight;
	bool jumping;
	bool falling;
	bool crouching;
	bool canGoForward(bool goRight, int *map);
	int STEP_LENGTH = 2;
	int FRAME_DELAY = 4;

};
