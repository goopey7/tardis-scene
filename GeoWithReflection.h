//Copyright Sam Collier 2022
#pragma once

#include "Geometry.h"
#include <memory>

class GeoWithReflection : public Geometry
{
	public:
		GeoWithReflection(std::unique_ptr<Geometry> geometry);
		void render() override;
		void update(const float dt);
		void load() override;
		bool load(const std::string& modelFilename, const std::string& textureFilename) override;
	private:
		float angle = 0.f;
		float rotationSpeed = 10.f;
		std::unique_ptr<Geometry> geometry;
};

