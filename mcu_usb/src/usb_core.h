#ifndef __USB_CORE_H__
#define __USB_CORE_H__

#include "lpc43xx_usb.h"
#include "mcu_usb.h"

#define NUM_USB_CONTROLLERS 2
#define NUM_USB0_ENDPOINTS 6
#define NUM_USB1_ENDPOINTS 4

typedef enum {
	USB_TRANSFER_DIRECTION_OUT = 0,
	USB_TRANSFER_DIRECTION_IN = 1,
} USBTransferDirection;



void usb_bus_reset(
	USBDevice* const device
);

void usb_controller_reset(
	USBDevice* const device
);

void usb_controller_run(
	const USBDevice* const device
);

void usb_run_tasks(
	const USBDevice* const device
);

USBSpeed usb_speed(
	const USBDevice* const device
);

uint32_t usb_get_status(
	const USBDevice* const device
);

uint32_t usb_get_endpoint_setup_status(
	const USBDevice* const device
);

void usb_clear_endpoint_setup_status(
	const uint32_t endpoint_setup_status,
	const USBDevice* const device
);

uint32_t usb_get_endpoint_ready(
	const USBDevice* const device
);

uint32_t usb_get_endpoint_complete(
	const USBDevice* const device
);

void usb_clear_endpoint_complete(
	const uint32_t endpoint_complete,
	const USBDevice* const device
);

void usb_set_address_immediate(
	const USBDevice* const device,
	const uint_fast8_t address
);

void usb_set_address_deferred(
	const USBDevice* const device,
	const uint_fast8_t address
);

void usb_endpoint_init_without_descriptor(
	const USBEndpoint* const endpoint,
  uint_fast16_t max_packet_size,
  USBTransferType transfer_type
);

void usb_endpoint_stall(
	const USBEndpoint* const endpoint
);

void usb_endpoint_disable(
	const USBEndpoint* const endpoint
);



bool usb_endpoint_is_ready(
	const USBEndpoint* const endpoint
);

// Schedule an already filled-in transfer descriptor for execution on
// the given endpoint, waiting until the endpoint has finished.
void usb_endpoint_schedule_wait(
	const USBEndpoint* const endpoint,
        USBTransferDescriptor* const td
);

// Schedule an already filled-in transfer descriptor for execution on
// the given endpoint, appending to the end of the endpoint's queue if
// there are pending TDs. Note that this requires that one knows the
// tail of the endpoint's TD queue. Moreover, the user is responsible
// for setting the TERMINATE bit of next_dtd_pointer if needed.
void usb_endpoint_schedule_append(
        const USBEndpoint* const endpoint,
        USBTransferDescriptor* const tail_td,
        USBTransferDescriptor* const new_td
);

#endif