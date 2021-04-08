#include <iostream>
#include <vector>
#include <random>

#include "LiteMath.h"
#include "Geometry.h"
#include "Camera.h"
#include "LightSource.h"

using namespace HydraLiteMath;

struct Scene {
    std::vector<std::shared_ptr<GeoObject>> geoObjects;
    std::vector<std::shared_ptr<LightSource>> lights;
};

void RenderScene(uint32_t w, uint32_t h, uint32_t num_samples, const Scene& scene, const Camera& cam, const std::string& filename)
{
    auto  background_color = float3(0.1f, 0.1f, 0.1f);
    auto  film = std::make_unique<Film>(w, h, num_samples);
    auto  tracer = std::make_unique<WhittedRT>(background_color);
    float invWidth = 1.0f / float(w);
    float invHeight = 1.0f / float(h);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float3 pixel_color = float3(0.0f, 0.0f, 0.0f);

            for (int s = 0; s < num_samples; ++s)
            {
                Ray ray = cam.genRay(w, h, x, h - y); //генерируем луч из камеры через текущий пиксель
                pixel_color += tracer->TraceRay(ray, scene.geoObjects, scene.lights); //трассируем луч и получаем цвет
            }
            pixel_color /= film->num_samples;      // усредняем полученные цвета
            pixel_color *= cam.getExposureTime();  // умножаем на время экспонирования сенсора - выдержка виртуальной камеры
            film->SetPixelColor(x, y, pixel_color); //записываем цвет на виртуальный сенсор
        }
    }
    film->SaveImagePPM(filename); //сохраняем картинку
}

void create_scene()
{
    Scene myScene;
    float3        eye(0.0f, 2.0f, 20.0f);
    float3        lookat(0.0f, 2.0f, 0.0f);
    float3        up(0.0f, 1.0f, 0.0f);
    float         field_of_view = 90.0f;
    unsigned int  w = 1920;
    unsigned int  h = 1080;
    Camera        cam(eye, lookat, up, field_of_view, float(w) / float(h));

    auto plane1 = std::make_shared<Plane>(float3(+0.0f, -1.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new Diffuse(float3(0.3f, 0.3f, 0.3f)));
    myScene.geoObjects.push_back(plane1);
   auto Sfera1 = std::make_shared<Sphere>(float3(-5.0f, 6.0f, 3.0f), 2, new Gloss(float3(0.10f, 0.25f, 1.00f)));
    myScene.geoObjects.push_back(Sfera1);
    //auto Sfera2 = std::make_shared<Sphere>(float3(-2.0f, 6.0f, 2.0f), 1, new IdealMirror(float3(1.0f, 1.00f, 1.0f)));
    //myScene.geoObjects.push_back(Sfera2);
    auto Sfera3 = std::make_shared<Sphere>(float3(0.0f, 5.0f, 5.0f), 1, new Glass(float3(0.8f, 1.0f, 1.0f) , float(2.0f)));
    myScene.geoObjects.push_back(Sfera3);
   // auto triangle1 = std::make_shared<Triangle>(float3(10.0f, 3.0f, 1.0f), float3(5.0f, 5.0f, 5.0f), float3(0.0f, 2.0f, 3.0f), new Gloss(float3(0.50f, 1.0f, 0.10f)));
   // myScene.geoObjects.push_back(triangle1);
    auto sq1 = std::make_shared<Square>(float3(0.0f, 4.0f, -6.0f), float (5.0f), new Gloss(float3(1.0f, 0.0f, 0.0f)));
    myScene.geoObjects.push_back(sq1);
    auto pp1 = std::make_shared<Parallelepiped>(float3(7.0f, 2.0f, 8.0f), float3(2.0f, 8.0f, 7.0f), new Glass(float3(1.0f, 1.0f, 1.0f), float(2.0f)));
    myScene.geoObjects.push_back(pp1);


    // освещение
    auto light1 = std::make_shared<LightSource>(float3(-10.0f, 20.0f, 30.0f), float3(1.0f, 1.0f, 1.0f), float(3.0f));
    auto light2 = std::make_shared<LightSource>(float3(10.0f, 10.0f, 20.0f), float3(1.0f, 0.1f, 0.1f), float(1.0f));
    myScene.lights.push_back(light1);
    myScene.lights.push_back(light2);
    RenderScene(w, h, 1, myScene, cam, "basic_scene");
}

int main()
{
    create_scene();

    return 0;
}
