#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"
#include "defs.h"
#include "common.h"
#include "include/zint.h"
#include <algorithm>
#pragma comment(lib, "comctl32.lib")
#pragma warning(disable: 4244)

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam);
VOID OnCommand(HWND hDlg, int id, HWND hwndCtl, UINT codeNotify);
BOOL OnNotify(HWND hWnd, UINT code, LPARAM lParam);

CmdLine cmline;

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR ptCmdLine, int nCmdShow)
{
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, BARCODEGEN_PROC_EVENT);
	DWORD err = GetLastError();
	if (hEvent && (err == ERROR_ALREADY_EXISTS)) // one process only
	{
		CloseHandle(hEvent);
		return 1;
	}
	/*cmline.AddOption(_T("-pref"), true, _T("prefix"));
	cmline.AddOption(_T("-post"), true, _T("postfix"));
	cmline.SetCmd(ptCmdLine);*/
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_STANDARD_CLASSES | ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icc);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINWND), NULL, MainDialogProc);
	CloseHandle(hEvent);
	return 0;
};

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return HANDLE_WM_INITDIALOG(hDlg, wParam, lParam, OnInitDialog);
	case WM_COMMAND:
		return HANDLE_WM_COMMAND(hDlg, wParam, lParam, OnCommand);
	case WM_NOTIFY:
		return OnNotify(hDlg, ((NMHDR*)lParam)->code, lParam);
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
BOOL OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam)
{
	BOOL res = TRUE;
	
	return res;
}
VOID OnCommand(HWND hDlg, int id, HWND hwndCtl, UINT codeNotify)
{

	switch (id)
	{
		case IDC_SAVE:
		{
			zint_symbol *my_sim = ZBarcode_Create();
			auto str = GetDlgItemString(hDlg, IDC_INPUT);
			auto safe_str = str + _T(".png");
			std::replace(safe_str.begin(), safe_str.end(), _T('/'), _T('_'));
			std::replace(safe_str.begin(), safe_str.end(), _T('\\'), _T('_'));
			_tcscpy_s(my_sim->outfile, FILENAME_MAX, safe_str.c_str());
			ZBarcode_Encode_and_Print(my_sim, (unsigned char*)str.c_str(), 0, 0);
			ZBarcode_Delete(my_sim);
			SetDlgItemText(hDlg, IDC_INPUT, _T(""));
		}
		break;
	}
}
BOOL OnNotify(HWND hDlg, UINT code, LPARAM lParam)
{
	return false;
}