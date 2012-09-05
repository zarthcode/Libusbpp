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
		
		/// Returns the raw libusb error code.
		int getLibusbErrorCode()const { return m_ErrorCode; };

		virtual const char* what();
	protected:

		int m_ErrorCode;
	private:
	};

}