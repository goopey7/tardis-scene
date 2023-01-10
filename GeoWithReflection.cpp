//Copyright Sam Collier 2022

#include "GeoWithReflection.h"
#include "Geometry.h"
#include <GL/gl.h>


void GeoWithReflection::render()
{
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	glDisable(GL_DEPTH_TEST);

	// render 2 unit quad
	glBegin(GL_QUADS);
		glVertex3f(-15.f,0.f,-15.f);
		glVertex3f(-15.f,0.f,15.f);
		glVertex3f(15.f,0.f,15.f);
		glVertex3f(15.f,0.f,-15.f);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glStencilFunc(GL_EQUAL,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

	glDisable(GL_LIGHTING);
	glPushMatrix();
		glScalef(1.f,-1.f,1.f);
		glTranslatef(0.f,10.f,0.f);
		glRotatef(angle,0.f,1.f,0.f);
		geometry->render();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glColor4f(0.2f,0.2f,0.2f,0.8f);

	glBegin(GL_QUADS);
		glVertex3f(-15.f,0.f,-15.f);
		glVertex3f(-15.f,0.f,15.f);
		glVertex3f(15.f,0.f,15.f);
		glVertex3f(15.f,0.f,-15.f);
	glEnd();

	glColor4f(1.f,1.f,1.f,1.f);

	glDisable(GL_BLEND);
	glPushMatrix();
		glTranslatef(0.f,10.f,0.f);
		glRotatef(angle,0.f,1.f,0.f);
		geometry->render();
	glPopMatrix();
}

void GeoWithReflection::update(const float dt)
{
	angle += rotationSpeed * dt;
}

void GeoWithReflection::load()
{
	geometry->load();
}

bool GeoWithReflection::load(const std::string& modelFilename, const std::string& textureFilename)
{
	return geometry->load(modelFilename, textureFilename);
}

GeoWithReflection::GeoWithReflection(std::unique_ptr<Geometry> geometry)
	: geometry(std::move(geometry))
{
}

bool GeoWithReflection::load(const std::string& modelFilename)
{
	return geometry->load(modelFilename);
}

