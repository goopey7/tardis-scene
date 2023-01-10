//Copyright Sam Collier 2022
#pragma once

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Camera.h"
#include "Input.h"
#include <stdio.h>
#include "SOIL.h"

class Skybox
{
	public:
		Skybox(const Skybox&) = delete;
		Skybox& operator=(const Skybox&) = delete;
		Skybox(Camera* cam);
		~Skybox() = default;

		void render();
		void updateCamera(Camera* cam);
	private:
		void renderGeometry();
		Camera* cam;
		GLuint skyTexture;
};

