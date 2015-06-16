#include "stdafx.h"
#include "AbstractVGRenderer.h"

const VGPen &AbstractVGRenderer::GetPen() const
{
    return m_pen;
}

const VGBrush &AbstractVGRenderer::GetBrush() const
{
    return m_brush;
}

void AbstractVGRenderer::SetPen(const VGPen &pen)
{
    m_pen = pen;
}

void AbstractVGRenderer::SetBrush(const VGBrush &brush)
{
    m_brush = brush;
}

AbstractVGRenderer::AbstractVGRenderer()
{
}

VGPen::VGPen(const vec4 &rgba, float width)
    : color(rgba)
    , width(width)
{
}

VGPen::VGPen(const vec3 &rgb, float width)
    : color(rgb, 1.f)
    , width(width)
{
}

VGBrush::VGBrush(const vec4 &color, Style style)
    : color(color)
    , style(style)
{
}

VGBrush::VGBrush(const vec3 &color, Style style)
    : color(color, 1.f)
    , style(style)
{
}

VGBrush::VGBrush(Style style)
    : style(style)
{
}
