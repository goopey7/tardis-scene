//Sam Collier 2022
#pragma once

#include "Vector3.h"

#define PI 3.14159265358979323846

struct Rotation
{
	float pitch=0.f;
	float yaw=0.f;
	float roll=0.f;
	void operator+=(const Rotation& r)
	{
		this->pitch += r.pitch;
		this->yaw += r.yaw;
		this->roll += r.roll;
	};
};

class Camera
{
	public:
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera();
		Camera(Vector3 pos);
		~Camera() = default;

		void update();
		void updateRotation();

		void translate(const Vector3& translation);
		void rotate(const Rotation& rotation);

		void setPosition(const Vector3& newPosition);
		void setRotation(const Rotation& newRotation);

		void setForwardVector(const Vector3& newForwardVector);

		const Vector3& getPosition() const;
		const Vector3& getForwardVector() const;
		const Vector3& getUpVector() const;
		const Vector3& getRightVector() const;
		const Vector3 getLookAtVector() const;
		const Rotation& getRotation() const;

	private:
		Vector3 pos;
		Vector3 forward;
		Vector3 up;
		Vector3 right;

		Rotation rotation;
};
