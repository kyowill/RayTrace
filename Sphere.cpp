#include "Sphere.h"

bool Sphere::intersect( const Ray& r , Hit& h , float tmin)
{
	Vector3f v=r.getOrigin()-center_;//Ray:Vector3f origin,Vector3f direction;

	// float a0=v.absSquared()-radius_*radius_;
	// //printf("a0:%f\n",a0);
	// float Ddotv=Vector3f::dot(r.getDirection().normalized(),v);//normalized direction


	// float discr=Ddotv*Ddotv - a0;

	float a=r.getDirection().absSquared();

	float b=2*(Vector3f::dot(r.getDirection(),v));

	float c=v.absSquared()-radius_*radius_;

	float discr=b*b-4*a*c;
	

	if(discr>=0)
	{
		//float t=-Ddotv-sqrt(discr);
		float t1=(-1*b-sqrt(discr))/(2*a);

		float t2=(-1*b+sqrt(discr))/(2*a);

		float t;
		if(c<0.0001)//ray goingin sphere , t2 is what we need
		{
			t=t2;
		}else
		{
			t=t1;
		}

		//if(!(t>=tmin&&t<FLT_MAX))
		if(!(t>tmin&&t<h.getT()))
		{
			return false;
		}

		//t=t/(r.getDirection().abs());
		
		Vector3f tmp=r.pointAtParameter(t);


		h.set(t,material,(tmp-center_).normalized());

		return true;
	}

	return false;
}
