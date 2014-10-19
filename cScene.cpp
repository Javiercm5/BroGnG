#include "cScene.h"
#include "Globals.h"
#include "iostream"

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

bool cScene::LoadLevel(int level)
{
	bool res;
	FILE *fd;
	char file[16];
	int i,j,px,py;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	float tileRate = 128.0f / 512.0f;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%c",&tile);
					if(tile==' ')
					{
						//Tiles must be != 0 !!!
						map[(j*SCENE_WIDTH)+i]=-1;
					}

					else {
						int iTile = tile - 48;
						map[(j*SCENE_WIDTH) + i] = iTile;

						float xo = ((iTile % 4)) * tileRate;
						float yo = ((iTile / 4)) * tileRate;

						glTexCoord2f(xo, yo);							glVertex2i(px, py);
						glTexCoord2f(xo + tileRate, yo);				glVertex2i(px + BLOCK_SIZE, py);
						glTexCoord2f(xo + tileRate, yo - tileRate);		glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
						glTexCoord2f(xo, yo - tileRate);				glVertex2i(px, py + BLOCK_SIZE);

					}
					px+=TILE_SIZE;
				}

				fscanf(fd,"%c",&tile); //pass enter

			}

			//----CURBS--- if not up-tile, print curb at z=1
			/*for (j = 0; j < SCENE_HEIGHT; j++){
				px = SCENE_Xo;
				py = SCENE_Yo + (j*TILE_SIZE);

				for (i = 0; i < SCENE_WIDTH; i++){
					
					int iTile = map[(j*SCENE_WIDTH) + i];
					float xo = ((iTile % 4)) * tileRate;
					float yo = ((iTile / 4)) * tileRate;
					
					if (map[(j*SCENE_WIDTH) + i] != -1 && map[((j + 1)*SCENE_WIDTH) + i] == -1){
						std::cout << iTile << std::endl;

						glTexCoord2f(xo + 2 * tileRate, yo);						glVertex3i(px, py, 1);
						glTexCoord2f(xo + tileRate + 2 * tileRate, yo);				glVertex3i(px + BLOCK_SIZE, py, 1);
						glTexCoord2f(xo + tileRate + 2 * tileRate, yo - tileRate);	glVertex3i(px + BLOCK_SIZE, py + BLOCK_SIZE, 1);
						glTexCoord2f(xo + 2 * tileRate, yo - tileRate);				glVertex3i(px, py + BLOCK_SIZE, 1);
					}
				}
			}*/


			

		glEnd();
	glEndList();

	fclose(fd);

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
}