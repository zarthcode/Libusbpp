#pragma once

#include "Interfaceimpl.h"
#include "deviceimpl.h"
#include "Endpoint.h"
#include "EndpointImpl.h"
#include "usbexception.h"
#include <sstream>

LibUSB::InterfaceImpl::InterfaceImpl( const libusb_interface* pInterface, std::weak_ptr<DeviceImpl> pDeviceImpl )
	: m_pInterface(pInterface), m_alternateSetting(0), m_bClaimed(false)
{

	if (!pDeviceImpl.expired())
	{
		m_pDeviceImpl = pDeviceImpl;
	} 
	else
	{
		throw std::logic_error("LibUSB::InterfaceImpl::InterfaceImpl() constructed with expired DeviceImpl.");
	}

}

LibUSB::InterfaceImpl::~InterfaceImpl()
{
	// Ensure all endpoints are released.
	
	// Release the device

	
	try
	{
		Release();
	}
	catch (...)
	{
		/// We're not going to throw, in this particular situation.
	}
}

uint8_t LibUSB::InterfaceImpl::Number() const
{

	// Return the interface number of the first entry.
	return m_pInterface->altsetting[0].bInterfaceNumber;

}

uint8_t LibUSB::InterfaceImpl::AlternateSetting() const
{

	// Paranoid much?
	if (m_pInterface->altsetting[m_alternateSetting].bAlternateSetting != m_alternateSetting)
	{
		throw std::logic_error("LibUSB::InterfaceImpl::AlternateSetting() totally expected bAlternateSetting to always match index. McK is not impressed.");
	}

	return m_alternateSetting;

}

uint8_t LibUSB::InterfaceImpl::Class() const
{

	return m_pInterface->altsetting[m_alternateSetting].bInterfaceClass;

}

uint8_t LibUSB::InterfaceImpl::SubClass() const
{

	return m_pInterface->altsetting[m_alternateSetting].bInterfaceSubClass;

}

uint8_t LibUSB::InterfaceImpl::Protocol() const
{

	return m_pInterface->altsetting[m_alternateSetting].bInterfaceProtocol;

}

std::wstring LibUSB::InterfaceImpl::DescriptorString() const
{

	// Get the index
	if (m_pInterface->altsetting[m_alternateSetting].iInterface == 0)
	{
		// No string is available.
		return L"No description.";
	}

	// Return the string
	if (m_pDeviceImpl.expired())
	{
		throw std::logic_error("LibUSB::InterfaceImpl::DescriptorString() constructed with expired DeviceImpl.");
	}

	return m_pDeviceImpl.lock()->getStringDescriptorW(m_pInterface->altsetting[m_alternateSetting].iInterface);

}

uint8_t LibUSB::InterfaceImpl::NumAlternateSettings() const
{

	return m_pInterface->num_altsetting;

}

void LibUSB::InterfaceImpl::Claim()
{
	
	

	if (m_pDeviceImpl.expired())
	{
		throw std::logic_error("LibUSB::InterfaceImpl::Claim() - called with expired DeviceImpl.");
	}

	// Claim the interface
	int Result = libusb_claim_interface(m_pDeviceImpl.lock()->m_pHandle.get(), m_pInterface->altsetting[m_alternateSetting].bInterfaceNumber);

	switch(Result)
	{
	case LIBUSB_SUCCESS:
		{
			m_bClaimed = true;

			SetAlternate(m_alternateSetting);

		}
		break;

	case LIBUSB_ERROR_NOT_FOUND:
		// The requested interface does not exist.
		throw std::runtime_error("LibUSB::InterfaceImpl::Claim() - The requested interface does not exist.");
		break;

	case LIBUSB_ERROR_NO_DEVICE:
		// The device has been disconnected.
		throw std::runtime_error("LibUSB::InterfaceImpl::Claim() - Device has been disconnected");
		break;

	default:

		throw LibUSBException("LibUSB::InterfaceImpl::Claim() failed. ", Result);
		break;

	}

	
	
}

void LibUSB::InterfaceImpl::Release()
{

	if (!m_bClaimed)
	{
		return;
	}

	if (m_pDeviceImpl.expired())
	{
		throw std::logic_error("LibUSB::InterfaceImpl::Release() called with expired DeviceImpl.");
	}

	if (m_pDeviceImpl.lock()->m_pHandle.get() == nullptr)
	{
		// There is no device handle.
		throw std::logic_error("LibUSB::InterfaceImpl::Release() called with no device handle.");
	}


	int Result = libusb_release_interface(m_pDeviceImpl.lock()->m_pHandle.get(), m_pInterface->altsetting[m_alternateSetting].bInterfaceNumber);

	switch(Result)
	{


	case LIBUSB_ERROR_NOT_FOUND:
		// The interface was not claimed.
		throw std::runtime_error("LibUSB::InterfaceImpl::Release() - The interface was not claimed successfully.");
		break;
	case LIBUSB_SUCCESS:
		// Done.
		
		break;

	case LIBUSB_ERROR_NO_DEVICE:
		// The device has been disconnected. (Is this a Bad Thing™ in this situation?)
		throw std::runtime_error("LibUSB::InterfaceImpl::Release() - Device has been disconnected");
		break;

	default:

		throw LibUSBException("LibUSB::InterfaceImpl::Claim() failed. ", Result);
		break;

	}

	m_bClaimed = false;
	
}

