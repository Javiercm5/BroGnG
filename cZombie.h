#pragma once
#include "cBicho.h"


class cZombie :
	public cBicho
{
public:
	cZombie();
	~cZombie();
	void intelligence(cGame& g);
	void Draw(int tex_id);
	void Update(cGame& g) override;
	void init() override;



private:
	bool goRight;
};

