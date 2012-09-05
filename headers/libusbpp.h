#pragma once

#include "device.h"
#include "Transfer.h"
#include "Endpointdefs.h"
#include <memory>
#include <list>

namespace LibUSB
{

	/// LibUSBImpl forward declaration
	class LibUSBImpl;
	
	class LibUSB
	{

	public:

		/// Function pointer to a LibUSB device object factory.
		typedef std::shared_ptr<Device> (*DeviceFactory_t)(std::shared_ptr<DeviceImpl>);
	
		/*!
		 * \brief
		 *
		 * Returns a list of devices (that can be opened) that match the given vendor/product id.
		 *
		 * \param vendorID (uint16_t): USB-IF vendor id of the desired device.
		 * \param deviceID (uint16_t): USB-IF product id of the desired device.
		 * \returns (std::list<std::shared_ptr<D>>): List of shared pointers to LibUSB::Device class objects.
		 * \sa 
		 * \note
		 * \warning Multiple devices can be returned via this method, if attached.
		 */
		static std::list<std::shared_ptr<Device>> FindDevice(uint16_t vendorID, uint16_t productID, DeviceFactory_t factory = nullptr);

		/*!
		 * \brief
		 *
		 * Returns a list of devices (that can be opened) that match the given vendor/product id.
		 *
		 * \param vendorID (uint16_t): USB-IF vendor id of the desired device.
		 * \param deviceID (uint16_t): USB-IF product id of the desired device.
		 * \param serialStr (std::wstring): Device unique serial number
		 * \returns (std::list<std::shared_ptr<D>>): List of shared pointers to LibUSB::Device class objects.
		 * \sa 
		 * \note
		 * \warning Multiple devices can be returned via this method, if attached.
		 */
		static std::list<std::shared_ptr<Device>> FindDevice(uint16_t vendorID, uint16_t productID, std::wstring serialStr, DeviceFactory_t factory = nullptr);

		/// Returns all devices attached to the system.
		static std::list<std::shared_ptr<Device>> FindAllDevices(DeviceFactory_t factory = nullptr);

		/// Starts a dedicated event handler thread.
		static void StartEventHandlerThread();

		/// Signals the event hander thread to stop.
		static void StopEventHandlerThread(bool blockUntilStopped = false);

		/// Manually Performs event handling
		static void HandleEvents(bool bBlock = false);

		/// Enables/Disables Synchronous Transfers
		static void SetAsynchronousTransferMode(bool enable = true);

		/// Returns TRUE if transfers will run in async mode.
		static bool AsynchronousTransferMode();

	private:

		friend class TransferImpl;
		
		static void Initialize();


		/// LibUSBImpl Singleton object
		static std::shared_ptr<LibUSBImpl> Impl_;



	};


}