/* Name: Kraiwin Namsri
 * UvA-netID: 15032094@uva.nl
 *
 * Welcome to Kraiwin's doctors queue. This is an implementation of a very good doctors queue.
 */
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prioq.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config {
  /* You can ignore these options until/unless you implement the */
  /* bonus features. */

  /* Set to 1 if -y is specified, 0 otherwise. */
  int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

typedef struct {
  char *name;
  int age;
  int duration;
  int healthy;
} patient_t;

/* Initializes a patient */
patient_t *init_patient(const char *name, int age, int duration) {
  patient_t *p = malloc(sizeof(patient_t));
  if (!p)
    return NULL;

  /* Make a copy of the name, to prevent loose pointers. */
  char *name_cpy = malloc(strlen(name) + 1);
  if (!name_cpy) {
    free(p);
    return NULL;
  }

  strcpy(name_cpy, name);
  p->name = name_cpy;
  p->age = age;
  p->duration = duration;
  p->healthy = 0;

  return p;
}

/* Cleans up a patient */
void cleanup_patient(patient_t *p) {
  free(p->name);
  free(p);
}

/* Makes a patient healthy.
 * Returns 1 if patient is healthy, 0 if he needs more session's with the doctor.
 */
static int treat_patient(patient_t *t) {
  t->duration--;
  if (t->duration <= 0)
    t->healthy = 1;
  return t->healthy;
}

static int compare_patient_name(const void *a, const void *b) {
  return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
}

static int compare_patient_age(const void *a, const void *b) {
  int a_age = ((const patient_t *)a)->age;
  int b_age = ((const patient_t *)b)->age;
  return a_age - b_age;
}

int main(int argc, char *argv[]) {
  prioq *queue;
  struct config cfg;

  if (parse_options(&cfg, argc, argv) != 0) {
    return EXIT_FAILURE;
  }

  if (cfg.year) {
    queue = prioq_init(&compare_patient_age);
  } else {
    queue = prioq_init(&compare_patient_name);
  }

  patient_t *p = NULL;
  for (int iterations = 0;;) {
    while (1) {
      char *s = fgets(buf, BUF_SIZE, stdin);
      if (s == NULL) {
        fprintf(stderr, "Unexpected end of file. exiting\n");
        return EXIT_FAILURE;
        prioq_cleanup(queue, (void(*)(void*))cleanup_patient);
      }

      /* read all the input lines until you reach a "." on a separate line */
      if (strstr(s, ".\n")) {
        break;
      }

      /* Accept all the patients "waiting at the door" at that time: */ 
      const char *delim = " ";
      char *name = strtok(s, delim);

      /* Parse the age */
      char *age_char = strtok(NULL, delim);
      int age = -1;
      if (age_char) {
        char *ptr;
        age = (int)strtol(age_char, &ptr, 10);
      }

      /* Parse the duration of the visit. */
      char *duration_char = strtok(NULL, delim);
      int duration = 1;
      if (duration_char) {
        char *ptr;
        duration = (int)strtol(duration_char, &ptr, 10);
      }

      /* If the person has a name, he may enter the queue. */
      if (name) {
        patient_t *p = init_patient(name, age, duration);
        prioq_insert(queue, p);
      }
    }

    /* Pick the first patient in alphabetic order using the priority queue; */
    if (!p)
      p = (patient_t *)prioq_pop(queue);

    /* Treat the patient */
    if (p && treat_patient(p)) {
      /* The patient is healthy. */
      printf("%s\n", p->name);
      cleanup_patient(p);
      p = NULL;
    }

    printf(".\n"); /* End turn. */

    /* "at the end of the day" (after ten iterations of the loop, so ten hours later) */
    if (++iterations == 10) {
      /* It could be that the doctor is not helping anyone. */
      if (!p)
        p = (patient_t *)prioq_pop(queue);

      /* Print the names of all patients still in the queue in alphabetical order.*/
      while (p) {
        printf("%s\n", p->name);
        cleanup_patient(p);
        p = (patient_t *)prioq_pop(queue);
      }

      prioq_cleanup(queue, (void(*)(void*))cleanup_patient);
      break;
    }
  }

  return EXIT_SUCCESS;
}

int parse_options(struct config *cfg, int argc, char *argv[]) {
  memset(cfg, 0, sizeof(struct config));
  int c;
  while ((c = getopt(argc, argv, "y")) != -1) {
    switch (c) {
      case 'y':
        cfg->year = 1;
        break;
      default:
        fprintf(stderr, "invalid option: -%c\n", optopt);
        return 1;
    }
  }
  return 0;
}
