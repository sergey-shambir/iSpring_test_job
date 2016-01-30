#include "stdafx.h"
#include "Direct2DRenderer.h"

Direct2DRenderer::Direct2DRenderer(CWindow window)
    : m_window(window)
{
    // TODO: check HRESULT
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2Factory.p);
}

Direct2DRenderer::~Direct2DRenderer()
{
}

void Direct2DRenderer::StartFrameRender()
{
    m_d2Result = CreateDeviceResources();
    if (SUCCEEDED(m_d2Result)) {
        m_d2RenderTarget->BeginDraw();
        m_d2RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_d2RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    }
}

void Direct2DRenderer::EndFrameRender()
{
    if (SUCCEEDED(m_d2Result))
        m_d2Result = m_d2RenderTarget->EndDraw();
    if (m_d2Result == D2DERR_RECREATE_TARGET)
        DiscardDeviceResources();
}

void Direct2DRenderer::OnWindowResize(int width, int height)
{
    if (m_d2RenderTarget)
        m_d2RenderTarget->Resize(D2D1::SizeU(width, height));
}

void Direct2DRenderer::SetPen(const VGPen &pen)
{
    AbstractVGRenderer::SetPen(pen);
    if (m_d2RenderTarget)
        m_d2RenderTarget->CreateSolidColorBrush(ToD2Color(pen.color), &m_d2StrokeBrush.p);
}

void Direct2DRenderer::SetBrush(const VGBrush &brush)
{
    AbstractVGRenderer::SetBrush(brush);
    if (m_d2RenderTarget)
        m_d2RenderTarget->CreateSolidColorBrush(ToD2Color(brush.color), &m_d2FillBrush.p);
}

void Direct2DRenderer::DrawRectangle(const rectangle &bounds) const
{
    if (!SUCCEEDED(m_d2Result) || !m_d2FillBrush && !m_d2StrokeBrush.p)
        return;

    auto d2Bounds = ToD2Rect(bounds);
    if (m_d2FillBrush)
        m_d2RenderTarget->FillRectangle(d2Bounds, m_d2FillBrush);
    if (m_d2StrokeBrush)
        m_d2RenderTarget->DrawRectangle(d2Bounds, m_d2StrokeBrush, GetPen().width);
}

void Direct2DRenderer::DrawPolygon(const std::vector<vec2> &points) const
{
    if (!SUCCEEDED(m_d2Result) || !m_d2FillBrush && !m_d2StrokeBrush || points.empty())
        return;

    CComPtr<ID2D1PathGeometry> path;
    m_d2Factory->CreatePathGeometry(&path.p);
    InitPath(path, points);
    if (m_d2FillBrush)
        m_d2RenderTarget->FillGeometry(path, m_d2FillBrush);
    if (m_d2StrokeBrush)
        m_d2RenderTarget->DrawGeometry(path, m_d2StrokeBrush, GetPen().width);
}

void Direct2DRenderer::DrawEllipse(const rectangle &bounds) const
{
    if (!SUCCEEDED(m_d2Result) || !m_d2FillBrush && !m_d2StrokeBrush)
        return;

    auto d2Ellipse = D2D1::Ellipse(ToD2Point(bounds.origin() + 0.5f * bounds.size()),
        0.5f * bounds.width,
        0.5f * bounds.height);
    if (m_d2FillBrush)
        m_d2RenderTarget->FillEllipse(d2Ellipse, m_d2FillBrush);
    if (m_d2StrokeBrush)
        m_d2RenderTarget->DrawEllipse(d2Ellipse, m_d2StrokeBrush, GetPen().width);
}

HRESULT Direct2DRenderer::CreateDeviceResources()
{
    HRESULT hr = S_OK;
    if (!m_d2RenderTarget) {
        RECT rect;
        ::GetClientRect(m_window, &rect);
        D2D1_SIZE_U size = D2D1::SizeU(
            rect.right - rect.left,
            rect.bottom - rect.top
            );
        // Create a Direct2D render target.
        hr = m_d2Factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_window, size),
            &m_d2RenderTarget.p
            );
    }
    return hr;
}

void Direct2DRenderer::DiscardDeviceResources()
{
    m_d2RenderTarget.Release();
    m_d2StrokeBrush.Release();
    m_d2FillBrush.Release();
}

D2D1_RECT_F Direct2DRenderer::ToD2Rect(const rectangle &rect) const
{
    return D2D1::RectF(rect.left(), rect.top(), rect.right(), rect.bottom());
}

D2D1_POINT_2F Direct2DRenderer::ToD2Point(const vec2 &point) const
{
    return D2D1::Point2F(point.x, point.y);
}

D2D1_COLOR_F Direct2DRenderer::ToD2Color(const vec4 &color) const
{
    return D2D1::ColorF(color.x, color.y, color.z, color.w);
}

void Direct2DRenderer::InitPath(ID2D1PathGeometry *path, const std::vector<vec2> &points) const
{
    CComPtr<ID2D1GeometrySink> pSink;
    if (SUCCEEDED(path->Open(&pSink.p)))
    {
        pSink->BeginFigure(ToD2Point(points[0]), D2D1_FIGURE_BEGIN_FILLED);
        for (size_t i = 1; i < points.size(); ++i)
            pSink->AddLine(ToD2Point(points[i]));
        pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
        pSink->Close();
    }
}
