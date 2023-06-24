#include "minunit.h"
#include "../src/stack_adt.c"

static StackADT s0, s1, s2, s3;

void test_setup(void)
{
    s0 = create_fixsize_stackadt(1);
    s1 = create_stackadt(1);
    s2 = create_fixsize_stackadt(5);
    s3 = create_stackadt(5);
    return;
}

void test_teardown(void)
{
    destroy_stackadt(s0);
    destroy_stackadt(s1);
    destroy_stackadt(s2);
    destroy_stackadt(s3);
    return;
}

/*
 * Testing `stack_type` creation goes smoothly, and some corner cases.
 */
MU_TEST(test_stack_type)
{
    errno = 0;
    mu_assert(create_stackadt(0) == NULL, 
            "Zero size stacks should not be allowed");
    mu_check(errno = EPERM);

    errno = 0;
    mu_assert(create_stackadt(0) == NULL, 
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
    push_stackadt(s0, "");
    mu_check(is_full(s0) == 1);
    push_stackadt(s3, "");
    push_stackadt(s3, "");
    mu_check(is_full(s3) == 0);
}

/* 
 * Tests whether a stack correctly doubles in size when more than 
 * `curr_max_size` items are pushed into it 
 */
MU_TEST(test_size_doubles_on_push)
{
    mu_check(s1->curr_max_size == 1);
    push_stackadt(s1, "");
    mu_check(s1->curr_max_size == 1);
    push_stackadt(s1, "");
    mu_check(s1->curr_max_size == 2);
    push_stackadt(s1, "");
    push_stackadt(s1, "");
    push_stackadt(s1, "");
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
        push_stackadt(s1, "Data");
    }
    mu_check(s1->curr_max_size == 64);

    /* Let only 14 in, usage = 14% */
    for (i = 0; i < 36 ; i++)
    {
        pop_stackadt(s1);
    }
    mu_check(s1->curr_max_size == 32);
}

/*
 * Test whether a stack halves in size when `make_empty_stackadt` is called
 */
MU_TEST(test_size_halves_on_make_empty)
{
    int i;
    mu_check(s1->curr_max_size == 1);
    /* Load 50 */
    for (i = 0; i < 65535; i++)
    {
        push_stackadt(s1, "Data");
    }
    mu_check(s1->curr_max_size == 65536);

    s1 = make_empty_stackadt(&s1);
    mu_check(s1->curr_max_size == s1->min_size);
}

MU_TEST_SUITE(test_suite) 
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
	MU_RUN_TEST(test_stack_type);
	MU_RUN_TEST(test_is_full);
	MU_RUN_TEST(test_size_doubles_on_push);
	MU_RUN_TEST(test_size_halves_on_pop);
	MU_RUN_TEST(test_size_halves_on_make_empty);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}
