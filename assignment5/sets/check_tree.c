#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

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
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    tree_cleanup(t);
}
END_TEST

/* test simple insert */
START_TEST(test_single_insert) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    tree_cleanup(t);
}
END_TEST

/* test simple inserts */
START_TEST(test_tree_inserts_sorted) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    ck_assert_int_eq(tree_insert(t, 2), 0);
    ck_assert_int_eq(tree_insert(t, 3), 0);
    tree_cleanup(t);
}
END_TEST

/* test simple inserts */
START_TEST(test_tree_inserts) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 5), 0);
    ck_assert_int_eq(tree_insert(t, 2), 0);
    ck_assert_int_eq(tree_insert(t, 6), 0);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    ck_assert_int_eq(tree_insert(t, 9), 0);
    ck_assert_int_eq(tree_insert(t, 8), 0);
    ck_assert_int_eq(tree_insert(t, 7), 0);
    // tree_dot(t, "tree.dot");
    tree_cleanup(t);
}
END_TEST

/* test double insert */
START_TEST(test_tree_double_insert) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 2), 0);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    ck_assert_int_eq(tree_insert(t, 3), 0);
    ck_assert_int_eq(tree_insert(t, 3), 1);
    tree_cleanup(t);
}
END_TEST

/* test remove */
START_TEST(test_remove) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    ck_assert_int_eq(tree_insert(t, 2), 0);
    ck_assert_int_eq(tree_remove(t, 2), 0);
    tree_cleanup(t);
}
END_TEST

/* test remove elment not in tree */
START_TEST(test_remove_not_found) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    ck_assert_int_eq(tree_insert(t, 2), 0);
    ck_assert_int_eq(tree_remove(t, 3), 1);
    tree_cleanup(t);
}
END_TEST

/* test find */
START_TEST(test_find) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    ck_assert_int_eq(tree_insert(t, 2), 0);
    ck_assert_int_eq(tree_find(t, 1), 1);
    ck_assert_int_eq(tree_find(t, 2), 1);
    ck_assert_int_eq(tree_find(t, 3), 0);
    tree_cleanup(t);
}
END_TEST

/* Duplicates are not allowed */
START_TEST(test_duplicates) {
    struct tree *t = tree_init(0);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(tree_insert(t, 1), 0);
    ck_assert_int_eq(tree_insert(t, 2), 0);
    ck_assert_int_eq(tree_insert(t, 2), 1); // not inserted again
    ck_assert_int_eq(tree_find(t, 1), 1);
    ck_assert_int_eq(tree_find(t, 2), 1);

    ck_assert_int_eq(tree_remove(t, 2), 0);
    ck_assert_int_eq(tree_find(t, 2), 0);
    tree_cleanup(t);
}
END_TEST

/* This is a very simple test just to loosely verify that it is not completely broken */
START_TEST(test_tree_check) {
  struct tree *t = tree_init(0);
  ck_assert_ptr_nonnull(t);

  for (int i = 0; i < 100; i++) {
    int val;
    if (i % 2) {
      val = i;
    } else {
      val = -i;
    }
    ck_assert_int_eq(tree_insert(t, val), 0);
    ck_assert_int_eq(tree_check(t), 0);
  }
  // tree_dot(t, "tree.dot");

  for (int i = 1; i < 100; i+=6) {
    int val;
    if (i % 2) {
      val = i;
    } else {
      val = -i;
    }
    ck_assert_int_eq(tree_remove(t, val), 0);
    ck_assert_int_eq(tree_check(t), 0);
  }

  tree_cleanup(t);
}

/* Slowness is not allowed */
START_TEST(test_worst_case) {
  struct tree *t = tree_init(0);
  ck_assert_ptr_nonnull(t);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(tree_insert(t, i), 0);
  }

  // tree_dot(t, "tree.dot");
  tree_cleanup(t);
}
END_TEST

Suite *tree_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Tree");
    /* Core test case */
    tc_core = tcase_create("Core");

    /* Regular tests. */
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_single_insert);
    tcase_add_test(tc_core, test_tree_inserts_sorted);
    tcase_add_test(tc_core, test_tree_inserts);
    tcase_add_test(tc_core, test_tree_double_insert);
    tcase_add_test(tc_core, test_remove);
    tcase_add_test(tc_core, test_remove_not_found);
    tcase_add_test(tc_core, test_find);
    tcase_add_test(tc_core, test_duplicates);
    tcase_add_test(tc_core, test_tree_check);
    tcase_add_test(tc_core, test_worst_case);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = tree_suite();
    sr = srunner_create(s);

    // srunner_run_all(sr, CK_NORMAL);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
