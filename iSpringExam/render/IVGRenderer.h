#pragma once
#include "../math/rectangle.h"

class IVGRenderer
{
public:
    virtual ~IVGRenderer() = default;

    virtual void DrawRectangle(const rectangle &bounds) const = 0;
    virtual void DrawEllipse(const rectangle &bounds) const = 0;
    virtual void DrawPolygon(const std::vector<vec2> &points) const = 0;
};
