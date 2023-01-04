//Sam Collier 2023

#include "PointLight.h"

void PointLight::render()
{
	glTranslatef(relativePos.x,relativePos.y,relativePos.z);
	glLightfv(lightID,GL_AMBIENT,&ambient[0]);
	glLightfv(lightID,GL_DIFFUSE,&diffuse[0]);
	glLightfv(lightID,GL_POSITION,&lightPosition[0]);
	glEnable(lightID);
}

PointLight::PointLight(Vector3 position, unsigned int lightID)
	: lightID(lightID), relativePos(position)
{
		ambient = {0.f,0.f,0.f,1.f};
		diffuse = {1.f,1.f,1.f,1.f};
		lightPosition = {0.f,0.f,10.f,1.f};
}

void PointLight::setAmbient(std::vector<GLfloat> ambient)
{
	this->ambient = ambient;
}

void PointLight::setDiffuse(std::vector<GLfloat> diffuse)
{
	this->diffuse = diffuse;
}

void PointLight::setLightPosition(std::vector<GLfloat> lightPosition)
{
	this->lightPosition = lightPosition;
}

void PointLight::setRelativePos(Vector3 relativePos)
{
	this->relativePos = relativePos;
}

