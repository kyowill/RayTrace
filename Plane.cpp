#include "Plane.h"


bool Plane::intersect( const Ray& r , Hit& h , float tmin)
{

	//float a=ray.direction_.normalize().dotMul(normal_);
	float a=Vector3f::dot(r.getDirection(),normal_);

	//printf("a:%f\n",a);

	if(a<0)
	{
		

		float b=Vector3f::dot(normal_,r.getOrigin()-normal_*offset_);

		float t=-b/a;


		//if(t>=tmin&&t<FLT_MAX) h.getT()
		if(t>=tmin&&t< h.getT())
		{
			h.set(t,material_,normal_);

			return true;
		}else
		{
			return false;
		}

	}else
	{
		return false;

	}


}