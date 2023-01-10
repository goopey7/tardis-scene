//Copyright Sam Collier 2022
#pragma once

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Camera.h"
#include "Input.h"
#include <stdio.h>
#include "SOIL.h"
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

