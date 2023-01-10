//Copyright Sam Collier 2022

#include "Tardis.h"
#include <GL/gl.h>


Tardis::Tardis()
	: exterior(new Model()),
	interior(new Model()),
	lamp(new Sphere(0.25f,10.f,10.f,"gfx/white.png")),
	light(std::move(lamp),{0.f,0.f,0.f},GL_LIGHT1)
{
	light.setLightPosition({ 0.f, 5.f, 0.f });
	light.setAmbient({ 0.2f, 0.2f, 0.2f });
	light.setDiffuse({dimmer,dimmer,dimmer});
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
		glColor4f(dimmer,dimmer,dimmer,dimmer);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glRotatef(angle,0.f,1.f,0.f);
		glRotatef(-90.f,0.f,1.f,0.f);
		glColor4f(dimmer,dimmer,dimmer,dimmer);
		exterior->render();
		light.render();
	glPopMatrix();

	glDisable(GL_BLEND);


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
	if(dimmer >= 1.f || dimmer <= 0.f)
	{
		dimmerSpeed = -dimmerSpeed;
	}

	dimmer += dimmerSpeed * dt;
	light.setDiffuse({dimmer,dimmer,dimmer});
}

void Tardis::load()
{
	exterior->load("models/TARDIS.obj","gfx/Wood.jpg");
	interior->load("models/interior/tardis.obj","gfx/crate.png");
	light.load();
}

