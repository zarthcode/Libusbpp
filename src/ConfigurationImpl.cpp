#include "ConfigurationImpl.h"
#include "deviceimpl.h"
#include "usbexception.h"
#include <stdexcept>

LibUSB::ConfigurationImpl::ConfigurationImpl( libusb_config_descriptor* pConfigDescriptor, std::weak_ptr<DeviceImpl> pDeviceImpl)
{

	// Store the config descriptor.
	m_pConfigDescriptor.reset(pConfigDescriptor, ConfigDescriptorDeleter());

	// Store parent device implementation.
	if(pDeviceImpl.expired())
	{
		throw std::runtime_error("ConfigurationImpl constructor has an expired DeviceImpl.");
	}

	m_pDeviceImpl = pDeviceImpl;

}

LibUSB::ConfigurationImpl::~ConfigurationImpl()
{

}

std::wstring LibUSB::ConfigurationImpl::DescriptorString( void ) const
{

	if(m_pDeviceImpl.expired())
	{
		throw std::runtime_error("LibUSB::ConfigurationImpl::DescriptorString() has an expired DeviceImpl");
	}

	if (m_pConfigDescriptor->iConfiguration == 0)
	{
		// There is no string descriptor
		return L"[No descriptor for this configuration available.]";
	}


	std::wstring resultStr;
	
	try
	{
		resultStr = m_pDeviceImpl.lock()->getStringDescriptorW(m_pConfigDescriptor->iConfiguration);
	}
	catch(LibUSB::LibUSBException &e)
	{
		/// \todo Add option/setting to ignore certain exceptions.

#ifdef LIBUSBPP_IGNORE_CONFIGDESCRIPTORSTRING_EXCEPTIONS
		throw e;
#endif // LIBUSBPP_IGNORE_CONFIGDESCRIPTORSTRING_EXCEPTIONS
	}


}

uint8_t LibUSB::ConfigurationImpl::getValue() const
{

	return m_pConfigDescriptor->bConfigurationValue;

}

uint8_t LibUSB::ConfigurationImpl::getMaxPower() const
{

	return m_pConfigDescriptor->MaxPower;
	
}

bool LibUSB::ConfigurationImpl::isSelfPowered() const
{
	// Return bit 6 of bmAttributes.
	return (1 == ((m_pConfigDescriptor->bmAttributes & (1 << 6)) >> 6));
}

bool LibUSB::ConfigurationImpl::supportsRemoteWakeup() const
{
	// Return bit 5 of bmAttributes
	return (1 == ((m_pConfigDescriptor->bmAttributes & (1 << 5)) >> 5));
}

bool LibUSB::ConfigurationImpl::hasExtraDescriptors() const
{
	return (m_pConfigDescriptor->extra_length > 0);
}

const unsigned char* LibUSB::ConfigurationImpl::getExtraDescriptors() const
{
	return m_pConfigDescriptor->extra;
}

int LibUSB::ConfigurationImpl::getExtraDescriptorSize() const
{
	return m_pConfigDescriptor->extra_length;
}

void LibUSB::ConfigurationImpl::SetAsActive()
{

	if(m_pDeviceImpl.expired())
	{
		throw std::runtime_error("LibUSB::ConfigurationImpl::SetAsActive() has an expired DeviceImpl");
	}

	m_pDeviceImpl.lock()->setActiveConfiguration(m_pConfigDescriptor->bConfigurationValue);


}

