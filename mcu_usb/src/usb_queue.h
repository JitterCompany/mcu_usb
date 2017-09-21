#ifndef __USB_QUEUE_H__
#define __USB_QUEUE_H__

#include "lpc43xx_usb.h"

#include "usb_type.h"
#include "mcu_usb.h"

typedef struct _usb_transfer_t usb_transfer_t;
typedef struct _usb_queue_t usb_queue_t;

// This is an opaque datatype. Thou shall not touch these members.
struct _usb_transfer_t {
        USBTransferDescriptor td ATTR_ALIGNED(64);
        struct _usb_transfer_t* next;
        unsigned int maximum_length;
        struct _usb_queue_t* queue;
        transfer_completion_cb completion_cb;
        void* user_data;
};
typedef struct _usb_transfer_t USBTransfer;

// This is an opaque datatype. Thou shall not touch these members.
struct _usb_queue_t {
        struct USBEndpoint* endpoint;
        unsigned int pool_size;
        usb_transfer_t* volatile free_transfers;
        usb_transfer_t* volatile active;
};
typedef struct _usb_queue_t USBQueue;

#define USB_DECLARE_QUEUE(endpoint_name)                                \
        struct _usb_queue_t endpoint_name##_queue;
#define USB_DEFINE_QUEUE(endpoint_name, _pool_size)                     \
        struct _usb_transfer_t endpoint_name##_transfers[_pool_size];   \
        struct _usb_queue_t endpoint_name##_queue = {                   \
                .endpoint = &endpoint_name,                             \
                .free_transfers = endpoint_name##_transfers,            \
                .pool_size = _pool_size                                 \
        };

void usb_queue_flush_endpoint(const USBEndpoint* const endpoint);

int usb_transfer_schedule_ack(
	const USBEndpoint* const endpoint
);

void usb_queue_init(
        usb_queue_t* const queue
);



#endif//__USB_QUEUE_H__
