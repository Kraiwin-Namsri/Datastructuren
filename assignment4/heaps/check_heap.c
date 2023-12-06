#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
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

    if (index * 2 + child_adjustment < prioq_size(p)) {
        left = p->compare(array_get(p->array, index * 2 + child_adjustment),
                          array_get(p->array, index));
        left = left >= 0 && check_min_heap(p, index * 2 + child_adjustment) > 0;
    }

    if (index * 2 + child_adjustment + 1 < prioq_size(p)) {
        right = p->compare(array_get(p->array, index * 2 + child_adjustment + 1),
                           array_get(p->array, index));
        right = right >= 0 && check_min_heap(p, index * 2 + child_adjustment + 1) > 0;
    }

    return left > 0 && right > 0;
}

int check_max_heap(prioq *p, int index) {
    int left = 1;
    int right = 1;

    if (index * 2 + child_adjustment < prioq_size(p)) {
        left = p->compare(array_get(p->array, index),
                          array_get(p->array, index * 2 + child_adjustment));
        left = left >= 0 && check_max_heap(p, index * 2 + child_adjustment) > 0;
    }

    if (index * 2 + child_adjustment + 1 < prioq_size(p)) {
        right = p->compare(array_get(p->array, index),
                           array_get(p->array, index * 2 + child_adjustment + 1));
        right = right >= 0 && check_max_heap(p, index * 2 + child_adjustment + 1) > 0;
    }

    return left > 0 && right > 0;
}

/* Returns 1 if the heap is correct, 0 otherwise. */
int check_heap(prioq *p) {
    // Set global child_adjustment var before verifying heap.
    child_adjustment = calc_child_adjustment();

    // 1-heaps start at index 1 but have no child_adjustment for
    // calculating parent/child indices.
    // 0-heaps have child_adjustment but start at index 0.
    int start_index = child_adjustment == 0 ? 1 : 0;

    return check_min_heap(p, start_index) || check_max_heap(p, start_index);
}

/* Helper functions to print the current int heap state. */
void print_int_heap(struct heap *h) {
    printf("--------------------------------\n");
    printf("printing heap array of size: %ld\n", array_size(h->array));
    for (int i = 0; i < array_size(h->array); i++) {
        int *val = (int *)array_get(h->array, i);
        if (val) {
            printf("%d: %d\n", i, *val);
        } else {
            printf("%d: null\n", i);
        }
    }
    printf("--------------------------------\n");
}

void free_nothing(void *item) {
  /* Do absolutely nothing */
}
/* Tests */

/* test init/cleanup */
START_TEST(test_init) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);
    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

START_TEST(test_percolate_up) {
  prioq *p = prioq_init(int_compare);
  ck_assert_ptr_nonnull(p);

  int a = 5;
  int b = 1;
  ck_assert_int_eq(prioq_insert(p, &a), 0);
  ck_assert_int_eq(prioq_insert(p, &b), 0);
  ck_assert_ptr_eq(array_get(p->array, 1), &b);
  ck_assert_ptr_eq(array_get(p->array, 2), &a);
  ck_assert_int_eq(prioq_cleanup(p, free_nothing), 0);
}
END_TEST

START_TEST(test_percolate_up10) {
  prioq *p = prioq_init(int_compare);
  ck_assert_ptr_nonnull(p);
  int values[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };

  for (int i = 0; i < 10; i++) {
    ck_assert_int_eq(prioq_insert(p, values + i), 0);
    ck_assert_ptr_eq(array_get(p->array, 1), values + i);
  }

  ck_assert_int_eq(prioq_cleanup(p, free_nothing), 0);
}
END_TEST

/* test insert/pop */
START_TEST(test_insert_pop_basic) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    int a = 5;
    ck_assert_int_eq(prioq_insert(p, &a), 0);
    ck_assert_int_eq(*((int *) prioq_pop(p)), a);
    ck_assert_int_eq(prioq_cleanup(p, free_nothing), 0);
}
END_TEST

