#include "Scene.h"
#include "Model.h"
#include "PointLight.h"
#include "Sphere.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
	: skybox(&fpsCam),
	    mercury(new Sphere(2.f,100,100,"gfx/mercury.jpg")),
		venus(new Sphere(2.5f,100,100,"gfx/venus.jpg")),
		earth(new Sphere(5.f,100,100,"gfx/earth.jpg")),
		moon(new Sphere(1.f,100,100,"gfx/moon.jpg")),
		mars(new Sphere(3.f,100,100,"gfx/mars.jpg")),
		jupiter(new Sphere(10.f,100,100,"gfx/jupiter.jpg")),
		saturn(new Sphere(8.f,100,100,"gfx/saturn.jpg")),
		uranus(new Sphere(6.f,100,100,"gfx/uranus.jpg")),
		neptune(new Sphere(5.f,100,100,"gfx/neptune.jpg")),
	  sunSphere(new Sphere(5.f,100,100,"gfx/sun.jpeg")),
	  sun(std::move(sunSphere), {0.f,0.f,0.f}, GL_LIGHT0)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();
	fpsCam.setPosition({0.f,0.f,6.f});
	fpsCam.setRotation({0.f,0.f,0.f});
	fpsCam.updateRotation();

	topDownCam.setPosition({0.f,800.f,0.f});
	topDownCam.setRotation({-90.f,0.f,0.f});
	topDownCam.updateRotation();

	glutSetCursor(GLUT_CURSOR_NONE);

	// Other OpenGL / render setting should be applied here.

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialise scene variables

	crate = SOIL_load_OGL_texture(
				"gfx/crate.png",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS|SOIL_FLAG_NTSC_SAFE_RGB|SOIL_FLAG_COMPRESS_TO_DXT
				);

	spaceship.load("models/spaceship.obj","models/spaceship.jpg");

	sun.load();
	sun.setAmbient({0.6f,0.6f,0.6f,1.f});
	sun.setAttenuation(1.f,0.f,0.f);

	mercury->load();
	venus->load();
	earth->load();
	moon->load();
	mars->load();
	jupiter->load();
	saturn->load();
	uranus->load();
	neptune->load();

	currentCam = &fpsCam;
	glDisable(GL_LIGHT1);;
	
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
		fpsCam.translate(fpsCam.getForwardVector() * dt * strafeSpeed);
	}

	if(input->isKeyDown('s'))
	{
		fpsCam.translate(fpsCam.getForwardVector().getOpposite() * dt * strafeSpeed);
	}

	if(input->isKeyDown('a'))
	{
		fpsCam.translate(fpsCam.getRightVector() * dt * strafeSpeed);
	}

	if(input->isKeyDown('d'))
	{
		fpsCam.translate(fpsCam.getRightVector().getOpposite() * dt * strafeSpeed);
	}

	if(input->isKeyDown('e'))
	{
		fpsCam.translate(fpsCam.getUpVector().getOpposite() * dt * strafeSpeed);
	}

	if(input->isKeyDown('q'))
	{
		fpsCam.translate(fpsCam.getUpVector() * dt * strafeSpeed);
	}

	// mouse input
	if(input->getMouseX() != width/2 && bCaptureMouse)
	{
		float mouseDiff = input->getMouseX() - width/2.f;
		fpsCam.rotate({0.f,rotateSpeed*mouseDiff,0.f});
		fpsCam.updateRotation();
	}
	if(input->getMouseY() != height/2 && bCaptureMouse)
	{
		float mouseDiff = input->getMouseY() - height/2.f;
		fpsCam.rotate({mouseDiff*rotateSpeed,0.f,0.f});
		fpsCam.updateRotation();
	}

	if(input->isKeyDown(';'))
	{
		if(bEnableSun)
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
		bEnableSun = !bEnableSun;
		input->setKeyUp(';');
	}

	if(input->isKeyDown(' '))
	{
		bIsPaused = !bIsPaused;
		input->setKeyUp(' ');
	}

	if(input->isKeyDown('1'))
	{
		currentCam = &fpsCam;
		skybox.updateCamera(currentCam);
		input->setKeyUp('1');
	}
	if(input->isKeyDown('2'))
	{
		currentCam = &topDownCam;
		skybox.updateCamera(currentCam);
		input->setKeyUp('2');
	}
	if(input->isKeyDown('m'))
	{
		bCaptureMouse = !bCaptureMouse;
	}

}

