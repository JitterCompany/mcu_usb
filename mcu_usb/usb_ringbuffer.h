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


/**
 * Claim the first available write pointer
 *
 * Every time you claim a pointer, you get a pointer to the next available
 * writeable element in the ringbuffer (if any).
 * A claimed pointer is available for writing untill you complete or cancel
 * the write. See complete_write() and cancel_write().
 *
 * @return              Pointer to the next available writeable element
 *                      if available, NULL if no more elements are available.
 */
void *usb_ringbuffer_claim_write_ptr(USBRingbuffer *ctx);


/**
 * Cancel the latest write
 *
 * Cancel writing to the pointer previously claimed with claim_write_ptr().
 *
 * NOTE: cancels (and conpletes) should be done in order: only the latest
 * write (that is not yet canceled or completed) can be canceled.
 * If you want to cancel multiple writes: first cancel the last write,
 * then the one before it, etc.
 *
 * @param write_ptr     Write pointer as returned from claim_write_ptr().
 *                      This is used to perform a sanity check: cancels should
 *                      be done in the right order.
 *                      NULL may be passed in case you don't have access to
 *                      the write_ptr. In this case the check is skipped.
 */
void usb_ringbuffer_cancel_write(USBRingbuffer *ctx, void *write_ptr);


/**
 * Complete the latest write
 *
 * Complete writing to the write pointer previously claimed with
 * claim_write_ptr().
 *
 * NOTE: completes (and cancels) should be done in order: only the first
 * write (that is not yet canceled or completed) can be completed.
 * If you want to complete multiple writes: first complete the oldest write,
 * then the one after it, etc.
 *
 * @param write_ptr     Write pointer as returned from claim_write_ptr().
 *                      This is used to perform a sanity check: completes
 *                      should be done in the right order.
 *                      NULL may be passed in case you don't have access to
 *                      the write_ptr. In this case the check is skipped.
 */
void usb_ringbuffer_complete_write(USBRingbuffer *ctx, void *write_ptr);

/**
 * Claim the first available read pointer
 *
 * Every time you claim a pointer, you get a pointer to the next available
 * readable element in the ringbuffer (if any).
 * A claimed pointer is available for reading untill you complete or cancel
 * the read. See complete_read() and cancel_read().
 *
 * @return              Pointer to the next available readable element
 *                      if available, NULL if no more elements are available.
 */
void *usb_ringbuffer_claim_read_ptr(USBRingbuffer *ctx);


/**
 * Cancel the latest read
 *
 * Cancel reading from the pointer previously claimed with claim_read_ptr().
 *
 * NOTE: cancels (and conpletes) should be done in order: only the latest
 * read (that is not yet canceled or completed) can be canceled.
 * If you want to cancel multiple reads: first cancel the last read,
 * then the one before it, etc.
 *
 * @param read_ptr      read pointer as returned from claim_read_ptr().
 *                      This is used to perform a sanity check: cancels should
 *                      be done in the right order.
 *                      NULL may be passed in case you don't have access to
 *                      the read_ptr. In this case the check is skipped.
 */
void usb_ringbuffer_cancel_read(USBRingbuffer *ctx, const void *read_ptr);


/**
 * Complete the latest read
 *
 * Complete reading from the read pointer previously claimed with
 * claim_read_ptr().
 *
 * NOTE: completes (and cancels) should be done in order: only the first
 * read (that is not yet canceled or completed) can be completed.
 * If you want to complete multiple reads: first complete the oldest read,
 * then the one after it, etc.
 *
 * @param read_ptr      read pointer as returned from claim_read_ptr().
 *                      This is used to perform a sanity check: completes
 *                      should be done in the right order.
 *                      NULL may be passed in case you don't have access to
 *                      the read_ptr. In this case the check is skipped.
 */
void usb_ringbuffer_complete_read(USBRingbuffer *ctx, const void *read_ptr);


void *usb_ringbuffer_claim_read_ptr(USBRingbuffer *ctx);
// TODO implement the rest of usb_ringbuffer
//


struct usb_ringbuffer {
    Ringbuffer *ring;
    volatile RingbufferIndex next_write;
    volatile RingbufferIndex next_read;
};

#endif

