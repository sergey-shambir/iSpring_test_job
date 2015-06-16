// iSpringExamView.h : interface of the CISpringExamView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../shapes/shapesscene.h"
#include "ShapeEditFrame.h"

class CGdiplusRenderer;

class CISpringExamView : public CWindowImpl<CISpringExamView>
{
public:
	DECLARE_WND_CLASS(NULL)

	CISpringExamView();
	~CISpringExamView();
	BOOL PreTranslateMessage(MSG* pMsg);
	void NewDocument();
	void OpenDocument();
	void SaveDocument();
	void SaveDocumentAs();
	void InsertEllipse();
	void InsertTriangle();
	void InsertRectangle();
	void Undo();
	void Redo();

	virtual void OnFinalMessage(HWND /*hWnd*/);

	BEGIN_MSG_MAP(CISpringExamView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnResize)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnMouseUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnResize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	bool EnsureUserChangesSaved();
	void InitSceneUpdates();
	void FinishEditFrameDrag();
	void WarnFailure(const std::string &caption, const std::string &errorReason);
	bool SaveDocumentToPath(const std::string &path);

	std::unique_ptr<ShapesScene> m_scene;
	std::unique_ptr<CGdiplusRenderer> m_gdiplusRenderer;
	std::string m_sceneSavePath;
	ShapeEditFrame m_editFrame;
	rectangle m_pickedBoundsBeforeDrag;
};
