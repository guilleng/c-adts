#include "minunit.h"
#include "../src/queue_adt.c"

static QueueADT *size_3_dyn, *size_3_fix, *size_1_dyn, *size_1_fix;
static QueueADT *size_48, *size_3;

void test_setup(void)
{
    size_3_dyn = cadtqueue_new(3);
    size_3_fix = cadtqueue_new_circular(3);
    size_1_dyn = cadtqueue_new(1);
    size_1_fix = cadtqueue_new_circular(1);
    size_3 = cadtqueue_new(3);
    /* 
     * Make Queue: | d | b | c |
     *               tl  hd
     */
    size_3->contents[0] = "d";
    size_3->contents[1] = "b";
    size_3->contents[2] = "c";
    size_3->head = 1;
    size_3->tail = 0;
    size_3->nelems = 3;

    size_48 = cadtqueue_new(48);
    /*
     *                 <--          -->
     * Make Queue: | r | s | ... | h | i | j | k | l | m | n | o | p | q |
     *             |   | tl| ... | hd
     */
    size_48->head = 38;
    size_48->tail = 1;
    size_48->nelems = 12;
    size_48->contents[0] = "r";
    size_48->contents[1] = "s";
    size_48->contents[38] = "h";
    size_48->contents[39] = "i";
    size_48->contents[40] = "j";
    size_48->contents[41] = "k";
    size_48->contents[42] = "l";
    size_48->contents[43] = "m";
    size_48->contents[44] = "n";
    size_48->contents[45] = "o";
    size_48->contents[46] = "p";
    size_48->contents[47] = "q";

    return;
}

void test_teardown(void)
{
    cadtqueue_destroy(size_3_dyn);
    cadtqueue_destroy(size_3_fix);
    cadtqueue_destroy(size_1_dyn);
    cadtqueue_destroy(size_1_fix);
    cadtqueue_destroy(size_3);
    cadtqueue_destroy(size_48);
    return;
}

/*
 * Testing `queue_type` creation goes smoothly, and some corner cases.
 */
MU_TEST(test_queue_type)
{
    errno = 0;
    mu_assert(cadtqueue_new(0) == NULL, 
            "Zero size queues should not be allowed");
    mu_check(errno == EINVAL);

    errno = 0;
    mu_assert(cadtqueue_new_circular(0) == NULL, 
            "Zero size queues should not be allowed");
    mu_check(errno == EINVAL);

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
MU_TEST(test_enqueue)
{
    /* Size 1 queue */
    mu_assert_string_eq("one", cadtqueue_enqueue(size_1_fix, "one"));
    errno = 0;
    mu_assert(cadtqueue_enqueue(size_1_fix, "two") == NULL, 
            "Queue should overflow");
    mu_check(errno == EPERM);

    /* Size 3 queue */
    mu_assert_string_eq("one", cadtqueue_enqueue(size_3_fix, "one"));
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 0);
    mu_check(size_3_fix->nelems == 1);

    mu_assert_string_eq("two", cadtqueue_enqueue(size_3_fix, "two"));
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 1);
    mu_check(size_3_fix->nelems == 2);

    mu_assert_string_eq("three", cadtqueue_enqueue(size_3_fix, "three"));
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 3);
    errno = 0;
    mu_assert(cadtqueue_enqueue(size_3_fix, "X") == NULL, 
            "Queue should overflow");
    mu_check(errno == EPERM);
}

/*
 * Testing dequeue operation
 */
MU_TEST(test_dequeue)
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
    mu_assert_string_eq("a", cadtqueue_dequeue(size_3_fix));
    mu_check(size_3_fix->head == 1);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 2);

    mu_assert_string_eq("b", cadtqueue_dequeue(size_3_fix));
    mu_check(size_3_fix->head == 2);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 1);

    mu_assert_string_eq("c", cadtqueue_dequeue(size_3_fix));
    mu_check(size_3_fix->head == 2);
    mu_check(size_3_fix->tail == 2);
    mu_check(size_3_fix->nelems == 0);
    mu_assert_string_eq("three", cadtqueue_enqueue(size_3_fix, "three"));

    errno = 0;
    mu_assert(cadtqueue_dequeue(size_1_fix) == NULL, 
            "Queue should overflow");
    mu_check(errno == EPERM);

}

/*
 * Tests wrap-around behavior. Depends on the enqueue and dequeue functions. 
 */
MU_TEST(test_wraparaoud)
{
    /* Set up */
    cadtqueue_enqueue(size_3_fix, "a");
    cadtqueue_enqueue(size_3_fix, "b");
    cadtqueue_enqueue(size_3_fix, "c");
    /* Queue is now: | a | b | c | */
    cadtqueue_dequeue(size_3_fix);
    cadtqueue_dequeue(size_3_fix);
    /* Queue is now: |   |   | c | */
    cadtqueue_enqueue(size_3_fix, "d");
    /* Queue is now: | d |   | c | */

    /* Tests */
    mu_check(size_3_fix->head == 2);
    mu_check(size_3_fix->tail == 0);
    mu_check(size_3_fix->nelems == 2);
    mu_assert_string_eq("d", size_3_fix->contents[0]);
    mu_assert_string_eq("c", size_3_fix->contents[2]);

    /* Set up again */
    cadtqueue_dequeue(size_3_fix);
    cadtqueue_enqueue(size_3_fix, "e");
    /* Queue should be now: | d | e |   | */

    /* Check */
    mu_check(size_3_fix->head == 0);
    mu_check(size_3_fix->tail == 1);
    mu_check(size_3_fix->nelems == 2);
    mu_assert_string_eq("d", size_3_fix->contents[0]);
    mu_assert_string_eq("e", size_3_fix->contents[1]);
}

