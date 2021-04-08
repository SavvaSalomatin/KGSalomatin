#include "Geometry.h"


bool Plane::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const
{
  surf.t = dot((point - ray.o), normal) / dot(ray.d, normal);

  if(surf.t > t_min && surf.t < t_max)
  {
    surf.hit      = true;
    surf.hitPoint = ray.o + surf.t * ray.d;
    surf.normal   = normal;
    surf.m_ptr    = m_ptr;
    return true;
  }

  return false;
}


bool Sphere::Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const
{
    float3 k = ray.o - center;

    float a = dot(ray.d, ray.d);
    float b = dot(2 * k, ray.d);
    float c = dot(k, k) - pow(r,2);

    float d = b * b - 4 * a * c;

    if (d < 0) return false;

    surf.t = (-b - sqrt(d)) / 2 * a;

    if (surf.t > t_min && surf.t < t_max)
    {
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normalize(surf.hitPoint - center);
        surf.m_ptr = m_ptr;
        return true;
    }

    return false;
}


bool Triangle::Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const  //проверка пересечения луча с поверхностью треугольника
{
    float3 T = ray.o - A;
    float3 E1 = B - A;
    float3 E2 = C - A;
    float3 Q = cross(T, E1);
    float3 W = cross(ray.d, E2);
    float det = dot(E1, W);

    if (det < t_min && det > t_max)      return false;

    float u = dot(T, W) / det; //барицентрические координаты (u, v)
    float v = dot(ray.d, Q) / det;

    if (u < 0 || v < 0 || u + v > 1)     return false; // проверяем, находится ли точка внутри треугольника

    surf.t = dot(E2, Q) / det;
    if (surf.t > t_min && surf.t < t_max) {
        surf.hit = true;
        surf.hitPoint = float3(surf.t, u, v);
        surf.normal = cross((B - A), (C - A));
        surf.m_ptr = m_ptr;
        return true;
    }
    return false;
}

bool Square::Intersect(const Ray& ray, float t_min, float t_max, SurfHit& surf) const
{
    //пересечение с плоскостью
    if (Plane::Intersect(ray, t_min, t_max, surf))
    {
        //проверяем, принадлежит ли квадрату
        float u, v;
        u = (surf.hitPoint.x - point.x) / len;
        v = (surf.hitPoint.y - point.y) / len;
        return !(u < 0 || v < 0 || u > 1 || v > 1);
    }
    else  return false;
}


bool Parallelepiped::Intersect(const Ray& ray, float tmin, float tmax, SurfHit& surf) const //ориентирован по осям
{
    //Пересечение луча с одной из прямой - подстановка уравнения луча в уравнение этой прямой

    //Направление луча по каждой оси
    float dirX = 1.0f / ray.d.x;
    float dirY = 1.0f / ray.d.y;
    float dirZ = 1.0f / ray.d.z;

    //Для каждой прямой по осям
    float x1 = (t_min.x - ray.o.x) * dirX;
    float x2 = (t_max.x - ray.o.x) * dirX;
    float y1 = (t_min.y - ray.o.y) * dirY;
    float y2 = (t_max.y - ray.o.y) * dirY;
    float z1 = (t_min.z - ray.o.z) * dirZ;
    float z2 = (t_max.z - ray.o.z) * dirZ;

    //Проверяем, лежит ли точка на параллепипеде    
    float tMin = max(max(min(x1, x2), min(y1, y2)), min(z1, z2));
    float tMax = min(min(max(x1, x2), max(y1, y2)), max(z1, z2));


    if (tMin <= tMax && tMax > 0 && tMin > tmin && tMin < tmax) {
        surf.t = tMax-tMin;
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normalize(surf.hitPoint);
        surf.m_ptr = m_ptr;
        return true;
    }
    return false;
}

