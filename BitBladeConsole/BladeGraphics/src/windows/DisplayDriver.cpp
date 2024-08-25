/* DisplayDriver.cpp
* windows implementation
*/

#include "DisplayDriver.h"
#include <ImageData.h>
#include <PixelColor.h>

#include <windows.h>
#include <vector>

// Explicit instantiation of the template
template void DisplayDriver::Render<BGRA>(const ImageData<BGRA>& data);

template<typename PixelColor>
void DisplayDriver::Render(const ImageData<PixelColor>& frameBuffer)
{
	const int width = frameBuffer.GetWidth();
	const int height = frameBuffer.GetHeight();

	// Create a simple window
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "RetroWindowClass";

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		wc.lpszClassName,               // Window class
		"Retro Control Engine",         // Window text
		WS_OVERLAPPEDWINDOW,            // Window style
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL,                           // Parent window    
		NULL,                           // Menu
		wc.hInstance,                   // Instance handle
		NULL                            // Additional application data
	);

	ShowWindow(hwnd, SW_SHOW);

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // Negative to indicate top-down row order
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	// Main loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		HDC hdc = GetDC(hwnd);
		StretchDIBits(hdc,
			0, 0, width, height,
			0, 0, width, height,
			reinterpret_cast<const BYTE*>(frameBuffer.GetBuffer()),  // Convert PixelColor* to BYTE* for GDI
			&bmi,
			DIB_RGB_COLORS,
			SRCCOPY);
		ReleaseDC(hwnd, hdc);
	}
}

LRESULT CALLBACK DisplayDriver::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
