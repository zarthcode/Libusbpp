#pragma once

#include "deviceimpl.h"
#include "usbexception.h"
#include <libusb/libusb.h>


LibUSB::DeviceImpl::DeviceImpl( libusb_device* device )
	: languageId(0)
{

	// Add a reference to pDevice and save it.
	m_pDevice.reset(libusb_ref_device(device), DeviceDeleter());

}


LibUSB::DeviceImpl::~DeviceImpl()
{

}

std::shared_ptr<libusb_device_descriptor> LibUSB::DeviceImpl::getDeviceDescriptor()
{

	if (deviceDescriptor.get() == nullptr)
	{
	
		deviceDescriptor.reset(new libusb_device_descriptor);

		// Obtain the usb device descriptors
		int Result = libusb_get_device_descriptor(m_pDevice.get(), deviceDescriptor.get());

		if (Result != LIBUSB_SUCCESS)
		{
			throw std::exception("libusb_get_device_descriptor() failed.");
		}
	
	}

	return deviceDescriptor;

}


bool LibUSB::DeviceImpl::isOpen() const
{

	return (m_pHandle.get() != nullptr);

}

void LibUSB::DeviceImpl::Open()
{

	if (m_pDevice.get() == nullptr)
	{
		throw std::exception("Open() failed - (There is no device!)");
	}

	if (m_pHandle.get() == nullptr)
	{

		// Open the device
		libusb_device_handle* pHandle;
		int Result = libusb_open(m_pDevice.get(), &pHandle);

		if (Result != LIBUSB_SUCCESS)
		{

			throw LibUSBException("libusb_open() failed.", Result);

		}

		m_pHandle.reset(pHandle, DeviceHandleDeleter());

	}
}

std::string LibUSB::DeviceImpl::getStringDescriptor( uint8_t index )
{


	unsigned char descStr[512];
	memset(descStr, 0, sizeof(descStr));

	int Result = libusb_get_string_descriptor_ascii(m_pHandle.get(), index, descStr, sizeof(descStr));

	if (Result < LIBUSB_SUCCESS)
	{
		throw LibUSBException("libusb_get_string_descriptor_ascii() failed.", Result);
	}

	std::string strResult;
	strResult.assign((const char*)descStr);

	return strResult;
}

std::wstring LibUSB::DeviceImpl::getStringDescriptorW( uint8_t index )
{
	unsigned char descStr[128];
	wmemset((wchar_t*)descStr, L'\0', sizeof(descStr)/sizeof(wchar_t));

	int Result = libusb_get_string_descriptor(m_pHandle.get(), index, getLangId(), (unsigned char*)descStr, sizeof(descStr));

	if (Result < LIBUSB_SUCCESS)
	{
		throw LibUSBException("libusb_get_string_descriptor() failed.", Result);
	}

	// First character is the size of the string descriptor, in bytes
	uint8_t descSize = descStr[0];

	// Second character is 0x03, always
	if (descStr[1] != 0x03)
	{
		throw std::runtime_error("USB string descriptor returned from device is invalid.");
	}



	std::wstring strResult;
	strResult.assign((const wchar_t*)descStr + 1, (descSize-2)/2);

	return strResult;
}

uint16_t LibUSB::DeviceImpl::getLangId()
{	

	/// \note This descriptor is described here: http://www.beyondlogic.org/usbnutshell/usb5.shtml

	if((languageId == 0) && isOpen())
	{
		uint8_t data[255];
		memset(data, '\0', sizeof(data));
		int Result = libusb_get_string_descriptor(m_pHandle.get(), 0, 0, data, sizeof(data));
		if (Result < LIBUSB_SUCCESS)
		{
			throw LibUSBException("libusb_get_string_descriptor() failed.", Result);
		}
		
		// First element is the size of the descriptor, in bytes
		uint8_t descriptorSize = data[0];

		// Second element should be 0x03
		if (data[1] != 0x03)
		{
			throw std::runtime_error("USB language string descriptor (index 0) is invalid.");
		}
		
		// Grab the first/default language.
		languageId = data[2] | data[3]<<8;

	}

	return languageId;

}
