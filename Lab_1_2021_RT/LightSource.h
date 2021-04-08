#pragma once

#include "LiteMath.h"
#include "RayTracer.h"

class LightSource {
public:
    explicit LightSource(const float3 a_position, const float3 a_color, const float a_intensity) : position(a_position), color(a_color), intensity(a_intensity) {};
    virtual ~LightSource() = default;

    float3 position;
    float3 color;
    float intensity;
};
