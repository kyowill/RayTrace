#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>
#include <vector>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:

  Group(){
      //cout<<"Group()"<<endl;
  }
	
  Group( int num_objects )
  {
      //cout<<"Group( int num_objects )"<<endl;
      //scene_.resize(num_objects);
      num_objects_=num_objects;
  }

  ~Group(){
    //delete this; 
  }

  virtual bool intersect( const Ray& r , Hit& h , float tmin ) ;

	
  void addObject( int index , Object3D* obj )
  {
      //scene_.push_back(obj);
      //scene_[index]=obj;
      if (index != scene_.size())
      {
          cerr << "ERROR: inconsitent indices!" << endl;
      }
      scene_.push_back(obj);
      if (scene_.size() > num_objects_)
      {
          cerr << "ERROR: number of actual objects greater than declared number!" << endl;
      }
  }

  int getGroupSize()
  { 
  
      return scene_.size();
  }
  vector<Object3D*> getScene()
  {
    return scene_;
  }


 private:

    vector<Object3D*>  scene_;

    int num_objects_;


};

#endif
	
