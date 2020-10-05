#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

int main(int argc, char* argv[]) {
  /* 1. Get command line arguments argv[1], argv[2], argv[3] - must convert to ints */

  int sleepBeforeTerminating = atoi(argv[1]);
  int numProds = atoi(argv[2]);
  int numCons = atoi(argv[3]);
  int i;

  /* 2. Initialize buffer, mutex, semaphores, other global vars */

  pthread_attr_t attr;
  pthread_t *tidProducers, *tidConsumers;
  initialize(&attr);
  tidProducers = (pthread_t*) malloc(numProds * sizeof(pthread_t));
  tidConsumers = (pthread_t*) malloc(numCons * sizeof(pthread_t));

  /* 3. Create producer thread(s) */

  for (i = 0; i < numProds; i++) {
    if (pthread_create(&(tidProducers[i]), &attr, producer, NULL)) {
      printf("Failed to create producer thread %d.\n", i);
    } else {
      printf("Producer thread %d created successfully.\n", i);
    }
  }

  /* 4. Create consumer thread(s) */

  for (i = 0; i < numCons; i++) {
    if (pthread_create(&(tidConsumers[i]), &attr, consumer, NULL)) {
      printf("Failed to create consumer thread %d.\n", i);
    } else {
      printf("Consumer thread %d created successfully.\n", i);
    }
  }

  /* 5. Sleep */

  sleep(sleepBeforeTerminating);

  /* 6. Release resources, e.g. destroy mutex and semaphores */

  destroy();

  for (i = 0; i < numProds; i++) {
    if (pthread_cancel(tidProducers[i])) {
      printf("Failed to cancel producer thread %d.\n", i);
    } else {
      printf("Producer thread %d cancelled successfully.\n", i);
    }
    if (pthread_join(tidProducers[i], NULL)) {
      printf("Failed to join producer thread %d.\n", i);
    } else {
      printf("Producer thread %d joined successfully.\n", i);
    }
  }

  for (i = 0; i < numCons; i++) {
    if (pthread_cancel(tidConsumers[i])) {
      printf("Failed to cancel consumer thread %d.\n", i);
    } else {
      printf("Consumer thread %d cancelled successfully.\n", i);
    }
    if (pthread_join(tidConsumers[i], NULL)) {
      printf("Failed to join consumer thread %d.\n", i);
    } else {
      printf("Consumer thread %d joined successfully.\n", i);
    }
  }

  /* 7. Exit */
  free(tidProducers);
  free(tidConsumers);
  return(0);
} 