void LibUSB::InterfaceImpl::SetAlternate( uint8_t AlternateSetting /*= 0*/ )
{

	// Validate the request.
	if (AlternateSetting >= NumAlternateSettings())
	{
		throw std::logic_error("Requested alternate setting not within expected range.");
	}

	if (AlternateSetting != m_alternateSetting)
	{
		// Reset endpoint objects!!
		ReleaseEndpoints();
	}

	m_alternateSetting = AlternateSetting;


	if (m_bClaimed)
	{
	
		// Set the alternate setting
		int Result = libusb_set_interface_alt_setting(m_pDeviceImpl.lock()->m_pHandle.get(), m_pInterface->altsetting[m_alternateSetting].bInterfaceNumber, m_pInterface->altsetting[m_alternateSetting].bAlternateSetting);

		switch (Result)
		{
		case LIBUSB_SUCCESS:
			// No error.
			break;
		case LIBUSB_ERROR_NOT_FOUND:
			// Invalid alt setting
			throw std::logic_error("LibUSB::InterfaceImpl::SetAlternate() - Requested alt setting not found.");
			break;
		case LIBUSB_ERROR_NO_DEVICE:
			throw std::runtime_error("LibUSB::InterfaceImpl::SetAlternate() - device disconnected.");
			break;
		default:
			throw LibUSB::LibUSBException("LibUSB::InterfaceImpl::SetAlternate() ", Result);
			break;
		}

	}



	CreateEndpoints();

}

uint8_t LibUSB::InterfaceImpl::NumEndpoints() const
{

	return m_pInterface->altsetting[m_alternateSetting].bNumEndpoints;
}

std::shared_ptr<LibUSB::Endpoint> LibUSB::InterfaceImpl::getEndpoint( uint8_t index )
{
	if (index == 0)
	{

		// Return the device control endpoint zero.
	
		if (m_pDeviceImpl.expired())
		{
			throw std::logic_error("LibUSB::InterfaceImpl::getEndpoint(0) - has an expired DeviceImpl.");
		}

		return m_pDeviceImpl.lock()->getControlEndpoint();

	}

	if (index > m_pInterface->altsetting[m_alternateSetting].bNumEndpoints)
	{
		throw std::logic_error("LibUSB::InterfaceImpl::getEndpoint() - index out of range.");
	}


	// Find the endpoint
	EndpointContainer_t::iterator itEndpoint = m_EndpointContainer.find(index);

	std::shared_ptr<LibUSB::Endpoint> pEndpoint;

	if (itEndpoint != m_EndpointContainer.end())
	{

		pEndpoint = m_EndpointContainer.find(index)->second;
		
		/// \note #1 Validate endpoint number against its index.
		if (pEndpoint->Number() != index)
		{
			throw std::logic_error("LibUSB::InterfaceImpl::getEndpoint() - endpoint and index do not match as expected! (note #1)");
		}

	}
	else
	{
		throw std::logic_error("LibUSB::InterfaceImpl::getEndpoint() - endpoint not found.");
	}
	

	return pEndpoint;
}

void LibUSB::InterfaceImpl::ReleaseEndpoints()
{

	// Each Endpoint must be released/destroyed.
	EndpointContainer_t::iterator itEndpoint = m_EndpointContainer.begin();
	while (itEndpoint != m_EndpointContainer.end())
	{
		// There must be no other references to the endpoint.
		if (m_EndpointContainer.begin()->second.unique())
		{
			// Release 
			m_EndpointContainer.erase(itEndpoint++);

		}
		
	}

	
	if (!m_EndpointContainer.empty())
	{
		
		// Unreleased objects remain - this is a problem.

		std::stringstream errorStr;
		errorStr << "LibUSB::InterfaceImpl::ReleaseEndpoints() - Could not release " << m_EndpointContainer.size()
			<< " Endpoints objects.";

		
		throw std::logic_error(errorStr.str());

	}

}

void LibUSB::InterfaceImpl::CreateEndpoints()
{

	if (m_EndpointContainer.empty())
	{

		// Make sure deviceimpl is still current.
		if (m_pDeviceImpl.expired())
		{
			throw std::logic_error("LibUSB::InterfaceImpl::CreateEndpoints() - DeviceImpl is expired.");
		}

		// Create each endpoint
		for (int i = 0; i < NumEndpoints(); i++)
		{

			// Create the endpoint implementation
			std::shared_ptr<EndpointImpl> pEPImpl = std::make_shared<EndpointImpl>(&(m_pInterface->altsetting[m_alternateSetting].endpoint[i]), m_pDeviceImpl);

			// Create the endpoint
			std::shared_ptr<Endpoint> pEndpoint = std::make_shared<Endpoint>(pEPImpl);

			// Store it
			m_EndpointContainer.insert(std::make_pair(pEndpoint->Number(), pEndpoint));

		}

	}
	else
	{
		
		// I'm not positive that I want to disallow reusing already existing endpoints (ie, claim() the same interface 

	}


}

bool LibUSB::InterfaceImpl::isClaimed() const
{

	return m_bClaimed;
}
