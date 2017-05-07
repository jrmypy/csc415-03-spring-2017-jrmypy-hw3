#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>


#define SIZE 10
int NUMB_THREADS = 10;
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

void insertItemBuffer(buffer_t value) {
  if(buffer_index < SIZE) {
    buffer[buffer_index++] = value;
  }
}

buffer_t removeItemBuffer() {
  if(buffer_index > 0) {
    return buffer[--buffer_index]; // buffer_index-- would be error!
  }
  return 0;
}


int isempty() {
  if (buffer_index == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int isfull() {
  if (buffer_index == SIZE) {
    return TRUE;
  } else {
    return FALSE;
  }
}

void *producer(void *thread_n) {
  int thread_number = *(int *)thread_n;
//  int thread_number = (int)(uintptr_t)thread_n;
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
    } while(isfull());
    insertItemBuffer(value);
    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&filledCount);
    printf("Producer %d added %d to buffer\n", thread_number, value);
//    if (i % 2 == 1) sleep(1);
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
      } while (isempty());
      value = removeItemBuffer(value);
      pthread_mutex_unlock(&buffer_mutex);
      sem_post(&emptyCount);
      printf("Consumer %d dequeue %d from buffer\n", thread_number, value);
    }
  } else if(numProducer < numConsumer){
    int difference = itemsProduced / numConsumer;

    while(i++ < difference) {
      pthread_mutex_lock(&buffer_mutex);
      do {
        count++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_wait(&filledCount);
        pthread_mutex_lock(&buffer_mutex);
      } while (isempty());
      value = removeItemBuffer(value);
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
      } while (isempty());
      value = removeItemBuffer(value);
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
  sem_init(&emptyCount, // sem_t *sem
           0, // int pshared. 0 = shared between threads of process,  1 = shared between processes
           SIZE); // unsigned int value. Initial value
  sem_init(&filledCount,
           0,
           0);
  /* emptyCount is initialized to buffer size because SIZE number of
     producers can add one element to buffer each. They will wait
     semaphore each time, which will decrement semaphore value.
     filledCount is initialized to 0, because buffer starts empty and
     consumer cannot take any element from it. They will have to wait
     until producer posts to that semaphore (increments semaphore
     value) */
  pthread_t thread[NUMB_THREADS];
  int thread_numb[NUMB_THREADS];
  int i;
  int totalItemsProduced = 0;
  int joined;

  if(numProducer > numConsumer) {
    joined = numProducer;
  } else {
    joined = numConsumer;
  }

  for (i = 0; i < numProducer; ) {
    thread_numb[i] = i;
    pthread_create(thread + i, NULL, producer, thread_numb + i);
    i++;
    thread_numb[i] = i;
    totalItemsProduced += itemsProduced;
  }

    for(i = 0; i < numConsumer; ) {
      pthread_create(&thread[i], NULL, consumer, &thread_numb[i]);
      i++;
    }

  for(i = 0; i < joined; i++)
    pthread_join(thread[i], NULL);

  pthread_mutex_destroy(&buffer_mutex);
  sem_destroy(&emptyCount);
  sem_destroy(&filledCount);

  printf("====== total items PRODUCED: %d ======\n", totalItemsProduced);
  printf("====== total items CONSUMED: %d ======\n", totalItemsProduced);

  return 0;
}