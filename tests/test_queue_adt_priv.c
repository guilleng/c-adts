#include "minunit.h"
#include "../src/queue_adt.c"

static QueueADT *size_3_dyn, *size_3_fix, *size_1_dyn, *size_1_fix;

void test_setup(void)
{
    size_3_dyn = create_queueadt(3);
    size_3_fix = create_fixsize_queueadt(3);
    size_1_dyn = create_queueadt(1);
    size_1_fix = create_fixsize_queueadt(1);
    return;
}

void test_teardown(void)
{
    destroy_queueadt(size_3_dyn);
    destroy_queueadt(size_3_fix);
    destroy_queueadt(size_1_dyn);
    destroy_queueadt(size_1_fix);
    return;
}

/*
 * Testing `queue_type` creation goes smoothly, and some corner cases.
 */
MU_TEST(test_queue_type)
{
    errno = 0;
    mu_assert(create_queueadt(0) == NULL, 
            "Zero size queues should not be allowed");
    mu_check(errno = EPERM);

    errno = 0;
    mu_assert(create_fixsize_queueadt(0) == NULL, 
            "Zero size queues should not be allowed");
    mu_check(errno = EPERM);

    mu_check(size_3_dyn->head == 0);
    mu_check(size_3_dyn->tail == 0);
    mu_check(size_3_dyn->min_size == 3);
    mu_check(size_3_dyn->curr_max_size == 3);
    mu_check(size_3_dyn->is_fix == 0);

    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 0);
    mu_check(size_3_fix->min_size == 3);
    mu_check(size_3_fix->curr_max_size == 3);
    mu_check(size_3_fix->is_fix == 1);
}

/*
 * Testing enqueue operation
 */
MU_TEST(test_enqueue_queueadt_fix)
{
    /* Size 1 queue */
    mu_assert_string_eq("one", enqueue_queueadt(size_1_fix, "one"));
    errno = 0;
    mu_assert(enqueue_queueadt(size_1_fix, "two") == NULL, 
            "Queue should overflow");
    mu_check(errno = EPERM);

    /* Size 3 queue */
    mu_assert_string_eq("one", enqueue_queueadt(size_3_fix, "one"));
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 0);
    mu_check(size_3_fix->nelems == 1);

    mu_assert_string_eq("two", enqueue_queueadt(size_3_fix, "two"));
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 1);
    mu_check(size_3_fix->nelems == 2);

    mu_assert_string_eq("three", enqueue_queueadt(size_3_fix, "three"));
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 3);
    errno = 0;
    mu_assert(enqueue_queueadt(size_3_fix, "X") == NULL, 
            "Queue should overflow");
    mu_check(errno = EPERM);
}

/*
 * Testing dequeue operation
 */
MU_TEST(test_dequeue_queueadt_fix)
{
    /* 
     * Set up queue
     * | a | b | c |
     */
    size_3_fix->contents[0] = "a";
    size_3_fix->contents[1] = "b";
    size_3_fix->contents[2] = "c";
    size_3_fix->head = 0;
    size_3_fix->tail = 2;
    size_3_fix->nelems = 3;

    /* Tests */
    mu_assert_string_eq("a", dequeue_queueadt(size_3_fix));
    mu_check(size_3_fix->head == 1);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 2);

    mu_assert_string_eq("b", dequeue_queueadt(size_3_fix));
    mu_check(size_3_fix->head == 2);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 1);

    mu_assert_string_eq("c", dequeue_queueadt(size_3_fix));
    mu_check(size_3_fix->head == 2);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 0);
    mu_assert_string_eq("three", enqueue_queueadt(size_3_fix, "three"));

    errno = 0;
    mu_assert(dequeue_queueadt(size_1_fix) == NULL, 
            "Queue should overflow");
    mu_check(errno = EPERM);

}

/*
 * Tests wrap-around behavior
 */
