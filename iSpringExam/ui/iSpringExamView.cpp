// iSpringExamView.cpp : implementation of the CISpringExamView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Ribbon.h"
#include "../resource.h"
#include "CustomDialogs.h"
#include "iSpringExamView.h"
#include "../shapes/trianglenode.h"
#include "../shapes/rectanglenode.h"
#include "../shapes/ellipsenode.h"
#include "BackBuffer.h"
#include "../render/GdiplusRenderer.h"

CISpringExamView::CISpringExamView() = default;

CISpringExamView::~CISpringExamView() = default;

BOOL CISpringExamView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

void CISpringExamView::NewDocument()
{
	if (!EnsureUserChangesSaved())
		return;
	m_sceneSavePath.clear();
	m_scene->clearScene();
}

void CISpringExamView::OpenDocument()
{
	CSceneOpenFileDialog dialog(*this);
	if (IDOK == dialog.DoModal()) {
		std::unique_ptr<ShapesScene> scene{ new ShapesScene };
		std::string path = dialog.GetStdFilePath();
		std::string errorReason;
		if (scene->addFromJsonFile(path, errorReason)) {
			m_scene.swap(scene);
			m_sceneSavePath = path;
			InitSceneUpdates();
			Invalidate(FALSE);
		} else {
			WarnFailure("Failed to Open Scene", errorReason);
		}
	}
}

void CISpringExamView::SaveDocument()
{
	if (m_sceneSavePath.empty()) {
		SaveDocumentAs();
	} else {
		SaveDocumentToPath(m_sceneSavePath);
	}
}

void CISpringExamView::SaveDocumentAs()
{
	CSceneSaveFileDialog dialog(*this);
	if (IDOK == dialog.DoModal()) {
		std::string path = dialog.GetStdFilePath();
		const char SCENE_FILE_EXT[] = ".json";
		if (path.rfind(SCENE_FILE_EXT) != path.size() - strlen(SCENE_FILE_EXT))
			path += ".json";
		if (SaveDocumentToPath(path))
			m_sceneSavePath = path;
	}
}

void CISpringExamView::InsertEllipse()
{
	auto command = std::make_shared<InsertShapeCommand>(EllipseNode::CLASS_NAME);
	m_scene->doCommand(command);
}

void CISpringExamView::InsertTriangle()
{
	auto command = std::make_shared<InsertShapeCommand>(TriangleNode::CLASS_NAME);
	m_scene->doCommand(command);
}

void CISpringExamView::InsertRectangle()
{
	auto command = std::make_shared<InsertShapeCommand>(RectangleNode::CLASS_NAME);
	m_scene->doCommand(command);
}

void CISpringExamView::Undo()
{
	m_scene->undo();
}

void CISpringExamView::Redo()
{
	m_scene->redo();
}

void CISpringExamView::OnFinalMessage(HWND /*hWnd*/)
{
	delete this;
}

LRESULT CISpringExamView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    std::unique_ptr<Gdiplus::Graphics> backGraphics{ m_backBuffer->StartRender() };
    backGraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    GdiplusRenderer renderer(*backGraphics);
    renderer.SetPen(VGPen(vec3(1, 0, 0), 2.f));
    renderer.SetBrush(VGBrush(vec3(1, 1, 0)));
    m_scene->render(renderer);
	if (m_scene->pickedNode())
		m_editFrame.Render(renderer);
	backGraphics.reset();

	CPaintDC dc(m_hWnd);
	std::unique_ptr<Gdiplus::Graphics> mainGraphics{ Gdiplus::Graphics::FromHDC(dc.m_hDC) };
	mainGraphics->DrawImage(m_backBuffer->GetBitmap(), 0, 0);
	mainGraphics.reset();

	return 0;
}

LRESULT CISpringExamView::OnResize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	const int width = LOWORD(lParam);
	const int height = HIWORD(lParam);
	m_scene->setMinimalSize(width, height);
	m_backBuffer->SetSize(Gdiplus::Size(width, height));
    m_editFrame.SetSceneBounds(rectangle(0, 0, float(width), float(height)));
	return 0;
}

LRESULT CISpringExamView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_scene.reset(new ShapesScene);
	InitSceneUpdates();
	m_backBuffer.reset(new CBackBuffer);

	return 0;
}

