#pragma once

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cmath> 
#include <gl/glut.h>
#include <string>
#include <c/bass.h>
#pragma comment(lib, "bass.lib")



class AABB
{
public:
	int minX, maxX, minY, maxY;
};

