//Copyright Sam Collier 2022
#pragma once

#include "Model.h"
#include "Sphere.h"
#include "PointLightGeo.h"
#include <memory>

class Tardis : public Geometry
{
	public:
		Tardis();
		void render() override;
		void update(const float dt);
	protected:
	private:
		std::unique_ptr<Model> exterior;
		std::unique_ptr<Model> interior;
		std::unique_ptr<Sphere> lamp;
		float angle = 0.f;
		float angleSpeed = 50.f;
		PointLightGeo light;
		float dimmer = 0.1f;
		float dimmerSpeed = 0.18f;
};

