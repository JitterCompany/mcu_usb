#ifndef __USB_STANDARD_REQUEST_H__
#define __USB_STANDARD_REQUEST_H__

#include "mcu_usb.h"

bool usb_set_configuration(
	USBDevice* const device,
	const uint_fast8_t configuration_number
);

#endif//__USB_STANDARD_REQUEST_H__