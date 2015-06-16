#include "stdafx.h"
#include "GdiplusRenderer.h"
#include "GdiplusBackBuffer.h"

CGdiplusRenderer::CGdiplusRenderer()
    : m_apiPen(Gdiplus::Color())
    , m_apiBrush(new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0)))
    , m_backBuffer(new CGdiplusBackBuffer)
{
}

void CGdiplusRenderer::StartFrameRender()
{
    m_backGraphics = m_backBuffer->StartRender();
    m_backGraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
}

void CGdiplusRenderer::EndFrameRender(HDC deviceContext)
{
    m_backGraphics.reset();
    std::unique_ptr<Gdiplus::Graphics> mainGraphics{ Gdiplus::Graphics::FromHDC(deviceContext) };
    mainGraphics->DrawImage(m_backBuffer->GetBitmap(), 0, 0);
    mainGraphics.reset();
}

void CGdiplusRenderer::OnWindowResize(int width, int height)
{
    m_backBuffer->SetSize(Gdiplus::Size(width, height));
}

CGdiplusRenderer::~CGdiplusRenderer()
{
}

void CGdiplusRenderer::SetPen(const VGPen &pen)
{
    AbstractVGRenderer::SetPen(pen);
    m_apiPen.SetColor(ToGdiplus(pen.color));
    m_apiPen.SetWidth(pen.width);
}

void CGdiplusRenderer::SetBrush(const VGBrush &brush)
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

void CGdiplusRenderer::DrawRectangle(const rectangle &bounds) const
{
    if (m_apiBrush)
        m_backGraphics->FillRectangle(m_apiBrush.get(), ToGdiplus(bounds));
    if (GetPen().width)
        m_backGraphics->DrawRectangle(&m_apiPen, ToGdiplus(bounds));
}

void CGdiplusRenderer::DrawPolygon(const std::vector<vec2> &points) const
{
    std::vector<Gdiplus::PointF> apiPoints(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        apiPoints[i] = ToGdiplus(points[i]);

    if (m_apiBrush)
        m_backGraphics->FillPolygon(m_apiBrush.get(), apiPoints.data(), apiPoints.size());
    if (GetPen().width)
        m_backGraphics->DrawPolygon(&m_apiPen, apiPoints.data(), apiPoints.size());
}

void CGdiplusRenderer::DrawEllipse(const rectangle &bounds) const
{
    if (m_apiBrush)
        m_backGraphics->FillEllipse(m_apiBrush.get(), ToGdiplus(bounds));
    if (GetPen().width)
        m_backGraphics->DrawEllipse(&m_apiPen, ToGdiplus(bounds));
}

Gdiplus::Color CGdiplusRenderer::ToGdiplus(const vec4 &color) const
{
    return Gdiplus::Color(int8_t(255 * color.w), int8_t(255 * color.x), int8_t(255 * color.y), int8_t(255 * color.z));
}

Gdiplus::RectF CGdiplusRenderer::ToGdiplus(const rectangle &rect) const
{
    return Gdiplus::RectF(rect.x, rect.y, rect.width, rect.height);
}

Gdiplus::PointF CGdiplusRenderer::ToGdiplus(const vec2 &point) const
{
    return Gdiplus::PointF(point.x, point.y);
}
