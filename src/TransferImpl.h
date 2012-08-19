#pragma once

#include <memory>
#include <libusb/libusb.h>

namespace LibUSB
{

	class TransferDeleter
	{
	public:
		void operator()(libusb_transfer* pTransfer) { libusb_free_transfer(pTransfer); };

	};

	class TransferImpl
	{
	public:

		TransferImpl();
		virtual ~TransferImpl();

	protected:

		/// 

		/// Transfer buffer
		std::shared_ptr<char> m_pTransferBuffer;

		/// Transfer buffer size
		size_t m_BufferSize;

		/// LibUSB transfer struct
		std::shared_ptr<libusb_transfer> m_pTransfer;
	};

}