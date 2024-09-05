/* BladeLink.h
*/

#ifndef BLADE_LINK
#define BLADE_LINK

static constexpr int GRAPHICS_BUFFER_LENGTH = 1000;

template<typename T>
inline void readFromBuffer(const char* buffer, T& valueOut, size_t& posOut) {
	memcpy(&valueOut, buffer + posOut, sizeof(T));
	posOut += sizeof(T);
}

#endif // BLADE_LINK