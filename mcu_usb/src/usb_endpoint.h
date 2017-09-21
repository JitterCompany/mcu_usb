#ifndef USB_ENDPOINT_H
#define USB_ENDPOINT_H

#include <stdint.h>
#include <stddef.h>
#include "mcu_usb.h"

typedef struct __attribute__((packed))
{
    uint8_t request_type;
    uint8_t request;
    union {
        struct
        {
            uint8_t value_l;
            uint8_t value_h;
        };
        uint16_t value;
    };
    union {
        struct
        {
            uint8_t index_l;
            uint8_t index_h;
        };
        uint16_t index;
    };
    union {
        struct
        {
            uint8_t length_l;
            uint8_t length_h;
        };
        uint16_t length;
    };
} USBSetup;

struct USBEndpoint
{
    USBSetup setup;
    uint8_t buffer[8]; // Buffer for use during IN stage.
    uint_fast8_t address;
    USBDevice *device;
    USBEndpoint *in;
    USBEndpoint *out;
    void (*setup_complete)(USBEndpoint *const endpoint);
    void (*transfer_complete)(USBEndpoint *const endpoint);
};

bool usb_endpoint_is_in(const uint_fast8_t endpoint_address);
	
const USBDescriptorEndpoint* usb_endpoint_descriptor(
	const USBEndpoint* const endpoint
);

USBTransferType usb_endpoint_descriptor_transfer_type(
	const USBDescriptorEndpoint* const endpoint_descriptor
);

#endif