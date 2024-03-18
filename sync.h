#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

typedef struct waiting_thread {
    pthread_cond_t cond;
    struct waiting_thread *next;
} WaitingThread;

typedef struct {
    int count;
    pthread_mutex_t mutex;
    WaitingThread *waiting_list;
} Semaphore;

void init_semaphore(Semaphore *sem, int initialCount);
void wait_semaphore(Semaphore *sem);
void post_semaphore(Semaphore *sem);
void destroy_semaphore(Semaphore *sem);

#endif
