#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

void* compute_square(void* arg) {
    int num = (intptr_t)arg;
    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        pthread_exit(NULL);
    }
    *result = num * num;
    pthread_exit((void*)result);
}

int main() {
    pthread_t threads[5];
    for (int i = 1; i <= 5; i++) {
        if (pthread_create(&threads[i-1], NULL, compute_square, (void*)(intptr_t)i) != 0) {
            exit(1);
        }
    }
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        void* result_ptr;
        pthread_join(threads[i], &result_ptr);
        int* result = (int*)result_ptr;
        sum += *result;
        free(result);
    }
    printf("Result: %d\n", sum);
    return 0;
}
