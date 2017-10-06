#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "unity.h"
#include "usb_ringbuffer.h"

void assert(bool sane)
{
    TEST_ASSERT_MESSAGE(sane, "Assertion failed!");
}

void test_init(void)
{
    uint8_t buffer[3*5];
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 5, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);
}

void test_implemented(void)
{
    uint8_t buffer[3*5];
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 5, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    void *write = usb_ringbuffer_claim_write_ptr(&usb_rb);
    usb_ringbuffer_cancel_write(&usb_rb, write);
    write = usb_ringbuffer_claim_write_ptr(&usb_rb);
    usb_ringbuffer_complete_write(&usb_rb, write);

    void *read = usb_ringbuffer_claim_read_ptr(&usb_rb);
    usb_ringbuffer_cancel_read(&usb_rb, read);
    read = usb_ringbuffer_claim_read_ptr(&usb_rb);
    usb_ringbuffer_complete_read(&usb_rb, read);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_implemented);

    UNITY_END();

    return 0;
}
