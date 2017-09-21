#ifndef __MCU_USB_H__
#define __MCU_USB_H__

//#include "src/usb_request.h"
//#include "src/usb_queue.h"

#include "src/usb_descriptors.h"


USBEndpoint* usb_endpoint_create(USBEndpoint *endpoint, uint8_t bEndpointAddress, 
    USBDevice *device, USBEndpoint *other_endpoint, 
    Endpoint_cb setup_complete, Endpoint_cb transfer_complete
    ); 

bool usb_endpoint_alloc_queue(USBEndpoint *endpoint, size_t pool_size, void*(*alloc_cb)(size_t));    
    
void usb_peripheral_reset();

void usb_device_init(
	USBDevice* const device
);

void usb_endpoint_init(
	const USBEndpoint* const endpoint
);

void usb_run(
	USBDevice* const device
);

void usb_set_configuration_changed_cb(
    void (*callback)(USBDevice* const)
);

USBRequestStatus usb_standard_request(
	USBEndpoint* const endpoint,
	const USBTransferStage stage
);

typedef void (*transfer_completion_cb)(void*, unsigned int);

int usb_transfer_schedule(
	const USBEndpoint* const endpoint,
	void* const data,
	const uint32_t maximum_length,
        const transfer_completion_cb completion_cb,
        void* const user_data
);

int usb_transfer_schedule_block(
	const USBEndpoint* const endpoint,
	void* const data,
	const uint32_t maximum_length,
        const transfer_completion_cb completion_cb,
        void* const user_data
);

void usb_queue_transfer_complete(
    USBEndpoint* const endpoint
);

void usb_setup_complete(
	USBEndpoint* const endpoint
);

void usb_control_in_complete(
	USBEndpoint* const endpoint
);

void usb_control_out_complete(
	USBEndpoint* const endpoint
);


#endif