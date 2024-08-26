/* DisplayDriver.h
*/

#ifndef DISPLAY_DRIVER_H 
#define DISPLAY_DRIVER_H 

#include <windows.h>

#include "ImageData.h"

class DisplayDriver {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Render(const ImageData& data);

private:
	void ConvertRGB565ToBGRA(const ImageData& src, BYTE* dest);
};

#endif // DISPLAY_DRIVER_H