#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t printers;
pthread_mutex_t count_mutex;

int current_printing = 0;
int max_concurrent = 0;

typedef struct {
    int id;
} job_args_t;

void *print_job(void *arg) {
    job_args_t *j = (job_args_t *)arg;

    sem_wait(&printers);

    pthread_mutex_lock(&count_mutex);
    current_printing++;
    if (current_printing > max_concurrent) {
        max_concurrent = current_printing;
    }
    printf("Thread %d is printing... (currently %d jobs)\n", j->id, current_printing);
    fflush(stdout);
    pthread_mutex_unlock(&count_mutex);

    usleep(100000 + (rand() % 400000));

    pthread_mutex_lock(&count_mutex);
    current_printing--;
    printf("Thread %d finished printing. (currently %d jobs)\n", j->id, current_printing);
    fflush(stdout);
    pthread_mutex_unlock(&count_mutex);

    sem_post(&printers);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <N threads> <K printers>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int K = atoi(argv[2]);

    if (N <= 0 || K <= 0) {
        printf("N and K must be positive.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    sem_init(&printers, 0, K);
    pthread_mutex_init(&count_mutex, NULL);

    pthread_t *threads = malloc(sizeof(pthread_t) * N);
    job_args_t *args = malloc(sizeof(job_args_t) * N);

    for (int i = 0; i < N; i++) {
        args[i].id = i;
        if (pthread_create(&threads[i], NULL, print_job, &args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Maximum concurrent printing jobs observed: %d (limit K = %d)\n",
           max_concurrent, K);

    sem_destroy(&printers);
    pthread_mutex_destroy(&count_mutex);
    free(threads);
    free(args);

    return 0;
}
