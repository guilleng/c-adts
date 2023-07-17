#include "minunit.h"
#include "../include/queue_adt.h"

static QueueADT *q1, *q2;
static char* elements[6] = { "Lorem", "ipsum", "dolor", "sit", "amet", 
                              "consectetur", };

void test_setup(void)
{
    q1 = cadtqueue_new_circular(3);
    q2 = cadtqueue_new(2);
    return;
}

void test_teardown(void)
{
    cadtqueue_destroy(q1);
    cadtqueue_destroy(q2);
    return;
}

MU_TEST(test_circular_queue)
{
    mu_check(cadtqueue_nelems(q1) == 0);

    mu_assert_string_eq("Lorem", cadtqueue_enqueue(q1, elements[0]));
    mu_assert_string_eq("ipsum", cadtqueue_enqueue(q1, elements[1]));
    mu_assert_string_eq("dolor", cadtqueue_enqueue(q1, elements[2]));
    mu_check(cadtqueue_nelems(q1) == 3);

    /* Queue overflow */
    errno = 0;
    mu_check(cadtqueue_enqueue(q1, elements[1]) == NULL);
    mu_check(errno == EPERM);

    mu_assert_string_eq("Lorem", cadtqueue_dequeue(q1));
    mu_assert_string_eq("sit", cadtqueue_enqueue(q1, elements[3]));

    mu_assert_string_eq("ipsum", cadtqueue_peek_first(q1));
    mu_assert_string_eq("sit", cadtqueue_peek_rear(q1));
    mu_check(cadtqueue_nelems(q1) == 3);

    mu_check(cadtqueue_clear(q1) != NULL);
    mu_check(cadtqueue_nelems(q1) == 0);

}

MU_TEST(test_non_circular_queue)
{
    mu_check(cadtqueue_nelems(q2) == 0);

    mu_assert_string_eq("Lorem", cadtqueue_enqueue(q2, elements[0]));
    mu_assert_string_eq("ipsum", cadtqueue_enqueue(q2, elements[1]));
    mu_assert_string_eq("dolor", cadtqueue_enqueue(q2, elements[2]));
    mu_assert_string_eq("sit", cadtqueue_enqueue(q2, elements[3]));
    mu_assert_string_eq("amet", cadtqueue_enqueue(q2, elements[4]));
    mu_assert_string_eq("consectetur", cadtqueue_enqueue(q2, elements[5]));

    mu_check(cadtqueue_nelems(q2) == 6);
    mu_assert_string_eq("Lorem", cadtqueue_peek_first(q2));
    mu_assert_string_eq("consectetur", cadtqueue_peek_rear(q2));

    mu_assert_string_eq("Lorem", cadtqueue_dequeue(q2));
    mu_assert_string_eq("ipsum", cadtqueue_peek_first(q2));

    /* Queue underflow */
    errno = 0;
    while (cadtqueue_dequeue(q2))
    {
        ;
    }
    mu_check(errno == EPERM);
}

MU_TEST_SUITE(test_suite) 
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	MU_RUN_TEST(test_circular_queue);
	MU_RUN_TEST(test_non_circular_queue);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}
