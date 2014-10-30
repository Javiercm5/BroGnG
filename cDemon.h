#pragma once
#include "cBicho.h"


class cDemon :
	public cBicho
{
public:
	cDemon();
	~cDemon();
	void intelligence(cGame& g);
	void Draw(int tex_id);
	void Update(cGame& g) override;
	void init() override;

private: 	
	bool goRight;
	bool waiting;

};

