#include "minunit.h"
#include "../include/stack_adt.h"

static StackADT *s1, *s2;
static char* elements[6] = { "Lorem", "ipsum", "dolor", "sit", "amet", 
                              "consectetur", };

void test_setup(void)
{
    s1 = create_fixsize_stackadt(2);
    s2 = create_stackadt(2);
    return;
}

void test_teardown(void)
{
    destroy_stackadt(s1);
    destroy_stackadt(s2);
    return;
}

MU_TEST(test_ezstack_fixsize)
{
    mu_check(is_fix_stackadt(s1) == 1);

    mu_check(nelems_of_stackadt(s1) == 0);

    mu_assert_string_eq("Lorem", push_stackadt(s1, elements[0]));
    mu_assert_string_eq("ipsum", push_stackadt(s1, elements[1]));
    mu_check(nelems_of_stackadt(s1) == 2);

    /* Stack overflow */
    errno = 0;
    mu_check(push_stackadt(s1, elements[2]) == NULL);
    mu_check(errno = EPERM);
    mu_check(nelems_of_stackadt(s1) == 2);

    mu_assert_string_eq("ipsum", pop_stackadt(s1));

    make_empty_stackadt(&s1);

    /* Stack underflow */
    errno = 0;
    mu_check(pop_stackadt(s1) == NULL);
    mu_check(errno = EPERM);
}

MU_TEST(test_ezstack_notfixsize)
{
    mu_check(is_fix_stackadt(s2) == 0);

    mu_check(nelems_of_stackadt(s2) == 0);

    mu_assert_string_eq("Lorem", push_stackadt(s2, elements[0]));
    mu_assert_string_eq("ipsum", push_stackadt(s2, elements[1]));
    mu_assert_string_eq("dolor", push_stackadt(s2, elements[2]));
    mu_assert_string_eq("sit", push_stackadt(s2, elements[3]));
    mu_assert_string_eq("amet", push_stackadt(s2, elements[4]));
    mu_assert_string_eq("consectetur", push_stackadt(s2, elements[5]));
    mu_assert_string_eq("Lorem", push_stackadt(s2, elements[0]));
    mu_assert_string_eq("ipsum", push_stackadt(s2, elements[1]));
    mu_assert_string_eq("dolor", push_stackadt(s2, elements[2]));
    mu_assert_string_eq("sit", push_stackadt(s2, elements[3]));
    mu_assert_string_eq("amet", push_stackadt(s2, elements[4]));
    mu_assert_string_eq("consectetur", push_stackadt(s2, elements[5]));
    mu_assert_string_eq("Lorem", push_stackadt(s2, elements[0]));
    mu_assert_string_eq("ipsum", push_stackadt(s2, elements[1]));
    mu_assert_string_eq("dolor", push_stackadt(s2, elements[2]));
    mu_assert_string_eq("sit", push_stackadt(s2, elements[3]));
    mu_assert_string_eq("amet", push_stackadt(s2, elements[4]));
    mu_assert_string_eq("consectetur", push_stackadt(s2, elements[5]));

    mu_assert_string_eq("consectetur", pop_stackadt(s2));
    mu_assert_string_eq("amet", pop_stackadt(s2));

    mu_check(nelems_of_stackadt(s2) == 16);
    make_empty_stackadt(&s2);

    /* Stack underflow */
    errno = 0;
    mu_check(pop_stackadt(s2) == NULL);
    mu_check(errno = EPERM);
}

MU_TEST_SUITE(test_suite) 
{
        MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
        MU_RUN_TEST(test_ezstack_fixsize);
        MU_RUN_TEST(test_ezstack_notfixsize);
}

int main(int argc, char *argv[]) 
{
        MU_RUN_SUITE(test_suite);
        MU_REPORT();

        return MU_EXIT_CODE;
}
