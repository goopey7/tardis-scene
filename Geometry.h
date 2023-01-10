//Copyright Sam Collier 2022
#pragma once

#include <string>

class Geometry
{
	public:
		Geometry(const Geometry&) = delete;
		Geometry& operator=(const Geometry&) = delete;
		Geometry() = default;
		virtual void render() {};
		virtual void load() {}
		virtual bool load(const std::string& modelFilename, const std::string& textureFilename) {return true;}
		virtual bool load(const std::string& modelFilename) {return true;}
	protected:
	private:
};

