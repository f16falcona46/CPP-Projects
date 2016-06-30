#include <windows.h>
#include "resource.h"

const char* blank_szClassName = "blank_window_class";

HWND g_hToolbox = NULL;

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

BOOL CALLBACK ToolboxDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG:
		return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_TOOLBOX_FIRST_BUTTON:
					MessageBox(hwnd, "First button clicked.", "Notice", MB_OK | MB_ICONINFORMATION);
				break;
				case IDC_TOOLBOX_SECOND_BUTTON:
					MessageBox(hwnd, "Second button clicked.", "Notice", MB_OK | MB_ICONINFORMATION);
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
	case WM_LBUTTONDOWN:
	{
		char szFileName[MAX_PATH];
		HINSTANCE hInstance = GetModuleHandle(NULL);
		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hwnd, szFileName, "This Program Is:", MB_OK | MB_ICONINFORMATION);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case ID_FILE_ABOUT:
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
			case ID_STUFF_GO:
				MessageBox(hwnd, "this is an amazing message box!!!", "important message", MB_OK | MB_ICONHAND);
			break;
			case ID_TOOLBOX_SHOW:
				if (g_hToolbox != NULL) {
					ShowWindow(g_hToolbox, SW_SHOW);
					EnableMenuItem(GetMenu(hwnd), ID_TOOLBOX_SHOW, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TOOLBOX_HIDE, MF_ENABLED);
				}
			break;
			case ID_TOOLBOX_HIDE:
				if (g_hToolbox != NULL) {
					ShowWindow(g_hToolbox, SW_HIDE);
					EnableMenuItem(GetMenu(hwnd), ID_TOOLBOX_SHOW, MF_ENABLED);
					EnableMenuItem(GetMenu(hwnd), ID_TOOLBOX_HIDE, MF_GRAYED);
				}
			break;
		}
	break;
	case WM_CREATE:
		g_hToolbox = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBOX), hwnd, ToolboxDlgProc);
		if (g_hToolbox != NULL) {
			ShowWindow(g_hToolbox, SW_SHOW);
			EnableMenuItem(GetMenu(hwnd), ID_TOOLBOX_SHOW, MF_GRAYED);
		}
		else {
			EnableMenuItem(GetMenu(hwnd), ID_TOOLBOX_SHOW, MF_GRAYED);
			EnableMenuItem(GetMenu(hwnd), ID_TOOLBOX_HIDE, MF_GRAYED);
			MessageBox(hwnd, "Couldn't create toolbox window!", "Error!", MB_OK | MB_ICONEXCLAMATION);
		}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
	break;
	case WM_DESTROY:
		DestroyWindow(g_hToolbox);
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
	wc.lpszClassName = blank_szClassName;
	wc.hIconSm = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		blank_szClassName,
		"Window Title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
		NULL, NULL, hInstance, NULL);
	
	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if (!IsDialogMessage(g_hToolbox, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
