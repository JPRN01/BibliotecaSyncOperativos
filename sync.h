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

// Estructura para un monitor
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int busy;
} Monitor;

// Estructura para una barrera
typedef struct {
    int count;
    int arrived;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Barrier;

void init_semaphore(Semaphore *sem, int initialCount);
void wait_semaphore(Semaphore *sem);
void post_semaphore(Semaphore *sem);
void destroy_semaphore(Semaphore *sem);

// Funciones para operaciones de monitor
void monitor_init(Monitor *monitor);
void monitor_enter(Monitor *monitor);
void monitor_exit(Monitor *monitor);

// Funciones para operaciones de barrera
void barrier_init(Barrier *barrier, int count);
void barrier_sync(Barrier *barrier);

#endif
