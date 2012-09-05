#pragma once
#include <memory>
#include <string>
#include <stdint.h>
#include "interface.h"

namespace LibUSB
{



	/// Configuration Implementation
	class ConfigurationImpl;

	class Configuration
	{
	public:
		Configuration(std::shared_ptr<ConfigurationImpl> pInit);
		~Configuration();

	// Configuration Descriptor Details.

		/// Returns the string descriptor describing this configuration
		std::wstring DescriptorString()const;

		/// Returns the identifier value of this configuration
		uint8_t Value()const;

		/// Returns the maximum amount of power this device will consume while fully operational. (mA)
		int MaxPower()const;

		/// Sets this configuration as the active configuration.
		void SetAsActive();

		/// Returns TRUE if the device is self powered.
		bool isSelfPowered()const;

		/// Returns TRUE if the device supports remote wakeup.
		bool supportsRemoteWakeup()const;

		/// Returns TRUE if there are extra descriptors present
		bool hasExtraDescriptors()const;

		/// Returns a pointer the the extra descriptors
		const unsigned char * getExtraDescriptors()const;

		/// Returns the size of the extra descriptors, in bytes.
		int getExtraDescriptorSize()const;

	// Interfaces

		/// Returns the number of interfaces supported by this configuration.
		int NumInterfaces()const;

		/// Returns the specified interface by index.
		std::shared_ptr<Interface> getInterfaceByIndex(int index)const;

		/// Returns the specified interface by Interface number.
		std::shared_ptr<Interface> getInterface(int InterfaceNumber)const;

	protected:


	private:

		/// Configuration Implementation
		std::shared_ptr<ConfigurationImpl> m_pConfigImpl_;
	};

}