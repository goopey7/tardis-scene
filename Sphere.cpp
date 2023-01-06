#include "Sphere.h"
#include <GL/gl.h>
#include <cmath>
#include <fstream>

Sphere::Sphere(float radius, unsigned long numLat, unsigned long numLong, const std::string& textureFile)
{
	loadTexture(textureFile.c_str());
	float theta = 2*PI/numLat;
	float delta = PI/numLong;
	for(int lats = 0; lats < numLat; lats++)
	{
		for(int longs = 0; longs < numLong; longs++)
		{

			int nextLat = lats+1;
			int nextLong = longs+1;

			// [lats][longs]
			vertex.push_back( radius * std::cos(lats*theta) * std::sin(longs*delta)); //x
			vertex.push_back( radius * std::cos(longs*delta)); //y
			vertex.push_back( radius * std::sin(lats*theta) * std::sin(longs*delta)); //z

			normals.push_back( std::cos(lats*theta) * std::sin(longs*delta)); //x
			normals.push_back( std::cos(longs*delta)); //y
			normals.push_back( std::sin(lats*theta) * std::sin(longs*delta)); //z

			// [lats][nextLong]
			vertex.push_back( radius * std::cos(lats*theta) * std::sin(nextLong*delta)); //x
			vertex.push_back( radius * std::cos(nextLong*delta)); //y
			vertex.push_back( radius * std::sin(lats*theta) * std::sin(nextLong*delta)); //z

			normals.push_back( std::cos(lats*theta) * std::sin(nextLong*delta)); //x
			normals.push_back( std::cos(nextLong*delta)); //y
			normals.push_back( std::sin(lats*theta) * std::sin(nextLong*delta)); //z

			// [nextLat][nextLong]
			vertex.push_back( radius * std::cos((nextLat)*theta) * std::sin((nextLong)*delta)); //x
			vertex.push_back( radius * std::cos((nextLong)*delta)); //y
			vertex.push_back( radius * std::sin((nextLat)*theta) * std::sin((nextLong)*delta)); //z

			normals.push_back( std::cos((nextLat)*theta) * std::sin((nextLong)*delta)); //x
			normals.push_back( std::cos((nextLong)*delta)); //y
			normals.push_back( std::sin((nextLat)*theta) * std::sin((nextLong)*delta)); //z

			// [nextLat][longs]
			vertex.push_back( radius * std::cos((nextLat)*theta) * std::sin(longs*delta)); //x
			vertex.push_back( radius * std::cos(longs*delta)); //y
			vertex.push_back( radius * std::sin((nextLat)*theta) * std::sin(longs*delta)); //z

			normals.push_back( std::cos((nextLat)*theta) * std::sin(longs*delta)); //x
			normals.push_back( std::cos(longs*delta)); //y
			normals.push_back( std::sin((nextLat)*theta) * std::sin(longs*delta)); //z

			float u = (float)(numLat-(lats-1))/(numLat+1);
			float v = (float)((numLong + 1) - longs)/(numLong+1);
			float u1 = (float)(numLat-lats)/(numLat+1);
			float v1 = (float)((numLong+1) - nextLong)/(numLong+1);

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
	
	glDrawArrays(GL_QUADS, 0, vertex.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glDisable(GL_TEXTURE_2D);
}

void Sphere::loadTexture(const char* filename)
{
	texture = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);

	//check for an error during the load process
	if (texture == 0)
	{
		std::printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
}

