#include "Triangle.h"

bool Triangle::intersect( const Ray& r,  Hit& h , float tmin)
{
	// Vector3f a_;
	// Vector3f b_;
	// Vector3f c_;
	// Material* m_;
	Vector3f r_d = r.getDirection();
	Vector3f r_o = r.getOrigin();

	Matrix3f A = Matrix3f(a_.x()-b_.x(), a_.x()-c_.x(), r_d.x(),
						a_.y()-b_.y(), a_.y()-c_.y(), r_d.y(),
						a_.z()-b_.z(), a_.z()-c_.z(), r_d.z());

	Matrix3f t_cramer = Matrix3f(a_.x()-b_.x(), a_.x()-c_.x(), a_.x()-r_o.x(),
									a_.y()-b_.y(), a_.y()-c_.y(), a_.y()-r_o.y(),
									a_.z()-b_.z(), a_.z()-c_.z(), a_.z()-r_o.z());
	
	Matrix3f beta_cramer = Matrix3f(a_.x()-r_o.x(), a_.x()-c_.x(), r_d.x(),
									a_.y()-r_o.y(), a_.y()-c_.y(), r_d.y(),
									a_.z()-r_o.z(), a_.z()-c_.z(), r_d.z());
	
	Matrix3f gamma_cramer = Matrix3f(a_.x()-b_.x(), a_.x()-r_o.x(), r_d.x(),
										a_.y()-b_.y(), a_.y()-r_o.y(), r_d.y(),
										a_.z()-b_.z(), a_.z()-r_o.z(), r_d.z());

	float detA = A.determinant();//|A|

	//if(detA==0.0)
	//printf("|A|:%f\n",detA);

	float alpha, beta, gamma, t;

	t = t_cramer.determinant() / detA;
	// if(t<tmin||t>=h.getT())
	// {
	// 	printf("t:%f\n",t);
	// }

	beta = beta_cramer.determinant() / detA;
	// if(beta>1||beta<0)
	// {
	// 	printf("beta:%f\n",beta);
	// }

	gamma = gamma_cramer.determinant() / detA;
	// if( beta<= 1.0  && beta >= 0.0 && (1.0-beta)>=gamma && gamma >= 0.0)
	// {
	// 	printf("beta+gamma:%f\n",beta+gamma);
	// }

	alpha = 1 - beta - gamma;

	if (t >=tmin && t < h.getT() && beta+gamma <= 1 && beta >= 0 && gamma >= 0) 
	{
		Vector3f normal = alpha * normals[0] + beta * normals[1] + gamma * normals[2];
		
		h.set(t, m_, normal.normalized());
		
		h.setTexCoord(alpha * texCoords[0] + beta * texCoords[1] + gamma * texCoords[2]);
		//printf("1\n");
		return true;
	}
		
	return false;




}