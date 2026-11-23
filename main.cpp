#include <iostream>
#include <vector>

#include "src/windowGPU.hpp"
#include "src/objRender.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    WinINIT app;
    ObjRender render;

    std::vector<Triangle> scene;

    auto floor = render.Rect(
        Point{-5.0f, -1.2f, -5.0f},
        10.0f,
        7.0f,
        RGBa{0.35f, 0.36f, 0.42f, 1.0f});

    // 2. Sfera a sinistra (appoggiata sul piano: y = -1.2 + r = -0.4)
    auto sphere = render.Sphere(
        Point{-1.3f, -0.4f, 0.0f},
        0.8f,
        RGBa{0.20f, 0.75f, 1.00f, 1.0f});

    // 3. Rettangolo a destra (su XZ a y = -0.5, spostato su +X)
    auto box = render.Parallelpiped(
        Point{0.7f, -1.2f, -0.6f},
        1.5f,
        1.1f,
        1.2f,
        RGBa{1.00f, 0.42f, 0.00f, 1.0f});

    render.Append(scene, floor);
    render.Append(scene, sphere);
    render.Append(scene, box);

    app.setTriangles(scene);
    return app.Run(hInstance, nCmdShow);

    return 0;
}