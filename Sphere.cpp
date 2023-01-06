#include "Sphere.h"
#include <GL/gl.h>
#include <cmath>

Sphere::Sphere(float radius, unsigned long numLat, unsigned long numLong, GLuint texture)
	: texture(texture)
{
	// TODO Don't regenerate the sphere every frame
	vertex.resize(numLat * numLong * 3);
	float theta = 2*PI/numLat;
	float delta = PI/numLong;
	float verts[numLat+1][numLong+1][3];
	//GLint texCoords[numLat+1][numLong+1][2];
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

			// push to vertex and normal vectors
			// [lats][longs]
			vertex.push_back(verts[lats][longs][0]);
			vertex.push_back(verts[lats][longs][1]);
			vertex.push_back(verts[lats][longs][2]);

			normals.push_back(verts[lats][longs][0] / radius);
			normals.push_back(verts[lats][longs][1] / radius);
			normals.push_back(verts[lats][longs][2] / radius);

			// [lats][nextLong]
			vertex.push_back(verts[lats][nextLong][0]);
			vertex.push_back(verts[lats][nextLong][1]);
			vertex.push_back(verts[lats][nextLong][2]);

			normals.push_back(verts[lats][nextLong][0] / radius);
			normals.push_back(verts[lats][nextLong][1] / radius);
			normals.push_back(verts[lats][nextLong][2] / radius);

			// [nextLat][nextLong]
			vertex.push_back(verts[nextLat][nextLong][0]);
			vertex.push_back(verts[nextLat][nextLong][1]);
			vertex.push_back(verts[nextLat][nextLong][2]);

			normals.push_back(verts[nextLat][nextLong][0] / radius);
			normals.push_back(verts[nextLat][nextLong][1] / radius);
			normals.push_back(verts[nextLat][nextLong][2] / radius);

			// [nextLat][longs]
			vertex.push_back(verts[nextLat][longs][0]);
			vertex.push_back(verts[nextLat][longs][1]);
			vertex.push_back(verts[nextLat][longs][2]);

			normals.push_back(verts[nextLat][longs][0] / radius);
			normals.push_back(verts[nextLat][longs][1] / radius);
			normals.push_back(verts[nextLat][longs][2] / radius);

			float u = (float)(numLat-(lats-1))/(numLat+1);
			float v = (float)(longs)/(numLong+1);
			float u1 = (float)(numLat-lats)/(numLat+1);
			float v1 = (float)(nextLong)/(numLong+1);

			// [lats][longs]
			texCoords.push_back(u);
			texCoords.push_back(v);

			// [lats][nextLong]
			texCoords.push_back(u);
			texCoords.push_back(v1);

			// [nextLat][nextLong]
			texCoords.push_back(u1);
			texCoords.push_back(v1);

			// [nextLat][longs]
			texCoords.push_back(u1);
			texCoords.push_back(v);

			/*
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
			*/
		}
	}
}

void Sphere::render()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	
	glDrawArrays(GL_QUADS, 0, vertex.size()/4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glDisable(GL_TEXTURE_2D);
}

