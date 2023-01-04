// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include <SOIL/SOIL.h>
#include <vector>
#include <string.h>

#include "Camera.h"
#include "PointLight.h"
#include "Sphere.h"
#include "Skybox.h"

#define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)


class Scene{

public:
	Scene(Input *in);
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

protected:
	// configure opengl render pipeline
	void initialiseOpenGL();
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();
	

	// draw primitive functions
	void renderSquare(float xOffset);
	void renderTriangle(float xOffset);
	void renderTriangleStrip(float xOffset);
	void renderTriangleFan(float xOffset);
	void renderQuads(float xOffset);
	void renderPentagon(float xOffset);
	void renderCube();
	void renderPlane();
	void renderPlaneDetailed();
	void renderSphere();
	void renderRoom();
	void renderDice();
	

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];
	char pos[40];
	char camRotationText[40];

	bool bIsWireframe = false;

	float earthAngle = 0.f;
	float strafeSpeed = 10.f;
	float rotateSpeed = 0.1f;

	Camera cam;
	Skybox skybox;

	//GLuint dice;
	GLuint crate;

	PointLight sunLight;
};

#endif
