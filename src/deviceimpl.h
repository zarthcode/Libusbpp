#pragma once

#include <libusb/libusb.h>
#include <memory>
#include <string>

namespace LibUSB
{

	class DeviceDeleter
	{
	public:
		void operator()(libusb_device* dev) { libusb_unref_device(dev); };

	};

	class DeviceHandleDeleter
	{
	public:
		void operator()(libusb_device_handle* devhandle) { libusb_close(devhandle); };

	};

	class DeviceImpl
	{

	public:

		DeviceImpl(libusb_device* pDevice);
		~DeviceImpl();

		/// Obtains the device descriptor.
		std::shared_ptr<libusb_device_descriptor> getDeviceDescriptor();

		/// Returns TRUE if the device is open
		bool isOpen()const;

		/// Opens a handle to the usb device.
		void Open();

		/// Obtains the language id
		uint16_t getLangId();

		/// Obtains the given ascii descriptor string
		std::string getStringDescriptor(uint8_t index);

		/// Obtains a unicode descriptor string
		std::wstring getStringDescriptorW(uint8_t index);

		std::shared_ptr<libusb_device> m_pDevice;
		std::shared_ptr<libusb_device_handle> m_pHandle;

	private:

		std::shared_ptr<libusb_device_descriptor> deviceDescriptor;

		/// Language ID
		uint16_t languageId;

	};



}