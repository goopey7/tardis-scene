#include "Scene.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
	: skybox(cam)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();
	cam.setPosition({0.f,0.f,6.f});
	cam.setRotation({0.f,0.f,0.f});
	cam.updateRotation();
	glutSetCursor(GLUT_CURSOR_NONE);

	// Other OpenGL / render setting should be applied here.

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);
	//glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialise scene variables

	crate = SOIL_load_OGL_texture(
				"gfx/earth.jpg",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS|SOIL_FLAG_NTSC_SAFE_RGB|SOIL_FLAG_COMPRESS_TO_DXT
				);
	/*
	dice = SOIL_load_OGL_texture(
				"gfx/diceTexture.png",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS|SOIL_FLAG_NTSC_SAFE_RGB|SOIL_FLAG_COMPRESS_TO_DXT
				);
				*/
	
}

void Scene::handleInput(float dt)
{
	// Handle user input
	bool anyKeyDown = input->anyKeyDown();

	if(input->isKeyDown('r'))
	{
		bIsWireframe = !bIsWireframe;
		input->setKeyUp('r');
	}

	if(input->isKeyDown('w'))
	{
		cam.translate(cam.getForwardVector() * dt * strafeSpeed);
	}

	if(input->isKeyDown('s'))
	{
		cam.translate(cam.getForwardVector().getOpposite() * dt * strafeSpeed);
	}

	if(input->isKeyDown('a'))
	{
		cam.translate(cam.getRightVector() * dt * strafeSpeed);
	}

	if(input->isKeyDown('d'))
	{
		cam.translate(cam.getRightVector().getOpposite() * dt * strafeSpeed);
	}

	if(input->isKeyDown('e'))
	{
		cam.translate(cam.getUpVector().getOpposite() * dt * strafeSpeed);
	}

	if(input->isKeyDown('q'))
	{
		cam.translate(cam.getUpVector() * dt * strafeSpeed);
	}

	// mouse input
	if(input->getMouseX() != width/2)
	{
		float mouseDiff = input->getMouseX() - width/2.f;
		cam.rotate({0.f,rotateSpeed*mouseDiff,0.f});
		cam.updateRotation();
	}
	if(input->getMouseY() != height/2)
	{
		float mouseDiff = input->getMouseY() - height/2.f;
		cam.rotate({mouseDiff*rotateSpeed,0.f,0.f});
		cam.updateRotation();
	}

}

void Scene::update(float dt)
{
	// update scene related variables.
	cam.update();

	// Calculate FPS for output
	calculateFPS();

	// Move mouse to center
	glutWarpPointer(width/2,height/2);
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	Vector3 camPos = cam.getPosition();
	Vector3 camLookAt = cam.getLookAtVector();
	Vector3 camUp = cam.getUpVector();

	gluLookAt(camPos.x,camPos.y,camPos.z,
			camLookAt.x,camLookAt.y,camLookAt.z,
			-camUp.x,-camUp.y,-camUp.z);

	if(bIsWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	skybox.render();

	// Render geometry/scene here -------------------------------------

	//Shape::renderDisc(111500, 5.f,crate);
	Shape::renderSphere(5.f,100,100,crate);

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.f,0.f,0.f,1.f);						// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending function
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	sprintf_s(pos, "Pos: {%f,%f,%f}", cam.getPosition().x,cam.getPosition().y,cam.getPosition().z);
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
	displayText(-1.f, 0.80f, 1.f, 0.f, 0.f, pos);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void Scene::renderSquare(float xOffset)
{
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-1.f+xOffset,-1.f,0.f);
		glVertex3f(1.f+xOffset,-1.f,0.f);
		glVertex3f(-1.f+xOffset,1.f,0.f);
		glVertex3f(1.f+xOffset,1.f,0.f);
	glEnd();
}

void Scene::renderTriangle(float xOffset)
{
	glBegin(GL_TRIANGLES);
		//glColor3f(1.f,0.f,0.f);
		glVertex3f(0.f+xOffset,1.f,0.f);
		//glColor3f(0.f,1.f,0.f);
		glVertex3f(1.f+xOffset,-1.f,0.f);
		//glColor3f(0.f,0.f,1.f);
		glVertex3f(-1.f+xOffset,-1.f,0.f);
	glEnd();
}

