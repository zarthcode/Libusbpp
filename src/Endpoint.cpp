#include "Endpoint.h"
#include "EndpointImpl.h"

LibUSB::Endpoint::Endpoint( std::shared_ptr<EndpointImpl> pImpl )
	: m_pEndpointImpl(pImpl)
{

	
}

LibUSB::Endpoint::~Endpoint()
{

	// Abort all queued transfers.

}

