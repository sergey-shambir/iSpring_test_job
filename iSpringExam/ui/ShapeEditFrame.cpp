#include "stdafx.h"
#include "ShapeEditFrame.h"

namespace {
	const float PEN_WIDTH = 2;
	const float CORNER_RADIUS = 2;
	const Gdiplus::Color PEN_COLOR(0x40, 0xa0, 0xff);
} // anonymous namespace

ShapeEditFrame::ShapeEditFrame()
{
}

ShapeEditFrame::~ShapeEditFrame()
{
	for (auto pair : m_cursors) {
		CCursorHandle cursor(pair.second);
		cursor.DestroyCursor();
	}
}

const rectangle &ShapeEditFrame::GetBounds() const
{
	return m_bounds;
}

bool ShapeEditFrame::IsVisible() const
{
	return m_isVisible;
}

bool ShapeEditFrame::IsInDragState() const
{
	return m_isInDragState;
}

void ShapeEditFrame::SetBounds(const rectangle &bounds)
{
	m_bounds = bounds;
}

void ShapeEditFrame::SetVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

void ShapeEditFrame::Render(Gdiplus::Graphics &graphics)
{
	if (!m_isVisible)
		return;

	Gdiplus::Pen framePen(PEN_COLOR, float(PEN_WIDTH));
	const rectangle &br = m_bounds;
	graphics.DrawRectangle(&framePen, br.x, br.y, br.width, br.height);
	auto drawCorner = [&](float x, float y) {
		graphics.DrawEllipse(&framePen, x - CORNER_RADIUS, y - CORNER_RADIUS,
			2 * CORNER_RADIUS, 2 * CORNER_RADIUS);
	};
	drawCorner(br.x, br.y);
	drawCorner(br.x + br.width, br.y);
	drawCorner(br.x, br.y + br.height);
	drawCorner(br.x + br.width, br.y + br.height);
}

bool ShapeEditFrame::StartDrag(int x, int y)
{
	UpdateCurrentZone(x, y);
	if (Outside == m_currentZone)
		return false;
	m_isInDragState = true;
	m_lastX = x;
	m_lastY = y;
	return true;
}

void ShapeEditFrame::FinishDrag()
{
	m_isInDragState = false;
}

void ShapeEditFrame::UpdateMousePos(int x, int y)
{
	if (!m_isVisible)
		return;
	if (m_isInDragState) {
		ApplyDragChanges(x, y);
	} else {
		UpdateCurrentZone(x, y);
	}
}

void ShapeEditFrame::ForceZoneOutside()
{
	m_currentZone = Outside;
}

void ShapeEditFrame::ShowInRect(const rectangle &bounds)
{
	m_isVisible = true;
	SetBounds(bounds);
}

void ShapeEditFrame::Hide()
{
	ForceZoneOutside();
	SetBounds(rectangle(0, 0, 0, 0));
	m_isVisible = false;
}

void ShapeEditFrame::ApplyDragChanges(int x, int y)
{
	rectangle br = GetBounds();
	const int dx = x - m_lastX;
	const int dy = y - m_lastY;
	m_lastX = x;
	m_lastY = y;
	switch (m_currentZone) {
	case Inside:
		br.x += dx;
		br.y += dy;
		break;
	case LeftEdge:
		br.x += dx;
		br.width -= dx;
		break;
	case RightEdge:
		br.width += dx;
		break;
	case TopEdge:
		br.y += dy;
		br.height -= dy;
		break;
	case BottomEdge:
		br.height += dy;
		break;
	case LeftTopCorner:
		br.x += dx;
		br.width -= dx;
		br.y += dy;
		br.height -= dy;
		break;
	case RightBottomCorner:
		br.width += dx;
		br.height += dy;
		break;
	case LeftBottomCorner:
		br.x += dx;
		br.width -= dx;
		br.height += dy;
		break;
	case RightTopCorner:
		br.width += dx;
		br.y += dy;
		br.height -= dy;
		break;
	}
	SetBounds(br);
}

void ShapeEditFrame::UpdateCurrentZone(int x, int y)
{
	Zone zone = DetectZone(x, y);
	if (zone != m_currentZone) {
		m_currentZone = zone;
		::SetCursor(LoadCursorForZone(m_currentZone));
	}
}

ShapeEditFrame::Zone ShapeEditFrame::DetectZone(int x, int y) const
{
	auto hitsRect = [&](const vec2 &centerOffset, const vec2 &size) -> bool {
		rectangle rect(m_bounds.origin() + centerOffset - size * 0.5f, size);
		return rect.hits(float(x), float(y));
	};
	const float cornerSize = PEN_WIDTH + 2 * CORNER_RADIUS;
	if (hitsRect(vec2(0, 0), vec2(cornerSize, cornerSize)))
		return LeftTopCorner;
	if (hitsRect(vec2(m_bounds.width, 0), vec2(cornerSize, cornerSize)))
		return RightTopCorner;
	if (hitsRect(vec2(0, m_bounds.height), vec2(cornerSize, cornerSize)))
		return LeftBottomCorner;
	if (hitsRect(vec2(m_bounds.width, m_bounds.height), vec2(cornerSize, cornerSize)))
		return RightBottomCorner;

	if (hitsRect(vec2(0, 0.5f * m_bounds.height), vec2(PEN_WIDTH, m_bounds.height + PEN_WIDTH)))
		return LeftEdge;
	if (hitsRect(vec2(m_bounds.width, 0.5f * m_bounds.height), vec2(PEN_WIDTH, m_bounds.height + PEN_WIDTH)))
		return RightEdge;
	if (hitsRect(vec2(0.5f * m_bounds.width, 0), vec2(m_bounds.width + PEN_WIDTH, PEN_WIDTH)))
		return TopEdge;
	if (hitsRect(vec2(0.5f * m_bounds.width, m_bounds.height), vec2(m_bounds.width + PEN_WIDTH, PEN_WIDTH)))
		return BottomEdge;

	if (m_bounds.hits(float(x), float(y)))
		return Inside;
	return Outside;
}

LPCTSTR ShapeEditFrame::GetCursorNameForZone(ShapeEditFrame::Zone zone) const
{
	switch (zone) {
	case Inside:
		return IDC_HAND;
	case LeftTopCorner:
	case RightBottomCorner:
		return IDC_SIZENWSE;
	case LeftBottomCorner:
	case RightTopCorner:
		return IDC_SIZENESW;
	case LeftEdge:
	case RightEdge:
		return IDC_SIZEWE;
	case TopEdge:
	case BottomEdge:
		return IDC_SIZENS;
	case Outside:
	default:
		return IDC_ARROW;
	}
}

HCURSOR ShapeEditFrame::LoadCursorForZone(ShapeEditFrame::Zone zone)
{
	LPCTSTR cursorName = GetCursorNameForZone(zone);
	auto it = m_cursors.find(cursorName);
	if (it != m_cursors.end()) {
		return it->second;
	}
	CCursorHandle cursor;
	cursor.LoadSysCursor(cursorName);
	m_cursors[cursorName] = cursor;
	return cursor;
}
