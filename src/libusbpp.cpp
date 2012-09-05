
#include "libusbpp.h"
#include "deviceimpl.h"
#include "libusbimpl.h"



std::list<std::shared_ptr<LibUSB::Device>> LibUSB::LibUSB::FindDevice( uint16_t vendorID, uint16_t productID, DeviceFactory_t factory )
{

	// Ensure libusb is initialized.
	Initialize();

	// Create a list of attached devices
	libusb_device **device_list = nullptr;

	ssize_t NumResults = libusb_get_device_list(Impl_->m_pLibusb_context.get(), &device_list);


	// Iterate each device.
	std::list<std::shared_ptr<Device>> deviceList;

	for (ssize_t i = 0; i < NumResults; i++)
	{

		// Create a device.
		std::shared_ptr<Device> pDevice;
		if(factory != nullptr)
		{
			
			pDevice = factory(std::make_shared<DeviceImpl>(device_list[i]));

		}
		else
		{
			pDevice.reset(new Device(std::make_shared<DeviceImpl>(device_list[i])));
		}

		pDevice->Init();

		// Check the device
		if ((pDevice->vendorID() == vendorID) && (pDevice->productID() == productID))
		{
			// Add device to the output list
			deviceList.push_back(pDevice);
		}

	}
	
	// Free the device list
	libusb_free_device_list(device_list, 1);

	return deviceList;

}

std::list<std::shared_ptr<LibUSB::Device>> LibUSB::LibUSB::FindDevice( uint16_t vendorID, uint16_t productID, std::wstring serialStr, DeviceFactory_t factory )
{

	// Get list of devices that match product/vendor id.
	std::list<std::shared_ptr<Device>> DeviceList = FindDevice(vendorID, productID, factory);

	std::list<std::shared_ptr<Device>> ResultList;

	// Open each device and locate a matching serial.
	for( std::shared_ptr<Device> &pDevice : DeviceList )
	{
		if(pDevice->SerialString() == serialStr)
		{
			
			ResultList.push_back(pDevice);
			break;
		}
	}
	
	return ResultList;
}

std::list<std::shared_ptr<LibUSB::Device>> LibUSB::LibUSB::FindAllDevices( DeviceFactory_t factory /*= nullptr*/ )
{

	// Ensure libusb is initialized.
	Initialize();

	// Create a list of attached devices
	libusb_device **device_list = nullptr;

	ssize_t NumResults = libusb_get_device_list(Impl_->m_pLibusb_context.get(), &device_list);


	// Iterate each device.
	std::list<std::shared_ptr<Device>> deviceList;

	for (ssize_t i = 0; i < NumResults; i++)
	{

		// Create a device.
		std::shared_ptr<Device> pDevice;
		if(factory != nullptr)
		{
			pDevice = factory(std::make_shared<DeviceImpl>(device_list[i]));
		}
		else
		{
			pDevice.reset(new Device(std::make_shared<DeviceImpl>(device_list[i])));
		}

		pDevice->Init();

		// Add device to the output list
		deviceList.push_back(pDevice);

	}

	// Free the device list
	libusb_free_device_list(device_list, 1);

	return deviceList;

}

void LibUSB::LibUSB::Initialize()
{
	// Ensure libusb is initialized.
	if (Impl_.get() == nullptr)
	{
		Impl_.reset(new LibUSBImpl());
	}
}

std::shared_ptr<LibUSB::LibUSBImpl> LibUSB::LibUSB::Impl_;
