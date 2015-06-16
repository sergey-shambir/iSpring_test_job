#pragma once
#include "IVGRenderer.h"
#include "../math/vec4.h"

class VGPen
{
public:
    vec4 color;
    float width = 1.f;

    VGPen() = default;
    explicit VGPen(const vec4 &rgba, float width = 1.f);
    explicit VGPen(const vec3 &rgb, float width = 1.f);
};

class VGBrush
{
public:
    enum Style {
        NoBrush,
        SolidBrush
    };

    vec4 color;
    Style style;

    VGBrush() = default;
    explicit VGBrush(const vec4 &color, Style style = SolidBrush);
    explicit VGBrush(const vec3 &color, Style style = SolidBrush);
    explicit VGBrush(Style style);
};

class AbstractVGRenderer : public IVGRenderer
{
public:
    const VGPen &GetPen() const;
    const VGBrush &GetBrush() const;
    virtual void SetPen(const VGPen &pen);
    virtual void SetBrush(const VGBrush &brush);

protected:
    AbstractVGRenderer();

private:
    VGPen m_pen;
    VGBrush m_brush;
};
