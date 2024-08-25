// BitBlade.cpp : Defines the entry point for the application.
//

#include "BitBlade.h"

using std::cout;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main()
{
	BladeConsole test;
	cout << test.TestFunction();
	cout << "\n";

	//const int width = 640;
	//const int height = 480;

	//// Create a simple window
	//WNDCLASS wc = {};
	//wc.lpfnWndProc = WindowProc;
	//wc.hInstance = GetModuleHandle(NULL);
	//wc.lpszClassName = "RetroWindowClass";

	//RegisterClass(&wc);

	//HWND hwnd = CreateWindowEx(
	//	0,                              // Optional window styles.
	//	wc.lpszClassName,               // Window class
	//	"Retro Control Engine",         // Window text
	//	WS_OVERLAPPEDWINDOW,            // Window style
	//	CW_USEDEFAULT, CW_USEDEFAULT, width, height,
	//	NULL,                           // Parent window    
	//	NULL,                           // Menu
	//	wc.hInstance,                   // Instance handle
	//	NULL                            // Additional application data
	//);

	//ShowWindow(hwnd, SW_SHOW);

	//// Create a buffer to hold pixel data
	//std::vector<BYTE> buffer(width * height * 4, 0);

	//// Fill buffer with some color data (e.g., blue background with red square)
	//for (int y = 0; y < height; ++y)
	//{
	//	for (int x = 0; x < width; ++x)
	//	{
	//		int index = (y * width + x) * 4;
	//		buffer[index + 0] = 255; // Blue
	//		buffer[index + 1] = 0;   // Green
	//		buffer[index + 2] = 0;   // Red
	//		buffer[index + 3] = 0;   // Alpha (unused)

	//		if (x > 200 && x < 440 && y > 160 && y < 320)
	//		{
	//			buffer[index + 0] = 0;   // Blue
	//			buffer[index + 1] = 0;   // Green
	//			buffer[index + 2] = 255; // Red
	//		}
	//	}
	//}

	//BITMAPINFO bmi = {};
	//bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//bmi.bmiHeader.biWidth = width;
	//bmi.bmiHeader.biHeight = -height; // Negative to indicate top-down row order
	//bmi.bmiHeader.biPlanes = 1;
	//bmi.bmiHeader.biBitCount = 32;
	//bmi.bmiHeader.biCompression = BI_RGB;

	//// Main loop
	//MSG msg = {};
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);

	//	HDC hdc = GetDC(hwnd);
	//	StretchDIBits(hdc,
	//		0, 0, width, height,
	//		0, 0, width, height,
	//		buffer.data(),
	//		&bmi,
	//		DIB_RGB_COLORS,
	//		SRCCOPY);
	//	ReleaseDC(hwnd, hdc);
	//}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
	}
	return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

