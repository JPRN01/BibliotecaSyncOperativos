#include "sync.h"
#include <stdlib.h>

void init_semaphore(Semaphore *sem, int initialCount) {
    sem->count = initialCount;
    sem->waiting_list = NULL;
    pthread_mutex_init(&sem->mutex, NULL);
}

void add_to_waiting_list(Semaphore *sem, WaitingThread *wt) {
    wt->next = sem->waiting_list;
    sem->waiting_list = wt;
}

void wait_semaphore(Semaphore *sem) {
    pthread_mutex_lock(&sem->mutex);

    if (sem->count > 0) {
        sem->count--;
        pthread_mutex_unlock(&sem->mutex);
    } else {
      WaitingThread wt;
      pthread_cond_init(&wt.cond, NULL);
      add_to_waiting_list(sem, &wt);
      pthread_cond_wait(&wt.cond, &sem->mutex);
    }
    pthread_mutex_unlock(&sem->mutex);
}

void post_semaphore(Semaphore *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->count++;
    if (sem->waiting_list != NULL) {
        WaitingThread *wt = sem->waiting_list;
        sem->waiting_list = wt->next;
        pthread_cond_signal(&wt->cond);
    } 
    pthread_mutex_unlock(&sem->mutex);
}

void destroy_semaphore(Semaphore *sem) {
    pthread_mutex_destroy(&sem->mutex);
    while (sem->waiting_list != NULL) {
        WaitingThread *wt = sem->waiting_list;
        sem->waiting_list = wt->next;
        pthread_cond_destroy(&wt->cond);
    }
}
