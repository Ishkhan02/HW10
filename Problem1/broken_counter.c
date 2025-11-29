#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N_THREADS 4
#define INCREMENTS 1000000LL

long long counter = 0;

typedef enum {
    MODE_NO_SYNC = 0,
    MODE_MUTEX,
    MODE_SPIN
} counter_mode_t;

pthread_mutex_t counter_mutex;
pthread_spinlock_t counter_spin;
counter_mode_t g_mode = MODE_NO_SYNC;

void *worker(void *arg) {
    (void)arg;
    for (long long i = 0; i < INCREMENTS; i++) {
        switch (g_mode) {
            case MODE_NO_SYNC:
                counter++;
                break;
            case MODE_MUTEX:
                pthread_mutex_lock(&counter_mutex);
                counter++;
                pthread_mutex_unlock(&counter_mutex);
                break;
            case MODE_SPIN:
                pthread_spin_lock(&counter_spin);
                counter++;
                pthread_spin_unlock(&counter_spin);
                break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <no_sync|mutex|spin>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "no_sync") == 0) {
        g_mode = MODE_NO_SYNC;
    } else if (strcmp(argv[1], "mutex") == 0) {
        g_mode = MODE_MUTEX;
        pthread_mutex_init(&counter_mutex, NULL);
    } else if (strcmp(argv[1], "spin") == 0) {
        g_mode = MODE_SPIN;
        pthread_spin_init(&counter_spin, PTHREAD_PROCESS_PRIVATE);
    } else {
        printf("Unknown mode.\n");
        return 1;
    }

    pthread_t threads[N_THREADS];
    long long expected = N_THREADS * INCREMENTS;

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    if (g_mode == MODE_MUTEX)
        pthread_mutex_destroy(&counter_mutex);

    if (g_mode == MODE_SPIN)
        pthread_spin_destroy(&counter_spin);

    printf("Mode: %s\n", argv[1]);
    printf("Expected: %lld\n", expected);
    printf("Actual:   %lld\n", counter);

    return 0;
}
