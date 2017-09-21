#include "usb_endpoint.h"
#include "usb_queue.h"

#define QUEUE_ALIGNMENT 64
#define DEFAULT_ALIGNMENT 4


bool usb_endpoint_alloc_queue(USBEndpoint *endpoint, size_t pool_size, Alloc_cb alloc_cb);


bool usb_endpoint_is_in(const uint_fast8_t endpoint_address) 
{
    return (endpoint_address & 0x80) ? true : false;
}

USBEndpoint *usb_endpoint_create(
    uint8_t bEndpointAddress,
    USBDevice *device, 
    Endpoint_cb setup_complete, 
    Endpoint_cb transfer_complete,
    size_t pool_size,
    Alloc_cb alloc_cb)
{

    USBEndpoint *endpoint = alloc_cb(sizeof(USBEndpoint), DEFAULT_ALIGNMENT);
    if (!endpoint) {
        return NULL;
    }

    endpoint->address = bEndpointAddress;
    endpoint->device = device;
   
    endpoint->setup_complete = setup_complete;
    endpoint->transfer_complete = transfer_complete;

    // if IN endpoint
    if (usb_endpoint_is_in(endpoint->address)) {
        endpoint->in =  endpoint;
        endpoint->out = NULL; 
    } else {
        endpoint->in = NULL;
        endpoint->out = endpoint;
    }

    if (!usb_endpoint_alloc_queue(endpoint, pool_size, alloc_cb)) {
        return NULL;
    }

    return endpoint;
}

bool usb_pair_endpoints(USBEndpoint *ep_a, USBEndpoint *ep_b)
{
    bool success = false;
    if (ep_a && ep_b) {
        if (usb_endpoint_is_in(ep_a->address) && !usb_endpoint_is_in(ep_b->address)) {
            ep_a->out = ep_b;
            ep_b->in = ep_a;
            success = true;
        } else if (!usb_endpoint_is_in(ep_a->address) && usb_endpoint_is_in(ep_b->address)) {
            ep_a->in = ep_b;
            ep_b->out = ep_a;
            success = true;
        }
    }

    return success;
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


USBTransferType usb_endpoint_descriptor_transfer_type(
	const USBDescriptorEndpoint* const endpoint_descriptor
) {
	return (endpoint_descriptor->bmAttributes & 0x3);
}

bool usb_endpoint_alloc_queue(USBEndpoint *endpoint, size_t pool_size, Alloc_cb alloc_cb)    
{
    USBTransfer *transfers = alloc_cb(sizeof(USBTransfer) * pool_size, QUEUE_ALIGNMENT);
    if (!transfers) {
        return false;
    }
    USBQueue *queue = alloc_cb(sizeof(USBQueue), DEFAULT_ALIGNMENT);
    if (!queue) {
        return false;
    }
    queue->endpoint = endpoint;
    queue->free_transfers = transfers;
    queue->pool_size = pool_size;
    usb_queue_init(queue);
    return true;
}