MU_TEST(test_wraparaoud)
{
    /* Set up */
    enqueue_queueadt(size_3_fix, "a");
    enqueue_queueadt(size_3_fix, "b");
    enqueue_queueadt(size_3_fix, "c");
    /* Queue is now: | a | b | c | */
    dequeue_queueadt(size_3_fix);
    dequeue_queueadt(size_3_fix);
    /* Queue is now: |   |   | c | */
    enqueue_queueadt(size_3_fix, "d");
    /* Queue is now: | d |   | c | */

    /* Tests */
    mu_check(size_3_fix->head == 2);
    mu_check(size_3_fix->tail == 0);
    mu_check(size_3_fix->nelems == 2);
    mu_assert_string_eq("d", size_3_fix->contents[0]);
    mu_assert_string_eq("c", size_3_fix->contents[2]);

    /* Set up again */
    dequeue_queueadt(size_3_fix);
    enqueue_queueadt(size_3_fix, "e");
    /* Queue should be now: | d | e |   | */

    /* Check */
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 1);
    mu_check(size_3_fix->nelems == 2);
    mu_assert_string_eq("d", size_3_fix->contents[0]);
    mu_assert_string_eq("e", size_3_fix->contents[1]);
}

/*
 * Test double contents array
 */
MU_TEST(test_double_contents_size)
{
    /* 
     * Make Queue: | d | b | c |
     *               tl  hd
     */
    size_3_dyn->contents[0] = "d";
    size_3_dyn->contents[1] = "b";
    size_3_dyn->contents[2] = "c";
    size_3_dyn->head = 1;
    size_3_dyn->tail = 0;
    size_3_dyn->nelems = 3;

    /* Assert conditions for doubling */
    mu_check(is_full(size_3_dyn));
    mu_check(!is_fix_queueadt(size_3_dyn));

    /* Double the size */
    mu_check(resize_contents_array(size_3_dyn, 2.0) != NULL);

    /* Check new internals:
     * 
     * Queue should be now: | b | c | d |  |  |  | 
     *                        hd      tl
     */
    mu_check(size_3_dyn->head == 0);
    mu_check(size_3_dyn->tail == 2);
    mu_check(size_3_dyn->nelems == 3);
    mu_check(size_3_dyn->min_size == 3);
    mu_check(size_3_dyn->curr_max_size == 6);
    mu_assert_string_eq("b", size_3_dyn->contents[0]);
    mu_assert_string_eq("c", size_3_dyn->contents[1]);
    mu_assert_string_eq("d", size_3_dyn->contents[2]);
    
    /*
     *                 <--   -->
     * Make Queue: | h | i | d | e | f | g |
     *             |   | tl| hd|   |   |   |
     */
    size_3_dyn->contents[0] = "h";
    size_3_dyn->contents[1] = "i";
    size_3_dyn->contents[2] = "d";
    size_3_dyn->contents[3] = "e";
    size_3_dyn->contents[4] = "f";
    size_3_dyn->contents[5] = "g";
    size_3_dyn->head = 2;
    size_3_dyn->tail = 1;
    size_3_dyn->nelems = 6;

    /* Double the size by inserting an element */
    enqueue_queueadt(size_3_dyn, "j");

    /* Check new internals: 
     *
     * Queue should be now: | d | e | f | g | h | i | j | ...|
     *                      | hd|   |   |   |   |   | tl| ...|
     */
    mu_check(size_3_dyn->head == 0);
    mu_check(size_3_dyn->tail == 6);
    mu_check(size_3_dyn->nelems == 7);
    mu_check(size_3_dyn->min_size == 3);
    mu_check(size_3_dyn->curr_max_size == 12);
    mu_assert_string_eq("d", size_3_dyn->contents[0]);
    mu_assert_string_eq("e", size_3_dyn->contents[1]);
    mu_assert_string_eq("g", size_3_dyn->contents[3]);
    mu_assert_string_eq("j", size_3_dyn->contents[6]);

    /* Some more tests with a size 1 queue */
    enqueue_queueadt(size_1_dyn, "a");
    enqueue_queueadt(size_1_dyn, "b");
    enqueue_queueadt(size_1_dyn, "c");
    enqueue_queueadt(size_1_dyn, "d");
    enqueue_queueadt(size_1_dyn, "e");
    mu_assert_string_eq("a", size_1_dyn->contents[0]);
    mu_assert_string_eq("c", size_1_dyn->contents[2]);
    mu_assert_string_eq("e", size_1_dyn->contents[4]);
    mu_check(size_1_dyn->curr_max_size == 8);
}

