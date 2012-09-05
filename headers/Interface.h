#pragma once

#include <memory>
#include <string>

#include "Endpoint.h"
#include <stdint.h>

namespace LibUSB
{

	/// Interface implementation
	class InterfaceImpl;

	/// USB Interface class
	class Interface
	{
	public:
		Interface(std::shared_ptr<InterfaceImpl> pImpl);
		~Interface();

		/// Returns the interface number
		int Number()const;

		/// Returns the alternate setting for this interface
		int AlternateSetting()const;

		/// Returns the interface class
		int Class()const;

		/// Returns the interface Subclass
		int SubClass()const;

		/// Returns the interface Protocol
		int Protocol()const;

		/// Returns a string descriptor for this interface.
		std::wstring DescriptorString()const;

		/// Returns the number of available alternate settings for this interface.
		int NumAlternateSettings()const;

		/// Claims this interface for use.
		void Claim();

		/// Returns TRUE if the interface is claimed.
		bool isClaimed()const;

		/// Releases this interface
		void Release();

		/// Sets the requested alternate setting
		void SetAlternate(uint8_t AlternateSetting = 0);

		/// Returns the number of endpoints this interface has.
		int NumEndpoints()const;

		/// Returns the specified endpoint
		std::shared_ptr<Endpoint> getEndpoint(int index);
		
	protected:
	private:


		/// Interface implementation object.
		std::shared_ptr<InterfaceImpl> m_pInterfaceImpl;
	};

}