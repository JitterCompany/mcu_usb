#include "usb_core.h"
#include "usb_request.h"
#include "usb_queue.h"

#include <stdbool.h>

static void usb_request(
	USBEndpoint* const endpoint,
	const USBTransferStage stage
) {
	const USBRequestHandlers* usb_request_handlers = endpoint->device->request_handlers;
	
	USBRequestStatus status = USB_REQUEST_STATUS_STALL;
	usb_request_handler_fn handler = 0;
	
	switch( endpoint->setup.request_type & USB_SETUP_REQUEST_TYPE_mask ) {
	case USB_SETUP_REQUEST_TYPE_STANDARD:
		handler = usb_request_handlers->standard;
		break;
	
	case USB_SETUP_REQUEST_TYPE_CLASS:
		handler = usb_request_handlers->class;
		break;
	
	case USB_SETUP_REQUEST_TYPE_VENDOR:
		handler = usb_request_handlers->vendor;
		break;
		
	case USB_SETUP_REQUEST_TYPE_RESERVED:
		handler = usb_request_handlers->reserved;
		break;
	}
	
	if( handler ) {
		status = handler(endpoint, stage);
	}

	if( status != USB_REQUEST_STATUS_OK ) {
		// USB 2.0 section 9.2.7 "Request Error"
		usb_endpoint_stall(endpoint);
	}
}

void usb_setup_complete(
	USBEndpoint* const endpoint
) {
	usb_request(endpoint, USB_TRANSFER_STAGE_SETUP);
}

void usb_control_out_complete(
	USBEndpoint* const endpoint
) {
	const bool device_to_host =
		endpoint->setup.request_type >> USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_shift;
	if( device_to_host ) {
		usb_request(endpoint, USB_TRANSFER_STAGE_STATUS);
	} else {
		usb_request(endpoint, USB_TRANSFER_STAGE_DATA);
	}
        usb_queue_transfer_complete(endpoint);
}

void usb_control_in_complete(
	USBEndpoint* const endpoint
) {
	const bool device_to_host =
		endpoint->setup.request_type >> USB_SETUP_REQUEST_TYPE_DATA_TRANSFER_DIRECTION_shift;
	if( device_to_host ) {
		usb_request(endpoint, USB_TRANSFER_STAGE_DATA);
	} else {
		usb_request(endpoint, USB_TRANSFER_STAGE_STATUS);
	}
        usb_queue_transfer_complete(endpoint);
}