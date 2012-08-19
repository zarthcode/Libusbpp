#include <string>


namespace LibUSB
{

	namespace Util
	{
	// Prototype for conversion functions
		std::wstring StringToWString(const std::string& s);
		std::string WStringToString(const std::wstring& s);
		
		
	}
}