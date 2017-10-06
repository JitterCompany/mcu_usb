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

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);

    UNITY_END();

    return 0;
}
