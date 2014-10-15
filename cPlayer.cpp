
#include "cPlayer.h"

cPlayer::cPlayer() {}
cPlayer::~cPlayer(){}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;
	float upp = 1.0f / 1024.0f;	//Units Per Pixel
	
	yo = upp * (facingRight ? 40.0f : 80.0f);
	if (jumping) {
		switch (GetState())
		{
		case STATE_LOOK:xo = 0.0f + upp*36.0f*7.0f; break;
		case STATE_WALK:xo = 0.0f + upp*36.0f*8.0f; break;
		case STATE_CROUCH:xo = 0.0f + upp*36.0f*7.0f; break;
		case STATE_SHOOT:	xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f));
				NextFrame(2); break;
		}
	}
	else{
		switch (GetState())
		{
			case STATE_LOOK:	xo = 0.0f; break;
			case STATE_WALK:	xo = 0.0f + (GetFrame()*(upp*36.0f));
								NextFrame(7); break;
			case STATE_CROUCH:	xo = 0.0f + upp*36.0f*11.0f; break;

			case STATE_SHOOT:	xo = 0.0f + upp*36.0f*12.0f + (GetFrame()*(upp*36.0f));
								NextFrame(2); break;
		}
	}

	xf = xo + upp * 36.0f;
	yf = yo - upp * 40.0f;

	DrawRect(tex_id,xo,yo,xf,yf);
}
