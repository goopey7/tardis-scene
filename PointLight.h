//Sam Collier 2023
#pragma once

#include <GL/gl.h>
#include "Vector3.h"
#include <vector>

class PointLight
{
	public:
		PointLight(const PointLight&) = delete;
		PointLight& operator=(const PointLight&) = delete;
		PointLight(Vector3 position = Vector3(0,0,0), unsigned int lightID = GL_LIGHT0);
		~PointLight() = default;
		void render();
		void setAmbient(std::vector<GLfloat> ambient);
		void setDiffuse(std::vector<GLfloat> diffuse);
		void setLightPosition(std::vector<GLfloat> lightPosition);
		void setRelativePos(Vector3 relativePos);
		std::vector<GLfloat> getLightPosition() const;
		void setAttenuation(float constant, float linear, float quadratic);
	private:
		std::vector<GLfloat> ambient;
		std::vector<GLfloat> diffuse;
		std::vector<GLfloat> lightPosition;
		unsigned int lightID;
		Vector3 relativePos;

		float constAttenuation = 1.f;
		float linearAttenuation = 0.2f;
		float quadAttenuation = 0.08f;
};

