#include "usb_ringbuffer.h"
#include <c_utils/assert.h>

void usb_ringbuffer_init(USBRingbuffer *ctx, Ringbuffer *ringbuffer)
{
    assert(ringbuffer_is_initialized(ringbuffer));
    ctx->ring = ringbuffer;
}
