/* BladeLinkBufferHelper.h
*/

#ifndef BLADE_LINK_BUFFER_HELPER_H
#define BLADE_LINK_BUFFER_HELPER_H

template<typename T>
inline void readFromBuffer(const char* buffer, T& valueOut, size_t& posOut) {
	memcpy(&valueOut, buffer + posOut, sizeof(T));
	posOut += sizeof(T);
}


#endif // BLADE_LINK_BUFFER_HELPER_H