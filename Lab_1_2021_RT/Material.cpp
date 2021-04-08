#include "Material.h"

SurfHit& SurfHit::operator= (const SurfHit &rhs)
{
  if (this == &rhs)
    return (*this);

  hit = rhs.hit;
  hitPoint = rhs.hitPoint;
  normal = rhs.normal;
  m_ptr = rhs.m_ptr;
  t = rhs.t;

  return (*this);
}


float3 reflect(const float3& v, const float3& normal)
{
    return v - 2 * dot(v, normal) * normal;
}

float3 prelom(const float3& v, const float3& normal,  float& prelomcoef)
{
    float3 N = normal;
    float cosi = dot(v, normal) / (length(v) * length(normal));
    float etai = 1;
    if (cosi <= 0) {
        cosi = -cosi;
        float buf = etai;
        etai = prelomcoef;
        prelomcoef = buf;
        N = -normal;
    }
    float eta = etai / prelomcoef;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? float3(0, 0, 0) : v * eta + N * (eta * cosi - sqrtf(k));
}


bool IdealMirror::Scatter(const Ray &ray_in, const SurfHit &surf, float3 &attenuation, Ray &ray_out)
{
    float3 reflection_dir = reflect(normalize(ray_in.d), surf.normal); 

    ray_out = Ray(surf.hitPoint + surf.normal * 1e-4, reflection_dir); 

    attenuation = color;

    return  (dot(ray_out.d, surf.normal) > 0);
}

bool Glass::Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out)
{
    float3 refraction_dir = prelom(normalize(ray_in.d), surf.normal, prelomcoef);

    ray_out = Ray(surf.hitPoint + surf.normal * 1e-4, refraction_dir);

    attenuation = color;

    return (dot(ray_out.d, surf.normal) < 0);
}

bool Diffuse::Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out)
{
    float coef = dot(normalize(ray_in.d), surf.normal); //Цвет поверхности пропорционален косинусу угла между нормалью к поверхности и направлением на источник света
    attenuation = color * max(coef, 0.0f); //коэффициент диффузного отражения поверхности
    return false;
}

bool Gloss::Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out)
{
    float coef = dot(normalize(surf.normal), ray_in.d); //Цвет поверхности пропорционален косинусу угла между нормалью к поверхности и направлением на источник света
    attenuation = color * max(coef, 0.0f); //коэффициент диффузного отражения поверхности
    return false;
}

bool Light::Scatter(const Ray& ray_in, const SurfHit& surf, float3& attenuation, Ray& ray_out)
{
   /* float coef = dot(normalize(surf.normal), ray_out.d);*/
    attenuation = intensity * color * attenuation/** coef*/;
    return true;
}

