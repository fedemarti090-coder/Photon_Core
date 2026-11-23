#include <iostream>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Point
{
    float x, y, z;
};

struct RGBa
{
    float R, G, B, a;
};

class ObjRender
{
private:
    static void pushQuad(
        std::vector<Triangle> &out,
        const Point &p0,
        const Point &p1,
        const Point &p2,
        const Point &p3,
        const RGBa &color)
    {
        out.push_back(Triangle{
            {p0.x, p0.y, p0.z, 1.0f},
            {p1.x, p1.y, p1.z, 1.0f},
            {p2.x, p2.y, p2.z, 1.0f},
            {color.R, color.G, color.B, color.a}});

        out.push_back(Triangle{
            {p0.x, p0.y, p0.z, 1.0f},
            {p2.x, p2.y, p2.z, 1.0f},
            {p3.x, p3.y, p3.z, 1.0f},
            {color.R, color.G, color.B, color.a}});
    }

public:
    std::vector<Triangle> Parallelpiped(Point Origin, float W, float H, float D, RGBa color)
    {
        std::vector<Triangle> tris;
        tris.reserve(12);

        float x0 = Origin.x;
        float y0 = Origin.y;
        float z0 = Origin.z;
        float x1 = Origin.x + W;
        float y1 = Origin.y + H;
        float z1 = Origin.z + D;

        Point p000{x0, y0, z0};
        Point p100{x1, y0, z0};
        Point p110{x1, y1, z0};
        Point p010{x0, y1, z0};

        Point p001{x0, y0, z1};
        Point p101{x1, y0, z1};
        Point p111{x1, y1, z1};
        Point p011{x0, y1, z1};

        pushQuad(tris, p001, p101, p111, p011, color);
        pushQuad(tris, p100, p000, p010, p110, color);
        pushQuad(tris, p101, p100, p110, p111, color);
        pushQuad(tris, p000, p001, p011, p010, color);
        pushQuad(tris, p011, p111, p110, p010, color);
        pushQuad(tris, p000, p100, p101, p001, color);

        return tris;
    }

    std::vector<Triangle> Sphere(Point origin, float r, RGBa color, int stacks = 16, int sectors = 24)
    {
        std::vector<Triangle> tris;
        if (stacks < 2 || sectors < 3)
            return tris;

        tris.reserve(static_cast<size_t>(stacks * sectors * 2));

        std::vector<std::vector<Point>> grid(
            static_cast<size_t>(stacks + 1),
            std::vector<Point>(static_cast<size_t>(sectors + 1)));

        for (int i = 0; i <= stacks; ++i)
        {
            float v = static_cast<float>(i) / static_cast<float>(stacks);
            float phi = v * static_cast<float>(M_PI); // 0 (polo +Y) -> PI (polo -Y)
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (int j = 0; j <= sectors; ++j)
            {
                float u = static_cast<float>(j) / static_cast<float>(sectors);
                float theta = u * 2.0f * static_cast<float>(M_PI);
                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                grid[static_cast<size_t>(i)][static_cast<size_t>(j)] = Point{
                    origin.x + r * sinPhi * cosTheta,
                    origin.y + r * cosPhi,
                    origin.z + r * sinPhi * sinTheta};
            }
        }

        for (int i = 0; i < stacks; ++i)
        {
            for (int j = 0; j < sectors; ++j)
            {
                const Point &a = grid[static_cast<size_t>(i)][static_cast<size_t>(j)];
                const Point &b = grid[static_cast<size_t>(i + 1)][static_cast<size_t>(j)];
                const Point &c = grid[static_cast<size_t>(i + 1)][static_cast<size_t>(j + 1)];
                const Point &d = grid[static_cast<size_t>(i)][static_cast<size_t>(j + 1)];

                // Calotta superiore (i == 0): a e d coincidono sul polo +Y -> 1 triangolo (a, b, c)
                if (i == 0)
                {
                    tris.push_back(Triangle{
                        {a.x, a.y, a.z, 1.0f},
                        {b.x, b.y, b.z, 1.0f},
                        {c.x, c.y, c.z, 1.0f},
                        {color.R, color.G, color.B, color.a}});
                }
                // Calotta inferiore (i == stacks - 1): b e c coincidono sul polo -Y -> 1 triangolo (a, b, d)
                else if (i == stacks - 1)
                {
                    tris.push_back(Triangle{
                        {a.x, a.y, a.z, 1.0f},
                        {b.x, b.y, b.z, 1.0f},
                        {d.x, d.y, d.z, 1.0f},
                        {color.R, color.G, color.B, color.a}});
                }
                // Fasce intermedie: quad diviso in 2 triangoli
                else
                {
                    tris.push_back(Triangle{
                        {a.x, a.y, a.z, 1.0f},
                        {b.x, b.y, b.z, 1.0f},
                        {c.x, c.y, c.z, 1.0f},
                        {color.R, color.G, color.B, color.a}});

                    tris.push_back(Triangle{
                        {a.x, a.y, a.z, 1.0f},
                        {c.x, c.y, c.z, 1.0f},
                        {d.x, d.y, d.z, 1.0f},
                        {color.R, color.G, color.B, color.a}});
                }
            }
        }

        return tris;
    }

    std::vector<Triangle> Rect(Point origin, float W, float D, RGBa color)
    {
        std::vector<Triangle> tris;
        tris.reserve(2);

        Point p0{origin.x, origin.y, origin.z};
        Point p1{origin.x + W, origin.y, origin.z};
        Point p2{origin.x + W, origin.y, origin.z + D};
        Point p3{origin.x, origin.y, origin.z + D};

        pushQuad(tris, p0, p3, p2, p1, color);
        return tris;
    }

    std::vector<Triangle> Append(std::vector<Triangle> &mainVec, const std::vector<Triangle> &newObj)
    {
        mainVec.reserve(mainVec.size() + newObj.size());
        mainVec.insert(mainVec.end(), newObj.begin(), newObj.end());
        return mainVec;
    }
};