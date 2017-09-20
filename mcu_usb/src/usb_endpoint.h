#ifndef USB_ENDPOINT_H
#define USB_ENDPOINT_H

#include "usb_type.h"
#include <stdint.h>
#include <stddef.h>

USBEndpoint* usb_endpoint_create(USBEndpoint *endpoint, uint8_t bEndpointAddress, 
    USBDevice *device, USBEndpoint *other_endpoint, 
    Endpoint_cb setup_complete, Endpoint_cb transfer_complete
    ); 

bool usb_endpoint_alloc_queue(USBEndpoint *endpoint, size_t pool_size, void*(*alloc_cb)(size_t));    

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