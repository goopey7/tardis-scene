//Copyright Sam Collier 2022

#include "SkySphere.h"
#include "SOIL.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


SkySphere::SkySphere(Camera* cam)
	: cam(cam),
	sphere(200.f,300.f,300.f,"gfx/stars.jpg")
{
	sphere.load();
}

void SkySphere::render()
{
	glDisable(GL_LIGHTING);
	Vector3 pos = cam->getPosition();
	glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glTranslatef(pos.x,pos.y,pos.z);
		sphere.render();
		glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void SkySphere::updateCamera(Camera* cam)
{
	this->cam = cam;
}

