#pragma once
#include "Configuration.h"

#include <stdint.h>
#include <string>
#include <memory>


namespace LibUSB
{

	/// Libusb Device Implementation Forward Declaration
	class DeviceImpl;

	/// Libusb device interface
	class Device
	{
	public:

		Device(std::shared_ptr<DeviceImpl> pInit);

		virtual ~Device();

	// Device State

		/// Returns TRUE if the device is open.
		bool isOpen();

		/// Opens the device.
		void Open();
	
	// Device Descriptor

		/// USB specification release number
		uint16_t USBSpecification();

		/// USB-IF class code
		uint8_t DeviceClass();

		/// USB-IF subclass code for the device, qualified by the bDeviceClass value. 
		uint8_t DeviceSubclass();

		/// USB-IF protocol code for the device, qualified by the bDeviceClass and bDeviceSubClass values. 
		uint8_t DeviceProtocol();

		// USB-IF vendor ID
		uint16_t vendorID();

		// USB-IF product ID
		uint16_t productID();

		/// Returns a string describing the product.
		std::wstring ProductString();

		/// Returns a string describing the manufacturer.
		std::wstring ManufacturerString();

		/// Returns the serial number string of the device.
		std::wstring SerialString();

		/// Returns the number of possible configurations for this device.
		uint8_t NumConfigurations();

	// Configurations

		/// Returns the requested configuration
		std::shared_ptr<Configuration> getConfiguration(uint8_t ConfigValue);

		/// Returns the activeConfiguration
		std::shared_ptr<Configuration> getActiveConfiguration();


	// Transfers

		/// Control transfer OUT

		/// Control transfer IN

		/// bulk transfer OUT (to the device)

		/// bulk transfer IN (from the device)

		/// interrupt transfer OUT (to the device)

		/// interrupt transfer IN (from the device)

		/// isochronous transfer OUT (to the device)

		/// isochronous transfer IN (from the device)

	protected:
		

	private:

		/// Cached Active Configuration
		std::shared_ptr<Configuration> m_pActiveConfiguration;

		/// Device Implementation Object
		std::shared_ptr<DeviceImpl> m_pDeviceImpl_;

		
		
	};

}