START_TEST(test_insert_pop10) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    int values[10] = { 5, 7, 2, 4, 0, 9, 12, 13, 28, 1 };
    int expected[10] = { 0, 1, 2, 4, 5, 7, 9, 12, 13, 28 };

    for (int i = 0; i < 10; i++) {
        // printf("inserting: %d\n", values[i]);
        ck_assert_int_eq(prioq_insert(p, values + i), 0);
        // print_int_heap(p);
    }

    for (int i = 0; i < 10; i++) {
        int x = *(int *) prioq_pop(p);
        // printf("popped value: %d\n", x);
        // print_int_heap(p);
        ck_assert_int_eq(x, expected[i]);
    }

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
    ck_assert_int_eq(check_heap(p), 1);

    for (unsigned int i = 0; i < amount; i++) {
        ck_assert_int_eq(*((int *) prioq_pop(p)), expected[i]);
    }

    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST

START_TEST(test_insert_pop_with_duplicates) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    int values[10] = { 75, 74, 57, 83, 8, 21, 92, 36, 92, 36 };
    int expected[10];

    for (unsigned int i = 0; i < 10; i++) {
        expected[i] = values[i];
        // printf("inserting: %d\n", values[i]);
        ck_assert_int_eq(prioq_insert(p, values + i), 0);
        // print_int_heap(p);
        ck_assert_int_eq(check_heap(p), 1);
    }

    qsort(expected, 10, sizeof(int), int_compare);

    for (unsigned int i = 0; i < 10; i++) {
        int x = *(int *) prioq_pop(p);
        // printf("popped value: %d\n", x);
        // print_int_heap(p);
        ck_assert_int_eq(x , expected[i]);
        ck_assert_int_eq(check_heap(p), 1);
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
        ck_assert_int_eq(check_heap(p), 1);
    }

    for (int i = 0; i < 10; i++) {
        prioq_pop(p);
        ck_assert_int_eq(check_heap(p), 1);
    }
    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST


// To run this test compile with: make CFLAGS=-DINTERNAL_TESTS=1
#ifdef INTERNAL_TESTS
/* Internal test case for check_heap checking function.
 * First we check that the valid heap is accepted.
 * Then we check that a invalid heap is detected as invalid.
 */
START_TEST(internal_test_check_heap) {
    prioq *p = prioq_init(int_compare);
    ck_assert_ptr_nonnull(p);

    int valid_heap[] = { 1, 5, 10, 8, 7, 11 };

    // Manually create the 0-based heap using array_append
    for (size_t i = 0; i < sizeof(valid_heap) / sizeof(valid_heap[0]); i++) {
        array_append(p->array, valid_heap + i);
    }
    ck_assert_int_eq(check_heap(p), 1);

    for (size_t i = 0; i < sizeof(valid_heap) / sizeof(valid_heap[0]); i++) {
        prioq_pop(p);
    }

    int invalid_heap[] = { 5, 1, 10, 8, 7, 11 };
    // Manually create the 0-based heap using array_append
    for (size_t i = 0; i < sizeof(invalid_heap) / sizeof(invalid_heap[0]); i++) {
        array_append(p->array, invalid_heap + i);
    }
    // print_int_heap(p);
    ck_assert_int_eq(check_heap(p), 0);

    for (size_t i = 0; i < sizeof(invalid_heap) / sizeof(invalid_heap[0]); i++) {
        prioq_pop(p);
    }

    int invalid_heap2[] = { 1, 5, 11, 8, 7, 10 };
    // Manually create the 0-based heap using array_append
    for (size_t i = 0; i < sizeof(invalid_heap2) / sizeof(invalid_heap2[0]); i++) {
        array_append(p->array, invalid_heap2 + i);
    }
    // print_int_heap(p);
    ck_assert_int_eq(check_heap(p), 0);

    for (size_t i = 0; i < sizeof(invalid_heap2) / sizeof(invalid_heap2[0]); i++) {
        prioq_pop(p);
    }

    ck_assert_int_eq(prioq_cleanup(p, NULL), 0);
}
END_TEST
#endif

Suite *heap_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Heap");
    /* Core test case */
    tc_core = tcase_create("Core");

    /* Regular tests. */
    tcase_add_test(tc_core, test_init);
  tcase_add_test(tc_core, test_percolate_up);
    tcase_add_test(tc_core, test_percolate_up10);
    tcase_add_test(tc_core, test_insert_pop_basic);
    tcase_add_test(tc_core, test_insert_pop10);
    tcase_add_test(tc_core, test_insert_pop_many);
    tcase_add_test(tc_core, test_insert_pop_with_duplicates);
    tcase_add_test(tc_core, test_pop_empty);
    tcase_add_test(tc_core, test_heap_valid);
#ifdef INTERNAL_TESTS
    tcase_add_test(tc_core, internal_test_check_heap);
#endif

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
