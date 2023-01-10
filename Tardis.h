//Copyright Sam Collier 2022
#pragma once

#include "Model.h"
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
		float angle;
};

