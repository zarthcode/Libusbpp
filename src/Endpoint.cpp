#include "Endpoint.h"
#include "EndpointImpl.h"

#include <stdexcept>

LibUSB::Endpoint::Endpoint( std::shared_ptr<EndpointImpl> pImpl )
	: m_pEndpointImpl(pImpl)
{

	
}

LibUSB::Endpoint::~Endpoint()
{

	// Abort all transfers.

}

int LibUSB::Endpoint::Number() const
{

	return m_pEndpointImpl->Number();
}

LibUSB::Direction_t LibUSB::Endpoint::Direction() const
{

	return m_pEndpointImpl->Direction();
}

LibUSB::Transfer_t LibUSB::Endpoint::TransferType() const
{

	return m_pEndpointImpl->TransferType();
}

LibUSB::Synchronization_t LibUSB::Endpoint::SynchronizationType() const
{

	return m_pEndpointImpl->SynchronizationType();
}

LibUSB::Usage_t LibUSB::Endpoint::UsageType() const
{

	return m_pEndpointImpl->UsageType();
}

int LibUSB::Endpoint::MaxPacketSize() const
{

	return m_pEndpointImpl->MaxPacketSize();
}

int LibUSB::Endpoint::PollingInterval() const
{

	return m_pEndpointImpl->PollingInterval();
}


std::shared_ptr<LibUSB::Transfer> LibUSB::Endpoint::CreateTransfer()
{
	return m_pEndpointImpl->CreateTransfer();
}

uint8_t LibUSB::Endpoint::Address() const
{

	return m_pEndpointImpl->Address();
}
