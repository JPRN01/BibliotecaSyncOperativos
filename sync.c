#include "sync.h"
#include <stdio.h>
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

// Implementación de funciones de monitor
void monitor_init(Monitor *monitor) {
    pthread_mutex_init(&monitor->mutex, NULL);
    pthread_cond_init(&monitor->cond, NULL);
    monitor->busy = 0;
}

void monitor_enter(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mutex);
    while (monitor->busy) {
        printf("Thread %ld is waiting.\n", pthread_self());
        pthread_cond_wait(&monitor->cond, &monitor->mutex);
    }
    monitor->busy = 1;
    pthread_mutex_unlock(&monitor->mutex);
}

void monitor_exit(Monitor *monitor) {
    pthread_mutex_lock(&monitor->mutex);
    monitor->busy = 0;
    pthread_cond_signal(&monitor->cond);
    pthread_mutex_unlock(&monitor->mutex);
}

// Implementación de funciones de barrera
void barrier_init(Barrier *barrier, int count) {
    barrier->count = count;
    barrier->arrived = 0;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
}

void barrier_sync(Barrier *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    barrier->arrived++;

    if (barrier->arrived == barrier->count) {
        barrier->arrived = 0; 
        pthread_cond_broadcast(&barrier->cond); // Despierta a todos los hilos en espera
    } else {
        // Espera hasta que el último hilo llegue a la barrera
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
}
