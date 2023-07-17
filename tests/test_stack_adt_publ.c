#include "minunit.h"
#include "../include/stack_adt.h"

static StackADT *s1, *s2;
static char* elements[6] = { "Lorem", "ipsum", "dolor", "sit", "amet", 
                              "consectetur", };

void test_setup(void)
{
    s1 = cadtstack_new_fix(2);
    s2 = cadtstack_new(2);
    return;
}

void test_teardown(void)
{
    cadtstack_destroy(s1);
    cadtstack_destroy(s2);
    return;
}

MU_TEST(test_fixsize_stack)
{
    mu_check(cadtstack_nelems(s1) == 0);

    mu_assert_string_eq("Lorem", cadtstack_push(s1, elements[0]));
    mu_assert_string_eq("ipsum", cadtstack_push(s1, elements[1]));
    mu_check(cadtstack_nelems(s1) == 2);

    /* Stack overflow */
    errno = 0;
    mu_check(cadtstack_push(s1, elements[2]) == NULL);
    mu_check(errno == EPERM);
    mu_check(cadtstack_nelems(s1) == 2);

    mu_assert_string_eq("ipsum", cadtstack_pop(s1));

    cadtstack_clear(s1);

    /* Stack underflow */
    errno = 0;
    mu_check(cadtstack_pop(s1) == NULL);
    mu_check(errno == EPERM);
}

MU_TEST(test_dynamic_stack)
{
    mu_check(cadtstack_nelems(s2) == 0);

    mu_assert_string_eq("Lorem", cadtstack_push(s2, elements[0]));
    mu_assert_string_eq("ipsum", cadtstack_push(s2, elements[1]));
    mu_assert_string_eq("dolor", cadtstack_push(s2, elements[2]));
    mu_assert_string_eq("sit", cadtstack_push(s2, elements[3]));
    mu_assert_string_eq("amet", cadtstack_push(s2, elements[4]));
    mu_assert_string_eq("consectetur", cadtstack_push(s2, elements[5]));
    mu_assert_string_eq("Lorem", cadtstack_push(s2, elements[0]));
    mu_assert_string_eq("ipsum", cadtstack_push(s2, elements[1]));
    mu_assert_string_eq("dolor", cadtstack_push(s2, elements[2]));
    mu_assert_string_eq("sit", cadtstack_push(s2, elements[3]));
    mu_assert_string_eq("amet", cadtstack_push(s2, elements[4]));
    mu_assert_string_eq("consectetur", cadtstack_push(s2, elements[5]));
    mu_assert_string_eq("Lorem", cadtstack_push(s2, elements[0]));
    mu_assert_string_eq("ipsum", cadtstack_push(s2, elements[1]));
    mu_assert_string_eq("dolor", cadtstack_push(s2, elements[2]));
    mu_assert_string_eq("sit", cadtstack_push(s2, elements[3]));
    mu_assert_string_eq("amet", cadtstack_push(s2, elements[4]));
    mu_assert_string_eq("consectetur", cadtstack_push(s2, elements[5]));

    mu_assert_string_eq("consectetur", cadtstack_pop(s2));
    mu_assert_string_eq("amet", cadtstack_pop(s2));

    mu_check(cadtstack_nelems(s2) == 16);
    s2 = cadtstack_clear(s2);

    /* Stack underflow */
    errno = 0;
    mu_check(cadtstack_pop(s2) == NULL);
    mu_check(errno == EPERM);
}

MU_TEST_SUITE(test_suite) 
{
        MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
        MU_RUN_TEST(test_fixsize_stack);
        MU_RUN_TEST(test_dynamic_stack);
}

int main(int argc, char *argv[]) 
{
        MU_RUN_SUITE(test_suite);
        MU_REPORT();

        return MU_EXIT_CODE;
}
