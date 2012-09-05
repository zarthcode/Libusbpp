#include "usbexception.h"
#include <libusb/libusb.h>
#include <sstream>
LibUSB::LibUSBException::LibUSBException( std::string text, int errorCode )
	: runtime_error(text.append(translateError(errorCode))), m_ErrorCode(errorCode)
{

}

std::string LibUSB::LibUSBException::translateError( int ErrorCode )
{
	std::string Result;

	switch(ErrorCode)
	{

	case LIBUSB_SUCCESS:
			Result = "LIBUSB_SUCCESS - Success (no error)";
			break;

	case LIBUSB_ERROR_IO:
			Result = "LIBUSB_ERROR_IO - Input/output error.";
			break;

	case LIBUSB_ERROR_INVALID_PARAM:
			Result = "LIBUSB_ERROR_INVALID_PARAM - Invalid parameter.";
			break;

	case LIBUSB_ERROR_ACCESS:
			Result = "LIBUSB_ERROR_ACCESS - Access denied (insufficient permissions)";
			break;

	case LIBUSB_ERROR_NO_DEVICE:
			Result = "LIBUSB_ERROR_NO_DEVICE - No such device (it may have been disconnected)";
			break;

	case LIBUSB_ERROR_NOT_FOUND:
			Result = "LIBUSB_ERROR_NOT_FOUND - Entity not found.";
			break;

	case LIBUSB_ERROR_BUSY:
			Result = "LIBUSB_ERROR_BUSY - Resource busy.";
			break;

	case LIBUSB_ERROR_TIMEOUT:
			Result = "LIBUSB_ERROR_TIMEOUT - Operation timed out.";
			break;

	case LIBUSB_ERROR_OVERFLOW:
			Result = "LIBUSB_ERROR_OVERFLOW - Overflow.";
			break;

	case LIBUSB_ERROR_PIPE:
			Result = "LIBUSB_ERROR_PIPE - Pipe error.";
			break;

	case LIBUSB_ERROR_INTERRUPTED:
			Result = "LIBUSB_ERROR_INTERRUPTED - System call interrupted (perhaps due to signal)";
			break;

	case LIBUSB_ERROR_NO_MEM:
			Result = "LIBUSB_ERROR_NO_MEM - Insufficient memory.";
			break;

	case LIBUSB_ERROR_NOT_SUPPORTED:
			Result = "LIBUSB_ERROR_NOT_SUPPORTED - Operation not supported or unimplemented on this platform.";
			break;

	case LIBUSB_ERROR_OTHER:
			Result = "LIBUSB_ERROR_OTHER - Other error.";
			break;
	}

	return Result;
}

const char* LibUSB::LibUSBException::what()
{

	std::stringstream errStr;
	errStr <<  exception::what() << this->translateError(m_ErrorCode);
	return errStr.str().c_str();
}
