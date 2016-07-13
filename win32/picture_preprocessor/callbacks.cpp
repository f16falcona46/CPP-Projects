#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "callbacks.hxx"

//callback for the main dialog
INT_PTR CALLBACK MainDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG:
		{
			HICON hIcon = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE));
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		}
		return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwnd, IDOK);
				break;
				case IDCANCEL:
					EndDialog(hwnd, IDCANCEL);
				break;
			}
		break;
		default:
		return FALSE;
	}
	return TRUE;
}

static HWND g_hProgressBar = NULL;
//callback for the progress bar dialog
INT_PTR CALLBACK ProgressDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG:
		{
			HICON hIcon = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE));
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			
			g_hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, "", PBS_SMOOTH | WS_CHILD, 20, 20, 280, 40, hwnd, NULL, GetModuleHandle(NULL), NULL);
			RECT rProgressBar;
			rProgressBar.left = 10;
			rProgressBar.right = 330;
			rProgressBar.top = 10;
			rProgressBar.bottom = 24;
			MapDialogRect(hwnd, &rProgressBar);
			MoveWindow(g_hProgressBar, rProgressBar.left, rProgressBar.top, rProgressBar.right - rProgressBar.left, rProgressBar.bottom - rProgressBar.top, TRUE);
			ShowWindow(g_hProgressBar, SW_SHOW);
			SendMessage(g_hProgressBar, PBM_SETRANGE, 0, (LPARAM)(MAKELONG(0,50)));
			SendMessage(g_hProgressBar, PBM_SETSTEP, (WPARAM)1, 0);
			SetTimer(hwnd, ID_TIMER_PROGRESSBAR, 200, NULL);
		}
		break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwnd, IDOK);
				break;
				case IDCANCEL:
					EndDialog(hwnd, IDCANCEL);
				break;
			}
		break;
		case WM_TIMER:
		switch (LOWORD(wParam)) {
			case ID_TIMER_PROGRESSBAR:
				SendMessage(g_hProgressBar, PBM_STEPIT, 0, 0);
				if (SendMessage(g_hProgressBar, PBM_GETPOS, 0, 0) == 50) {
					KillTimer(hwnd, ID_TIMER_PROGRESSBAR);
					EndDialog(hwnd, IDOK);
				}
			break;
			default: return FALSE;
		}
		break;
		default:
		return FALSE;
	}
	return TRUE;
}