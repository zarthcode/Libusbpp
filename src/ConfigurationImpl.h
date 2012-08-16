#pragma once
#include <memory>
#include <libusb/libusb.h>
#include <string>

namespace LibUSB
{
	/// Allows shared ptr to delete the configuration descriptor object.
	class ConfigDescriptorDeleter
	{
	public:
		void operator()(libusb_config_descriptor* configdescriptor) { libusb_free_config_descriptor(configdescriptor); };

	};

	/// DeviceImpl Forward Declaration
	class DeviceImpl;

	/// Configuration Implementation
	class ConfigurationImpl
	{
	public:
		
		/// Constructor - initializes 
		ConfigurationImpl(libusb_config_descriptor* pConfigDescriptor, std::weak_ptr<DeviceImpl> pParentDeviceImpl);
		~ConfigurationImpl();

		/// Returns the string descriptor describing this configuration
		std::wstring DescriptorString(void)const;

		/// Returns the identifier value of this configuration
		uint8_t getValue()const;

		/// Returns the maximum amount of power this device will consume while fully operational. (units of 2mA)
		uint8_t getMaxPower()const;

		/// Sets this configuration as the active configuration.
		void SetAsActive();

		/// Returns TRUE if the device is self powered.
		bool isSelfPowered()const;

		/// Returns TRUE if the device supports remote wakeup.
		bool supportsRemoteWakeup()const;

		/// Returns TRUE if there are extra descriptors present
		bool hasExtraDescriptors()const;

		/// Returns a pointer the the extra descriptors
		const unsigned char* getExtraDescriptors()const;

		/// Returns the size of the extra descriptors, in bytes.
		int getExtraDescriptorSize()const;

	protected:



	private:

		/// Configuration Descriptor
		std::shared_ptr<libusb_config_descriptor> m_pConfigDescriptor;

		/// Parent Device
		std::weak_ptr<DeviceImpl> m_pDeviceImpl;
	};

}