void Scene::renderTriangleStrip(float xOffset)
{
	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-1.f+xOffset,-1.f,0.f);
		glColor3f(0.75f, 0.25f, 0.f);
		glVertex3f(1.f+xOffset,-1.f,0.f);
		glColor3f(0.5f, 0.5f, 0.f);
		glVertex3f(0.f+xOffset,1.f,0.f);
		glColor3f(0.f, 1.f, 0.5f);
		glVertex3f(2.f+xOffset,1.f,-3.f);
		glColor3f(1.f, 0.3f, 1.f);
		glVertex3f(2.f+xOffset,2.f,-3.f);
		glVertex3f(3.f+xOffset,1.5f,0.f);
		glVertex3f(-1.f+xOffset,2.f,0.f);
	glEnd();
}

void Scene::renderTriangleFan(float xOffset)
{
	//glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(-1.f+xOffset,0.f,0.f);
		glVertex3f(1.f+xOffset,0.5f,0.f);
		glVertex3f(1.f+xOffset,1.f,0.f);
		glVertex3f(1.f+xOffset,1.5f,0.f);
		glVertex3f(0.5f+xOffset,2.f,0.f);
		glVertex3f(0.f+xOffset,2.f,0.f);
		glVertex3f(-0.5f+xOffset,2.f,0.f);
		glVertex3f(-1.f+xOffset,2.f,0.f);
		glVertex3f(-1.5f+xOffset,1.5f,0.f);
		glVertex3f(-2.f+xOffset,1.f,0.f);
		glVertex3f(-2.5f+xOffset,1.f,0.f);
		glVertex3f(-3.f+xOffset,1.f,0.f);
		glVertex3f(-3.f+xOffset,0.5f,0.f);
		glVertex3f(-2.5f+xOffset,0.f,0.f);
	glEnd();
}

void Scene::renderQuads(float xOffset)
{
	glBegin(GL_QUADS);
		glVertex3f(-2.f+xOffset,-2.f,0.f);
		glVertex3f(0.f+xOffset,-2.f,0.f);
		glVertex3f(0.f+xOffset,0.f,0.f);
		glVertex3f(-2.f+xOffset,0.f,0.f);

		glVertex3f(0.f+xOffset,0.f,0.f);
		glVertex3f(2.f+xOffset,0.f,0.f);
		glVertex3f(2.f+xOffset,2.f,0.f);
		glVertex3f(0.f+xOffset,2.f,0.f);

		glVertex3f(-4.f+xOffset,0.f,0.f);
		glVertex3f(-2.f+xOffset,0.f,0.f);
		glVertex3f(-2.f+xOffset,2.f,0.f);
		glVertex3f(-4.f+xOffset,2.f,0.f);

		glVertex3f(2.f+xOffset,-2.f,0.f);
		glVertex3f(4.f+xOffset,-2.f,0.f);
		glVertex3f(4.f+xOffset,0.f,0.f);
		glVertex3f(2.f+xOffset,0.f,0.f);

	glEnd();
}

void Scene::renderPentagon(float xOffset)
{
	glColor3f(1.f,1.f,0.f);
	glBegin(GL_POLYGON);
		glVertex3f(0.f,1.f,0.f);
		glVertex3f(-1.f,0.f,0.f);
		glVertex3f(-0.5f,-1.f,0.f);
		glVertex3f(0.5f,-1.f,0.f);
		glVertex3f(1.f,0.f,0.f);
	glEnd();
}

