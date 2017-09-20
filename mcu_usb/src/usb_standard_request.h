#ifndef __USB_STANDARD_REQUEST_H__
#define __USB_STANDARD_REQUEST_H__

#include "usb_type.h"
#include "usb_request.h"

void usb_set_configuration_changed_cb(
        void (*callback)(USBDevice* const)
);

USBRequestStatus usb_standard_request(
	USBEndpoint* const endpoint,
	const USBTransferStage stage
);

bool usb_set_configuration(
	USBDevice* const device,
	const uint_fast8_t configuration_number
);

#endif//__USB_STANDARD_REQUEST_H__