#HW3 Producer-Consumer using Pthreads
#### PaoYang Tan

## Write-Up of Homework:
Each producer produces 32 items.
1 producer and 2 consumers: Each consumer consumes 16 items.
2 producers and 1 consumer: Consumer consumes 64 items.
2 producers and 4 consumers: Each consumer consumes 8 items.
4 producers and 2 consumers: Each consumer consumes 64 items. 

## Problem statement:

Implement a multi-threaded producer-consumer program using a bounded buffer. Use full and empty semaphores to keep track of the numbers of full and empty slots available to the consumers and producers, respectively. Use a mutex to coordinate access to the buffer (once the thread determines that there is an available slot via the appropriate semaphore). The number of producers, the number of consumers, and the number of items each producer produces should be specified by their binary log as command-line parameters. Thus, the following command should generate 8 producers, 16 consumers, and 64 items produced by each producer:

```
./producer-consumer 3 4 6
```
The consumers need to each consume the same number of items before exiting. Your code should calculate the number of items each consumer needs to consume so that every item produced gets consumed. The main thread should parse all of the command-line parameters, print them in a message, initialize the synchronization objects, spawn all of the threads, wait for them to complete, and then print a final message. The items produced by the producer threads should be integers, obtained using the following expression:
`thread_number * num_produced + counter`
where thread_number is an integer passed to each thread ranging from 0 to num_threads - 1, num_produced is the number of items produced by each producer, and counter is a local variable in each thread that gets initialized to 0 and incremented every time a new item is produced. The consumer threads should consume these items by simply printing them. (Printing is the only required "consumption".)
Use `pthread_mutex_init, pthread_mutex_lock, pthread_mutex_unlock, sem_init, sem_wait and sem_post` for synchronization in POSIX

You are required to write a short writeup and ouput with various inputs. test your code with 1 producer and 2 consumers, 2 producers and 1 consumer, 2 producers and 4 consumers, and 4 consumers and 2 producers, each time with the producers producing 32 items each.


## Travis
This program has no tests. You will have to make this program run without errors on Travis. Travis will be used to build your solution

## Plagiarism
Please do not copy-paste from the internet or from friends. The plagiarism detection system will flag it.

## Note
* There is **no partial credit** for code that does not compile
* The program would not compile at first because the program has errors
* It is required that you add your **name** and **travis ci badge** to your readme.
* Check the logs from Travis to make sure that your program is compiling.
* Make sure your last push is before the deadline. Your last push will be considered as your final submission.
* If you need to be considered for partial grade for any reason(failing tests on travis,etc). Then email the staff before the deadline. Late email requests may not be considered.
* Vagrant is entirely optional. Use it only if your development environment is giving you problems.
* Post questions on Piazza if you have any questions.
* Please contact the course staff if you run into issues. We are here to help you!