/*
 * Test helper function shift elements
 */
MU_TEST(test_shift_elements)
{
    Element *new;
    /* 
     * Use Queue: | d | b | c |
     *              tl  hd
     */

    new = malloc(3 * sizeof(Element));
    if (new == NULL)
    {
        perror("malloc failed allocating Element array: ");
        exit(EXIT_FAILURE);
    }

    shift_elements(size_3, new);
    /* 
     * Array should be now: | b | c | d |
     */
    mu_assert_string_eq(new[0], "b");
    mu_assert_string_eq(new[1], "c");
    mu_assert_string_eq(new[2], "d");
    free(new);

    new = malloc(48 * sizeof(Element));
    if (new == NULL)
    {
        perror("malloc failed allocating Element array: ");
        exit(EXIT_FAILURE);
    }
    /*
     *                <--          -->
     * Use Queue: | r | s | ... | h | i | j | k | l | m | n | o | p | q |
     *            |   | tl| ... | hd
     */
    shift_elements(size_48, new);

    /* 
     * Array should be now: | h | i | j | k | l | m | n | ...|
     */
    mu_assert_string_eq(new[0], "h");
    mu_assert_string_eq(new[3], "k");
    mu_assert_string_eq(new[9], "q");
    mu_assert_string_eq(new[11], "s");
    free(new);
}

/*
 * Test whether `resize_contents_array` correctly doubles contents array. 
 * Depends on a correct implementation of the `shift_elements` function.
 */
MU_TEST(test_double_contents_size)
{
    /* 
     * Use Queue: | d | b | c |
     *              tl  hd
     */

    /* Assert conditions for doubling */
    mu_check(is_full(size_3));
    mu_check(!is_fix(size_3));

    /* Double the size */
    mu_check(resize_contents_array(size_3, 2.0) != NULL);

    /* Check new internals:
     * 
     * Queue should be now: | b | c | d |  |  |  | 
     *                        hd      tl
     */
    mu_check(size_3->head == 0);
    mu_check(size_3->tail == 2);
    mu_check(size_3->nelems == 3);
    mu_check(size_3->min_size == 3);
    mu_check(size_3->curr_max_size == 6);
    mu_assert_string_eq("b", size_3->contents[0]);
    mu_assert_string_eq("c", size_3->contents[1]);
    mu_assert_string_eq("d", size_3->contents[2]);
    
}

/*
 * Test whether `resize_contents_array` correctly halves contents array. 
 * Depends on a correct implementation of the `shift_elements` function.
 */
MU_TEST(test_halve_content_size)
{
    /*
     *                <--          -->
     * Use Queue: | r | s | ... | h | i | j | k | l | m | n | o | p | q |
     *            |   | tl| ... | hd
     *
     * Mock the queue's current minimum size 
     */
    size_48->min_size = 3;

    mu_check(resize_contents_array(size_48, 0.5) != NULL);
    /* 
     * Queue should be now: | h | i | j | k | l | m | n | ...|
     *                      | hd|   |   |   |   |   |   | ...|
     */
    mu_check(size_48->head == 0);
    mu_check(size_48->tail == 11);
    mu_check(size_48->nelems == 12);
    mu_check(size_48->min_size == 3);
    mu_check(size_48->curr_max_size == 24);
    mu_assert_string_eq("h", size_48->contents[0]);
    mu_assert_string_eq("k", size_48->contents[3]);
    mu_assert_string_eq("q", size_48->contents[9]);
    mu_assert_string_eq("s", size_48->contents[11]);

    /* Testing if dequeueing automatically triggers resizing */
    cadtqueue_dequeue(size_48); /* pop h */
    cadtqueue_dequeue(size_48); /* pop i */
    cadtqueue_dequeue(size_48); /* pop j */
    cadtqueue_dequeue(size_48); /* pop k */
    cadtqueue_dequeue(size_48); /* pop l */
    cadtqueue_dequeue(size_48); /* pop m */
    cadtqueue_dequeue(size_48); /* pop n */
    cadtqueue_dequeue(size_48); /* pop o */
    /* 
     * Queue should be now: |   | p | q | r | s |   |   | ...|
     *                      |   | hd|   |   | tl|   |   | ...|
     */
    mu_check(size_48->head == 1);
    mu_check(size_48->tail == 4);
    mu_check(size_48->nelems == 4);
    mu_check(size_48->min_size == 3);
    mu_check(size_48->curr_max_size == 12);
    mu_assert_string_eq("p", size_48->contents[1]);

    /* Notice that hd is at index 1. This is the expected behavior, the pop 
     * operation is done after the resizing takes place */
}

MU_TEST_SUITE(test_suite) 
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	MU_RUN_TEST(test_queue_type);
    MU_RUN_TEST(test_enqueue);
    MU_RUN_TEST(test_dequeue);
    MU_RUN_TEST(test_wraparaoud);
    MU_RUN_TEST(test_shift_elements);
    MU_RUN_TEST(test_double_contents_size);
    MU_RUN_TEST(test_halve_content_size);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}
