#pragma once
#include <exception>
#include <stdexcept>

namespace LibUSB
{

	class LibUSBException : public std::runtime_error
	{
	public:
		LibUSBException(std::string text, int errorCode);
		std::string translateError(int ErrorCode);
		
	protected:

	private:
	};

}