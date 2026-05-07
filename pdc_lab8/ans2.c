#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int tag = 0;
    int msg_size = 1; 
    char *buf;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0)
            printf("This program requires exactly 3 processes.\n");
        MPI_Finalize();
        return 0;
    }

    printf("Process %d started\n", rank);

    while (msg_size <= 1000000) {
        buf = (char *)malloc(msg_size);
        if (rank == 0) {
            MPI_Send(buf, msg_size, MPI_BYTE, 1, tag, MPI_COMM_WORLD);
            printf("Process 0 sent %d bytes to Process 1\n", msg_size);
            MPI_Recv(buf, msg_size, MPI_BYTE, 2, tag, MPI_COMM_WORLD, &status);
            printf("Process 0 received %d bytes from Process 2\n", msg_size);
        }
        else if (rank == 1) {
            MPI_Recv(buf, msg_size, MPI_BYTE, 0, tag, MPI_COMM_WORLD, &status);
            printf("Process 1 received %d bytes from Process 0\n", msg_size);
            MPI_Send(buf, msg_size, MPI_BYTE, 2, tag, MPI_COMM_WORLD);
            printf("Process 1 sent %d bytes to Process 2\n", msg_size);
        }
        else if (rank == 2) {
            MPI_Recv(buf, msg_size, MPI_BYTE, 1, tag, MPI_COMM_WORLD, &status);
            printf("Process 2 received %d bytes from Process 1\n", msg_size);
            MPI_Send(buf, msg_size, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
            printf("Process 2 sent %d bytes to Process 0\n", msg_size);
        }

        free(buf);
        msg_size *= 2;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