void Scene::update(float dt)
{
	// update scene related variables.
	currentCam->update();

	// Calculate FPS for output
	calculateFPS();

	// Move mouse to center
	if(bCaptureMouse)
	{
		glutWarpPointer(width/2,height/2);
	}

	if(!bIsPaused)
	{
		earthAngle+=dt*10.f;
		spaceshipAngle+=dt*60.f;
		earthSunRotation+=dt;

		tardis.update(dt);
	}

}

void Scene::render()
{

	// Clear Color, Depth and Stencil Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera
	Vector3 camPos = currentCam->getPosition();
	Vector3 camLookAt = currentCam->getLookAtVector();
	Vector3 camUp = currentCam->getUpVector();

	gluLookAt(camPos.x,camPos.y,camPos.z,
			camLookAt.x,camLookAt.y,camLookAt.z,
			-camUp.x,-camUp.y,-camUp.z);

	float camMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, camMatrix);

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

	glPushMatrix();

		if(bEnableSun)
		{
			sun.render();
		}


		// mercury
		glPushMatrix();
			glRotatef(earthSunRotation,0.f,1.f,0.f);
			glTranslatef(0.f,0.f,10.f);
			glRotatef(earthAngle+72,0.f,1.f,0.f);
			mercury->render();
		glPopMatrix();

		// venus
		glPushMatrix();
			glRotatef(earthSunRotation+131,0.f,1.f,0.f);
			glTranslatef(3.f,0.f,30.f);
			glRotatef(earthAngle,0.f,1.f,0.f);
			venus->render();
		glPopMatrix();

		// earth
		glPushMatrix();
			glRotatef(earthSunRotation,0.f,1.f,0.f);
			glTranslatef(50.f,0.f,10.f);
			glRotatef(earthAngle,0.f,1.f,0.f);
			earth->render();

			// tardis
			glPushMatrix();
				glScalef(0.25f,0.25f,0.25f);
				glRotatef(spaceshipAngle,0.f,1.f,1.f);
				glTranslatef(55.f,0.f,0.f);
				tardis.render();
			glPopMatrix();

			// moon
			glPushMatrix();
				glRotatef(earthSunRotation,0.f,1.f,0.f);
				glTranslatef(10.f,0.f,5.f);
				moon->render();
			glPopMatrix();
		glPopMatrix();

		// mars
		glPushMatrix();
			glRotatef(earthSunRotation+192,0.f,1.f,0.f);
			glTranslatef(40.f,0.f,70.f);
			glRotatef(earthAngle,0.f,1.f,0.f);
			mars->render();
		glPopMatrix();

		// jupiter
		glPushMatrix();
			glRotatef(earthSunRotation+252,0.f,1.f,0.f);
			glTranslatef(0.f,0.f,130.f);
			glRotatef(earthAngle,0.f,1.f,0.f);
			jupiter->render();
		glPopMatrix();

		// saturn
		glPushMatrix();
			glRotatef(earthSunRotation+44,0.f,1.f,0.f);
			glTranslatef(10.f,0.f,170.f);
			glRotatef(earthAngle,0.f,1.f,0.f);
			saturn->render();

			// spaceship orbiting saturn
			glPushMatrix();
				glRotatef(-spaceshipAngle,0.f,1.f,0.f);
				glTranslatef(6.f,0.f,17.f);
				glRotatef(90.f,0.f,1.f,0.f);
				spaceship.render();
			glPopMatrix();
		glPopMatrix();

		// uranus
		glPushMatrix();
			glRotatef(earthSunRotation+156,0.f,1.f,0.f);
			glTranslatef(20.f,0.f,200.f);
			glRotatef(earthAngle,0.f,1.f,0.f);
			uranus->render();
		glPopMatrix();

		// neptune
		glPushMatrix();
			glRotatef(earthSunRotation+234,0.f,1.f,0.f);
			glTranslatef(30.f,0.f,220.f);
			glRotatef(earthAngle,0.f,1.f,0.f);
			neptune->render();
		glPopMatrix();

	glPopMatrix();

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
	farPlane = 1000.0f;

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
	glDisable(GL_LIGHTING);
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	sprintf_s(pos, "Pos: {%f,%f,%f}", currentCam->getPosition().x,currentCam->getPosition().y,currentCam->getPosition().z);
	sprintf_s(camRotationText, "cam pit,yaw: {%f,%f}", currentCam->getRotation().pitch,currentCam->getRotation().yaw);
	displayText(-1.f, 0.96f, 1.f, 1.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 1.f, 0.f, fps);
	displayText(-1.f, 0.84f, 1.f, 1.f, 0.f, pos);
	displayText(-1.f, 0.7f, 1.f, 1.f, 0.f, spa);
	displayText(-1.f, 0.78f, 1.f, 1.f, 0.f, camRotationText);
	glEnable(GL_LIGHTING);
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

