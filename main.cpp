#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include "RayTracer.h"

#include <string.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

int get(int x, int max)
{
    if (x < 0) 
    {
        return 0;
    }else if (x >= max)
    {
        return max-1;
    }else
    {
        return x;
    }
}

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{

    SceneParser SP(argv[1]);//
    Group* scene=SP.getGroup();
    Camera* camera=SP.getCamera();
    int len=500;

    Image image( 500, 500 );
    srand(time(NULL));
    Vector3f pixelColor(0,0,0);
    vector<vector<Vector3f> > jitter;
    vector<Vector3f> col(len*3,Vector3f(0,0,0));
    float kernel[5]={0.1201, 0.2339, 0.2931, 0.2339, 0.1201};
    ////////////////////////jittered sampling////////////
    for(int i=0;i<len*3;++i)
    {
        //col.empty();
        for(int j=0;j<len*3;++j)
        {
            //Vector2f point(-1+j/250.0,-1+i/250.0);

            
            RayTracer RT(&SP,atoi(argv[2]));


            float ranI=(rand()%10)/10.0-0.5;
            float ranJ=(rand()%10)/10.0-0.5;


            Vector2f point(-1+(i+ranI)/(3*len/2),-1+(j+ranJ)/(3*len/2));

            Ray r(camera->generateRay(point));

            Hit h;
            Vector3f ori=r.getOrigin();
            Vector3f dir=r.getDirection();

            pixelColor=RT.traceRay(r,0,0,1,h);


            col[j]=pixelColor;
             

        }
        jitter.push_back(col);
    }

    ////////////////Gaussian blur/////////////////

    vector<vector<Vector3f> > blurimagehozi(jitter);

    vector<vector<Vector3f> > blurimage(jitter);

    for(int i=0;i<len*3;++i)
    {
        for(int j=0;j<len*3;++j)
        {
            blurimagehozi[i][j]=kernel[0]*jitter[get(i,3*len)][get(j-2,3*len)]+
                                kernel[1]*jitter[get(i,3*len)][get(j-1,3*len)]+
                                kernel[2]*jitter[get(i,3*len)][get(j,3*len)]+
                                kernel[3]*jitter[get(i,3*len)][get(j+1,3*len)]+
                                kernel[4]*jitter[get(i,3*len)][get(j+2,3*len)];
        }
    }

    for(int i=0;i<len*3;++i)
    {
        for(int j=0;j<len*3;++j)
        {
            blurimage[i][j]=kernel[0]*blurimagehozi[get(i-2,3*len)][get(j,3*len)]+
                            kernel[1]*blurimagehozi[get(i-1,3*len)][get(j,3*len)]+
                            kernel[2]*blurimagehozi[get(i,3*len)][get(j,3*len)]+
                            kernel[3]*blurimagehozi[get(i+1,3*len)][get(j,3*len)]+
                            kernel[4]*blurimagehozi[get(i+2,3*len)][get(j,3*len)];
        }
    }




    for(int i=0;i<len;++i)
    {
        for(int j=0;j<len;++j)
        {
            
            pixelColor=blurimage[3*i][3*j]+
                       blurimage[3*i][3*j+1]+
                       blurimage[3*i][3*j+2]+
                       blurimage[3*i+1][3*j]+
                       blurimage[3*i+1][3*j+1]+
                       blurimage[3*i+1][3*j+2]+
                       blurimage[3*i+2][3*j]+
                       blurimage[3*i+2][3*j+1]+
                       blurimage[3*i+2][3*j+2];

            image.SetPixel( i,j, 1.0/9*pixelColor);
        }
    }


    //image.SetPixel( 9,14, pixelColor );//(0,0)在左下角(width-1,height-1)在右上角 
    image.SaveImage("yk.bmp");
    return 0;
}

