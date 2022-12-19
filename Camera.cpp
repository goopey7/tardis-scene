//Sam Collier 2022

#include "Camera.h"


Camera::Camera()
{
	forward.set(0.f,0.f,1.f);
	up.set(0.f,1.f,0.f);
	updateRotation();
}

Camera::Camera(Vector3 pos)
{
	this->pos = pos;
	Camera();
}

void Camera::update()
{
}

const Vector3& Camera::getPosition() const
{
	return pos;
}

const Vector3& Camera::getForwardVector() const
{
	return forward;
}

const Vector3& Camera::getUpVector() const
{
	return up;
}

const Vector3& Camera::getRightVector() const
{
	return right;
}

void Camera::translate(const Vector3& translation)
{
	pos += translation;
}

void Camera::rotate(const Rotation& rotation)
{
	this->rotation += rotation;
}

void Camera::setPosition(const Vector3& newPosition)
{
	pos.set(newPosition);
}

void Camera::setRotation(const Rotation& newRotation)
{
	rotation = newRotation;
}

void Camera::updateRotation()
{
	float cosP, cosY, cosR;
	float sinP, sinY, sinR;

	cosP = cosf(rotation.pitch * PI / 180.f);
	cosY = cosf(rotation.yaw * PI / 180.f);
	cosR = cosf(rotation.roll * PI / 180.f);
	sinP = sinf(rotation.pitch * PI / 180.f);
	sinY = sinf(rotation.yaw * PI / 180.f);
	sinR = sinf(rotation.roll * PI / 180.f);

	forward.set(sinY * cosP, sinP, cosP * -cosY);

	up.set(-cosY * sinR - sinY * sinP * cosR,
			cosP * cosR,
			-sinY * sinR - sinP * cosR * -cosY);

	right.set(forward.cross(up));
}

const Vector3 Camera::getLookAtVector() const
{
	return getPosition() + getForwardVector();
}

