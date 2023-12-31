#ifndef _SHAPE_H
#define _SHAPE_H

#include "Geometry.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include "SOIL.h"
#include "Vector3.h"
#include <string>

#define PI 3.14159265358979323846

class Sphere : public Geometry
{
public:
	Sphere(float radius, unsigned long numLat, unsigned long numLong, const std::string& textureFile = nullptr);
	void load() override;
	void render() override;
private:
	void loadTexture(const char* filename);
	std::vector<float> vertex, normals, texCoords;
	GLuint texture;
	float radius;
	unsigned int numLat, numLong;
};
#endif 