/*
 * Test halving contents array
 */
MU_TEST(test_halve_content_size)
{
    /* Double size a couple of times */
    mu_check(resize_contents_array(size_3_dyn, 2.0) != NULL);
    mu_check(resize_contents_array(size_3_dyn, 2.0) != NULL);
    mu_check(resize_contents_array(size_3_dyn, 2.0) != NULL);
    mu_check(resize_contents_array(size_3_dyn, 2.0) != NULL);
    mu_check(size_3_dyn->curr_max_size == 48);

    /*
     *                 <--          -->
     * Make Queue: | r | s | ... | h | i | j | k | l | m | n | o | p | q |
     *             |   | tl| ... | hd
     */
    size_3_dyn->contents[0] = "r";
    size_3_dyn->contents[1] = "s";
    size_3_dyn->contents[38] = "h";
    size_3_dyn->contents[39] = "i";
    size_3_dyn->contents[40] = "j";
    size_3_dyn->contents[41] = "k";
    size_3_dyn->contents[42] = "l";
    size_3_dyn->contents[43] = "m";
    size_3_dyn->contents[44] = "n";
    size_3_dyn->contents[45] = "o";
    size_3_dyn->contents[46] = "p";
    size_3_dyn->contents[47] = "q";
    size_3_dyn->head = 38;
    size_3_dyn->tail = 1;
    size_3_dyn->nelems = 12;

    mu_check(resize_contents_array(size_3_dyn, 0.5) != NULL);
    /* 
     * Queue should be now: | h | i | j | k | l | m | n | ...|
     *                      | hd|   |   |   |   |   |   | ...|
     */
    mu_check(size_3_dyn->head == 0);
    mu_check(size_3_dyn->tail == 11);
    mu_check(size_3_dyn->nelems == 12);
    mu_check(size_3_dyn->min_size == 3);
    mu_check(size_3_dyn->curr_max_size == 24);
    mu_assert_string_eq("h", size_3_dyn->contents[0]);
    mu_assert_string_eq("k", size_3_dyn->contents[3]);
    mu_assert_string_eq("q", size_3_dyn->contents[9]);
    mu_assert_string_eq("s", size_3_dyn->contents[11]);

    /* Testing if dequeueing automatically triggers resizing */
    dequeue_queueadt(size_3_dyn); /* pop h */
    dequeue_queueadt(size_3_dyn); /* pop i */
    dequeue_queueadt(size_3_dyn); /* pop j */
    dequeue_queueadt(size_3_dyn); /* pop k */
    dequeue_queueadt(size_3_dyn); /* pop l */
    dequeue_queueadt(size_3_dyn); /* pop m */
    dequeue_queueadt(size_3_dyn); /* pop n */
    dequeue_queueadt(size_3_dyn); /* pop o */
    /* 
     * Queue should be now: |   | p | q | r | s |   |   | ...|
     *                      |   | hd|   |   | tl|   |   | ...|
     */
    mu_check(size_3_dyn->head == 1);
    mu_check(size_3_dyn->tail == 4);
    mu_check(size_3_dyn->nelems == 4);
    mu_check(size_3_dyn->min_size == 3);
    mu_check(size_3_dyn->curr_max_size == 12);
    mu_assert_string_eq("p", size_3_dyn->contents[1]);

    /* Notice that hd is at index 1. This is the expected behavior, the pop 
     * operation is done after the resizing takes place */
}

MU_TEST_SUITE(test_suite) 
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	MU_RUN_TEST(test_queue_type);
    MU_RUN_TEST(test_enqueue_queueadt_fix);
    MU_RUN_TEST(test_dequeue_queueadt_fix);
    MU_RUN_TEST(test_wraparaoud);
    MU_RUN_TEST(test_double_contents_size);
    MU_RUN_TEST(test_halve_content_size);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}