void Scene::renderCube()
{
	glBegin(GL_QUADS);

		// front face
		glColor3f(0.f,1.f,0.f);
		glNormal3f(0.f,0.f,1.f);
		glVertex3f(-0.5f,0.5f,0.5f);
		glVertex3f(-0.5f,-0.5f,0.5f);
		glVertex3f(0.5f,-0.5f,0.5f);
		glVertex3f(0.5f,0.5f,0.5f);

		// right face
		glColor3f(1.f,0.f,0.f);
		glNormal3f(1.f,0.f,0.f);
		glVertex3f(0.5f,0.5f,0.5f);
		glVertex3f(0.5f,-0.5f,0.5f);
		glVertex3f(0.5f,-0.5f,-0.5f);
		glVertex3f(0.5f,0.5f,-0.5f);

		//bottom face
		glColor3f(0.f,0.f,1.f);
		glNormal3f(0.f,-1.f,0.f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glVertex3f(-0.5f,-0.5f,0.5f);
		glVertex3f(0.5f,-0.5f,0.5f);
		glVertex3f(0.5f,-0.5f,-0.5f);

		//top face
		glColor3f(0.f,0.f,1.f);
		glNormal3f(0.f,1.f,0.f);
		glVertex3f(-0.5f,0.5f,-0.5f);
		glVertex3f(-0.5f,0.5f,0.5f);
		glVertex3f(0.5f,0.5f,0.5f);
		glVertex3f(0.5f,0.5f,-0.5f);

		// left face
		glColor3f(1.f,0.f,0.f);
		glNormal3f(-1.f,0.f,0.f);
		glVertex3f(-0.5f,0.5f,-0.5f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glVertex3f(-0.5f,-0.5f,0.5f);
		glVertex3f(-0.5f,0.5f,0.5f);

		// back face
		glColor3f(0.f,1.f,0.f);
		glNormal3f(0.f,0.f,-1.f);
		glVertex3f(0.5f,0.5f,-0.5f);
		glVertex3f(0.5f,-0.5f,-0.5f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glVertex3f(-0.5f,0.5f,-0.5f);

	glEnd();
}

void Scene::renderPlane()
{
	glPushMatrix();
	glBegin(GL_QUADS);
	
		// front face
		glColor3f(1.f,1.f,1.f);
		glNormal3f(0.f,1.f,0.f);

		glVertex3f(-100.5f,-0.5f,0.5f);
		glVertex3f(100.5f,-0.5f,0.5f);

		glVertex3f(-100.5f,-0.5f,100.5f);
		glVertex3f(100.5f,-0.5f,100.5f);

	glEnd();
	glPopMatrix();
}

void Scene::renderPlaneDetailed()
{
	glPushMatrix();
	glBegin(GL_QUADS);
	
		glColor3f(1.f,0.2f,0.2f);
		glNormal3f(0.f,1.f,0.f);
		for(float x=-1.f;x<1.f;x+=.05f)
		{
			for(float z=-1.f;z<1.f;z+=.05f)
			{
				glVertex3f(-x,-1.f,-z);
				glVertex3f(-x,-1.f,z);
				glVertex3f(x,-1.f,z);
				glVertex3f(x,-1.f,-z);
			}
		}

	glEnd();
	glPopMatrix();
}

void Scene::renderSphere()
{
	glutSolidSphere(0.5f, 40.f, 40.f);
}

void Scene::renderRoom()
{
	glScalef(100.f,100.f,100.f);
	renderCube();
}

void Scene::renderDice()
{
	//glBindTexture(GL_TEXTURE_2D,dice);
	glColor3f(1.f,1.f,1.f);

	glBegin(GL_QUADS);

	// front face
	glNormal3f(0.f, 0.f, 1.f);

	glTexCoord2f(0.5f,0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glTexCoord2f(0.5f,0.75f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glTexCoord2f(0.75f,0.75f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glTexCoord2f(0.75f,0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);

	// right face
	glNormal3f(1.f, 0.f, 0.f);

	glTexCoord2f(0.75f,0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);

	glTexCoord2f(0.75f,0.75f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glTexCoord2f(1.f,0.75f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glTexCoord2f(1.f,0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	//bottom face
	glNormal3f(0.f, -1.f, 0.f);

	glTexCoord2f(0.5f,0.75f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glTexCoord2f(0.5f,1.f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glTexCoord2f(0.75f,1.f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glTexCoord2f(0.75f,0.75f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	//top face
	glNormal3f(0.f, 1.f, 0.f);

	glTexCoord2f(0.5f,0.25f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glTexCoord2f(0.5f,0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glTexCoord2f(0.75f,0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);

	glTexCoord2f(0.75f,0.25f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	// left face
	glNormal3f(-1.f, 0.f, 0.f);

	glTexCoord2f(0.25f,0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glTexCoord2f(0.25f,0.75f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glTexCoord2f(0.5f,0.75f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glTexCoord2f(0.5f,0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	// back face
	glNormal3f(0.f, 0.f, -1.f);

	glTexCoord2f(0.f,0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	glTexCoord2f(0.f,0.75f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glTexCoord2f(0.25f,0.75f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glTexCoord2f(0.25f,0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);

	glEnd();
}

