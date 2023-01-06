#ifndef _SHAPE_H
#define _SHAPE_H

#include "Geometry.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <SOIL/SOIL.h>
#include "Vector3.h"
#include <string>

#define PI 3.14159265358979323846

class Sphere : public Geometry
{
public:
	Sphere(float radius, unsigned long numLat, unsigned long numLong, const std::string& textureFile);
	void render() override;
private:
	void loadTexture(const char* filename);
	std::vector<float> vertex, normals, texCoords;
	GLuint texture;
};
#endif 
