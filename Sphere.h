#ifndef _SHAPE_H
#define _SHAPE_H

#include "Geometry.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#define PI 3.14159265358979323846

class Sphere : public Geometry
{
public:
	Sphere(float radius, unsigned long numLat, unsigned long numLong, GLuint texture);
	void render() override;
private:
	std::vector<float> vertex, normals, texCoords;
	GLuint texture;
};
#endif 
