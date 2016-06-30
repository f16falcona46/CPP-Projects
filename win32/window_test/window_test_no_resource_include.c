#include <windows.h>

#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002
#define ID_STUFF_GRAYED 9003

const char* blank_szClassName = "blank_window_class";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE:
	{
		HMENU hMenu = CreateMenu();
		HMENU hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
		AppendMenu(hSubMenu, MF_STRING | MF_GRAYED, ID_STUFF_GRAYED, "G&rayed");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");
		SetMenu(hwnd, hMenu);
		
		HICON hIcon = LoadImage(NULL, "icons.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		if (hIcon) {
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		}
		else {
			MessageBox(hwnd, "Couldn't set large icon!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		}
		HICON hIconSm = LoadImage(NULL, "icons.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		if (hIconSm) {
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIconSm);
		}
		else {
			MessageBox(hwnd, "Couldn't set small icon!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		char szFileName[MAX_PATH];
		HINSTANCE hInstance = GetModuleHandle(NULL);
		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hwnd, szFileName, "This Program Is:", MB_OK | MB_ICONINFORMATION);
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
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = blank_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		blank_szClassName,
		"Window Title",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL);
	
	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
