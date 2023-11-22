#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

// For older versions of the check library
#ifndef ck_assert_ptr_nonnull
#define ck_assert_ptr_nonnull(X) _ck_assert_ptr(X, !=, NULL)
#endif
#ifndef ck_assert_ptr_null
#define ck_assert_ptr_null(X) _ck_assert_ptr(X, ==, NULL)
#endif

/* Tests */

/* test init/cleanup */
START_TEST(test_init) {
    struct array *a;
    a = array_init(2);
    ck_assert_ptr_nonnull(a);
    array_cleanup(a);
}
END_TEST

/* test add/size */
START_TEST(test_add_basic) {
    struct array *a;
    a = array_init(2);

    ck_assert_ptr_nonnull(a);
    ck_assert_uint_eq(array_size(a), 0);

    ck_assert_int_eq(array_append(a, 3), 0);
    ck_assert_int_eq(array_append(a, 5), 0);
    ck_assert_uint_eq(array_size(a), 2);

    ck_assert_int_eq(array_get(a, 0), 3);
    ck_assert_int_eq(array_get(a, 1), 5);
    array_cleanup(a);
}
END_TEST

/* test add with resize */
START_TEST(test_add_resize) {
    struct array *a;
    a = array_init(2);

    ck_assert_ptr_nonnull(a);
    ck_assert_uint_eq(array_size(a), 0);

    ck_assert_int_eq(array_append(a, 3), 0);
    ck_assert_int_eq(array_append(a, 5), 0);
    ck_assert_int_eq(array_append(a, 7), 0);
    ck_assert_int_eq(array_append(a, 2), 0);
    ck_assert_int_eq(array_append(a, 1), 0);
    ck_assert_uint_eq(array_size(a), 5);

    ck_assert_int_eq(array_get(a, 0), 3);
    ck_assert_int_eq(array_get(a, 1), 5);
    ck_assert_int_eq(array_get(a, 2), 7);
    ck_assert_int_eq(array_get(a, 3), 2);
    ck_assert_int_eq(array_get(a, 4), 1);
    array_cleanup(a);
}
END_TEST

Suite *array_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Array");
    /* Core test case */
    tc_core = tcase_create("Core");

    /* Regular tests. */
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_add_basic);
    tcase_add_test(tc_core, test_add_resize);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = array_suite();
    sr = srunner_create(s);

    // srunner_run_all(sr, CK_NORMAL);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
