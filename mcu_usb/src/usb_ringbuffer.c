#include "usb_ringbuffer.h"
#include <c_utils/assert.h>

void usb_ringbuffer_init(USBRingbuffer *ctx, Ringbuffer *ringbuffer)
{
    assert(ringbuffer_is_initialized(ringbuffer));
    ctx->ring = ringbuffer;
}

void *usb_ringbuffer_claim_write_ptr(USBRingbuffer *ctx)
{
    // TODO implement
    return NULL;
}

void usb_ringbuffer_cancel_write(USBRingbuffer *ctx, void *write_ptr)
{
    // TODO implement
}
void usb_ringbuffer_complete_write(USBRingbuffer *ctx, void *write_ptr)
{
    // TODO implement
}
void *usb_ringbuffer_claim_read_ptr(USBRingbuffer *ctx)
{
    // TODO implement
    return NULL;
}
void usb_ringbuffer_cancel_read(USBRingbuffer *ctx, void *read_ptr)
{
    // TODO implement
}
void usb_ringbuffer_complete_read(USBRingbuffer *ctx, void *read_ptr)
{
    // TODO implement
}
