#ifndef USB_RINGBUFFER_H
#define USB_RINGBUFFER_H

#include <c_utils/ringbuffer.h>

/** usb_ringbuffer: use a standard ringbuffer (@see c_utils package) with mcu_usb.
 *
 * USBRingbuffer is an access layer to allow mcu_usb to access a ringbuffer.
 * This layer is required because the usb library wants to do some advanced
 * stuff such as claiming multipe write (or read) pointers ahead.
 *
 * Note: never mix writing to a ringbuffer via usb_ringbuffer with writing
 * directly to the ringbuffer.
 * Similarly, never mix reading via usb_ringbuffer with reading directly
 * from the ringbuffer.
 */

typedef struct usb_ringbuffer USBRingbuffer;
/**
 * Initialize a USBRingbuffer object.
 *
 * @param ctx           USBRingbuffer object to initialize.
 *                      This object holds all the state and should be passed
 *                      to all other usb_ringbuffer_ functions.
 *
 * @param ringbuffer    Normal ringbuffer, @see package c_utils/ringbuffer.h.
 *                      NOTE: this ringbuffer should have been initialized
 *                      with ringbuffer_init()!
 */
void usb_ringbuffer_init(USBRingbuffer *ctx, Ringbuffer *ringbuffer);

// TODO implement the rest of usb_ringbuffer
//


struct usb_ringbuffer {
    Ringbuffer *ring;
};

#endif

