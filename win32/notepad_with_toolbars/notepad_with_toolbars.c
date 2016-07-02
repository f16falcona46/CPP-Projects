#include <windows.h>
#include <commctrl.h>
#include "resource.h"

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480
#define MINIMUM_WINDOW_WIDTH 320
#define MINIMUM_WINDOW_HEIGHT 240

#define IDC_MAIN_EDIT 51
#define IDC_MAIN_STATUSBAR 52
#define IDC_MAIN_TOOLBAR 53

const char* notepad_szClassName = "notepad_window_class";

char g_szOpenFilename[MAX_PATH] = "";

//enables or disables the Undo menu item based on whether the edit control can be undone.
void UpdateUndoButton(HWND hwnd) {
	if (SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_CANUNDO, (WPARAM)0, (LPARAM)0) == TRUE)
		EnableMenuItem(GetMenu(hwnd), ID_EDIT_UNDO, MF_ENABLED);
	else
		EnableMenuItem(GetMenu(hwnd), ID_EDIT_UNDO, MF_GRAYED);
}

//updates the Modified/Unmodified part of the status bar based on whether the edit control is modified.
void UpdateStatusBar(HWND hwnd) {
	BOOL bStatusChanged = TRUE;
	LPSTR pszText = GlobalAlloc(GMEM_FIXED, LOWORD(SendMessage(GetDlgItem(hwnd, IDC_MAIN_STATUSBAR), SB_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0))+1);
	if (pszText != NULL) {
		SendMessage(GetDlgItem(hwnd, IDC_MAIN_STATUSBAR), SB_GETTEXT, (WPARAM)0, (LPARAM)pszText);
		if (!lstrcmp(pszText, "Modified")) bStatusChanged = FALSE;
		GlobalFree(pszText);
	}
	if (SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_GETMODIFY, (WPARAM)0, (LPARAM)0)) {
		if (bStatusChanged)
			SendMessage(GetDlgItem(hwnd, IDC_MAIN_STATUSBAR), SB_SETTEXT, 0, (LPARAM)"Modified");
	}
	else
		SendMessage(GetDlgItem(hwnd, IDC_MAIN_STATUSBAR), SB_SETTEXT, 0, (LPARAM)"Unmodified");
}

//updates the title text of the main window
void updateTitleText(HWND hwnd) {
	if (lstrcmp(g_szOpenFilename, "")) {
		char szTitleText[MAX_PATH + 25]; //24, but an extra one to be safe.
		lstrcpy(szTitleText, g_szOpenFilename);
		lstrcat(szTitleText, " - Notepad with Toolbars");
		SetWindowText(hwnd, szTitleText);
	}
	else {
		SetWindowText(hwnd, "[New File] - Notepad with Toolbars");
	}
}

