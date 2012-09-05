#pragma once
#include <libusb/libusb.h>
#include <string>
#include <memory>
#include <map>
#include "Endpoint.h"



namespace LibUSB
{

	/// Device Implementation
	class DeviceImpl;

	class InterfaceImpl
	{
	public:
		InterfaceImpl(const libusb_interface* pInterface, std::weak_ptr<DeviceImpl> pDeviceImpl);
		~InterfaceImpl();

		/// Returns the interface number
		uint8_t Number()const;

		/// Returns the alternate setting for this interface
		uint8_t AlternateSetting()const;

		/// Returns the interface class
		uint8_t Class()const;

		/// Returns the interface Subclass
		uint8_t SubClass()const;

		/// Returns the interface Protocol
		uint8_t Protocol()const;

		/// Returns a string descriptor for this interface.
		std::wstring DescriptorString()const;

		/// Returns the number of available alternate settings for this interface.
		uint8_t NumAlternateSettings()const;

		/// Claims this interface for use.
		void Claim();

		/// Releases this interface
		void Release();

		/// Returns TRUE if the interface is claimed
		bool isClaimed()const;
		
		/*!
		 * \brief Selects an alternate interface setting.
		 * 
		 *
		 * \param AlternateSetting (uint8_t): the index of the alternate setting to select/use.
		 * \throws (std::logic_error) if the alternate setting is out-of-range.
		 * \note
		 * \sa NumAlternateSettings() const
		 * \warning If the interface is currently claimed, changing the alternate setting will affect the endpoints.
		 */
		void SetAlternate(uint8_t AlternateSetting = 0);

		/// Returns the number of endpoints this interface/altsetting has.
		uint8_t NumEndpoints()const;

		/// Returns the specified endpoint
		std::shared_ptr<Endpoint> getEndpoint(uint8_t index);

	protected:

		/// Resets all endpoints
		void ReleaseEndpoints();

		/// Creates all endpoint objects.
		void CreateEndpoints();

	private:

		/// Current alternate setting
		uint8_t m_alternateSetting;

		/// Collection of alternate settings for THIS interface.
		const libusb_interface* m_pInterface;

		/// Weak pointer to the deviceimpl/parent
		std::weak_ptr<DeviceImpl> m_pDeviceImpl;

		/// Indicates that there has been through a successful Claim()
		bool m_bClaimed;

		/// Endpoint container type
		typedef std::map<uint8_t, std::shared_ptr<Endpoint>> EndpointContainer_t;

		/// Endpoint container
		EndpointContainer_t m_EndpointContainer;

	};

}