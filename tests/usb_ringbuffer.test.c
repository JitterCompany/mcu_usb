#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "unity.h"
#include "usb_ringbuffer.h"

int g_remaining_asserts = 0;


void assert(bool sane)
{
    if(g_remaining_asserts) {
        if(!sane) {
            g_remaining_asserts--;
        }
    } else {
        TEST_ASSERT_MESSAGE(sane, "Assertion failed!");
    }
}

void test_init(void)
{
    g_remaining_asserts = 0;

    uint8_t buffer[3*5];
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 5, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);
}

void test_implemented(void)
{
    g_remaining_asserts = 0;

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

void test_claim_write(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    char *c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('A', *c);

    c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('B', *c);

    c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('C', *c);
    TEST_ASSERT_FALSE(ringbuffer_is_overflowed(&rb));

    c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NULL(c);
    TEST_ASSERT_FALSE(ringbuffer_is_overflowed(&rb));
}

void test_claim_write__writes(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    char *c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('A', *c);
    *c = 'P';
    
    c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('B', *c);

    char *c2 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('C', *c2);

    *c = 'Q';
    *c2 = 'R';
    
    TEST_ASSERT_NULL(usb_ringbuffer_claim_write_ptr(&usb_rb));
    TEST_ASSERT_EQUAL_STRING("PQR", buffer);
}


void test_cancel_write__null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    char *c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('A', *c);
    
    // passing NULL should cancel the last write    
    usb_ringbuffer_cancel_write(&usb_rb, NULL);

    c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('A', *c);

    usb_ringbuffer_cancel_write(&usb_rb, NULL);

    // expect assertion failure: cannot cancel more than claimed
    g_remaining_asserts = 1;
    usb_ringbuffer_cancel_write(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}

void test_cancel_write__non_null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    char *c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('A', *c);
    
    // passing last claimed pointer should cancel this last write    
    usb_ringbuffer_cancel_write(&usb_rb, c);

    c = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('A', *c);

    char *c2 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('B', *c2);

    // expect assertion failure: cannot cancel non-last write pointer
    g_remaining_asserts = 1;
    usb_ringbuffer_cancel_write(&usb_rb, c);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}


void test_complete_write__null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    char *c1 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    char *c2 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    char *c3 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c3);

    *c1 = '1';
    *c2 = '2';
    *c3 = '3';
    TEST_ASSERT(ringbuffer_is_empty(&rb));
    TEST_ASSERT_EQUAL(0, ringbuffer_used_count(&rb));
    usb_ringbuffer_complete_write(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(1, ringbuffer_used_count(&rb));
    usb_ringbuffer_complete_write(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(2, ringbuffer_used_count(&rb));
    usb_ringbuffer_complete_write(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(3, ringbuffer_used_count(&rb));
    TEST_ASSERT(ringbuffer_is_full(&rb));

    // overflow should only trigger if claim_write_ptr was called
    // when the ringbuffer was full
    TEST_ASSERT_FALSE(ringbuffer_is_overflowed(&rb));
    TEST_ASSERT_NULL(usb_ringbuffer_claim_write_ptr(&usb_rb));
    TEST_ASSERT_TRUE(ringbuffer_is_overflowed(&rb));

    // overflow shoud reset after a succesful claim
    ringbuffer_flush(&rb, 1);
    TEST_ASSERT_NOT_NULL(usb_ringbuffer_claim_write_ptr(&usb_rb));
    usb_ringbuffer_complete_write(&usb_rb, NULL);
    TEST_ASSERT_FALSE(ringbuffer_is_overflowed(&rb));
    TEST_ASSERT(ringbuffer_is_full(&rb));

    // expect assertion failure: cannot complete more than claimed
    g_remaining_asserts = 1;
    usb_ringbuffer_complete_write(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}


void test_complete_write__non_null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    char *c1 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    char *c2 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    char *c3 = usb_ringbuffer_claim_write_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c3);

    *c1 = '1';
    *c2 = '2';
    *c3 = '3';
    TEST_ASSERT(ringbuffer_is_empty(&rb));
    TEST_ASSERT_EQUAL(0, ringbuffer_used_count(&rb));
    usb_ringbuffer_complete_write(&usb_rb, c1);
    TEST_ASSERT_EQUAL(1, ringbuffer_used_count(&rb));
    
    // expect assertion failure: cannot complete out-of-order
    g_remaining_asserts = 1;
    usb_ringbuffer_complete_write(&usb_rb, c3);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}


void test_claim_read(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    // ringbuffer is empty: nothing to read
    TEST_ASSERT_NULL(usb_ringbuffer_claim_read_ptr(&usb_rb));
    
    // write two elements to the ringbuffer    
    ringbuffer_write(&rb, "KL", 2);

    char *c = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('K', *c);

    c = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('L', *c);

    // no more data to read
    TEST_ASSERT_NULL(usb_ringbuffer_claim_read_ptr(&usb_rb));

    // write last element
    ringbuffer_write(&rb, "M", 1);
    TEST_ASSERT_TRUE(ringbuffer_is_full(&rb));
    TEST_ASSERT_FALSE(ringbuffer_is_overflowed(&rb));

    // overflow 
    ringbuffer_write(&rb, "N", 1);
    TEST_ASSERT_TRUE(ringbuffer_is_full(&rb));
    TEST_ASSERT_TRUE(ringbuffer_is_overflowed(&rb));

    c = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('M', *c);


    // no more reads available: prevous reads are still not completed
    TEST_ASSERT_NULL(usb_ringbuffer_claim_read_ptr(&usb_rb));

    TEST_ASSERT_TRUE(ringbuffer_is_overflowed(&rb));
}


void test_cancel_read__null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);
    
    ringbuffer_write(&rb, "KLM", 3);

    char *c = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('K', *c);
    
    // passing NULL should cancel the last read    
    usb_ringbuffer_cancel_read(&usb_rb, NULL);

    c = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('K', *c);

    usb_ringbuffer_cancel_read(&usb_rb, NULL);

    // expect assertion failure: cannot cancel more than claimed
    g_remaining_asserts = 1;
    usb_ringbuffer_cancel_read(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}

