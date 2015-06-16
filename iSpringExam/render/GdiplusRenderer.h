#pragma once
#include "AbstractVGRenderer.h"
#include <gdiplus.h>
#include <memory>

class GdiplusRenderer : public AbstractVGRenderer
{
public:
    explicit GdiplusRenderer(Gdiplus::Graphics &graphics);
    ~GdiplusRenderer();

    void SetPen(const VGPen &pen) override;
    void SetBrush(const VGBrush &brush) override;
    void DrawRectangle(const rectangle &bounds) const override;
    void DrawPolygon(const std::vector<vec2> &points) const override;
    void DrawEllipse(const rectangle &bounds) const override;

private:
    inline Gdiplus::Color ToGdiplus(const vec4 &color) const;
    inline Gdiplus::RectF ToGdiplus(const rectangle &rect) const;
    inline Gdiplus::PointF ToGdiplus(const vec2 &point) const;

    Gdiplus::Pen m_apiPen;
    std::unique_ptr<Gdiplus::Brush> m_apiBrush;
    Gdiplus::Graphics &m_apiGraphics;
};
