/* DisplayDriver.cpp
* windows implementation
*/

#include "DisplayDriver.h"
#include <ImageData.h>

#include <windows.h>

// Helper method to convert RGB565 to BGRA
void DisplayDriver::ConvertRGB565ToBGRA(const ImageData& src, BYTE* dest) {

	const int width = src.GetWidth();
	const int height = src.GetHeight();
	const uint16_t* buffer = src.GetBuffer();

	for (int i = 0; i < width * height; ++i) {
		uint16_t rgb565 = buffer[i];

		uint8_t r = (rgb565 >> 11) & 0x1F;
		uint8_t g = (rgb565 >> 5) & 0x3F;
		uint8_t b = rgb565 & 0x1F;

		dest[i * 4 + 0] = (b << 3) | (b >> 2);  // Blue
		dest[i * 4 + 1] = (g << 2) | (g >> 4);  // Green
		dest[i * 4 + 2] = (r << 3) | (r >> 2);  // Red
		dest[i * 4 + 3] = 255;                  // Alpha (full opacity)
	}
}

void DisplayDriver::Render(const ImageData& frameBuffer) {
	const int width = frameBuffer.GetWidth();
	const int height = frameBuffer.GetHeight();

	// Allocate memory for the BGRA buffer
	BYTE* bgraBuffer = new BYTE[width * height * 4];  // 4 bytes per pixel for BGRA

	// Convert the RGB565 data to BGRA
	ConvertRGB565ToBGRA(frameBuffer, bgraBuffer);

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
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		HDC hdc = GetDC(hwnd);
		StretchDIBits(hdc,
			0, 0, width, height,
			0, 0, width, height,
			bgraBuffer,  // Use the converted BGRA buffer
			&bmi,
			DIB_RGB_COLORS,
			SRCCOPY);
		ReleaseDC(hwnd, hdc);
	}

	// Free the BGRA buffer
	delete[] bgraBuffer;
}

LRESULT CALLBACK DisplayDriver::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
	} return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
