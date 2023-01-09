// Model class, represents a loaded OBJ model
// handles the loading/processing of the model file. Also loads and stores its own texture
// NOT complete, for students to complete. Model data requires being sorted and the render function needs completed.
#ifndef _MODEL_H_
#define _MODEL_H_

// INCLUDES //
#include "Geometry.h"
#include <GL/freeglut.h>
#include <fstream>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

// MY CLASS INCLUDES //
#include <vector>
#include "Vector3.h"
#include <SOIL/SOIL.h>

class Model : public Geometry
{

public:

	bool load(const std::string& modelFilename, const std::string& textureFilename) override;
	virtual void render() override;

private:

	void loadTexture(const char*);
	bool loadModel(const char*);

	// model texture
	GLuint texture;

	// Stoagre for sorted data
	vector<float> vertex, normals, texCoords;

};

#endif
