#include "stdafx.h"
#include "GdiplusRenderer.h"

GdiplusRenderer::GdiplusRenderer(Gdiplus::Graphics &graphics)
    : m_apiGraphics(graphics)
    , m_apiPen(Gdiplus::Color())
    , m_apiBrush(new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0)))
{
}

GdiplusRenderer::~GdiplusRenderer()
{
}

void GdiplusRenderer::SetPen(const VGPen &pen)
{
    AbstractVGRenderer::SetPen(pen);
    m_apiPen.SetColor(ToGdiplus(pen.color));
    m_apiPen.SetWidth(pen.width);
}

void GdiplusRenderer::SetBrush(const VGBrush &brush)
{
    AbstractVGRenderer::SetBrush(brush);
    switch (brush.style) {
    case VGBrush::SolidBrush:
        m_apiBrush.reset(new Gdiplus::SolidBrush(ToGdiplus(brush.color)));
        break;
    case VGBrush::NoBrush:
        m_apiBrush.reset();
        break;
    default:
        break;
    }
}

void GdiplusRenderer::DrawRectangle(const rectangle &bounds) const
{
    if (m_apiBrush)
        m_apiGraphics.FillRectangle(m_apiBrush.get(), ToGdiplus(bounds));
    if (GetPen().width)
        m_apiGraphics.DrawRectangle(&m_apiPen, ToGdiplus(bounds));
}

void GdiplusRenderer::DrawPolygon(const std::vector<vec2> &points) const
{
    std::vector<Gdiplus::PointF> apiPoints(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        apiPoints[i] = ToGdiplus(points[i]);

    if (m_apiBrush)
        m_apiGraphics.FillPolygon(m_apiBrush.get(), apiPoints.data(), apiPoints.size());
    if (GetPen().width)
        m_apiGraphics.DrawPolygon(&m_apiPen, apiPoints.data(), apiPoints.size());
}

void GdiplusRenderer::DrawEllipse(const rectangle &bounds) const
{
    if (m_apiBrush)
        m_apiGraphics.FillEllipse(m_apiBrush.get(), ToGdiplus(bounds));
    if (GetPen().width)
        m_apiGraphics.DrawEllipse(&m_apiPen, ToGdiplus(bounds));
}

Gdiplus::Color GdiplusRenderer::ToGdiplus(const vec4 &color) const
{
    return Gdiplus::Color(int8_t(255 * color.w), int8_t(255 * color.x), int8_t(255 * color.y), int8_t(255 * color.z));
}

Gdiplus::RectF GdiplusRenderer::ToGdiplus(const rectangle &rect) const
{
    return Gdiplus::RectF(rect.x, rect.y, rect.width, rect.height);
}

Gdiplus::PointF GdiplusRenderer::ToGdiplus(const vec2 &point) const
{
    return Gdiplus::PointF(point.x, point.y);
}
