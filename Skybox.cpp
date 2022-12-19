//Copyright Sam Collier 2022

#include "Skybox.h"
#include <SOIL/SOIL.h>
#include <GL/gl.h>

Skybox::Skybox(const Camera& cam)
	: cam(cam),
	skyTexture(SOIL_load_OGL_texture(
				"gfx/skybox.png",
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
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LEQUAL);
	Vector3 pos = cam.getPosition();
	glPushMatrix();
		glTranslatef(pos.x,pos.y,pos.z);
		renderGeometry();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glDisable(GL_TEXTURE_2D);
}

void Skybox::renderGeometry()
{
	glBindTexture(GL_TEXTURE_2D,skyTexture);
	glColor3f(1.f,1.f,1.f);

	glBegin(GL_QUADS);

		// front face
		glTexCoord2f(0.5f,0.25f);
		glVertex3f(-0.5f,0.5f,0.5f);
		glTexCoord2f(0.5f,0.5f);
		glVertex3f(-0.5f,-0.5f,0.5f);
		glTexCoord2f(0.25f,0.5f);
		glVertex3f(0.5f,-0.5f,0.5f);
		glTexCoord2f(0.25f,0.25f);
		glVertex3f(0.5f,0.5f,0.5f);

		// right face
		glTexCoord2f(0.25f,0.25f);
		glVertex3f(0.5f,0.5f,0.5f);
		glTexCoord2f(0.25f,0.5f);
		glVertex3f(0.5f,-0.5f,0.5f);
		glTexCoord2f(0.0f,0.5f);
		glVertex3f(0.5f,-0.5f,-0.5f);
		glTexCoord2f(0.0f,0.25f);
		glVertex3f(0.5f,0.5f,-0.5f);

		//bottom face
		glTexCoord2f(0.5f,0.75f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glTexCoord2f(0.5f,0.5f);
		glVertex3f(-0.5f,-0.5f,0.5f);
		glTexCoord2f(0.25f,0.5f);
		glVertex3f(0.5f,-0.5f,0.5f);
		glTexCoord2f(0.25f,0.75f);
		glVertex3f(0.5f,-0.5f,-0.5f);

		//top face
		glTexCoord2f(0.5f,0.0f);
		glVertex3f(-0.5f,0.5f,-0.5f);
		glTexCoord2f(0.5f,0.25f);
		glVertex3f(-0.5f,0.5f,0.5f);
		glTexCoord2f(0.25f,0.25f);
		glVertex3f(0.5f,0.5f,0.5f);
		glTexCoord2f(0.25f,0.0f);
		glVertex3f(0.5f,0.5f,-0.5f);

		// left face
		glTexCoord2f(0.75f,0.25f);
		glVertex3f(-0.5f,0.5f,-0.5f);
		glTexCoord2f(0.75f,0.5f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glTexCoord2f(0.5f,0.5f);
		glVertex3f(-0.5f,-0.5f,0.5f);
		glTexCoord2f(0.5f,0.25f);
		glVertex3f(-0.5f,0.5f,0.5f);

		// back face
		glTexCoord2f(1.f,0.25f);
		glVertex3f(0.5f,0.5f,-0.5f);
		glTexCoord2f(1.f,0.5f);
		glVertex3f(0.5f,-0.5f,-0.5f);
		glTexCoord2f(0.75f,0.5f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glTexCoord2f(0.75f,0.25f);
		glVertex3f(-0.5f,0.5f,-0.5f);
	glEnd();
}

