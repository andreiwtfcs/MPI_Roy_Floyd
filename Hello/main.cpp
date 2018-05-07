#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <cstdlib>


#define INF 99999
#define NR 5

int main(int argc, char *argv[])
{
        int numprocs, procid;
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numprocs);


        MPI_Comm_rank(MPI_COMM_WORLD, &procid);

        int matrixGraph[][] = {
                {0, 2, INF, 10, INF},
                {2, 0, 3, INF, INF},
                {INF, 3, 0, 1, 8},
                {10, INF, 1, 0, INF},
                {INF, INF, 8, INF, 0}
        };

        if (rank == 0) {
            MPI_Bcast(matrixGraph, NR*NR, MPI_INT, 0, MPI_COMM_WORLD);

            for (int k = 0; k < NR; k++) {
                for (int j = 0; j < NR; j++) {
                    if (matrixGraph[0][k] + matrixGraph[k][j] < matrixGraph[0][j]) {
                        matrixGraph[0][j] = matrixGraph[0][k] + matrixGraph[k][j];
                    }
                }
            }

            for (int i = 0; i < NR; i++) {
                masterGraph[0][i] = matrixGraph[0][i];
            }

        }
        if (rank != 0) {

            MPI_Recv(matrixGraph, NR, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

            for (int k = 0; k < NR; k++) {
                for (int j = 0; j < NR; j++) {
                    if (matrixGraph[rank][k] + matrixGraph[k][j] < matrixGraph[rank][j]) {
                        matrixGraph[rank][j] = matrixGraph[rank][k] + matrixGraph[k][j];
                    }
                }
            }
            MPI_Alltoall(matrixGraph, NR, MPI_INT, matrixGraph, NR, MPI_INT, MPI_COMM_WORLD);
        }
        if (rank == 0) {

            for (int p = 1; p < size; p++) {
                MPI_Recv(matrixGraph, NR, MPI_INT, p, 1, MPI_COMM_WORLD, &status);
                for (int j = p; j < matrixGraph.length; j++) {
                    for (int k = 0; k < matrixGraph.length; k++) {
                        masterGraph[j][k] = matrixGraph[j][k];
                    }
                }
            }
        }
        if(rank==0) {
            for (int i = 0; i < NR; i++) {
                for (int j = 0; j < NR; j++) {
                    System.out.print(masterGraph[i][j] + "   ");
                }
            }
        }

        MPI_Finalize();
}
