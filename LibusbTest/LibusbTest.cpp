// LibusbTest.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include "libusbpp.h"
#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <ios>

int _tmain(int argc, _TCHAR* argv[])
{

// TEST 1 - Enumerate all attached devices.

	// Get a list of all devices
	std::list<std::shared_ptr<LibUSB::Device>> deviceList = LibUSB::LibUSB::FindAllDevices();


	
	// Open the device.
	BOOST_FOREACH(std::shared_ptr<LibUSB::Device> pDevice, deviceList)
	{
		try
		{

			std::wcout << L"\nDevice\n\tVendorID: 0x" << std::hex << pDevice->vendorID() << std::endl;
			std::wcout << L"\tProductID: 0x" << std::hex << pDevice->productID() << std::endl;

			pDevice->Open();

			// Get the manufacturer, product, and serial.
			std::wcout << L"\tManufacturer: " << pDevice->ManufacturerString() << std::endl;
			std::wcout << L"\tProduct: " << pDevice->ProductString() << std::endl;
			std::wcout << L"\tSerial: " << pDevice->SerialString() << std::endl << std::endl;

			// Obtain a configuration descriptor
			std::shared_ptr<LibUSB::Configuration> pActiveConfiguration = pDevice->getActiveConfiguration();

			if (pActiveConfiguration.get() == nullptr)
			{
				std::wcout << std::endl << L"\tDevice is NOT configured." << std::endl;
			}
			else
			{

				std::wcout << std::endl << L"\tActive Configuration:" << std::endl;
				std::wcout << std::endl << L"\t\tValue: " << pActiveConfiguration->Value() << std::endl;
				std::wcout << std::endl << L"\t\tDescription: " << pActiveConfiguration->DescriptorString() << std::endl;
				std::wcout << std::endl << L"\t\tMax Power: " << std::dec << pActiveConfiguration->MaxPower() << "mA" << std::endl;
				std::wcout << std::endl << L"\t\tIs " << (pActiveConfiguration->isSelfPowered() ? "" : "NOT ") << "Self-powered." << std::endl;
				std::wcout << std::endl << L"\t\tDoes " << (pActiveConfiguration->supportsRemoteWakeup() ? "" : "NOT ") << "support remote wakeup." << std::endl;
				std::wcout << std::endl << L"\t\tDoes " << (pActiveConfiguration->hasExtraDescriptors() ? "" : "NOT ") << "have extra descriptors." << std::endl;

			}

		}
		catch (std::exception& e)
		{
			std::wcout << L"\nException Thrown: \n\t" << e.what() << std::endl;
		}
		
	}

	// wait for user input.
	int a;
	std::cin >> a;
	return 0;
}

