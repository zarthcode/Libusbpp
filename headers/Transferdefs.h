#pragma once

namespace LibUSB
{

	/// Control Request Direction
	typedef enum { HOST_TO_DEVICE, DEVICE_TO_HOST } DataTransferDirection_t;

	/// Control Request Type
	typedef enum { REQ_STANDARD, REQ_CLASS, REQ_VENDOR, REQ_RESERVED } RequestType_t;

	/// Control request recipient Type
	typedef enum { REC_DEVICE, REC_INTERFACE, REC_ENDPOINT, REC_OTHER } RequestRecipient_t;

	/// Transfer Results
	typedef enum { COMPLETED, FAILED, TIMED_OUT, CANCELLED, STALLED, NO_DEVICE, DATA_OVERFLOW} TransferResult_t;

}