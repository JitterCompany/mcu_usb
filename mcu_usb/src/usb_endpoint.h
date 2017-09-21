#ifndef USB_ENDPOINT_H
#define USB_ENDPOINT_H

#include <stdint.h>
#include <stddef.h>
#include "mcu_usb.h"

bool usb_endpoint_is_in(const uint_fast8_t endpoint_address);
	
const USBDescriptorEndpoint* usb_endpoint_descriptor(
	const USBEndpoint* const endpoint
);

USBTransferType usb_endpoint_descriptor_transfer_type(
	const USBDescriptorEndpoint* const endpoint_descriptor
);

#endif