#include "device.h"
#include "deviceimpl.h"


LibUSB::Device::Device( std::shared_ptr<DeviceImpl> pInit )
{
	
	pDeviceImpl_ = pInit;

	// Obtain the descriptor
	pDeviceImpl_->getDeviceDescriptor();

}

LibUSB::Device::~Device()
{



}

uint16_t LibUSB::Device::USBSpecification()
{

	return pDeviceImpl_->getDeviceDescriptor()->bcdUSB;

}

uint8_t LibUSB::Device::DeviceClass()
{
	return pDeviceImpl_->getDeviceDescriptor()->bDeviceClass;
}

uint8_t LibUSB::Device::DeviceSubclass()
{
	return pDeviceImpl_->getDeviceDescriptor()->bDeviceSubClass;
}

uint8_t LibUSB::Device::DeviceProtocol()
{
	return pDeviceImpl_->getDeviceDescriptor()->bDeviceProtocol;
}

uint16_t LibUSB::Device::vendorID()
{
	return pDeviceImpl_->getDeviceDescriptor()->idVendor;
}

uint16_t LibUSB::Device::productID()
{
	return pDeviceImpl_->getDeviceDescriptor()->idProduct;
}

uint8_t LibUSB::Device::NumConfigurations()
{
	return pDeviceImpl_->getDeviceDescriptor()->bNumConfigurations;
}

std::wstring LibUSB::Device::ProductString()
{

	// Device must be open
	if (!isOpen())
	{

		throw std::runtime_error("ProductString() - device must be open!");
		
	}

	// Validate the descriptor index
	if(pDeviceImpl_->getDeviceDescriptor()->iProduct == 0)
	{
		// throw std::runtime_error("Serial Number is not supported on this device.");
		return L"Not supported.";
	}

	// Return an product string.
	return pDeviceImpl_->getStringDescriptorW(pDeviceImpl_->getDeviceDescriptor()->iProduct);

}

std::wstring LibUSB::Device::ManufacturerString()
{

	// Device must be open
	if (!isOpen())
	{

		throw std::runtime_error("ProductString() - device must be open!");

	}

	// Validate the descriptor index
	if(pDeviceImpl_->getDeviceDescriptor()->iManufacturer == 0)
	{
		// throw std::runtime_error("Serial Number is not supported on this device.");
		return L"Not supported.";
	}

	// Return a manufacturer string.
	std::wstring resultStr = pDeviceImpl_->getStringDescriptorW(pDeviceImpl_->getDeviceDescriptor()->iManufacturer);

	return resultStr;

}

std::wstring LibUSB::Device::SerialString()
{

	// Device must be open
	if (!isOpen())
	{

		throw std::runtime_error("ProductString() - device must be open!");

	}

	// Validate the descriptor index
	if(pDeviceImpl_->getDeviceDescriptor()->iSerialNumber == 0)
	{
		// throw std::runtime_error("Serial Number is not supported on this device.");
		return L"Not supported.";
	}

	// Return a serial string.
	return pDeviceImpl_->getStringDescriptorW(pDeviceImpl_->getDeviceDescriptor()->iSerialNumber);

}

void LibUSB::Device::Open()
{

	pDeviceImpl_->Open();

}

bool LibUSB::Device::isOpen()
{
	return pDeviceImpl_->isOpen();
	
}




