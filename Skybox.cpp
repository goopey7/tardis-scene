//Copyright Sam Collier 2022

#include "Skybox.h"
#include <SOIL/SOIL.h>
#include <GL/gl.h>

Skybox::Skybox(const Camera& cam)
	: cam(cam),
	skyTexture(SOIL_load_OGL_texture(
				"gfx/stars.png",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS|SOIL_FLAG_NTSC_SAFE_RGB|SOIL_FLAG_COMPRESS_TO_DXT
				)
			)
{
}

void Skybox::render()
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	Vector3 pos = cam.getPosition();
	glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glTranslatef(pos.x,pos.y,pos.z);
		renderGeometry();
		glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
}

void Skybox::renderGeometry()
{
	glBindTexture(GL_TEXTURE_2D,skyTexture);
	glColor3f(1.f,1.f,1.f);

	glBegin(GL_QUADS);

		// front face
		glTexCoord2f(0.25f,0.25f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		glTexCoord2f(0.25f,0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		glTexCoord2f(0.5f,0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		glTexCoord2f(0.5f,0.25f);
		glVertex3f(0.5f, 0.5f, 0.5f);

		// right face
		glTexCoord2f(0.5f,0.25f);
		glVertex3f(0.5f, 0.5f, 0.5f);

		glTexCoord2f(0.5f,0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		glTexCoord2f(0.75f,0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		glTexCoord2f(0.75f,0.25f);
		glVertex3f(0.5f, 0.5f, -0.5f);

		//bottom face
		glTexCoord2f(0.25f,0.5f);
		glVertex3f(-0.5f, -0.45f, -0.5f);

		glTexCoord2f(0.25f,0.75f);
		glVertex3f(-0.5f, -0.45f, 0.5f);

		glTexCoord2f(0.5f,0.75f);
		glVertex3f(0.5f, -0.45f, 0.5f);

		glTexCoord2f(0.5f,0.5f);
		glVertex3f(0.5f, -0.45f, -0.5f);

		//top face
		glTexCoord2f(0.25f,0.f);
		glVertex3f(-0.5f, 0.48f, -0.5f);

		glTexCoord2f(0.25f,0.25f);
		glVertex3f(-0.5f, 0.48f, 0.5f);

		glTexCoord2f(0.5f,0.25f);
		glVertex3f(0.5f, 0.48f, 0.5f);

		glTexCoord2f(0.5f,0.f);
		glVertex3f(0.5f, 0.48f, -0.5f);

		// left face
		glTexCoord2f(0.f,0.25f);
		glVertex3f(-0.5f, 0.5f, -0.5f);

		glTexCoord2f(0.f,0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);

		glTexCoord2f(0.25f,0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		glTexCoord2f(0.25f,0.25f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		// back face
		glTexCoord2f(0.75f,0.25f);
		glVertex3f(0.5f, 0.5f, -0.5f);

		glTexCoord2f(0.75f,0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		glTexCoord2f(1.f,0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);

		glTexCoord2f(1.f,0.25f);
		glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();
}