LRESULT CISpringExamView::OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (wParam == VK_DELETE && m_scene->pickedNode()) {
		auto command = std::make_shared<DeleteShapeCommand>();
		m_scene->doCommand(command);
		m_editFrame.Hide();
	}

	return 0;
}

LRESULT CISpringExamView::OnMouseUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	const int x = GET_X_LPARAM(lParam);
	const int y = GET_Y_LPARAM(lParam);
	if (m_editFrame.IsInDragState()) {
		m_editFrame.UpdateMousePos(x, y);
		FinishEditFrameDrag();
	} else if (m_scene->pickNode(x, y)) {
		m_editFrame.ShowInRect(m_scene->pickedNode()->boundingRect());
		POINT point;
		if (::GetCursorPos(&point) && ::ScreenToClient(m_hWnd, &point))
			m_editFrame.UpdateMousePos(x, y);
	} else {
		m_editFrame.Hide();
	}
	Invalidate(FALSE);
	return 0;
}

LRESULT CISpringExamView::OnMouseDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	const int x = GET_X_LPARAM(lParam);
	const int y = GET_Y_LPARAM(lParam);
	if (m_scene->pickedNode() && m_editFrame.StartDrag(x, y)) {
		m_pickedBoundsBeforeDrag = m_scene->pickedNode()->boundingRect();
	}
	return 0;
}

LRESULT CISpringExamView::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	const int x = GET_X_LPARAM(lParam);
	const int y = GET_Y_LPARAM(lParam);
	const bool leftButtonPressed = wParam & MK_LBUTTON;

	if (!leftButtonPressed && m_editFrame.IsInDragState()) {
		FinishEditFrameDrag();
	}
	m_editFrame.UpdateMousePos(x, y);
	if (leftButtonPressed && m_editFrame.IsInDragState()) {
		rectangle bounds = m_editFrame.GetBounds();
		m_scene->pickedNode()->setBoundingRect(bounds);
		Invalidate(FALSE);
	}

	// Track next mouse leave event
	TRACKMOUSEEVENT trackInfo;
	trackInfo.cbSize = sizeof(trackInfo);
	trackInfo.hwndTrack = m_hWnd;
	trackInfo.dwFlags = TME_HOVER | TME_LEAVE;
	trackInfo.dwHoverTime = HOVER_DEFAULT;
	::TrackMouseEvent(&trackInfo);

	return 0;
}

LRESULT CISpringExamView::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	m_editFrame.DropCustomCursor();
	return 0;
}

LRESULT CISpringExamView::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Ignore event to ensure that edit frame custom cursor never reset
	return 0;
}

bool CISpringExamView::EnsureUserChangesSaved()
{
	if (!m_scene->isModified())
		return true;

	auto text = _T("Do you want to save changes you did?");
	auto caption = _T("Save changes");
	int result = MessageBox(text, caption, MB_YESNOCANCEL | MB_APPLMODAL);
	if (result == IDCANCEL)
		return false;
	if (result == IDYES)
		SaveDocument();
	return true;
}

void CISpringExamView::InitSceneUpdates()
{
	m_scene->setUpdateCallback([this]() -> void {
		Invalidate(FALSE);
	});
}

void CISpringExamView::FinishEditFrameDrag()
{
	m_editFrame.FinishDrag();
	if (NodePtr node = m_scene->pickedNode()) {
		auto command = std::make_shared<SetShapeBoundsCommand>();
		command->setNewBounds(m_editFrame.GetBounds());
		node->setBoundingRect(m_pickedBoundsBeforeDrag);
		m_scene->doCommand(command);
	}
}

void CISpringExamView::WarnFailure(const std::string &caption, const std::string &errorReason)
{
	CA2T text(errorReason.c_str());
	CA2T captionW(caption.c_str());
	MessageBox(text, captionW, MB_OK | MB_ICONWARNING | MB_APPLMODAL);
}

bool CISpringExamView::SaveDocumentToPath(const std::string &path)
{
	std::string errorReason;
	if (m_scene->saveToJsonFile(path, errorReason)) {
		return true;
	}
	WarnFailure("Failed to Save Scene", errorReason);
	return false;
}
