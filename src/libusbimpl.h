#pragma once

#include <libusb/libusb.h>
#include <memory>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace LibUSB
{


	class LibUSBImpl
	{
	public:

		LibUSBImpl();
		~LibUSBImpl();

		static std::shared_ptr<libusb_context> m_pLibusb_context;
		

	};

}