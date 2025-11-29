#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 8

int buffer[BUFFER_SIZE];
int in_pos = 0;
int out_pos = 0;

sem_t empty_slots;
sem_t full_slots;
pthread_mutex_t buffer_mutex;

int next_item = 0;

typedef struct {
    int items_per_producer;
} producer_args_t;

void *producer(void *arg) {
    producer_args_t *pargs = (producer_args_t *)arg;
    for (int i = 0; i < pargs->items_per_producer; i++) {
        sem_wait(&empty_slots);

        pthread_mutex_lock(&buffer_mutex);
        int item = ++next_item;
        buffer[in_pos] = item;
        in_pos = (in_pos + 1) % BUFFER_SIZE;
        printf("Producer produced: %d\n", item);
        pthread_mutex_unlock(&buffer_mutex);

        sem_post(&full_slots);
    }
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    while (1) {
        sem_wait(&full_slots);

        pthread_mutex_lock(&buffer_mutex);
        int item = buffer[out_pos];
        out_pos = (out_pos + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer_mutex);

        sem_post(&empty_slots);

        if (item == -1) {
            break;
        } else {
            printf("Consumer consumed: %d\n", item);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <P producers> <C consumers> <K items per producer>\n", argv[0]);
        return 1;
    }

    int P = atoi(argv[1]);
    int C = atoi(argv[2]);
    int K = atoi(argv[3]);

    if (P <= 0 || C <= 0 || K <= 0) {
        printf("P, C, and K must be positive.\n");
        return 1;
    }

    long long total_items = (long long)P * K;
    printf("Total items to produce: %lld\n", total_items);

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);

    pthread_t *producers = malloc(sizeof(pthread_t) * P);
    pthread_t *consumers = malloc(sizeof(pthread_t) * C);

    producer_args_t pargs;
    pargs.items_per_producer = K;

    for (int i = 0; i < P; i++) {
        if (pthread_create(&producers[i], NULL, producer, &pargs) != 0) {
            perror("pthread_create producer");
            return 1;
        }
    }

    for (int i = 0; i < C; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, NULL) != 0) {
            perror("pthread_create consumer");
            return 1;
        }
    }

    for (int i = 0; i < P; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < C; i++) {
        sem_wait(&empty_slots);
        pthread_mutex_lock(&buffer_mutex);
        buffer[in_pos] = -1;
        in_pos = (in_pos + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_slots);
    }

    for (int i = 0; i < C; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);

    free(producers);
    free(consumers);

    return 0;
}
