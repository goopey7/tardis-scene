//Copyright Sam Collier 2022
#pragma once

#include "Geometry.h"
#include "PointLight.h"
#include <memory>

class PointLightGeo : public PointLight, public Geometry
{
	public:
		PointLightGeo(const PointLightGeo&) = delete;
		PointLightGeo& operator=(const PointLightGeo&) = delete;
		PointLightGeo(std::unique_ptr<Geometry> geometry, Vector3 position = Vector3(0,0,0), unsigned int lightID = GL_LIGHT0);
		void render() override;
		void load() override;
		bool load(const std::string& modelFilename, const std::string& textureFilename) override;
	protected:
	private:
		std::unique_ptr<Geometry> geometry;
};

