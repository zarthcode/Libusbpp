#pragma once

#include <libusb/libusb.h>
#include <memory>


namespace LibUSB
{

	class LibUSBImpl
	{
	public:

		LibUSBImpl();
		~LibUSBImpl();

		static std::shared_ptr<libusb_context> pLibusb_context;

		// Find Device
	};

}