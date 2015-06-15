// iSpringExam.cpp : main source file for iSpringExam.exe
//

#include "stdafx.h"
#include <atlribbon.h>
#include <gdiplus.h>

#include "Ribbon.h"

#include "resource.h"

#include "ui/iSpringExamView.h"
#include "ui/aboutdlg.h"
#include "ui/MainFrm.h"

namespace {
	const int WINDOW_WIDTH = 1200;
	const int WINDOW_HEIGHT = 800;
} // anonymous namespace

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}
	wndMain.ResizeClient(WINDOW_WIDTH, WINDOW_HEIGHT);

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

#ifdef DEBUG
// Entry point when using subsystem CONSOLE
int wmain(int argc, wchar_t *argv[])
{
	const int DEFAULT_CMD_SHOW = 10;
	wchar_t EMTRY_CMD_LINE[] = _T("");
	wWinMain(::GetModuleHandle(NULL), NULL, EMTRY_CMD_LINE, DEFAULT_CMD_SHOW);
	return 0;
}
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	::CoUninitialize();

	return nRet;
}