void test_cancel_read__non_null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    ringbuffer_write(&rb, "KLM", 3);

    char *c = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('K', *c);
    
    // passing last claimed pointer should cancel this last read    
    usb_ringbuffer_cancel_read(&usb_rb, c);

    c = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('K', *c);

    char *c2 = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL('L', *c2);

    // expect assertion failure: cannot cancel non-last read pointer
    g_remaining_asserts = 1;
    usb_ringbuffer_cancel_read(&usb_rb, c);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}


void test_complete_read__null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    ringbuffer_write(&rb, "123", 3);

    char *c1 = usb_ringbuffer_claim_read_ptr(&usb_rb);
    char *c2 = usb_ringbuffer_claim_read_ptr(&usb_rb);
    char *c3 = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c1);
    TEST_ASSERT_NOT_NULL(c2);
    TEST_ASSERT_NOT_NULL(c3);
    TEST_ASSERT_EQUAL(*c1, '1');
    TEST_ASSERT_EQUAL(*c2, '2');
    TEST_ASSERT_EQUAL(*c3, '3');

    TEST_ASSERT(ringbuffer_is_full(&rb));
    TEST_ASSERT_EQUAL(0, ringbuffer_free_count(&rb));
    usb_ringbuffer_complete_read(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(1, ringbuffer_free_count(&rb));
    usb_ringbuffer_complete_read(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(2, ringbuffer_free_count(&rb));
    usb_ringbuffer_complete_read(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(3, ringbuffer_free_count(&rb));
    TEST_ASSERT(ringbuffer_is_empty(&rb));

    // expect assertion failure: cannot complete more than claimed
    g_remaining_asserts = 1;
    usb_ringbuffer_complete_read(&usb_rb, NULL);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}


void test_complete_read__non_null(void)
{
    g_remaining_asserts = 0;

    char buffer[] = "ABC";
    Ringbuffer rb;
    ringbuffer_init(&rb, buffer, 1, 3);
    USBRingbuffer usb_rb;
    usb_ringbuffer_init(&usb_rb, &rb);

    ringbuffer_write(&rb, "123", 3);

    char *c1 = usb_ringbuffer_claim_read_ptr(&usb_rb);
    char *c2 = usb_ringbuffer_claim_read_ptr(&usb_rb);
    char *c3 = usb_ringbuffer_claim_read_ptr(&usb_rb);
    TEST_ASSERT_NOT_NULL(c3);

    *c1 = '1';
    *c2 = '2';
    *c3 = '3';
    TEST_ASSERT(ringbuffer_is_full(&rb));
    TEST_ASSERT_EQUAL(0, ringbuffer_free_count(&rb));
    usb_ringbuffer_complete_read(&usb_rb, c1);
    TEST_ASSERT_EQUAL(1, ringbuffer_free_count(&rb));
    
    // expect assertion failure: cannot complete out-of-order
    g_remaining_asserts = 1;
    usb_ringbuffer_complete_read(&usb_rb, c3);
    TEST_ASSERT_EQUAL(0, g_remaining_asserts);
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_implemented);

    RUN_TEST(test_claim_write);
    RUN_TEST(test_claim_write__writes);
    RUN_TEST(test_cancel_write__null);
    RUN_TEST(test_cancel_write__non_null);
    RUN_TEST(test_complete_write__null);
    RUN_TEST(test_complete_write__non_null);
    
    RUN_TEST(test_claim_read);
    RUN_TEST(test_cancel_read__null);
    RUN_TEST(test_cancel_read__non_null);
    RUN_TEST(test_complete_read__null);
    RUN_TEST(test_complete_read__non_null);

    UNITY_END();

    return 0;
}

