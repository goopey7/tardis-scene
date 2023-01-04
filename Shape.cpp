#include "Shape.h"
#include <GL/gl.h>
#include <cmath>

void Shape::renderSphere(float radius, unsigned long numLat, unsigned long numLong, GLuint texture)
{
	// TODO Don't regenerate the sphere every frame
	float theta = 2*PI/numLat;
	float delta = PI/numLong;
	float verts[numLat+1][numLong+1][3];
	GLint texCoords[numLat+1][numLong+1][2];
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture);
	for(int lats = 0; lats < numLat; lats++)
	{
		glVertexPointer(3,GL_FLOAT,0,verts[lats]);
		for(int longs = 0; longs < numLong; longs++)
		{

			int nextLat = lats+1;
			int nextLong = longs+1;

			// [lats][longs]
			verts[lats][longs][0]= radius * std::cos(lats*theta) * std::sin(longs*delta); //x
			verts[lats][longs][1]= radius * std::cos(longs*delta); //y
			verts[lats][longs][2]= radius * std::sin(lats*theta) * std::sin(longs*delta); //z

			// [lats][nextLong]
			verts[lats][nextLong][0]= radius * std::cos(lats*theta) * std::sin(nextLong*delta); //x
			verts[lats][nextLong][1]= radius * std::cos(nextLong*delta); //y
			verts[lats][nextLong][2]= radius * std::sin(lats*theta) * std::sin(nextLong*delta); //z

			// [nextLat][nextLong]
			verts[nextLat][nextLong][0]= radius * std::cos((nextLat)*theta) * std::sin((nextLong)*delta); //x
			verts[nextLat][nextLong][1]= radius * std::cos((nextLong)*delta); //y
			verts[nextLat][nextLong][2]= radius * std::sin((nextLat)*theta) * std::sin((nextLong)*delta); //z

			// [nextLat][longs]
			verts[nextLat][longs][0]= radius * std::cos((nextLat)*theta) * std::sin(longs*delta); //x
			verts[nextLat][longs][1]= radius * std::cos(longs*delta); //y
			verts[nextLat][longs][2]= radius * std::sin((nextLat)*theta) * std::sin(longs*delta); //z

			// render
			glBegin(GL_QUADS);

				float u = (float)(numLat-(lats-1))/(numLat+1);
				float v = (float)(longs)/(numLong+1);
				float u1 = (float)(numLat-lats)/(numLat+1);
				float v1 = (float)(nextLong)/(numLong+1);

				glTexCoord2f(u,v);
				glNormal3f(
						verts[lats][longs][0] / radius,
						verts[lats][longs][1] / radius,
						verts[lats][longs][2] / radius
						);
				glVertex3fv(verts[lats][longs]);

				glTexCoord2f(u,v1);
				glNormal3f(
						verts[lats][nextLong][0] / radius,
						verts[lats][nextLong][1] / radius,
						verts[lats][nextLong][2] / radius
						);
				glVertex3fv(verts[lats][nextLong]);

				glTexCoord2f(u1,v1);
				glNormal3f(
						verts[nextLat][nextLong][0] / radius,
						verts[nextLat][nextLong][1] / radius,
						verts[nextLat][nextLong][2] / radius
						);
				glVertex3fv(verts[nextLat][nextLong]);

				glTexCoord2f(u1,v);
				glNormal3f(
						verts[nextLat][longs][0] / radius,
						verts[nextLat][longs][1] / radius,
						verts[nextLat][longs][2] / radius
						);
				glVertex3fv(verts[nextLat][longs]);

			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}

