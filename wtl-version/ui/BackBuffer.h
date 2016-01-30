#pragma once
#include <gdiplus.h>

class CBackBuffer
{
public:
	CBackBuffer();
	~CBackBuffer();
	
	std::unique_ptr<Gdiplus::Graphics> StartRender();
	void SetSize(const Gdiplus::Size &size);
	Gdiplus::Bitmap *GetBitmap() const;

private:
	Gdiplus::Size m_size;
	std::unique_ptr<Gdiplus::Bitmap> m_bitmap;
};

