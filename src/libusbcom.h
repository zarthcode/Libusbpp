#include <libusb/libusb.h>


namespace LibUSB
{

	

	class DeviceHandleDeleter
	{
	public:
		void operator()(libusb_device_handle* devhandle) { libusb_close(devhandle); };

	};


	class ConfigDescriptorDeleter
	{
	public:
		void operator()(libusb_config_descriptor* configdescriptor) { libusb_free_config_descriptor(configdescriptor); };

	};

}