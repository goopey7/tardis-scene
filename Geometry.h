//Copyright Sam Collier 2022
#pragma once


class Geometry
{
	public:
		Geometry(const Geometry&) = delete;
		Geometry& operator=(const Geometry&) = delete;
		Geometry() = default;
		virtual void render() = 0;
	protected:
	private:
};

