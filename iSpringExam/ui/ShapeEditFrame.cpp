#include "stdafx.h"
#include "ShapeEditFrame.h"
#include "../render/AbstractVGRenderer.h"

namespace {
	const float PEN_WIDTH = 2;
	const float CORNER_RADIUS = 2;
	const vec3 PEN_RGB(0.25f, 0.6f, 1.f);
    const float MIN_SHAPE_SIZE = 40;
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

const rectangle &ShapeEditFrame::GetSceneBounds() const
{
    return m_sceneBounds;
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

void ShapeEditFrame::SetSceneBounds(const rectangle &bounds)
{
    m_sceneBounds = bounds;
}

void ShapeEditFrame::SetVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

void ShapeEditFrame::Render(AbstractVGRenderer &renderer)
{
	if (!m_isVisible)
		return;

    renderer.SetBrush(VGBrush(VGBrush::NoBrush));
    renderer.SetPen(VGPen(PEN_RGB, PEN_WIDTH));
    renderer.DrawRectangle(m_bounds);
	const rectangle &br = m_bounds;
	auto drawCorner = [&](float x, float y) {
        renderer.DrawEllipse(rectangle(x - CORNER_RADIUS, y - CORNER_RADIUS,
            2 * CORNER_RADIUS, 2 * CORNER_RADIUS));
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

    vec2 dragTopLeft(m_sceneBounds.origin());
    vec2 dragBottomRight(m_sceneBounds.size());
    switch (m_currentZone) {
    case Inside:
        dragBottomRight.x -= m_bounds.width;
        dragBottomRight.y -= m_bounds.height;
        break;
    case LeftTopCorner:
    case LeftEdge:
    case TopEdge:
        dragBottomRight.x = m_bounds.x + m_bounds.width - MIN_SHAPE_SIZE;
        dragBottomRight.y = m_bounds.y + m_bounds.height - MIN_SHAPE_SIZE;
        break;
    case RightBottomCorner:
    case RightEdge:
    case BottomEdge:
        dragTopLeft.x = m_bounds.x + MIN_SHAPE_SIZE;
        dragTopLeft.y = m_bounds.y + MIN_SHAPE_SIZE;
        break;
    case LeftBottomCorner:
        dragBottomRight.x = m_bounds.x + m_bounds.width - MIN_SHAPE_SIZE;
        dragTopLeft.y = m_bounds.y + MIN_SHAPE_SIZE;
        break;
    case RightTopCorner:
        dragTopLeft.x = m_bounds.x + MIN_SHAPE_SIZE;
        dragBottomRight.y = m_bounds.y + m_bounds.height - MIN_SHAPE_SIZE;
        break;
    default:
        break;
    }
    m_dragBounds = rectangle(dragTopLeft, dragBottomRight - dragTopLeft);
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
		if (m_cursorNeedsReset) {
			m_cursorNeedsReset = false;
			::SetCursor(LoadCursorForZone(m_currentZone));
		}
		ApplyDragChanges(x, y);
	} else {
		UpdateCurrentZone(x, y);
	}
}

void ShapeEditFrame::DropCustomCursor()
{
	m_cursorNeedsReset = true;
}

void ShapeEditFrame::ShowInRect(const rectangle &bounds)
{
	m_isVisible = true;
	SetBounds(bounds);
}

void ShapeEditFrame::Hide()
{
	m_currentZone = Outside;
	SetBounds(rectangle(0, 0, 0, 0));
	m_isVisible = false;
}

void ShapeEditFrame::ApplyDragChanges(int x, int y)
{
	rectangle br = GetBounds();
	const float dx = float(x - m_lastX);
    const float dy = float(y - m_lastY);
	m_lastX = x;
	m_lastY = y;
    vec2 topLeft = br.origin();
    vec2 bottomRight = topLeft + br.size();
	switch (m_currentZone) {
    case Inside:
        topLeft = m_dragBounds.clampToBounds(br.origin() + vec2(dx, dy));
        bottomRight = topLeft + br.size();
		break;
    case LeftEdge:
        topLeft.x = m_dragBounds.clampXToBounds(topLeft.x + dx);
		break;
	case RightEdge:
        bottomRight.x = m_dragBounds.clampXToBounds(bottomRight.x + dx);
		break;
    case TopEdge:
        topLeft.y = m_dragBounds.clampYToBounds(topLeft.y + dy);
		break;
    case BottomEdge:
        bottomRight.y = m_dragBounds.clampYToBounds(bottomRight.y + dy);
		break;
    case LeftTopCorner:
        topLeft.x = m_dragBounds.clampXToBounds(br.left() + dx);
        topLeft.y = m_dragBounds.clampYToBounds(br.top() + dy);
		break;
    case RightBottomCorner:
        bottomRight.x = m_dragBounds.clampXToBounds(br.right() + dx);
        bottomRight.y = m_dragBounds.clampYToBounds(br.bottom() + dy);
		break;
    case LeftBottomCorner:
        topLeft.x = m_dragBounds.clampXToBounds(br.left() + dx);
        bottomRight.y = m_dragBounds.clampYToBounds(br.bottom() + dy);
		break;
    case RightTopCorner:
        bottomRight.x = m_dragBounds.clampXToBounds(br.right() + dx);
        topLeft.y = m_dragBounds.clampYToBounds(br.top() + dy);
		break;
	}
#if 0
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
#endif
    SetBounds(rectangle(topLeft, bottomRight - topLeft));
}

void ShapeEditFrame::UpdateCurrentZone(int x, int y)
{
	Zone zone = DetectZone(x, y);
	if (zone != m_currentZone || m_cursorNeedsReset) {
		m_cursorNeedsReset = false;
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
