#include "usb_ringbuffer.h"
#include <c_utils/assert.h>


void usb_ringbuffer_init(USBRingbuffer *ctx, Ringbuffer *ringbuffer)
{
    assert(ringbuffer_is_initialized(ringbuffer));
    ctx->ring = ringbuffer;

    ctx->next_write = ringbuffer->write;
    ctx->next_read = ringbuffer->read;
}

// return the next index relative to the supplied one
static RingbufferIndex next_index(const Ringbuffer *ringbuffer,
        RingbufferIndex index)
{
    index.offset+= ringbuffer->elem_sz;

    // index past end, wrap: move to first item, toggle wrap bit
    if(index.offset >= ringbuffer->num_bytes) {
        index.offset = 0;
        index.wrap^=1;
    }
    return index;
}
// return the previous index relative to the supplied one
static RingbufferIndex prev_index(const Ringbuffer *ringbuffer,
        RingbufferIndex index)
{
    index.offset-= ringbuffer->elem_sz;

    // index underflow, undo wrap: move to last item, toggle wrap bit
    if(index.offset >= ringbuffer->num_bytes) {
        index.offset = ringbuffer->num_bytes - ringbuffer->elem_sz;
        index.wrap^=1;
    }
    return index;
}

// return true if no more writeable space is available for claiming
static inline bool usb_ringbuffer_is_full(USBRingbuffer *ctx)
{
    const RingbufferIndex read = ctx->ring->read;
    const RingbufferIndex write = ctx->next_write;
    
    return ((read.offset == write.offset)
            && (read.wrap != write.wrap));
}
// return true if no more readable space is available for claiming
static inline bool usb_ringbuffer_is_empty(USBRingbuffer *ctx)
{
    const RingbufferIndex read = ctx->next_read;
    const RingbufferIndex write = ctx->ring->write;
    
    return (read.raw == write.raw);
}

void *usb_ringbuffer_claim_write_ptr(USBRingbuffer *ctx)
{
    Ringbuffer *ring = ctx->ring;

    const bool full = usb_ringbuffer_is_full(ctx);
    ring->overflow = full;

    if(full) {
        return NULL;
    }

    const RingbufferIndex next_w = ctx->next_write;
    ctx->next_write = next_index(ring, next_w);
    return ring->first_elem + next_w.offset;
}

void usb_ringbuffer_cancel_write(USBRingbuffer *ctx, void *write_ptr)
{
    const Ringbuffer *ring = ctx->ring;
    const RingbufferIndex prev = prev_index(ring, ctx->next_write);
    
    // assertion: cannot cancel more writes than claimed
    assert(ctx->next_write.raw != ring->write.raw); 

    if(write_ptr) {
        // assertion: canceled write should be the last claimed write
        assert(write_ptr == (ring->first_elem + prev.offset));
    }

    ctx->next_write = prev;
}

void usb_ringbuffer_complete_write(USBRingbuffer *ctx, void *write_ptr)
{
    if(write_ptr) {
        // assertion: completed write should be the first non-completed write
        assert(ringbuffer_get_writeable(ctx->ring) == write_ptr);
    }

    // commit: assert the ringbuffer is not full.
    // the ringbuffer should never be full at this point,
    // because the write_ptr was claimed earlier.
    assert(ringbuffer_commit(ctx->ring));
}

void *usb_ringbuffer_claim_read_ptr(USBRingbuffer *ctx)
{
    Ringbuffer *ring = ctx->ring;

    if(usb_ringbuffer_is_empty(ctx)) {
        return NULL;
    }

    const RingbufferIndex next_r = ctx->next_read;
    ctx->next_read = next_index(ring, next_r);
    return ring->first_elem + next_r.offset;
}
void usb_ringbuffer_cancel_read(USBRingbuffer *ctx, const void *read_ptr)
{
    const Ringbuffer *ring = ctx->ring;
    const RingbufferIndex prev = prev_index(ring, ctx->next_read);
    
    // assertion: cannot cancel more reads than claimed
    assert(ctx->next_read.raw != ring->read.raw); 

    if(read_ptr) {
        // assertion: canceled read should be the last claimed read
        assert(read_ptr == (ring->first_elem + prev.offset));
    }

    ctx->next_read = prev;

}
void usb_ringbuffer_complete_read(USBRingbuffer *ctx, const void *read_ptr)
{
    if(read_ptr) {
        // assertion: completed read should be the first non-completed read
        assert(ringbuffer_get_readable(ctx->ring) == read_ptr);
    }

    // advance: assert the ringbuffer is not empty.
    // the ringbuffer should never be empty at this point,
    // because the read_ptr was claimed earlier.
    assert(ringbuffer_advance(ctx->ring));
}

