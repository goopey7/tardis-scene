//Sam Collier 2023

#include "PointLight.h"

void PointLight::render()
{
	//glTranslatef(relativePos.x,relativePos.y,relativePos.z);
	glLightfv(lightID,GL_AMBIENT,&ambient[0]);
	glLightfv(lightID,GL_DIFFUSE,&diffuse[0]);
	glLightfv(lightID,GL_POSITION,&lightPosition[0]);
	glLightf(lightID, GL_CONSTANT_ATTENUATION, constAttenuation);
	glLightf(lightID, GL_LINEAR_ATTENUATION, linearAttenuation);
	glLightf(lightID, GL_QUADRATIC_ATTENUATION, quadAttenuation);
	glEnable(lightID);
}

PointLight::PointLight(Vector3 position, unsigned int lightID)
	: lightID(lightID), relativePos(position)
{
		ambient = {0.f,0.f,0.f,1.f};
		diffuse = {1.f,1.f,1.f,1.f};
		lightPosition = {0.f,0.f,0.f,1.f};
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

std::vector<GLfloat> PointLight::getLightPosition() const
{
	return lightPosition;
}

void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
	constAttenuation = constant;
	linearAttenuation = linear;
	quadAttenuation = quadratic;
}

