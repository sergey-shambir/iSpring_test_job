#pragma once
#include "AbstractVGRenderer.h"
#include <d2d1.h>
#include <d2d1helper.h>

class Direct2DRenderer : public AbstractVGRenderer
{
public:
    Direct2DRenderer();
    ~Direct2DRenderer();

    void StartFrameRender();
    void EndFrameRender(HDC deviceContext);
    void OnWindowResize(int width, int height);

#if 0
    void SetPen(const VGPen &pen) override;
    void SetBrush(const VGBrush &brush) override;
    void DrawRectangle(const rectangle &bounds) const override;
    void DrawPolygon(const std::vector<vec2> &points) const override;
    void DrawEllipse(const rectangle &bounds) const override;
#endif

private:
};
