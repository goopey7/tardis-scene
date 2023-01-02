#include "Shape.h"
#include <GL/gl.h>
#include <cmath>


void Shape::render1()
{
	// add code to render the cube (above) using method 1

	glBegin(GL_QUADS);
	for(int i=0;i<4;i++)
		glArrayElement(i);
	glEnd();
}

void Shape::render2()
{
	// add code to render the cube (above) using method 2
	glDrawArrays(GL_QUADS,0,4);
}

void Shape::render3()
{
	// add code to render the cube (above) using method 3
	unsigned int indices[] = {
								0,1,2,3,
								0,4,7,3,
								0,1,5,4,
								4,5,6,7,
								1,5,6,2,
								7,6,2,3
								};
	glIndexPointer(GL_UNSIGNED_INT,0,indices);
	glDrawElements(GL_QUADS,24,GL_UNSIGNED_INT,indices);
}

Shape::Shape()
{
	verts = new float[]{
								-1.0, 1.0, 1.0,			// Vertex #0
								-1.0, -1.0, 1.0,		// Vertex #1
								1.0,  -1.0, 1.0,		// Vertex #2
								1.0,  1.0, 1.0,			// Vertex #3
								-1.0, 1.0, -1.0,		// Vertex #4
								-1.0, -1.0, -1.0,		// Vertex #5
								1.0,  -1.0, -1.0,		// Vertex #6
								1.0,  1.0, -1.0,		// Vertex #7
	};

		norms = new float[]{
							0.0, 0.0, 1.0,		//0
							0.0, 0.0, 1.0,		//1
							0.0, 0.0, 1.0,		//2
							0.0, 0.0, 1.0,		//3
	};


	texcoords = new float[]{ 0.5, 0.5, 		//0
								0.5, 0.75, 		//1
								0.75, 0.75, 		//2
								0.75, 0.5, 		//3
	};
}

void Shape::renderDisc(unsigned long res, float radius, GLuint discTexture)
{
	//TODO Don't regenerate the disc every frame
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,discTexture);

	float vertices[res*3+3];
	float interval = 2*PI/res;
	float theta = 0.f;
	unsigned int texCoords[res*2+2];
	float d = 2*radius;

	// center
	vertices[0] = 0.f;
	vertices[1] = 0.f;
	vertices[2] = 0.f;

	texCoords[0] = 0;
	texCoords[1] = 1 / d + 0.5f;

	// add every vertex in the disc to the vertex buffer
	int t = 2;
	for(int i=3;i<res*3+3;i+=3)
	{
		vertices[i] = radius*std::cos(theta);
		vertices[i+1] = radius * std::sin(theta);
		vertices[i+2] = 0.f;

		texCoords[t] = std::cos(theta) / d + 0.5f;
		texCoords[t+1] = std::sin(theta) / d + 0.5f;
		theta+=interval;
		t+=2;
	}

	// create index buffer
	unsigned int indices[res*3];
	int index=0;
	for(int i=0;index<res*3;i++)
	{
		indices[index] = 0;
		indices[index+1] = i+1;
		indices[index+2] = i+2;
		index+=3;
	}
	indices[res*3-3] = 0;
	indices[res*3-2] = res;
	indices[res*3-1] = 1;

	glVertexPointer(3,GL_FLOAT,0,vertices);
	glTexCoordPointer(2,GL_UNSIGNED_INT,0,texCoords);

	// render array
	//glColor4f(1.f,0.f,0.f,1.f);
	glDrawElements(GL_TRIANGLES,res*3,GL_UNSIGNED_INT,indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

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
	for(int longs = 0; longs < numLong; longs++)
	{
		for(int lats = 0; lats < numLat; lats++)
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
				glVertex3fv(verts[lats][longs]);

				glTexCoord2f(u,v1);
				glVertex3fv(verts[lats][nextLong]);

				glTexCoord2f(u1,v1);
				glVertex3fv(verts[nextLat][nextLong]);

				glTexCoord2f(u1,v);
				glVertex3fv(verts[nextLat][longs]);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}

