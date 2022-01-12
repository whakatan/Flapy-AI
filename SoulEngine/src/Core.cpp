#include "pch.h"
#include "Core.h"

#ifdef _PLATFORM_WINDOWS

void GetExecutableDirectory(std::string& buffer)
{
	std::wstring wbuffer;
	wbuffer.resize(MAX_PATH + 1);
	GetModuleFileName(NULL, &wbuffer[0], MAX_PATH);

	std::transform(wbuffer.begin(), wbuffer.end(), std::back_inserter(buffer), [](wchar_t c) {
		return static_cast<char>(c);
	});

	size_t last = buffer.find_last_of(PATH_SEPARATOR);
	buffer = buffer.substr(0, last) + PATH_SEPARATOR;
}
#else
void GetExecutableDirectory(char_t* buffer)
{
}
#endif