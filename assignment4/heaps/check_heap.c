#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "prioq.h"

// For older versions of the check library
#ifndef ck_assert_ptr_nonnull
#define ck_assert_ptr_nonnull(X) _ck_assert_ptr(X, !=, NULL)
#endif
#ifndef ck_assert_ptr_null
#define ck_assert_ptr_null(X) _ck_assert_ptr(X, ==, NULL)
#endif

int int_compare(const void *a, const void *b) {
    int x = *((const int *) a);
    int y = *((const int *) b);

    return x - y;
}

// 'child_adjustment' must be set to 0 or 1 depending on the heap
// implementation. Heaps that start at array position 0 need a
// child_adjustment of 1, and heaps that start at array position 1
// need an child_adjustment of 0.
static int child_adjustment = 1;

/* Return the position of the first element if it is 0 or
 * 1, otherwise return -1.  */
int first_heap_element(void) {
    int heap_type = -1;
    int *elem_p = malloc(sizeof(int));
    prioq *q = prioq_init(int_compare);
    if (!q) {
        return -1;
    }
    prioq_insert(q, elem_p);
    int *pos0_p = (int *) array_get(q->array, 0);
    int *pos1_p = (int *) array_get(q->array, 1);

    if (pos0_p == pos1_p) { // weird?
        heap_type = -1;
    } else if (pos0_p == elem_p) {
        heap_type = 0;
    } else if (pos1_p == elem_p) {
        heap_type = 1;
    }

    prioq_cleanup(q, free);
    return heap_type;
}

// Return child_adjustment {0,1} to use in heap verification.
// or -1 if we cannot find it.
int calc_child_adjustment(void) {
    int first_pos = first_heap_element();
    if (first_pos == 0) {
        return 1;
    } else if (first_pos == 1) {
        return 0;
    }
    return -1;
}

int check_min_heap(prioq *p, int index) {
    int left = 1;
    int right = 1;

    if (index * 2 + 1 < prioq_size(p)) {
        left = p->compare(array_get(p->array, index * 2 + child_adjustment),
                          array_get(p->array, index));
        left = left > 0 && check_min_heap(p, index * 2 + child_adjustment) > 0;
    }

    if (index * 2 + 2 < prioq_size(p)) {
        right = p->compare(array_get(p->array, index * 2 + child_adjustment + 1),
                           array_get(p->array, index));
        right = right > 0 && check_min_heap(p, index * 2 + child_adjustment + 1) > 0;
    }

    return left > 0 && right > 0;
}

int check_max_heap(prioq *p, int index) {
    int left = 1;
    int right = 1;

    if (index * 2 + 1 < prioq_size(p)) {
        left = p->compare(array_get(p->array, index),
                          array_get(p->array, index * 2 + child_adjustment));
        left = left > 0 && check_max_heap(p, index * 2 + child_adjustment) > 0;
    }

    if (index * 2 + 2 < prioq_size(p)) {
        right = p->compare(array_get(p->array, index),
                           array_get(p->array, index * 2 + child_adjustment + 1));
        right = right > 0 && check_max_heap(p, index * 2 + child_adjustment + 1) > 0;
    }

    return left > 0 && right > 0;
}

int check_heap(prioq *p) {
    // Set global child_adjustment var before verifying heap.
    child_adjustment = calc_child_adjustment();

    // 1-heaps start at index 1 but have no child_adjustment for
    // calculating parent/child indices.
    // 0-heaps have child_adjustment but start at index 0.
    int start_index = child_adjustment == 0 ? 1 : 0;

    return check_min_heap(p, start_index) || check_max_heap(p, start_index);
}

/* Tests */

/* test init/cleanup */
START_TEST(test_init) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);
    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

/* test insert/pop */
START_TEST(test_insert_pop_basic) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    int a = 5;
    ck_assert_int_eq(prioq_insert(p, &a), 0);
    ck_assert_int_eq(*((int *) prioq_pop(p)), a);
    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

START_TEST(test_insert_pop10) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    int values[10] = { 5, 7, 2, 4, 0, 9, 12, 13, 28, 1 };
    int expected[10] = { 0, 1, 2, 4, 5, 7, 9, 12, 13, 28 };

    for (int i = 0; i < 10; i++)
        ck_assert_int_eq(prioq_insert(p, values + i), 0);

    for (int i = 0; i < 10; i++)
        ck_assert_int_eq(*((int *) prioq_pop(p)), expected[i]);

    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

/* Test parent-childern heap properties, and array reallocation. */
/* array.c is provided it will probably not fail on reallocation. */
START_TEST(test_insert_pop_many) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    unsigned long int amount = 16384;
    int values[amount];
    int expected[amount];

    for (unsigned int i = 0; i < amount; i++) {
        values[i] = rand();
        expected[i] = values[i];
        ck_assert_int_eq(prioq_insert(p, values + i), 0);
    }

    qsort(expected, amount, sizeof(int), int_compare);
    ck_assert_int_gt(check_heap(p), 0);

    for (unsigned int i = 0; i < amount; i++) {
        ck_assert_int_eq(*((int *) prioq_pop(p)), expected[i]);
    }

    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

/* Test pop on empty heap. */
START_TEST(test_pop_empty) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);
    ck_assert_ptr_null(prioq_pop(p));

    /* Insert and double pop. */
    int a = 2;
    ck_assert_int_eq(prioq_insert(p, &a), 0);
    ck_assert_int_eq(*((int *) prioq_pop(p)), a);
    ck_assert_ptr_null(prioq_pop(p));
    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

/* Heap verification. */
START_TEST(test_heap_valid) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    int values[10] = { 8, 34, 2, 4, 3, 9, 12, 13, 28, 1 };

    for (int i = 0; i < 10; i++) {
        ck_assert_int_eq(prioq_insert(p, values + i), 0);
        ck_assert_int_gt(check_heap(p), 0);
    }

    for (int i = 0; i < 10; i++) {
        prioq_pop(p);
        ck_assert_int_gt(check_heap(p), 0);
    }
    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

Suite *heap_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Heap");
    /* Core test case */
    tc_core = tcase_create("Core");

    /* Regular tests. */
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_insert_pop_basic);
    tcase_add_test(tc_core, test_insert_pop10);
    tcase_add_test(tc_core, test_insert_pop_many);
    tcase_add_test(tc_core, test_pop_empty);
    tcase_add_test(tc_core, test_heap_valid);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = heap_suite();
    sr = srunner_create(s);

    // srunner_run_all(sr, CK_NORMAL);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
