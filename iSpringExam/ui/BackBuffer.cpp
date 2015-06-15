#include "stdafx.h"
#include "BackBuffer.h"

CBackBuffer::CBackBuffer()
{
	/*
	parent.GetClipBox(&m_rect);
	CreateCompatibleDC(parent);
	m_bitmap.CreateCompatibleBitmap(parent, m_rect.Width(), m_rect.Height());
	m_savedBitmap = static_cast<HBITMAP>(::SelectObject(*this, &m_bitmap));
	SetWindowOrg(m_rect.left, m_rect.top);
	*/
}

CBackBuffer::~CBackBuffer()
{
	/*
	m_parent.BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
		*this, m_rect.left, m_rect.top, SRCCOPY);
	SelectObject(*this, m_savedBitmap);
	*/
}

std::unique_ptr<Gdiplus::Graphics> CBackBuffer::StartRender()
{
	if (!m_bitmap) {
		m_bitmap.reset(new Gdiplus::Bitmap(m_size.Width, m_size.Height));
	}
	const Gdiplus::Color WHITE_COLOR(0xff, 0xff, 0xff);
	std::unique_ptr<Gdiplus::Graphics> ret{ Gdiplus::Graphics::FromImage(m_bitmap.get()) };
	ret->Clear(WHITE_COLOR);
	return ret;
}

void CBackBuffer::SetSize(const Gdiplus::Size &size)
{
	if (m_size.Width != size.Width || m_size.Height != size.Height) {
		m_size = size;
		m_bitmap.reset();
	}
}

Gdiplus::Bitmap *CBackBuffer::GetBitmap() const
{
	return m_bitmap.get();
}
