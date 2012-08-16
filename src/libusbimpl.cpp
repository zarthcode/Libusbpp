#include "libusbimpl.h"


/// Used by shared_ptr to delete a libusb context
class ContextDeleter
{
public:
	void operator()(libusb_context* ctx) { libusb_exit(ctx); };

};

LibUSB::LibUSBImpl::LibUSBImpl()
{
	// Create the libusb context
	libusb_context* pContext = nullptr;
	int Result = libusb_init(&pContext);
	if (Result != LIBUSB_SUCCESS)
	{
		throw std::exception("libusb_init() failed.");
	}

	// Store in a shared_ptr
	pLibusb_context.reset(pContext, ContextDeleter());


}

LibUSB::LibUSBImpl::~LibUSBImpl()
{

}

std::shared_ptr<libusb_context> LibUSB::LibUSBImpl::pLibusb_context;
