#include "minunit.h"
#include "../src/stack_adt.c"

static StackADT *s0, *s1, *s2, *s3;

void test_setup(void)
{
    s0 = cadtstack_new_fix(1);
    s1 = cadtstack_new(1);
    s2 = cadtstack_new_fix(5);
    s3 = cadtstack_new(5);
    return;
}

void test_teardown(void)
{
    cadtstack_destroy(s0);
    cadtstack_destroy(s1);
    cadtstack_destroy(s2);
    cadtstack_destroy(s3);
    return;
}

/*
 * Testing `stack_type` creation goes smoothly, and some corner cases.
 */
MU_TEST(test_stack_type)
{
    errno = 0;
    mu_assert(cadtstack_new(0) == NULL, 
            "Zero size stacks should not be allowed");
    mu_check(errno = EPERM);

    errno = 0;
    mu_assert(cadtstack_new(0) == NULL, 
            "Zero size stacks should not be allowed");
    mu_check(errno = EPERM);

    mu_check(s0->min_size == 1);
    mu_check(s1->min_size == 1);
    mu_check(s2->min_size == 5);
    mu_check(s3->min_size == 5);

    mu_check(s0->curr_max_size == 1);
    mu_check(s1->curr_max_size == 1);
    mu_check(s2->curr_max_size == 5);
    mu_check(s3->curr_max_size == 5);

    mu_check(s0->top == 0);
    mu_check(s1->top == 0);
    mu_check(s2->top == 0);
    mu_check(s3->top == 0);

    mu_check(s0->is_fix == 1);
    mu_check(s1->is_fix == 0);
    mu_check(s2->is_fix == 1);
    mu_check(s3->is_fix == 0);
} 
/*
 * Test private function
 */
MU_TEST(test_is_full)
{
    mu_check(is_full(s0) == 0);
    cadtstack_push(s0, "");
    mu_check(is_full(s0) == 1);
    cadtstack_push(s3, "");
    cadtstack_push(s3, "");
    mu_check(is_full(s3) == 0);
    
    mu_check(is_fix(s3) == 0);
}

/* 
 * Tests whether a stack correctly doubles in size when more than 
 * `curr_max_size` items are pushed into it 
 */
MU_TEST(test_size_doubles_on_push)
{
    mu_check(s1->curr_max_size == 1);
    cadtstack_push(s1, "");
    mu_check(s1->curr_max_size == 1);
    cadtstack_push(s1, "");
    mu_check(s1->curr_max_size == 2);
    cadtstack_push(s1, "");
    cadtstack_push(s1, "");
    cadtstack_push(s1, "");
    mu_check(s1->curr_max_size == 8);
}

/*
 * Tests whether a stack halves when conditions occur during a `pop` operation
 */
MU_TEST(test_size_halves_on_pop)
{
    int i;
    mu_check(s1->curr_max_size == 1);
    /* Load 50 */
    for (i = 0; i < 50; i++)
    {
        cadtstack_push(s1, "Data");
    }
    mu_check(s1->curr_max_size == 64);

    /* Let only 14 in, usage = 14% */
    for (i = 0; i < 36 ; i++)
    {
        cadtstack_pop(s1);
    }
    mu_check(s1->curr_max_size == 32);
}

/*
 * Test whether a stack halves in size when `cadtstack_clear` is called
 */
MU_TEST(test_size_halves_on_clear)
{
    int i;
    mu_check(s1->curr_max_size == 1);
    /* Load 50 */
    for (i = 0; i < 65535; i++)
    {
        cadtstack_push(s1, "Data");
    }
    mu_check(s1->curr_max_size == 65536);

    mu_check(cadtstack_clear(s1) != NULL);
    mu_check(s1->curr_max_size == s1->min_size);
}

MU_TEST_SUITE(test_suite) 
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	MU_RUN_TEST(test_stack_type);
	MU_RUN_TEST(test_is_full);
	MU_RUN_TEST(test_size_doubles_on_push);
	MU_RUN_TEST(test_size_halves_on_pop);
	MU_RUN_TEST(test_size_halves_on_clear);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}
