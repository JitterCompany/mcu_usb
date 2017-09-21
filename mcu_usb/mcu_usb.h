#ifndef __MCU_USB_H__
#define __MCU_USB_H__

#include "descriptor_types.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct USBEndpoint USBEndpoint;

typedef enum {
    USB_TRANSFER_TYPE_CONTROL = 0,
    USB_TRANSFER_TYPE_ISOCHRONOUS = 1,
    USB_TRANSFER_TYPE_BULK = 2,
    USB_TRANSFER_TYPE_INTERRUPT = 3,
} USBTransferType;

typedef enum {
    USB_SPEED_LOW = 0,
    USB_SPEED_FULL = 1,
    USB_SPEED_HIGH = 2,
    USB_SPEED_SUPER = 3,
} USBSpeed;

typedef struct
{
    USBDescriptorConfiguration *descriptor;
    const uint32_t number;
    const USBSpeed speed;
} USBConfiguration;

typedef enum {
    USB_TRANSFER_STAGE_SETUP,
    USB_TRANSFER_STAGE_DATA,
    USB_TRANSFER_STAGE_STATUS,
} USBTransferStage;

typedef enum {
    USB_REQUEST_STATUS_OK = 0,
    USB_REQUEST_STATUS_STALL = 1,
} USBRequestStatus;

typedef USBRequestStatus (*usb_request_handler_fn)(
    USBEndpoint *const endpoint,
    const USBTransferStage stage);

typedef struct
{
    usb_request_handler_fn standard;
    usb_request_handler_fn class;
    usb_request_handler_fn vendor;
    usb_request_handler_fn reserved;
} USBRequestHandlers;

typedef struct
{
    USBDescriptorDevice *descriptor;
    const USBDescriptorString **descriptor_strings;
    const uint8_t *const qualifier_descriptor;

    USBConfiguration *(*configurations)[];
    const USBConfiguration *configuration; // Pointer to current configuration
    uint8_t controller;                    // USB0 or USB1 peripheral;
    const USBRequestHandlers *request_handlers;
} USBDevice;

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

typedef void (*Endpoint_cb)(USBEndpoint *const endpoint);

USBEndpoint *usb_endpoint_create(USBEndpoint *endpoint, uint8_t bEndpointAddress,
                                 USBDevice *device, USBEndpoint *other_endpoint,
                                 Endpoint_cb setup_complete, Endpoint_cb transfer_complete);

bool usb_endpoint_alloc_queue(USBEndpoint *endpoint, size_t pool_size, void *(*alloc_cb)(size_t));

void usb_peripheral_reset();

void usb_device_init(
    USBDevice *const device);

void usb_endpoint_init(
    const USBEndpoint *const endpoint);

void usb_run(
    USBDevice *const device);

void usb_set_configuration_changed_cb(
    void (*callback)(USBDevice *const));

USBRequestStatus usb_standard_request(
    USBEndpoint *const endpoint,
    const USBTransferStage stage);

typedef void (*transfer_completion_cb)(void *, unsigned int);

int usb_transfer_schedule(
    const USBEndpoint *const endpoint,
    void *const data,
    const uint32_t maximum_length,
    const transfer_completion_cb completion_cb,
    void *const user_data);

int usb_transfer_schedule_block(
    const USBEndpoint *const endpoint,
    void *const data,
    const uint32_t maximum_length,
    const transfer_completion_cb completion_cb,
    void *const user_data);

void usb_queue_transfer_complete(
    USBEndpoint *const endpoint);

void usb_setup_complete(
    USBEndpoint *const endpoint);

void usb_control_in_complete(
    USBEndpoint *const endpoint);

void usb_control_out_complete(
    USBEndpoint *const endpoint);

#endif