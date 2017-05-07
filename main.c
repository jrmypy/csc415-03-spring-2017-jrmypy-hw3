#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define SIZE 10
int NUMBER_OF_THREADS = 10;
double itemsProduced;
double numProducer;
double numConsumer;
int totalConsumed = 0;

#define TRUE 1
#define FALSE 0

typedef int buffer_t;
buffer_t buffer[SIZE];
int buffer_index;

sem_t emptyCount;
sem_t filledCount;
pthread_mutex_t buffer_mutex;

void insertItemToBuffer(buffer_t value) {
  if(buffer_index < SIZE) {
    buffer[buffer_index++] = value;
  }
}

buffer_t removeItemFromBuffer() {
  if(buffer_index > 0) {
    return buffer[--buffer_index];
  }
  return 0;
}


int isEmpty() {
  if(buffer_index == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int isFull() {
  if(buffer_index == SIZE) {
    return TRUE;
  } else {
    return FALSE;
  }
}

void *producer(void *thread_n) {
  int thread_number = *(int *)thread_n;
  buffer_t value;
  int count = 0;
  int i=0;

  while(i++ < itemsProduced) {
    value = thread_number * itemsProduced + count;
    pthread_mutex_lock(&buffer_mutex);
    do {
      count++;
      pthread_mutex_unlock(&buffer_mutex);
      sem_wait(&emptyCount);
      pthread_mutex_lock(&buffer_mutex);
    } while(isFull());
    insertItemToBuffer(value);
    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&filledCount);
    printf("Producer %d added %d to buffer\n", thread_number, value);
  }
  pthread_exit(0);
}

void *consumer(void *thread_n) {
  int thread_number = *(int *)thread_n;
  buffer_t value;
  int count = 0;
  int i = 0;

  if(numProducer == numConsumer) {
    while(i++ < itemsProduced) {
      pthread_mutex_lock(&buffer_mutex);
      do {
        count++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_wait(&filledCount);
        pthread_mutex_lock(&buffer_mutex);
      } while (isEmpty());
      value = removeItemFromBuffer(value);
      pthread_mutex_unlock(&buffer_mutex);
      sem_post(&emptyCount);
      printf("Consumer %d dequeue %d from buffer\n", thread_number, value);
    }
  } else if(numProducer < numConsumer) {
    int difference = itemsProduced / numConsumer;

    while(i++ < difference) {
      pthread_mutex_lock(&buffer_mutex);
      do {
        count++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_wait(&filledCount);
        pthread_mutex_lock(&buffer_mutex);
      } while (isEmpty());
      value = removeItemFromBuffer(value);
      pthread_mutex_unlock(&buffer_mutex);
      sem_post(&emptyCount);
      printf("Consumer %d dequeue %d from buffer\n", thread_number, value);
    }
  } else {
    int difference = itemsProduced * numProducer;

    while(i++ < difference) {
      pthread_mutex_lock(&buffer_mutex);
      do {
        count++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_wait(&filledCount);
        pthread_mutex_lock(&buffer_mutex);
      } while (isEmpty());
      value = removeItemFromBuffer(value);
      pthread_mutex_unlock(&buffer_mutex);
      sem_post(&emptyCount);
      printf("Consumer %d dequeue %d from buffer\n", thread_number, value);
    }
  }

  totalConsumed += count;
  printf("***** Consumer %d consumed %d items. *****\n" , thread_number, count);
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  buffer_index = 0;
  numProducer = pow(2, (double)atoi(argv[1]));
  numConsumer = pow(2, (double)atoi(argv[2]));
  itemsProduced = pow(2, (double)atoi(argv[3]));

  pthread_mutex_init(&buffer_mutex, NULL);
  sem_init(&emptyCount, 0, SIZE);
  sem_init(&filledCount, 0, 0);
  pthread_t thread[NUMBER_OF_THREADS];
  int thread_numb[NUMBER_OF_THREADS];
  int i;
  int totalItemsProduced = 0;
  int joined;

  if(numProducer > numConsumer) {
    joined = numProducer;
  } else {
    joined = numConsumer;
  }

  for (i = 0; i < numProducer;) {
    thread_numb[i] = i;
    pthread_create(thread + i, NULL, producer, thread_numb + i);
    i++;
    thread_numb[i] = i;
    totalItemsProduced += itemsProduced;
  }

  for(i = 0; i < numConsumer;) {
    pthread_create(&thread[i], NULL, consumer, &thread_numb[i]);
    i++;
  }

  for(i = 0; i < joined; i++) {
    pthread_join(thread[i], NULL);
  }

  pthread_mutex_destroy(&buffer_mutex);
  sem_destroy(&emptyCount);
  sem_destroy(&filledCount);

  printf("====== total items PRODUCED: %d ======\n", totalItemsProduced);
  printf("====== total items CONSUMED: %d ======\n", totalItemsProduced);

  return 0;
}