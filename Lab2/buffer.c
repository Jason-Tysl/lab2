#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define RAND_NUM 10
#define RAND_SLEEP 10

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
sem_t full, empty;
pthread_attr_t attr;
int bufferIndex;

void initialize() {
    /* create the mutex lock */
    pthread_mutex_init(&mutex, NULL);
    /* create the full semaphore and initialize it to BUFFER_SIZE */
    sem_init(&full, 0, BUFFER_SIZE);
    /* create the empty semaphore and initialize it to 0 */
    sem_init(&empty, 0, 0);
    /* get the default attribute */
    pthread_attr_init(&attr);
    /* initialize buffer index */
    bufferIndex = 0;
}

/* insert an item into buffer */
int insert_item(buffer_item item) {

    /* return 0 if successful (buffer isn't full), otherwise
       return -1 indicating an error condition */
    if (bufferIndex < BUFFER_SIZE) {
        buffer[bufferIndex++] = item;
        printf("producer produced  %d\n", item);
        return 0;
    } else {
        printf("producer failed to add item\n");
        return -1;
    }

}

/* remove an object from buffer and placing it in item*/
int remove_item(buffer_item *item) {

    /* return 0 if successful, otherwise
       return -1 indicating an error condition */
    if (bufferIndex > 0) {
        /* Decrement before calling because bufferIndex points to the open spot */
        *item = buffer[--bufferIndex];
        printf("consumer consumed %d\n", rand);
        return 0;
    } else {
        printf("consumer failed to remove item\n");
        return -1;
    }
}

void *producer(void *param) {
    buffer_item rand;
    while (1) {
        /* sleep for a random period of time */
        unsigned int seed = (time(NULL));
        sleep(rand() % RAND_SLEEP);

        /* generate a random number */
        rand = rand_r(&seed) % RAND_NUM;

        /* acquire the semaphore */
        sem_wait(&empty);
        /* acquire the mutex lock */
        pthread_mutex_lock(&mutex);
        
        /*** critical section ***/
        /* insert item and print appropriate message */
        if (insert_item(rand) < 0) {
            printf("Producer failed to insert item.\n"); // report error condition
        } else {
            printf("Producer inserted item successfully.\n");
        }

        /* release the mutex lock */
        pthread_mutex_unlock(&mutex);
        /* release the semaphore */
        sem_post(&empty);
    }
}

void *consumer(void *param) {
    buffer_item  rand;
    while (1) {
        /* sleep for a random period of time */
        unsigned int seed = (time(NULL));
        sleep(rand() % RAND_SLEEP);

        /* acquire the semaphore */
        sem_wait(&full);
        /* acquire the mutex lock */
        pthread_mutex_lock(&mutex);

        /*** critical section ***/
        /* remove item from buffer and print appropriate message */
        if (remove_item(&rand) < 0){
            printf("Consumer failed to remove item.\n"); // report error condition
        } else {
            printf("Consumer removed item successfully.\n");
        }

        /* release the mutex lock */
        pthread_mutex_unlock(&mutex);
        /* release the semaphore */
        sem_post(&full);
    }
}

