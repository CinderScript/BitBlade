/* DisplayDriver.h
*/

#ifndef DISPLAY_DRIVER_H 
#define DISPLAY_DRIVER_H 

#include <windows.h>
#include <vector>

#include "ImageData.h"

class DisplayDriver {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	template<typename PixelColor>
	void Render(const ImageData<PixelColor>& data);
};

#endif // DISPLAY_DRIVER_H