#pragma once

#include "Interface.h"
#include "Interfaceimpl.h"

#include <libusb/libusb.h>


LibUSB::Interface::Interface( std::shared_ptr<InterfaceImpl> pImpl )
	:m_pInterfaceImpl(pImpl)
{
	
}


LibUSB::Interface::~Interface()
{

	// Destroy Implementation.
	m_pInterfaceImpl.reset();

}


int LibUSB::Interface::Number() const
{
	return m_pInterfaceImpl->Number();
}

int LibUSB::Interface::AlternateSetting() const
{
	return m_pInterfaceImpl->AlternateSetting();
}

int LibUSB::Interface::Class() const
{
	return m_pInterfaceImpl->Class();
}

int LibUSB::Interface::SubClass() const
{
	return m_pInterfaceImpl->SubClass();
}

int LibUSB::Interface::Protocol() const
{
	return m_pInterfaceImpl->Protocol();
}

std::wstring LibUSB::Interface::DescriptorString() const
{
	return m_pInterfaceImpl->DescriptorString();
}

int LibUSB::Interface::NumAlternateSettings() const
{
	return m_pInterfaceImpl->NumAlternateSettings();
}

void LibUSB::Interface::Claim()
{

	m_pInterfaceImpl->Claim();

}

void LibUSB::Interface::Release()
{
	m_pInterfaceImpl->Release();
}

void LibUSB::Interface::SetAlternate( uint8_t AlternateSetting /*= 0*/ )
{
	m_pInterfaceImpl->SetAlternate(AlternateSetting);
}

int LibUSB::Interface::NumEndpoints() const
{
	return m_pInterfaceImpl->NumEndpoints();
}

std::shared_ptr<LibUSB::Endpoint> LibUSB::Interface::getEndpoint( int index )
{
	return m_pInterfaceImpl->getEndpoint(index);
}

