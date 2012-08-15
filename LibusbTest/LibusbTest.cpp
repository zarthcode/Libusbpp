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

