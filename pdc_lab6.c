#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void floydWarshall(int **dist, int n) {
    #pragma omp parallel shared(dist, n)
    {
        for (int k = 0; k < n; k++) {
            #pragma omp for
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    #pragma omp task firstprivate(i, j, k)
                    {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                        }
                    }
                }
            }
            #pragma omp taskwait
            
            #pragma omp barrier
        }
    }
}

int main() {
    int n = 4;
    int **dist = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        dist[i] = (int *)malloc(n * sizeof(int));
    }

    int INF = 99999;
    dist[0][0] = 0; dist[0][1] = 5; dist[0][2] = INF; dist[0][3] = 10;
    dist[1][0] = INF; dist[1][1] = 0; dist[1][2] = 3; dist[1][3] = INF;
    dist[2][0] = INF; dist[2][1] = INF; dist[2][2] = 0; dist[2][3] = 1;
    dist[3][0] = INF; dist[3][1] = INF; dist[3][2] = INF; dist[3][3] = 0;

    floydWarshall(dist, n);

    printf("Result:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", dist[i][j] == INF ? -1 : dist[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) free(dist[i]);
    free(dist);
    return 0;
}