//loads a filename from an open file dialog into g_szOpenFilename
//can spawn a messagebox if the file dialog fails
//TRUE if g_szOpenFilename changed, FALSE if not
BOOL LoadOpenFileName(HWND hwnd, LPSTR pszFilename) {
	OPENFILENAME ofn;
	char szFilename[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFilename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";
	
	if (GetOpenFileName(&ofn)) {
		LPSTR ptr = lstrcpy(pszFilename, szFilename);
		if (ptr == NULL) {
			MessageBox(hwnd, "Couldn't load filename!", "Error!", MB_OK | MB_ICONERROR);
		}
		else {
			updateTitleText(hwnd);
			return TRUE;
		}
	}
	else {
		if (CommDlgExtendedError() != 0) {
			MessageBox(hwnd, "Couldn't load filename!", "Error!", MB_OK | MB_ICONERROR);
		}
		else return FALSE;
	}
}

//loads a filename from a save file dialog into g_szOpenFilename
//can spawn a messagebox if the file dialog fails
//TRUE if g_szOpenFilename changed, FALSE if not
BOOL LoadSaveFileName(HWND hwnd, LPSTR pszFilename) {
	OPENFILENAME ofn;
	char szFilename[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFilename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "txt";
	
	if (GetSaveFileName(&ofn)) {
		LPSTR ptr = lstrcpy(pszFilename, szFilename);
		if (ptr == NULL) {
			MessageBox(hwnd, "Couldn't load filename!", "Error!", MB_OK | MB_ICONERROR);
		}
		else {
			updateTitleText(hwnd);
			return TRUE;
		}
	}
	else {
		if (CommDlgExtendedError()) {
			MessageBox(hwnd, "Couldn't load filename!", "Error!", MB_OK | MB_ICONERROR);
		}
		else return FALSE;
	}
}

//saves the edit control's text into g_szOpenFilename
//returns TRUE if it was successful, FALSE if not
BOOL SaveTextToFile(HWND hEdit, LPCSTR pszFilename) {
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(pszFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwTextLength = GetWindowTextLength(hEdit);
		LPSTR pszText = GlobalAlloc(GMEM_FIXED, dwTextLength+1);
		if (pszText != NULL) {
			if (GetWindowText(hEdit, pszText, dwTextLength+1)) {
				DWORD dwWritten; //dummy param
				if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
					bSuccess = TRUE;
			}
			GlobalFree(pszText);
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

//loads the text in g_szOpenFilename into the edit control
//returns TRUE if it was successful, FALSE if not
BOOL LoadTextFileIntoEdit(HWND hEdit, LPCSTR pszFilename) {
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(pszFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xffffffff) {
			LPSTR pszFileText = GlobalAlloc(GMEM_FIXED, dwFileSize+1);
			if (pszFileText != NULL) {
				DWORD dwRead; //dummy param
				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL)) {
					pszFileText[dwFileSize] = 0;
					if (SetWindowText(hEdit, pszFileText))
						bSuccess = TRUE;
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

//callback for the About dialog
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG:
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

//callback for the main window
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND:
		if ((HWND)lParam == GetDlgItem(hwnd, IDC_MAIN_EDIT)) {
			if (HIWORD(wParam) == EN_UPDATE) { //edit control's text changed
				UpdateUndoButton(hwnd);
				UpdateStatusBar(hwnd);
			}
		}
		switch (LOWORD(wParam)) { //menu stuff
			case ID_FILE_NEW:
				if (SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_GETMODIFY, (WPARAM)0, (LPARAM)0)) { //test if edit control's text was modified
					if (MessageBox(hwnd, "Open a new file and lose these changes?", "Unsaved Changes", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) { //confirmation
						break;
					}
				}
				if (SetWindowText(GetDlgItem(hwnd, IDC_MAIN_EDIT), "") == TRUE) { //test if edit control's text was set successfully
					g_szOpenFilename[0] = '\0'; //no open file
					SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_SETMODIFY, (WPARAM)FALSE, (LPARAM)0);
					UpdateUndoButton(hwnd);
					UpdateStatusBar(hwnd);
				}
				else {
					MessageBox(hwnd, "Couldn't make new file!", "Error!", MB_OK | MB_ICONERROR);
				}
			break;
			case ID_FILE_OPEN:
				if (SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_GETMODIFY, (WPARAM)0, (LPARAM)0)) { //test if edit control's text was modified
					if (MessageBox(hwnd, "Open a new file and lose these changes?", "Unsaved Changes", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) { //confirmation
						break;
					}
				}
				if (LoadOpenFileName(hwnd, g_szOpenFilename) == TRUE) {
					if (LoadTextFileIntoEdit(GetDlgItem(hwnd, IDC_MAIN_EDIT), g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't open file!", "Error!", MB_OK | MB_ICONERROR);
					else {
						SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_SETMODIFY, (WPARAM)FALSE, (LPARAM)0);
						UpdateUndoButton(hwnd);
						UpdateStatusBar(hwnd);
					}
				}
			break;
			case ID_FILE_SAVE:
				if (lstrcmp(g_szOpenFilename, "") == 0) {
					if (LoadSaveFileName(hwnd, g_szOpenFilename) != TRUE) break;
				}
				if (SaveTextToFile(GetDlgItem(hwnd, IDC_MAIN_EDIT), g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't save file!", "Error!", MB_OK | MB_ICONERROR);
				else {
					SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_SETMODIFY, (WPARAM)FALSE, (LPARAM)0);
					UpdateStatusBar(hwnd);
				}
			break;
			case ID_FILE_SAVE_AS:
				if (LoadSaveFileName(hwnd, g_szOpenFilename) == TRUE) {
					if (SaveTextToFile(GetDlgItem(hwnd, IDC_MAIN_EDIT), g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't save file!", "Error!", MB_OK | MB_ICONERROR);
					else {
						SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_SETMODIFY, (WPARAM)FALSE, (LPARAM)0);
						UpdateStatusBar(hwnd);
					}
				}
			break;
			case ID_EDIT_UNDO:
				SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_UNDO, (WPARAM)0, (LPARAM)0);
			break;
			case ID_HELP_ABOUT:
			{
				int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
				if (ret == IDOK) {
					MessageBox(hwnd, "You are an okay person.", "Notice", MB_OK | MB_ICONINFORMATION);
				}
				else if (ret = IDCANCEL) {
					MessageBox(hwnd, "This fool does not like memes.", "Notice", MB_OK | MB_ICONINFORMATION);
				}
				else if (ret == -1) {
					MessageBox(hwnd, "Dialog failed!", "Error!", MB_OK | MB_ICONERROR);
				}
				else {
					MessageBox(hwnd, "Dialog exited with something else.", "Notice", MB_OK | MB_ICONINFORMATION);
				}
			}
			break;
			case ID_FILE_EXIT:
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
	break;
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			0, 0, 100, 100, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		HWND hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
			0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_STATUSBAR, GetModuleHandle(NULL), NULL);
		HWND hTool = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd,
			(HMENU)IDC_MAIN_TOOLBAR, GetModuleHandle(NULL), NULL);
		if (hEdit == NULL || hStatus == NULL || hTool == NULL) {
			MessageBox(hwnd, "Couldn't create controls!", "Error!", MB_OK | MB_ICONERROR);
		}
		else {
			HFONT hDefault = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hDefault, MAKELPARAM(FALSE, 0));
			UpdateUndoButton(hwnd);
			
			int statuswidths[] = {100, -1};
			SendMessage(hStatus, SB_SETPARTS, sizeof(statuswidths)/sizeof(int), (LPARAM)statuswidths);
			UpdateStatusBar(hwnd);
			
			SendMessage(hTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), (LPARAM)0);
			
			TBADDBITMAP tbab;
			tbab.hInst = HINST_COMMCTRL;
			tbab.nID = IDB_STD_SMALL_COLOR;
			SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)(&tbab));
			
			TBBUTTON tbb[3];
			ZeroMemory(tbb, sizeof(tbb));
			tbb[0].iBitmap = STD_FILENEW;
			tbb[0].fsState = TBSTATE_ENABLED;
			tbb[0].fsStyle = TBSTYLE_BUTTON;
			tbb[0].idCommand = ID_FILE_NEW;

			tbb[1].iBitmap = STD_FILEOPEN;
			tbb[1].fsState = TBSTATE_ENABLED;
			tbb[1].fsStyle = TBSTYLE_BUTTON;
			tbb[1].idCommand = ID_FILE_OPEN;

			tbb[2].iBitmap = STD_FILESAVE;
			tbb[2].fsState = TBSTATE_ENABLED;
			tbb[2].fsStyle = TBSTYLE_BUTTON;
			tbb[2].idCommand = ID_FILE_SAVE_AS;
			
			SendMessage(hTool, TB_ADDBUTTONS, sizeof(tbb)/sizeof(TBBUTTON), (LPARAM)(&tbb));
		}
	}
	break;
	case WM_SIZE:
	{
		RECT rcClient;
		GetClientRect(hwnd, &rcClient);
		
		RECT rcEdit;
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		
		RECT rcStatusbar;
		HWND hStatusbar = GetDlgItem(hwnd, IDC_MAIN_STATUSBAR);
		SendMessage(hStatusbar, WM_SIZE, 0, 0);
		GetWindowRect(hStatusbar, &rcStatusbar);
		int iStatusbarHeight = rcStatusbar.bottom - rcStatusbar.top;
		
		RECT rcToolbar;
		HWND hToolbar = GetDlgItem(hwnd, IDC_MAIN_TOOLBAR);
		SendMessage(hToolbar, WM_SIZE, 0, 0);
		GetWindowRect(hToolbar, &rcToolbar);
		int iToolbarHeight = rcToolbar.bottom - rcToolbar.top;
		
		int iEditHeight = rcClient.bottom - iStatusbarHeight - iToolbarHeight;
		SetWindowPos(hEdit, NULL, 0, iToolbarHeight, rcClient.right, iEditHeight, SWP_NOZORDER);
	}
	break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* mmi = (MINMAXINFO*)lParam;
		mmi->ptMinTrackSize.x = MINIMUM_WINDOW_WIDTH;
		mmi->ptMinTrackSize.y = MINIMUM_WINDOW_HEIGHT;
	}
	break;
	case WM_CLOSE:
		if (SendMessage(GetDlgItem(hwnd, IDC_MAIN_EDIT), EM_GETMODIFY, (WPARAM)0, (LPARAM)0)) {
			if (MessageBox(hwnd, "Quit without saving?", "Unsaved Changes", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) DestroyWindow(hwnd);
			else break;
		}
		else DestroyWindow(hwnd);
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, char* lpCmdLine, int nCmdShow) {
	InitCommonControls();
	
	WNDCLASSEX wc;
	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	wc.lpszClassName = notepad_szClassName;
	wc.hIconSm = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONERROR | MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		notepad_szClassName,
		"unconstructed title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
		NULL, NULL, hInstance, NULL);
	
	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
		return 0;
	}
	
	updateTitleText(hwnd);
	
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(KB_ACCELERATORS));
	if (hAccelTable == NULL) {
		MessageBox(NULL, "Loading Keyboard Hotkeys Failed!", "Error!", MB_ICONERROR | MB_OK);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if (hAccelTable) {
			if (!TranslateAccelerator(hwnd, hAccelTable, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
