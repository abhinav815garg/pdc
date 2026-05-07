#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

const int arr[10] = {56, 10, 67, 496, 43, 12, 140, 6, 8, 2};

int perfect_count = 0;
pthread_mutex_t count_mutex;

int is_perfect(int n) {
    if (n <= 1) return 0;
    int sum = 1;
    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return (sum == n);
}

void* check_perfect(void* arg) {
    int idx = (intptr_t)arg;
    int num = arr[idx];

    int* result = NULL;
    if (is_perfect(num)) {
        pthread_mutex_lock(&count_mutex);
        perfect_count++;
        pthread_mutex_unlock(&count_mutex);
        result = (int*)malloc(sizeof(int));
        if (result == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            pthread_exit(NULL);
        }
        *result = num;
    }
    pthread_exit((void*)result);
}

int main() {
    pthread_mutex_init(&count_mutex, NULL);
    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        if (pthread_create(&threads[i], NULL, check_perfect, (void*)(intptr_t)i) != 0) {
            fprintf(stderr, "Failed, thread: %d\n", i);
            exit(1);
        }
    }
    printf("Perfect numbers found: ");
    for (int i = 0; i < 10; i++) {
        void* result_ptr;
        pthread_join(threads[i], &result_ptr);
        if (result_ptr != NULL) {
            int* val = (int*)result_ptr;
            printf("%d ", *val);
            free(val);
        }
    }
    printf("\n");
    printf("Total count: %d\n", perfect_count);
    pthread_mutex_destroy(&count_mutex);
    return 0;
}
