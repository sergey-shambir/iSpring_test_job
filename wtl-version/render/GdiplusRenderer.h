#pragma once
#include "AbstractVGRenderer.h"
#include <gdiplus.h>
#include <memory>
#include <vector>
#include <gdiplus.h>

class CGdiplusBackBuffer;

class CGdiplusRenderer : public AbstractVGRenderer
{
public:
    explicit CGdiplusRenderer();
    ~CGdiplusRenderer();

    void StartFrameRender();
    void EndFrameRender(HDC deviceContext);
    void OnWindowResize(int width, int height);

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
    std::unique_ptr<Gdiplus::Graphics> m_backGraphics;
    std::unique_ptr<CGdiplusBackBuffer> m_backBuffer;
};
