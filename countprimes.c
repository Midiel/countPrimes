/*
 * Midiel Rodriguez
 * ID: xxxxxx
 * Course: COP-4338
 * Lab: 6
 *
 * This MPI program counts the number of primes from 1 to n (inclusive) where n
 * is a long integer. Each process prints the process number and the prime
 * number when found.
 *
 *
 * Disclaimer:
 * I hereby certify that this code is my work and my work alone and understand
 * the syllabus regarding plagiarized code.
 * Copyright 2018 Midiel
 */

/*
 * File:   countprimes.c
 * Author: Midiel Rodriguez
 *
 * Created on July 18, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#define UPPER_LIMIT 50000               // limit to search for
#define MASTER_PROCESS 0                // defines the main process

/*
 * This method finds if a number is prime or not.
 * If the number is prime, it return "1" and prints the process the found it
 * and the number. If not prime, then it return "0" and does not print anything.
 *
 * id       - the process id
 * number   - the number to check if it's prime
 */
int isPrime(int id, long int n) {
    long int i;

    // validates for numbers greater than 1
    if(n <= 1) {
        return 0;
    }

    // return 0 if the number is even
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    // if previous loop fails, then number is prime, and prints the information
    // and return "1"
    printf("%d) %ld is prime\n", id, n);
    fflush(stdout);
    return 1;
}

/*
 *
 */
int main(int argc, char** argv) {

    int count;            /* Solutions found by this proc */
    double elapsed_time;  /* Time to find, count solutions */
    int global_count;     /* Total number of solutions */
    long int i;
    int id;               /* Process rank */
    int p;                /* Number of processes */


    // starts mpi
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);

    /* Start timer */
    MPI_Barrier (MPI_COMM_WORLD);
    elapsed_time = - MPI_Wtime();

    // local count
    count = 0;

    // loop to get the number of primes
    for (i = (long int)id; i <= UPPER_LIMIT; i += p) {
        count += isPrime(id, i);
    }

    // reduce to master process
    MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM, MASTER_PROCESS,
                MPI_COMM_WORLD);

    /* Stop timer */
    elapsed_time += MPI_Wtime();

    // teminates the mpi
    MPI_Finalize();

    // prints the execution time
    if (!id) {
        printf ("Execution time %8.6f\n", elapsed_time);
        fflush (stdout);
    }

    // prints the number of solutions
    if (!id) {
        printf("There are %d different solutions\n",
                     global_count);
    }

    return 0;
}
