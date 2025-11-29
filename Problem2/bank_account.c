#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define DEPOSIT_THREADS 4
#define WITHDRAW_THREADS 4
#define OPERATIONS_PER_THREAD 100000

long long balance = 0;

pthread_mutex_t balance_mutex;
pthread_spinlock_t balance_spin;

typedef enum {
    LOCK_MUTEX = 0,
    LOCK_SPIN
} lock_mode_t;

typedef enum {
    CS_SHORT = 0,
    CS_LONG
} cs_mode_t;

lock_mode_t g_lock_mode = LOCK_MUTEX;
cs_mode_t g_cs_mode = CS_SHORT;

static void lock_balance(void) {
    if (g_lock_mode == LOCK_MUTEX) {
        pthread_mutex_lock(&balance_mutex);
    } else {
        pthread_spin_lock(&balance_spin);
    }
}

static void unlock_balance(void) {
    if (g_lock_mode == LOCK_MUTEX) {
        pthread_mutex_unlock(&balance_mutex);
    } else {
        pthread_spin_unlock(&balance_spin);
    }
}

void *deposit_thread(void *arg) {
    (void)arg;
    for (long long i = 0; i < OPERATIONS_PER_THREAD; i++) {
        lock_balance();
        balance++;
        if (g_cs_mode == CS_LONG) {
            usleep(100);
        }
        unlock_balance();
    }
    return NULL;
}

void *withdraw_thread(void *arg) {
    (void)arg;
    for (long long i = 0; i < OPERATIONS_PER_THREAD; i++) {
        lock_balance();
        balance--;
        if (g_cs_mode == CS_LONG) {
            usleep(100);
        }
        unlock_balance();
    }
    return NULL;
}

double elapsed_seconds(struct timeval start, struct timeval end) {
    double s = (double)(end.tv_sec - start.tv_sec);
    double us = (double)(end.tv_usec - start.tv_usec) / 1000000.0;
    return s + us;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <mutex|spin> <short|long>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "mutex") == 0) {
        g_lock_mode = LOCK_MUTEX;
        pthread_mutex_init(&balance_mutex, NULL);
    } else if (strcmp(argv[1], "spin") == 0) {
        g_lock_mode = LOCK_SPIN;
        pthread_spin_init(&balance_spin, PTHREAD_PROCESS_PRIVATE);
    } else {
        printf("Unknown lock mode.\n");
        return 1;
    }

    if (strcmp(argv[2], "short") == 0) {
        g_cs_mode = CS_SHORT;
    } else if (strcmp(argv[2], "long") == 0) {
        g_cs_mode = CS_LONG;
    } else {
        printf("Unknown critical section mode.\n");
        return 1;
    }

    pthread_t deposits[DEPOSIT_THREADS];
    pthread_t withdraws[WITHDRAW_THREADS];

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < DEPOSIT_THREADS; i++) {
        if (pthread_create(&deposits[i], NULL, deposit_thread, NULL) != 0) {
            perror("pthread_create deposit");
            return 1;
        }
    }

    for (int i = 0; i < WITHDRAW_THREADS; i++) {
        if (pthread_create(&withdraws[i], NULL, withdraw_thread, NULL) != 0) {
            perror("pthread_create withdraw");
            return 1;
        }
    }

    for (int i = 0; i < DEPOSIT_THREADS; i++) {
        pthread_join(deposits[i], NULL);
    }

    for (int i = 0; i < WITHDRAW_THREADS; i++) {
        pthread_join(withdraws[i], NULL);
    }

    gettimeofday(&end, NULL);
    double sec = elapsed_seconds(start, end);

    printf("Lock: %s, critical section: %s\n",
           (g_lock_mode == LOCK_MUTEX) ? "mutex" : "spin",
           (g_cs_mode == CS_SHORT) ? "short" : "long");
    printf("Final balance: %lld (expected 0)\n", balance);
    printf("Elapsed time: %.6f seconds\n", sec);

    if (g_lock_mode == LOCK_MUTEX) {
        pthread_mutex_destroy(&balance_mutex);
    } else {
        pthread_spin_destroy(&balance_spin);
    }

    return 0;
}
