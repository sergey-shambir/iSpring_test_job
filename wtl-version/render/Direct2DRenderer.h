#pragma once
#include "AbstractVGRenderer.h"
#include <d2d1.h>
#include <d2d1helper.h>

class Direct2DRenderer : public AbstractVGRenderer
{
public:
    Direct2DRenderer(CWindow window);
    ~Direct2DRenderer();

    void StartFrameRender();
    void EndFrameRender();
    void OnWindowResize(int width, int height);

    void SetPen(const VGPen &pen) override;
    void SetBrush(const VGBrush &brush) override;
    void DrawRectangle(const rectangle &bounds) const override;
    void DrawPolygon(const std::vector<vec2> &points) const override;
    void DrawEllipse(const rectangle &bounds) const override;

private:
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();
    
    D2D1_RECT_F ToD2Rect(const rectangle &rect) const;
    D2D1_POINT_2F ToD2Point(const vec2 &point) const;
    D2D1_COLOR_F ToD2Color(const vec4 &color) const;
    void InitPath(ID2D1PathGeometry *path, const std::vector<vec2> &points) const;

    CWindow m_window;
    CComPtr<ID2D1Factory> m_d2Factory = nullptr;
    CComPtr<ID2D1HwndRenderTarget> m_d2RenderTarget = nullptr;
    CComPtr<ID2D1SolidColorBrush> m_d2StrokeBrush = nullptr;
    CComPtr<ID2D1SolidColorBrush> m_d2FillBrush = nullptr;
    CComPtr<ID2D1LinearGradientBrush> m_pLinearGradientBrush = nullptr;
    HRESULT m_d2Result = S_OK;
};
