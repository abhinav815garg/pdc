#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>         

const int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* compute_factorial(void* arg) {
    int idx = (intptr_t)arg;
    int p = primes[idx];
    mpz_t fact;
    mpz_init(fact);                
    mpz_set_ui(fact, 1);           
    for (int i = 2; i <= p; i++) {
        mpz_mul_ui(fact, fact, i); 
    }
    mpz_t* result = (mpz_t*)malloc(sizeof(mpz_t));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        mpz_clear(fact);
        pthread_exit(NULL);
    }
    mpz_init(*result);
    mpz_set(*result, fact);
    mpz_clear(fact);
    return (void*)result;
}

int main() {
    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        if (pthread_create(&threads[i], NULL, compute_factorial, (void*)(intptr_t)i) != 0) {
            fprintf(stderr, "Failed, thread%d\n", i);
            exit(1);
        }
    }
    mpz_t total;
    mpz_init(total);
    mpz_set_ui(total, 0);
    for (int i = 0; i < 10; i++) {
        void* result_ptr;
        pthread_join(threads[i], &result_ptr);

        mpz_t* fact_ptr = (mpz_t*)result_ptr;
        mpz_add(total, total, *fact_ptr);  
        mpz_clear(*fact_ptr);
        free(fact_ptr);
    }
    printf("Sum: ");
    mpz_out_str(stdout, 10, total);  
    printf("\n");
    mpz_clear(total);
    return 0;
}
