#include "usb_endpoint.h"
#include "usb_queue.h"

USBEndpoint* usb_endpoint_create(USBEndpoint *endpoint, uint8_t bEndpointAddress, 
    USBDevice *device, USBEndpoint *other_endpoint, 
    Endpoint_cb setup_complete, Endpoint_cb transfer_complete
    ) 
{
    endpoint->address = bEndpointAddress;
    endpoint->device = device;
   
    endpoint->setup_complete = setup_complete;
    endpoint->transfer_complete = transfer_complete;

    // if IN endpoint
    if (endpoint->address & 0x80) {
        endpoint->in =  endpoint;
        endpoint->out = other_endpoint;
    } else {
        endpoint->in = other_endpoint;
        endpoint->out = endpoint;
    }

    return endpoint;
}

const USBDescriptorEndpoint *usb_endpoint_descriptor(
    const USBEndpoint *const endpoint)
{
    const USBConfiguration *const configuration = endpoint->device->configuration;
    if (configuration) {
        const USBDescriptorConfiguration *config_desc = configuration->descriptor;
        const USBDescriptorInterface *interface_desc = (USBDescriptorInterface*)((uint8_t*)config_desc + config_desc->bLength);
        const USBDescriptorEndpoint *ep_descriptor = (USBDescriptorEndpoint*)((uint8_t*)interface_desc + interface_desc->bLength);

        for (size_t i = 0; i < interface_desc->bNumEndpoints; i++) {
            if (ep_descriptor->bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT) {
                if (ep_descriptor->bEndpointAddress == endpoint->address) {
                    return ep_descriptor;
                }
            }
            ep_descriptor++;
        }
    }

    return 0;
}

uint_fast16_t usb_endpoint_descriptor_max_packet_size(
	const USBDescriptorEndpoint* const endpoint_descriptor
) {
	return endpoint_descriptor->wMaxPacketSize;
}

USBTransferType usb_endpoint_descriptor_transfer_type(
	const USBDescriptorEndpoint* const endpoint_descriptor
) {
	return (endpoint_descriptor->bmAttributes & 0x3);
}

bool usb_endpoint_alloc_queue(USBEndpoint *endpoint, size_t pool_size, void*(*alloc_cb)(size_t))    
{
    USBTransfer *transfers = alloc_cb(sizeof(USBTransfer) * pool_size);
    if (!transfers) {
        return false;
    }
    USBQueue *queue = alloc_cb(sizeof(USBQueue));
    if (!queue) {
        return false;
    }
    queue->endpoint = endpoint;
    queue->free_transfers = transfers;
    queue->pool_size = pool_size;
    usb_queue_init(queue);
    return true;
}