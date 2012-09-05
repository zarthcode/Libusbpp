#pragma once

#include "Device.h"
#include <libusb/libusb.h>
#include <memory>
#include <string>
#include <map>

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

	class DeviceImpl : public std::enable_shared_from_this<DeviceImpl>
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

		/// Returns the index of the active configuration.
		bool getActiveConfiguration( uint8_t &index )const;

		/// Sets the active configuration
		void setActiveConfiguration( uint8_t index );

		/// Returns the requested the configuration descriptor.
		std::shared_ptr<Configuration> getConfiguration(uint8_t ConfigValue);

		/// Device object
		std::shared_ptr<libusb_device> m_pDevice;

		/// Device handle
		std::shared_ptr<libusb_device_handle> m_pHandle;


		/// Returns the LibUSB++ device object/owner
		std::weak_ptr<Device> getDevice()const;

		/// Allows device to set the parent device after initial construction.
		void setParentDevice(std::weak_ptr<Device> pParentDevice);

		/// Returns endpoint 0
		std::shared_ptr<Endpoint> getControlEndpoint();

	private:

	
		/// Weak_ptr collection of other configuration objects.
		std::map<uint8_t, std::weak_ptr<Configuration>> m_ConfigurationMap;

		/// Device Descriptor
		std::shared_ptr<libusb_device_descriptor> m_pDeviceDescriptor;

		/// Libusb++ device/parent
		std::weak_ptr<Device> m_ParentDevice;

		/// Language ID
		uint16_t languageId;

		/// Dummy descriptor for Endpoint zero.
		libusb_endpoint_descriptor m_EndpointZeroDescriptor;

		std::shared_ptr<Endpoint> m_pEndpointZero;

	};



}