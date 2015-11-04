#include "Transform.h"

Transform::Transform( const Matrix4f& m, Object3D* obj ):o(obj)
{
	//TMatrix_=m.inverse();

	TMatrix_=m;
}
bool Transform::intersect( const Ray& r , Hit& h , float tmin)
{
    Matrix4f iMatrix=TMatrix_.inverse();
	Matrix4f tMatrix=TMatrix_.transposed() ;
    
    //Ray newRay = Ray((iMatrix * Vector4f(r.getOrigin(), 1.0f)).xyz(), (iMatrix* Vector4f(r.getDirection(), 0.0f)).xyz().normalized());
	Ray newRay = Ray((iMatrix * Vector4f(r.getOrigin(), 1.0f)).xyz(), (iMatrix* Vector4f(r.getDirection(), 0.0f)).xyz());
    if(o->intersect(newRay, h , tmin))
    {
    	
		h.set(h.getT(), h.getMaterial(), (iMatrix.transposed() * Vector4f(h.getNormal(), 0.0f)).xyz().normalized());
		return true;

	}

    return false;

}