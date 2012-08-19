#include "wideconvert.h"
#include <algorithm>

std::wstring LibUSB::Util::StringToWString( const std::string& ns )
{

	size_t bufferSize;

	// first call to wcstombs_s to get the target buffer size
	mbstowcs_s(&bufferSize, NULL, 0, ns.c_str(), ns.size());

	// create target buffer with required size
	wchar_t* buffer = new wchar_t[bufferSize];

	// second call to do the actual conversion
	mbstowcs_s(&bufferSize, buffer, bufferSize, ns.c_str(), ns.size());

	std::wstring result(buffer, bufferSize);
	delete[] buffer;
	return result;

}

std::string LibUSB::Util::WStringToString( const std::wstring& ws )
{

	size_t bufferSize;

	// first call to wcstombs_s to get the target buffer size
	wcstombs_s(&bufferSize, NULL, 0, ws.c_str(), ws.size());

	// create target buffer with required size
	char* buffer = new char[bufferSize];

	// second call to do the actual conversion
	wcstombs_s(&bufferSize, buffer, bufferSize, ws.c_str(), ws.size());

	std::string result(buffer, bufferSize);
	delete[] buffer;
	return result;

}
