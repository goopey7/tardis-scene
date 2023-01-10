//Copyright Sam Collier 2022

#include "PointLightGeo.h"
#include "PointLight.h"
#include <GL/gl.h>

PointLightGeo::PointLightGeo(std::unique_ptr<Geometry> geometry, Vector3 position, unsigned int lightID)
	: geometry(std::move(geometry)),
	PointLight(position,lightID)
{
}

void PointLightGeo::render()
{
	PointLight::render();
	glDisable(GL_LIGHTING);
	Vector3 pos = {PointLight::getLightPosition()[0],PointLight::getLightPosition()[1],PointLight::getLightPosition()[2]};
	glTranslatef(pos.x,pos.y,pos.z);
	geometry->render();
	glEnable(GL_LIGHTING);
}

void PointLightGeo::load()
{
	geometry->load();
}

bool PointLightGeo::load(const std::string& modelFilename, const std::string& textureFilename)
{
	return geometry->load(modelFilename,textureFilename);
}

