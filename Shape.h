#ifndef _SHAPE_H
#define _SHAPE_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define PI 3.14159265358979323846

class Shape
{
public:
	void render1();
	void render2();
	void render3();

	static void renderDisc(unsigned long res, float radius, GLuint discTexture);
	static void renderSphere(float radius, unsigned long numLat, unsigned long numLong, GLuint texture);

	Shape();

	float* verts;
	float* norms;
	float* texcoords;
};
#endif 
