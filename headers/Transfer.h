#pragma once

#include <memory>


namespace LibUSB
{
	class TransferImpl;

	/// USB Data transfer object.
	class Transfer
	{

	public:
		
		/// Constructor
		Transfer(std::shared_ptr<TransferImpl> pTransferImpl);

		/// Destructor
		virtual ~Transfer();

		/// Returns TRUE if the transfer is complete.
		bool Complete()const;

		/// Sets transfer buffer and size
		void SetBuffer(std::shared_ptr<char> pBuffer, size_t bufferSize);

		/// Returns transfer buffer
		std::shared_ptr<char> getBuffer();

		/// Returns the size of the data written to the buffer.


		/// Starts the transfer
		virtual void Start();

	protected:

	private:

		/// Transfer Implementation
		std::shared_ptr<TransferImpl> m_pTransferImpl;
	};

}