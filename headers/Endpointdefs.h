#pragma once

namespace LibUSB
{

	/// Direction Type
	typedef enum { DIR_OUT, DIR_IN } Direction_t;

	/// Transfer Types
	typedef enum { CONTROL, ISOCHRONOUS, BULK, INTERRUPT } Transfer_t;

	/// Synchronization Types 
	typedef enum { NONE, ASYNCRONOUS , ADAPTIVE, SYNCRONOUS } Synchronization_t;

	/// Usage Type
	typedef enum { DATA, FEEDBACK, EXPLICITFEEDBACKDATA, RESERVED } Usage_t;


}