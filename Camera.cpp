#include "Camera.h"

PerspectiveCamera::PerspectiveCamera(const Vector3f& center, const Vector3f& dir,const Vector3f& upp , float angle)
{
	this->center=center;
	Vector3f w=dir.normalized();
	Vector3f u=Vector3f::cross(dir ,upp).normalized();
	Vector3f v=Vector3f::cross(u,dir).normalized();

	this->direction=w;
	this->horizontal=u;
	this->up=v;
	angle_=angle;
}

Ray PerspectiveCamera::generateRay( const Vector2f& point)//<=> Vector3f(Vector2f,9) center(0,0,10)
{
	float disToScreen=1.0/(tan(angle_/2));

	Vector3f originScreen=center+direction*disToScreen;

	//Vector3f originScreen=center+direction*disToScreen;

	Vector3f position=originScreen+this->up*point.y()+this->horizontal*point.x();

	Vector3f direction=(position-center).normalized();

	return Ray(center,direction);
}
