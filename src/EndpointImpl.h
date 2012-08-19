#pragma once

#include <libusb/libusb.h>
#include <memory>
#include <stdint.h>

#include "Endpointdefs.h"

namespace LibUSB
{
	/// Device Implementation
	class DeviceImpl;

	/// Endpoint Implementation
	class EndpointImpl
	{
	public:

		/// Constructor
		EndpointImpl(const libusb_endpoint_descriptor* pDescriptor, std::weak_ptr<DeviceImpl> pDeviceImpl);

		/// Destructor
		~EndpointImpl();

		/// Endpoint Address
		uint8_t Number()const;

		/// Returns the direction of this endpoint
		Direction_t Direction()const;

		/// Returns the transfer type of this endpoint
		Transfer_t TransferType()const;

		/// Returns the synchronization type (ISOCHRONOUS MODE ONLY!)
		Synchronization_t SynchronizationType()const;

		/// Returns the usage type (ISOCHRONOUS MODE ONLY!)
		Usage_t UsageType()const;

		/// Returns the maximum packet size for this endpoint
		uint16_t MaxPacketSize()const;

		/// Returns the polling interval (in frames, 1f = 1mS @ low/full speed, 125uS at high), ignore for bulk/control, 1 for iso, 1-255 for interrupt.
		uint8_t PollingInterval()const;

	// Data transfer methods.

	private:

		/// Endpoint Descriptor
		const libusb_endpoint_descriptor* m_pEndpointDescriptor;

		/// Weak pointer to the deviceimpl/parent
		std::weak_ptr<DeviceImpl> m_pDeviceImpl;

	};

}