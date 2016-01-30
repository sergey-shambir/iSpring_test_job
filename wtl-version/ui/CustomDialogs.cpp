#include "stdafx.h"
#include "CustomDialogs.h"

static const LPCTSTR SCENE_FILE_FILTER = _T("JSON scene file\0*.json\0");

CBaseSceneFileDialog::CBaseSceneFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
	LPCTSTR lpszDefExt,
	LPCTSTR lpszFileName,
	DWORD dwFlags,
	LPCTSTR lpszFilter,
	HWND hWndParent)
	: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, hWndParent)
{
}

LPCTSTR CBaseSceneFileDialog::GetFilePath() const
{
	return m_ofn.lpstrFile;
}

std::string CBaseSceneFileDialog::GetStdFilePath() const
{
	CT2A path(GetFilePath());
	return std::string(path);
}

CSceneOpenFileDialog::CSceneOpenFileDialog(const CWindow &parent)
	: CBaseSceneFileDialog(TRUE, nullptr, nullptr, OFN_HIDEREADONLY, SCENE_FILE_FILTER, parent)
{
	m_ofn.lpstrTitle = _T("Select JSON Scene File");
}

CSceneSaveFileDialog::CSceneSaveFileDialog(const CWindow &parent)
	: CBaseSceneFileDialog(FALSE, nullptr, nullptr, OFN_HIDEREADONLY, SCENE_FILE_FILTER, parent)
{
	m_ofn.lpstrTitle = _T("Select JSON Scene File");
}
