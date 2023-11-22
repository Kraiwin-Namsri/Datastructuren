#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "hash_func.h"
#include "hash_table.h"

// For older versions of the check library
#ifndef ck_assert_ptr_nonnull
#define ck_assert_ptr_nonnull(X) _ck_assert_ptr(X, !=, NULL)
#endif
#ifndef ck_assert_ptr_null
#define ck_assert_ptr_null(X) _ck_assert_ptr(X, ==, NULL)
#endif

START_TEST(test_array_add) {

    struct table *t;
    t = table_init(8, 0.6, hash_too_simple);
    ck_assert_ptr_nonnull(t);

    char *a = malloc(sizeof(char) * 4);
    memcpy(a, "abc", sizeof(char) * 4);
    char *b = malloc(sizeof(char) * 4);
    memcpy(b, "def", sizeof(char) * 4);

    char *c = malloc(sizeof(char) * 4);
    memcpy(c, "abc", sizeof(char) * 4);
    char *d = malloc(sizeof(char) * 4);
    memcpy(d, "def", sizeof(char) * 4);

    char *e = malloc(sizeof(char) * 4);
    memcpy(e, "abc", sizeof(char) * 4);

    ck_assert_int_eq(table_insert(t, a, 14), 0);
    ck_assert_int_eq(table_insert(t, b, 8), 0);
    ck_assert_int_eq(table_insert(t, c, 12), 0);
    ck_assert_int_eq(table_insert(t, d, 22), 0);
    ck_assert_int_eq(table_insert(t, e, 3), 0);

    ck_assert_int_eq(array_get(table_lookup(t, a), 0), 14);
    ck_assert_int_eq(array_get(table_lookup(t, b), 0), 8);
    ck_assert_int_eq(array_get(table_lookup(t, a), 1), 12);
    ck_assert_int_eq(array_get(table_lookup(t, b), 1), 22);
    ck_assert_int_eq(array_get(table_lookup(t, a), 2), 3);

    table_cleanup(t);
    free(a);
    free(b);
    free(c);
    free(d);
    free(e);
}
END_TEST

START_TEST(test_array_with_resize) {
    struct table *t;
    t = table_init(2, 0.6, hash_too_simple);
    ck_assert_ptr_nonnull(t);

    char *a = malloc(sizeof(char) * 4);
    memcpy(a, "abc", sizeof(char) * 4);
    char *b = malloc(sizeof(char) * 4);
    memcpy(b, "abc", sizeof(char) * 4);

    char *c = malloc(sizeof(char) * 4);
    memcpy(c, "abc", sizeof(char) * 4);
    char *d = malloc(sizeof(char) * 4);
    memcpy(d, "def", sizeof(char) * 4);

    char *e = malloc(sizeof(char) * 4);
    memcpy(e, "ghi", sizeof(char) * 4);

    ck_assert_int_eq(table_insert(t, a, 14), 0);
    ck_assert_int_eq(table_insert(t, b, 8), 0);
    ck_assert_int_eq(table_insert(t, c, 12), 0);
    ck_assert_int_eq(table_insert(t, d, 22), 0);
    ck_assert_int_eq(table_insert(t, e, 3), 0);

    ck_assert_int_eq(array_get(table_lookup(t, a), 0), 14);
    ck_assert_int_eq(array_get(table_lookup(t, a), 1), 8);
    ck_assert_int_eq(array_get(table_lookup(t, a), 2), 12);
    ck_assert_int_eq(array_get(table_lookup(t, d), 0), 22);
    ck_assert_int_eq(array_get(table_lookup(t, e), 0), 3);

    table_cleanup(t);
    free(a);
    free(b);
    free(c);
    free(d);
    free(e);
}
END_TEST

Suite *hash_table_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Hash Table");
    /* Core test case */
    tc_core = tcase_create("Core");

    /* Regular tests. */
    tcase_add_test(tc_core, test_array_add);
    tcase_add_test(tc_core, test_array_with_resize);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = hash_table_suite();
    sr = srunner_create(s);

    // srunner_run_all(sr, CK_NORMAL);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
