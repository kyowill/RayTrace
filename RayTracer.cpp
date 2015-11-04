#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS.
//These function definitions are mere suggestions. Change them as you like.
static Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
    
    float V_N=Vector3f::dot(incoming,normal);
    

    Vector3f R=incoming - 2 * V_N * normal;
    return R.normalized();
}

static bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
    float d_N=Vector3f::dot(incoming,normal);

    float s_root=1 - index_n*index_n*(1-d_N*d_N)/(index_nt*index_nt);

    if(s_root<0)
    {
        return false;
    }


    transmitted=(index_n*(incoming-normal*d_N)/index_nt - normal*sqrt(s_root)).normalized();

    //return Vector3f(index_n*(incoming-normal*d_N)/index_nt - normal*sqrt(s_root));
    return true;
}

static float getSchlickWeight(Vector3f d, Vector3f t, Vector3f N, float n, float nt)
{
    float R_o = pow((nt-n)/(nt+n), 2);
    float c = (n <= nt) ? abs(Vector3f::dot(d, N)) : abs(Vector3f::dot(t, N));
    return R_o + (1 - R_o) * pow(1-c, 5);
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces 
  //more arguments if you need...
                      ) :
  m_scene(scene)

{
    g=scene->getGroup();
    m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit ) const //refr_index光线所在的介质的折射指数
{
    if (bounces > m_maxBounces)
    {
        return Vector3f(0,0,0);
    }
    int numLights = m_scene->getNumLights();
    if(g->intersect(ray, hit, tmin + EPSILON))
    {
        Vector3f totalColor = m_scene->getAmbientLight();
        if (hit.hasTex && hit.getMaterial()->t.valid())
        {
            totalColor = totalColor * hit.getMaterial()->t(hit.texCoord[0], hit.texCoord[1]);
        } else
        {
            totalColor = totalColor * hit.getMaterial()->getDiffuseColor();
        }
        for (int k=0; k < numLights; k++)
        {
            Light* light = m_scene->getLight(k);
            Vector3f dirToLight, col;
            float distToLight;
            light->getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, col, distToLight);

          // shadows
            Ray shadowRay = Ray(ray.pointAtParameter(hit.getT()), dirToLight);
            Hit shadowHit = Hit(distToLight, NULL, NULL);
            if (!(g->intersect(shadowRay, shadowHit, tmin + EPSILON)))
            {
                totalColor += hit.getMaterial()->Shade(ray, hit, dirToLight, col);
            }
        }

        Vector3f N = hit.getNormal();
        Vector3f d = ray.getDirection();

        // reflection
        Ray reflectedRay = Ray(ray.pointAtParameter(hit.getT()), mirrorDirection(N, d).normalized()); 
        Hit reflectedHit = Hit(); 
        Vector3f reflectedColor = traceRay(reflectedRay, tmin, bounces + 1, refr_index, reflectedHit) * hit.getMaterial()->getSpecularColor();
        //totalColor += reflectedColor;
        
        // refraction
        if (hit.getMaterial()->getRefractionIndex() > 0)
        {
            bool goingIn = Vector3f::dot(N, d) < 0;
            float index_nt = goingIn ? hit.getMaterial()->getRefractionIndex() : 1.0f;
            Vector3f refractedDirection;
            bool hasRefract;
            float R;
            if (!goingIn)
            {
                N = -N;
            }
            hasRefract = transmittedDirection(N, d, refr_index, index_nt, refractedDirection);
            R = getSchlickWeight(d, refractedDirection, N, refr_index, index_nt);
            if (hasRefract)
            {
                Hit refractedHit = Hit();
                Ray refractedRay = Ray(ray.pointAtParameter(hit.getT()), refractedDirection.normalized());
                Vector3f refractedColor = traceRay(refractedRay, tmin, bounces + 1, index_nt, refractedHit) * hit.getMaterial()->getSpecularColor();
                totalColor += R * reflectedColor + (1-R) * refractedColor;
            } else 
            {
                totalColor += reflectedColor;
            }
        } else
        {
            totalColor += reflectedColor;
        }
        
        return totalColor;

    } else 
    {
        return m_scene->getBackgroundColor(ray.getDirection());
    }
}
