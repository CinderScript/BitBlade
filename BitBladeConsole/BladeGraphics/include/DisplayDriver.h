/* DisplayDriver.h
*/

#ifndef DISPLAY_DRIVER_H 
#define DISPLAY_DRIVER_H 

#include <windows.h>
#include <vector>

#include "FrameBuffer.h"

class DisplayDriver {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	template<typename PixelColor>
	void Render(const FrameBuffer<PixelColor>& buffer);
};

#endif // DISPLAY_DRIVER_H