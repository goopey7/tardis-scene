//Copyright Sam Collier 2022
#pragma once

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Camera.h"
#include "Input.h"
#include <stdio.h>
#include <SOIL/SOIL.h>
#include "Sphere.h"
class SkySphere
{
	public:
		SkySphere(const SkySphere&) = delete;
		SkySphere& operator=(const SkySphere&) = delete;
		SkySphere(Camera* cam);
		~SkySphere() = default;
		void render();
		void updateCamera(Camera* cam);
	private:
		Camera* cam;
		Sphere sphere;
};

