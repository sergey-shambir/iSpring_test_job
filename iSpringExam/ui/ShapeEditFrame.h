#pragma once
#include <gdiplus.h>
#include "../math/rectangle.h"
#include <map>

class ShapeEditFrame
{
public:
	enum Zone {
		Outside = 0,
		Inside,
		LeftEdge,
		RightEdge,
		TopEdge,
		BottomEdge,
		LeftTopCorner,
		LeftBottomCorner,
		RightTopCorner,
		RightBottomCorner
	};

	ShapeEditFrame();
	~ShapeEditFrame();

	const rectangle &GetBounds() const;
	bool IsVisible() const;
	bool IsInDragState() const;
	void SetBounds(const rectangle &bounds);
	void SetVisible(bool isVisible);
	void Render(Gdiplus::Graphics &graphics);
	//! Starts dragging if (x,y) is not outside frame
	bool StartDrag(int x, int y);
	void FinishDrag();
	void UpdateMousePos(int x, int y);
	void DropCustomCursor();
	void ShowInRect(const rectangle &bounds);
	void Hide();

private:
	void ApplyDragChanges(int x, int y);
	void UpdateCurrentZone(int x, int y);
	Zone DetectZone(int x, int y) const;
	LPCTSTR GetCursorNameForZone(Zone zone) const;
	HCURSOR LoadCursorForZone(Zone zone);

	rectangle m_bounds;
	std::map<LPCTSTR, HCURSOR> m_cursors;
	bool m_isVisible = true;
	bool m_isInDragState = false;
	bool m_cursorNeedsReset = false;
	int m_lastX = 0;
	int m_lastY = 0;
	Zone m_currentZone = Outside;
};

