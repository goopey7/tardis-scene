#ifndef _SHAPE_H
#define _SHAPE_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define PI 3.14159265358979323846

class Sphere
{
public:
	static void render(float radius, unsigned long numLat, unsigned long numLong, GLuint texture);
};
#endif 
