#include <windows.h>
#include "resource.h"

#define IDC_MAIN_EDIT 51

const char* notepad_szClassName = "notepad_window_class";

char g_szOpenFilename[MAX_PATH] = "";

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
		if (ptr == NULL) return FALSE;
		else return TRUE;
	}
	return FALSE;
}

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
	
	if (GetOpenFileName(&ofn)) {
		LPSTR ptr = lstrcpy(pszFilename, szFilename);
		if (ptr == NULL) return FALSE;
		else return TRUE;
	}
	return FALSE;
}

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case ID_FILE_OPEN:
				if (LoadOpenFileName(hwnd, g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't load filename!", "Error!", MB_OK | MB_ICONERROR);
				else {
					if (LoadTextFileIntoEdit(GetDlgItem(hwnd, IDC_MAIN_EDIT), g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't open file!", "Error!", MB_OK | MB_ICONERROR);
				}
			break;
			case ID_FILE_SAVE:
				if (lstrcmp(g_szOpenFilename, "") == 0) {
					if (LoadSaveFileName(hwnd, g_szOpenFilename) != TRUE) {
						MessageBox(hwnd, "Couldn't load filename!", "Error!", MB_OK | MB_ICONERROR);
						break;
					}
				}
				if (SaveTextToFile(GetDlgItem(hwnd, IDC_MAIN_EDIT), g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't save file!", "Error!", MB_OK | MB_ICONERROR);
			break;
			case ID_FILE_SAVE_AS:
				if (LoadSaveFileName(hwnd, g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't load filename!", "Error!", MB_OK | MB_ICONERROR);
				else if (SaveTextToFile(GetDlgItem(hwnd, IDC_MAIN_EDIT), g_szOpenFilename) != TRUE) MessageBox(hwnd, "Couldn't save file!", "Error!", MB_OK | MB_ICONERROR);
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
		if (hEdit == NULL) {
			MessageBox(hwnd, "Couldn't create Edit control!", "Error!", MB_OK | MB_ICONERROR);
		}
		else {
			HFONT hDefault = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hDefault, MAKELPARAM(FALSE, 0));
		}
	}
	break;
	case WM_SIZE:
	{
		RECT rcClient;
		GetClientRect(hwnd, &rcClient);
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
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
		"Simple Notepad",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
		NULL, NULL, hInstance, NULL);
	
	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONERROR | MB_OK);
		return 0;
	}
	
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
