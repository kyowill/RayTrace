#include "Group.h"
using  namespace std;


bool Group::intersect( const Ray& r , Hit& h , float tmin ) 
{
	Hit result;
	int sum=0;
	for(unsigned i=0;i<scene_.size();++i)
	{
		
		if(scene_[i]->intersect(r,h,tmin))
		{
			if(h.getT()<result.getT())
			{
				result.set(h.getT(),h.getMaterial(),h.getNormal());
			}
			sum++;
		}
	}
	h.set(result.getT(),result.getMaterial(),result.getNormal());

	return sum>0;

}