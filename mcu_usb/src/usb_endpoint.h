#ifndef USB_ENDPOINT_H
#define USB_ENDPOINT_H

#include <stdint.h>
#include <stddef.h>
#include "mcu_usb.h"



const USBDescriptorEndpoint* usb_endpoint_descriptor(
	const USBEndpoint* const endpoint
);

uint_fast16_t usb_endpoint_descriptor_max_packet_size(
	const USBDescriptorEndpoint* const endpoint_descriptor
);

USBTransferType usb_endpoint_descriptor_transfer_type(
	const USBDescriptorEndpoint* const endpoint_descriptor
);

#endif