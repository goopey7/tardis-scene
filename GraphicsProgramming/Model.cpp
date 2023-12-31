// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Model.h"
#include <string.h>

bool Model::load(const std::string& modelFilename, const std::string& textureFilename)
{
	bool result;

	// Load in the model data,
	result = loadModel(modelFilename.c_str());
	if (!result)
	{
		//MessageBox(NULL, "Model failed to load", "Error", MB_OK);
		return false;
	}

	// Load the texture for this model.
	loadTexture(textureFilename.c_str());

	return true;
}

void Model::render()
{
	// TODO: Need to add code here to render the loaded model
	// How this is done is based on how you stored and sorted the data
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,texture);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glVertexPointer(3, GL_FLOAT, 0, &vertex[0]);
	glTexCoordPointer(3, GL_FLOAT, 0, &texCoords[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	
	glDrawArrays(GL_TRIANGLES, 0, vertex.size()/3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glDisable(GL_TEXTURE_2D);
}


// Modified from a mulit-threaded version by Mark Ropper.
bool Model::loadModel(const char* filename)
{
	vector<Vector3> verts;
	vector<Vector3> norms;
	vector<Vector3> texCs;
	vector<unsigned int> faces;
	
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		return false;
	}
	while (true)
	{
		char lineHeader[128];

		// Read first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				verts.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector3 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				texCs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				norms.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2],
																			&face[3], &face[4], &face[5],
																			&face[6], &face[7], &face[8]);
				if (matches != 9)
				{
					// Parser error, or not triangle faces
					//return false;
				}
				
				for (int i = 0; i < 9; i++)
				{
					faces.push_back(face[i]);
				}


			}
		}
	}

#pragma region unroll_data
	// "Unroll" the loaded obj information into a list of triangles.
	// TODO: By this point all model has been read from the file, but is not in the correct order.
	// You NEED to loop over all the data and sort it into a render ready order/format.

	for(int i = 0; i < faces.size(); i += 9)
	{
		//v1
		vertex.push_back(verts[faces[i]-1].x);
		vertex.push_back(verts[faces[i]-1].y);
		vertex.push_back(verts[faces[i]-1].z);

		texCoords.push_back(texCs[faces[i + 1]-1].x);
		texCoords.push_back(texCs[faces[i + 1]-1].y);
		texCoords.push_back(texCs[faces[i + 1]-1].z);

		normals.push_back(norms[faces[i + 2]-1].x);
		normals.push_back(norms[faces[i + 2]-1].y);
		normals.push_back(norms[faces[i + 2]-1].z);

		//v2
		vertex.push_back(verts[faces[i + 3]-1].x);
		vertex.push_back(verts[faces[i + 3]-1].y);
		vertex.push_back(verts[faces[i + 3]-1].z);

		texCoords.push_back(texCs[faces[i + 4]-1].x);
		texCoords.push_back(texCs[faces[i + 4]-1].y);
		texCoords.push_back(texCs[faces[i + 4]-1].z);
		
		normals.push_back(norms[faces[i + 5]-1].x);
		normals.push_back(norms[faces[i + 5]-1].y);
		normals.push_back(norms[faces[i + 5]-1].z);

		//v3
		vertex.push_back(verts[faces[i + 6]-1].x);
		vertex.push_back(verts[faces[i + 6]-1].y);
		vertex.push_back(verts[faces[i + 6]-1].z);

		texCoords.push_back(texCs[faces[i + 7]-1].x);
		texCoords.push_back(texCs[faces[i + 7]-1].y);
		texCoords.push_back(texCs[faces[i + 7]-1].z);

		normals.push_back(norms[faces[i + 8]-1].x);
		normals.push_back(norms[faces[i + 8]-1].y);
		normals.push_back(norms[faces[i + 8]-1].z);
	}

#pragma endregion unroll_data
		
	// Once data has been sorted clear read data (which has been copied and are not longer needed).
	verts.clear();
	norms.clear();
	texCs.clear();
	faces.clear();

	return true;
}

void Model::loadTexture(const char* filename)
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
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

}


bool Model::load(const std::string& modelFilename)
{
	bool result;

	// Load in the model data,
	result = loadModel(modelFilename.c_str());
	if (!result)
	{
		//MessageBox(NULL, "Model failed to load", "Error", MB_OK);
		return false;
	}
	return true;
}

