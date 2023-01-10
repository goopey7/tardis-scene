//Copyright Sam Collier 2022

#include "Tardis.h"
#include <GL/gl.h>


Tardis::Tardis()
	: exterior(new Model()),
	interior(new Model())
{
	exterior->load("models/TARDIS.obj","gfx/Wood.jpg");
	interior->load("models/interior/tardis.obj","gfx/crate.png");
}

void Tardis::render()
{
	glRotatef(angle,0.f,1.f,0.f);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

	// render tardis stencil
	glPushMatrix();
		glRotatef(angle, 0.f,1.f,0.f);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBegin(GL_QUADS);
			glVertex3f(-2.f,2.6f,3.f);
			glVertex3f(-2.f,-5.5f,3.f);
			glVertex3f(2.f,-5.5f,3.f);
			glVertex3f(2.f,2.6f,3.f);
		glEnd();
	glPopMatrix();


	glDisable(GL_CULL_FACE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glStencilFunc(GL_EQUAL,1,1);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

	glColor4f(1.f,1.f,1.f,1.f);

	// render exterior
	glEnable(GL_LIGHTING);
	glPushMatrix();
		glRotatef(angle,0.f,1.f,0.f);
		glRotatef(-90.f,0.f,1.f,0.f);
		exterior->render();
	glPopMatrix();


	// render interior and disable depth to render on top of exterior
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glTranslatef(1.f,-5.f,-1.f);
		glScalef(0.25f,0.25f,0.25f);
		glRotatef(angle,0.f,1.f,0.f);
		interior->render();
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}


void Tardis::update(const float dt)
{
	angle += angleSpeed * dt;
}

