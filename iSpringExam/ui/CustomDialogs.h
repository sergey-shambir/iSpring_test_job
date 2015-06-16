#pragma once

class CBaseSceneFileDialog : public CFileDialog
{
public:
	LPCTSTR GetFilePath() const;
	std::string GetStdFilePath() const;

protected:
	CBaseSceneFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt,
		LPCTSTR lpszFileName,
		DWORD dwFlags,
		LPCTSTR lpszFilter,
		HWND hWndParent);
};

/*!
	\code
	SceneOpenFileDialog dialog;
	if (IDOK == dialog.DoModal()) {
		LPCTSTR path = dialog.GetFilePath()
		// Do anything with file path
	}
	\endcode
 */
class CSceneOpenFileDialog : public CBaseSceneFileDialog
{
public:
	CSceneOpenFileDialog(const CWindow &parent = nullptr);
};

/*!
	\code
	CSceneSaveFileDialog dialog;
	if (IDOK == dialog.DoModal()) {
		LPCTSTR path = dialog.GetFilePath()
		// Do anything with file path
	}
	\endcode
*/
class CSceneSaveFileDialog : public CBaseSceneFileDialog
{
public:
	CSceneSaveFileDialog(const CWindow &parent = nullptr);
};