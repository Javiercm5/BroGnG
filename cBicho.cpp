#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"

cBicho::cBicho(void)
{
	seq = 0;
	delay = 0;
	falling = false;
	jumping = false;
	crouching = false;
	facingRight = true;
	alive = false;
	health = 2;
	shootDelay = 0;
	bichoDelay = 15;
	dyingDelay = 0;

	stateMaxFrames[STATE_LOOK] = 1;
	stateMaxFrames[STATE_WALK] = 1;
	stateMaxFrames[STATE_SHOOT] = 1;
	STEP_LENGTH = 2;
}
cBicho::~cBicho(void){}
cBicho::cBicho(int posx, int posy, int width, int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}


void cBicho::SetPosition(int posx, int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx, int *posy)
{
	*posx = x;
	*posy = y;
}
void cBicho::SetDirection(bool r)
{
	facingRight = r;
}
int cBicho::GetPositionX()
{
	return x;
}
int cBicho::GetPositionY()
{
	return y;
}

void cBicho::SetTile(int tx, int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx, int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}


void cBicho::SetWidthHeight(int width, int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width, int *height)
{
	*width = w;
	*height = h;
}


// COLLISION CONTROL
bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x + w<rc->right) && (y>rc->bottom) && (y + h<rc->top));
}
bool cBicho::CollidesMapWall(int *map, bool right)
{
	int tile_x, tile_y;
	int j;
	int width_tiles, height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if (right)	tile_x += width_tiles;

	for (j = 0; j<height_tiles; j++)
	{
		if (map[tile_x + ((tile_y + j)*SCENE_WIDTH)] != -1)	return true;
	}

	return false;
}
bool cBicho::CollidesMapFloor(int *map)
{
	int tile_x, tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if ((x % TILE_SIZE) != 0) width_tiles++;

	on_base = false;
	i = 0;
	while ((i<width_tiles) && !on_base)
	{
		if ((y % TILE_SIZE) == 0)
		{
			if (map[(tile_x + i) + ((tile_y - 1) * SCENE_WIDTH)] != -1)
				on_base = true;
		}
		else
		{
			if (map[(tile_x + i) + (tile_y * SCENE_WIDTH)] != -1)
			{
				y = (tile_y + 1) * TILE_SIZE;
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}
bool cBicho::CollidesMapCeiling(int *map)
{
	int tile_x, tile_y;
	int width_tiles;
	int heigh_tiles;
	bool ceiling;
	int i, j;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	heigh_tiles = h / TILE_SIZE;

	if ((x % TILE_SIZE) != 0) width_tiles++;
	if ((y % TILE_SIZE) != 0) heigh_tiles++;

	ceiling = false;
	i = 0;
	while ((i<heigh_tiles) && !ceiling)
	{
		j = 0;
		while ((j < width_tiles) && !ceiling){
			if (map[(tile_x + j) + ((tile_y + i) * SCENE_WIDTH)] != -1)	ceiling = true;
			j++;
		}
		i++;
	}
	return ceiling;
}
bool cBicho::canGoForward(bool right, int *map)
{
	int xaux;

	//Whats next tile?
	if ((x % TILE_SIZE) == 0)
	{
		xaux = x;
		(right ? x += STEP_LENGTH : x -= STEP_LENGTH);

		if (CollidesMapWall(map, right))
		{
			x = xaux;
			return false;
		}
		else{
			int yaux = y;
			y -= 30;
			if (!CollidesMapWall(map, right))
			{
				x = xaux;
				y = yaux;
				return false;
			}
			y = yaux;
			x = xaux;
			return true;
		}
	}
	return true;
}

AABB cBicho::getAABB()
{
	AABB aabb;
	aabb.minX = x;
	aabb.minY = y;
	aabb.maxX = x + w;
	aabb.maxY = y + h;
	return aabb;
}
bool cBicho::collidesWith(AABB other)
{
	if (x + w < other.minX || x > other.maxX) return false;
	if (y + h < other.minY || y > other.maxY) return false;
	return true;
}





void cBicho::setAlive(bool sAlive)
{
	alive = sAlive;
}


void cBicho::die(int player)
{
	dying = true;
	dyingDelay = 40;
	deadBy = player;
}


bool cBicho::isAlive()
{
	return alive;
}
void cBicho::impact(int damage, int player)
{
	health -= damage;
	if (health <= 0) die(player);
}


void cBicho::GetArea(cRect *rc)
{
	rc->left = x;
	rc->right = x + w;
	rc->bottom = y;
	rc->top = y + h;
}

void cBicho::DrawRect(int tex_id, float xo, float yo, float xf, float yf)
{
	int screen_x, screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

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

//============LOGIC / MOVEMENTS
void cBicho::MoveLeft(int *map)
{
	int xaux;
	facingRight = false;

	//Whats next tile?
	if ((x % TILE_SIZE) == 0)
	{
		xaux = x;
		x -= STEP_LENGTH;

		if (CollidesMapWall(map, false))
		{
			x = xaux;
			state = STATE_LOOK;
		}
	}
	//Advance, no problem
	else
	{
		x -= STEP_LENGTH;
		if (state != STATE_WALK)
		{
			state = STATE_WALK;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::MoveRight(int *map)
{
	int xaux;
	facingRight = true;
	//Whats next tile?
	if ((x % TILE_SIZE) == 0)
	{
		xaux = x;
		x += STEP_LENGTH;

		if (CollidesMapWall(map, true))
		{
			x = xaux;
			state = STATE_LOOK;
		}
	}
	//Advance, no problem
	else
	{

		x += STEP_LENGTH;

		if (state != STATE_WALK)
		{
			state = STATE_WALK;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::Stop()
{
	switch (state)
	{
	case STATE_WALK:	state = STATE_LOOK;	break;
	}
	falling = false;
	crouching = false;
}
bool cBicho::Jump(int *map)
{
	if (!jumping)
	{
		if (CollidesMapFloor(map))
		{
			jumping = true;
			jump_alfa = 0;
			jump_y = y;
		}
	}
	return jumping;
}

void cBicho::shoot()
{
	if (shootDelay == 0) {
		SetState(STATE_SHOOT);
		seq = 0;
		shootDelay = bichoDelay;
	}
}

void cBicho::Logic(int *map)
{
	float alfa;
	falling = false;
	if (CollidesMapCeiling(map)) jumping = false;
	if (jumping)
	{
		jump_alfa += JUMP_STEP;

		if (jump_alfa == 180)
		{
			jumping = false;
			y = jump_y;
		}
		else
		{
			alfa = ((float)jump_alfa) * 0.017453f;
			y = jump_y + (int)(((float)JUMP_HEIGHT) * sin(alfa));

			if (jump_alfa > 90)
			{
				//Over floor?
				jumping = !CollidesMapFloor(map);
			}
		}
	}
	else
	{
		//Over floor?
		if (!CollidesMapFloor(map)){
			y -= (2 * STEP_LENGTH);

			falling = true;
		}
	}

}


void cBicho::NextFrame(int max)
{
	delay++;
	if (delay == FRAME_DELAY)
	{
		++seq;
		if (seq >= max) {
			seq %= max;
			if (state == STATE_SHOOT) SetState(STATE_LOOK);
		}
		delay = 0;
	}

}
int cBicho::GetFrame()
{
	return seq;
}

int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}

bool cBicho::isFacingRight()
{
	return facingRight;
}


void cBicho::Update(cGame& g)
{

}

void cBicho::init()
{

}

int cBicho::getDamage()
{
	return damage;
}

bool cBicho::isDying()
{
	return dying;
}

bool cBicho::isInTheAir()
{
	return (jumping || falling);
}