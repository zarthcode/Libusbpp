#include "EndpointImpl.h"
#include "TransferImpl.h"
#include "deviceimpl.h"
#include <stdexcept>


LibUSB::EndpointImpl::EndpointImpl( const libusb_endpoint_descriptor* pDescriptor, std::weak_ptr<DeviceImpl> pDeviceImpl )
	: m_pEndpointDescriptor(pDescriptor), m_pDeviceImpl(pDeviceImpl)
{

	if (m_pDeviceImpl.expired())
	{
		throw std::logic_error("LibUSB::InterfaceImpl::CreateEndpoints() - DeviceImpl is expired.");
	}

}

LibUSB::EndpointImpl::~EndpointImpl()
{


}

uint8_t LibUSB::EndpointImpl::Address() const
{

	return m_pEndpointDescriptor->bEndpointAddress;

}


uint8_t LibUSB::EndpointImpl::Number() const
{

	// Retrieve bits 0-3 of the endpoint address
	return (m_pEndpointDescriptor->bEndpointAddress & 0x0F);

}

LibUSB::Direction_t LibUSB::EndpointImpl::Direction() const
{
	// Bit 7 holds the direction.
	bool bDirection = (((m_pEndpointDescriptor->bEndpointAddress & 0x80) >> 7) != 0);

	return bDirection ? Direction_t::DIR_IN : Direction_t::DIR_OUT;
}

LibUSB::Transfer_t LibUSB::EndpointImpl::TransferType() const
{
	// Bits 0..1 hold transfer type
	uint8_t transferType = (m_pEndpointDescriptor->bmAttributes & 0x03);

	return (Transfer_t)transferType;

}

LibUSB::Synchronization_t LibUSB::EndpointImpl::SynchronizationType() const
{

	if (TransferType() != Transfer_t::ISOCHRONOUS)
	{
		// This value shouldn't be checked.
		return NONE;
	}

	// Returns bits 2..3
	uint8_t synchtype = ((m_pEndpointDescriptor->bmAttributes & 0x0C) >> 2);

	return (Synchronization_t)synchtype;

}

LibUSB::Usage_t LibUSB::EndpointImpl::UsageType() const
{

	if (TransferType() != Transfer_t::ISOCHRONOUS)
	{
		// This value shouldn't be checked.
		return DATA;
	}

	// Returns bits 4..5
	uint8_t usagetype = ((m_pEndpointDescriptor->bmAttributes & 0x18) >> 3);

	return (Usage_t)usagetype;
}

uint16_t LibUSB::EndpointImpl::MaxPacketSize() const
{

	return m_pEndpointDescriptor->wMaxPacketSize;

}

uint8_t LibUSB::EndpointImpl::PollingInterval() const
{

	return m_pEndpointDescriptor->bInterval;

}


std::weak_ptr<LibUSB::DeviceImpl> LibUSB::EndpointImpl::getDeviceImpl() const
{

	if (m_pDeviceImpl.expired())
	{
		throw std::logic_error("LibUSB::EndpointImpl::getDeviceImpl() - expired DeviceImpl.");
	}

	return m_pDeviceImpl;
}

std::shared_ptr<LibUSB::Transfer> LibUSB::EndpointImpl::CreateTransfer()
{


	std::shared_ptr<LibUSB::Transfer> pTransferObj;

	// Create the proper transfer implementation
	switch (TransferType())
	{
	case Transfer_t::CONTROL:

		{

			std::shared_ptr<LibUSB::TransferImpl> pImpl = std::static_pointer_cast<LibUSB::TransferImpl>(std::make_shared<LibUSB::ControlTransferImpl>(shared_from_this()));

			pTransferObj = std::static_pointer_cast<LibUSB::Transfer>(std::make_shared<LibUSB::ControlTransfer>(pImpl));

			pTransferObj->Init();	// Required.

		}

		break;

	case Transfer_t::INTERRUPT:
		
		{

			std::shared_ptr<LibUSB::TransferImpl> pImpl = std::static_pointer_cast<LibUSB::TransferImpl>(std::make_shared<LibUSB::InterruptTransferImpl>(shared_from_this()));

			pTransferObj = std::static_pointer_cast<LibUSB::Transfer>(std::make_shared<LibUSB::InterruptTransfer>(pImpl));

			pTransferObj->Init();	// Required.

		}

		break;

	case Transfer_t::BULK:
		{

			std::shared_ptr<LibUSB::TransferImpl> pImpl = std::static_pointer_cast<LibUSB::TransferImpl>(std::make_shared<LibUSB::BulkTransferImpl>(shared_from_this()));

			pTransferObj = std::static_pointer_cast<LibUSB::Transfer>(std::make_shared<LibUSB::BulkTransfer>(pImpl));

			pTransferObj->Init();	// Required.

		}
		break;

	case Transfer_t::ISOCHRONOUS:
		{
			std::shared_ptr<LibUSB::TransferImpl> pImpl = std::static_pointer_cast<LibUSB::TransferImpl>(std::make_shared<LibUSB::IsochronousTransferImpl>(shared_from_this()));

			pTransferObj = std::static_pointer_cast<LibUSB::Transfer>(std::make_shared<LibUSB::IsochronousTransfer>(pImpl));

			pTransferObj->Init();	// Required.

		}
		break;

	default:

		throw std::logic_error("LibUSB::Endpoint::CreateTransfer() - Unrecognized transfer type!");
		break;
	}


	// Validate the transfer object
	if (pTransferObj.get() == nullptr)
	{
		throw std::logic_error("LibUSB::Endpoint::CreateTransfer() - Allocation failed!");
	}



	return pTransferObj;

	
}
