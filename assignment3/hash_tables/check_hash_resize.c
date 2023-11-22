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

/* test resize */
START_TEST(test_add_resize) {
    struct table *t;
    double max_load_factor = 0.6;
    t = table_init(2, max_load_factor, hash_too_simple);
    ck_assert_ptr_nonnull(t);

    char *a = malloc(sizeof(char) * 3);
    memcpy(a, "ba", sizeof(char) * 3);
    char *b = malloc(sizeof(char) * 3);
    memcpy(b, "cd", sizeof(char) * 3);
    char *c = malloc(sizeof(char) * 3);
    memcpy(c, "fe", sizeof(char) * 3);
    char *d = malloc(sizeof(char) * 3);
    memcpy(d, "gh", sizeof(char) * 3);

    ck_assert_msg((int) table_load_factor(t) == 0,
                  "Load factor of empty hash table should be 0.");
    ck_assert_int_eq(table_insert(t, a, 4), 0);
    ck_assert_int_eq(table_insert(t, b, 9), 0);
    ck_assert_int_eq(table_insert(t, c, 22), 0);
    ck_assert_int_eq(table_insert(t, d, 17), 0);

    ck_assert_msg(table_load_factor(t) <= max_load_factor,
                  "Load factor cannot be higher than max load factor.");

    ck_assert_int_eq(array_get(table_lookup(t, a), 0), 4);
    ck_assert_int_eq(array_get(table_lookup(t, b), 0), 9);
    ck_assert_int_eq(array_get(table_lookup(t, c), 0), 22);
    ck_assert_int_eq(array_get(table_lookup(t, d), 0), 17);

    table_cleanup(t);
    free(a);
    free(b);
    free(c);
    free(d);
}
END_TEST

/* test resize with literal strings */
START_TEST(test_add_resize_literal_str) {
    struct table *t;
    double max_load_factor = 0.6;
    t = table_init(2, max_load_factor, hash_too_simple);
    ck_assert_ptr_nonnull(t);

    // ubuntu 18 has check lib 0.10, does not support double compare funcs.
    ck_assert_msg((int) table_load_factor(t) == 0,
                  "Load factor of empty hash table should be 0.");
    ck_assert_int_eq(table_insert(t, "ba", 4), 0);
    ck_assert_int_eq(table_insert(t, "cd", 9), 0);
    ck_assert_int_eq(table_insert(t, "fe", 22), 0);
    ck_assert_int_eq(table_insert(t, "gh", 17), 0);

    ck_assert_msg(table_load_factor(t) <= max_load_factor,
                  "Load factor cannot be higher than max load factor.");

    ck_assert_int_eq(array_get(table_lookup(t, "ba"), 0), 4);
    ck_assert_int_eq(array_get(table_lookup(t, "cd"), 0), 9);
    ck_assert_int_eq(array_get(table_lookup(t, "fe"), 0), 22);
    ck_assert_int_eq(array_get(table_lookup(t, "gh"), 0), 17);

    table_cleanup(t);
}
END_TEST


START_TEST(test_chaining_resize) {
    struct table *t;
    double max_load_factor = 0.6;
    t = table_init(2, max_load_factor, hash_too_simple);
    ck_assert_ptr_nonnull(t);

    char *a = malloc(sizeof(char) * 4);
    memcpy(a, "abc", sizeof(char) * 4);
    char *b = malloc(sizeof(char) * 4);
    memcpy(b, "ade", sizeof(char) * 4);
    char *c = malloc(sizeof(char) * 4);
    memcpy(c, "afg", sizeof(char) * 4);
    char *d = malloc(sizeof(char) * 4);
    memcpy(d, "ahi", sizeof(char) * 4);

    ck_assert_msg((int) table_load_factor(t) == 0,
                  "Load factor of empty hash table should be 0.");
    ck_assert_int_eq(table_insert(t, a, 3), 0);
    ck_assert_int_eq(table_insert(t, b, 5), 0);
    ck_assert_int_eq(table_insert(t, c, 7), 0);
    ck_assert_int_eq(table_insert(t, d, 11), 0);

    ck_assert_msg(table_load_factor(t) <= max_load_factor,
                  "Load factor cannot be higher than max load factor.");
    ck_assert_int_eq(array_get(table_lookup(t, a), 0), 3);
    ck_assert_int_eq(array_get(table_lookup(t, b), 0), 5);
    ck_assert_int_eq(array_get(table_lookup(t, c), 0), 7);
    ck_assert_int_eq(array_get(table_lookup(t, d), 0), 11);

    table_cleanup(t);
    free(a);
    free(b);
    free(c);
    free(d);
}
END_TEST

START_TEST(test_chaining_resize_literal_str) {
    struct table *t;
    double max_load_factor = 0.6;
    t = table_init(2, max_load_factor, hash_too_simple);
    ck_assert_ptr_nonnull(t);

    ck_assert_msg((int) table_load_factor(t) == 0,
                  "Load factor of empty hash table should be 0.");

    ck_assert_int_eq(table_insert(t, "abc", 3), 0);
    ck_assert_int_eq(table_insert(t, "ade", 5), 0);
    ck_assert_int_eq(table_insert(t, "afg", 7), 0);
    ck_assert_int_eq(table_insert(t, "ahi", 11), 0);
    ck_assert_msg(table_load_factor(t) <= max_load_factor,
                  "Load factor cannot be higher than max load factor.");

    ck_assert_int_eq(array_get(table_lookup(t, "abc"), 0), 3);
    ck_assert_int_eq(array_get(table_lookup(t, "ade"), 0), 5);
    ck_assert_int_eq(array_get(table_lookup(t, "afg"), 0), 7);
    ck_assert_int_eq(array_get(table_lookup(t, "ahi"), 0), 11);

    table_cleanup(t);
}
END_TEST


Suite *hash_table_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Hash Table");
    /* Core test case */
    tc_core = tcase_create("Core");

    /* Regular tests. */
    tcase_add_test(tc_core, test_add_resize);
    tcase_add_test(tc_core, test_add_resize_literal_str);
    tcase_add_test(tc_core, test_chaining_resize);
    tcase_add_test(tc_core, test_chaining_resize_literal_str);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = hash_table_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return number_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
