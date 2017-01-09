/*
 * TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
 * 
 * Copyright (C) 2015 to 2016 by Cornell University. All Rights Reserved.
 * 
 * Written by Hugo Fernando Maia Milan.
 * 
 * Free for educational, research and non-profit purposes.
 * Refer to the license file for details.
 */

/* 
 * File:   libbenchmatmult.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 8, 2016.
 *
 *
 *
 * Revision history:
 * 
 * Date: 
 * Who: 
 * Description: 
 *
 */

/*
 * Description of this file:
 * 
 *
 */

/* TODO:
 * 1) manage allocation failures
 * 2) tests sparse matrices from Eigen
 */


#include "libbenchmatmult.h"

/*
 * benchmarkMatrixMult: 
 */
unsigned int benchmarkSAXPY(long long unsigned int L1, long long unsigned int L2,
        int repetitions, double lowerTime, double higherTime, int numberOfSparsityTests,
        FILE *file, long long unsigned int maxElement) {
    unsigned int errorTLMnumber = 0;
    int numberOfTests = 1;
    int i;
    clock_t start, end;
    start = clock();
    double runtime, metaSparsity;

    printf("Expected time to finish this test is between %g s and %g s\n",
            (numberOfTests * repetitions * lowerTime * numberOfSparsityTests),
            (numberOfTests * repetitions * higherTime * numberOfSparsityTests));

    // vector that will allocate the results
    double **timePerMatrixElement;
    timePerMatrixElement = (double**) malloc(sizeof (double*)*numberOfTests * numberOfSparsityTests);
    // the last element is the mean value
    for (int i = 0; i < numberOfTests * numberOfSparsityTests; i++)
        timePerMatrixElement[i] = (double*) calloc((repetitions + 1), sizeof (double));


    // writing results in the file
    fprintf(file, "SAXPY\n");
    fprintf(file, "{\n");

    // testing Eigen dense matrix
    metaSparsity = 1;
    // testing the Eigen library
    if ((errorTLMnumber = call_from_c_eigen_saxpy_benchmark(L1, L2, repetitions, timePerMatrixElement[i],
            lowerTime, higherTime, file, metaSparsity, maxElement)) != 0) {
        return errorTLMnumber;
    }

    // testing Eigen sparse matrix
    if ((errorTLMnumber = call_from_c_eigen_saxpy_sparse_benchmark(L1, L2, repetitions, timePerMatrixElement[i],
            lowerTime, higherTime, file, metaSparsity, maxElement)) != 0) {
        return errorTLMnumber;
    }

    // testing a fool implementation of saxpy
    if ((errorTLMnumber = test_simple_axpy_d(L1, L2, repetitions, timePerMatrixElement[i],
            lowerTime, higherTime, file, metaSparsity, maxElement)) != 0) {
        return errorTLMnumber;
    }


    fprintf(file, "Recommendation:\n");

    // ????

    fprintf(file, "}\n");

    end = clock();

    runtime = (double) (end - start) / CLOCKS_PER_SEC;

    printf("Benchmark for SAXPY is done. It took %g s.\n", runtime);

    return errorTLMnumber;
}

unsigned int test_simple_axpy_d(long long unsigned int L1, long long unsigned int L2,
        int repetitions, double *timePerElement, double lowerTime, double higherTime,
        FILE *file, double metaOfSparsity, long long unsigned int maxElement) {
    double **A, *y, *x;
    long long unsigned int L1_old = 0, L2_old = 0; // old values for L1 and L2
    long long unsigned int i, j, nnz = 0; // nnz = number of non-zeros
    int stayIn = 1, haverun = 0, running = 0;
    double sparsity, temp;


    clock_t start, end;
    double runtime;

    // pseud-random number generator
    srand((unsigned) time(NULL));

    // initializing
    A = NULL;
    y = NULL;
    x = NULL;

    printf("Running simplified matrix multiplication (simple axpy)\n");
    while (stayIn) {
        // DEBUG: See that it is allocating
//         printf("Allocating the pointer with sizes (%llu, %llu).\n", L1, L2);
        x = (double*) realloc(x, sizeof (double)*L1);
        y = (double*) realloc(y, sizeof (double)*L1);
        A = (double**) realloc(A, sizeof (double*)*L1);

        for (long long unsigned int i = 0; i < L1; i++) {
            if (i >= L1_old)
                A[i] = NULL;

            A[i] = (double *) realloc(A[i], sizeof (double)*L2);
        }

        // For lines that are already allocated
        for (i = 0; i < L1_old; i++)
            for (j = L2_old; j < L2; j++) {
                temp = ((double) rand() / (double) RAND_MAX);
                if (temp <= metaOfSparsity) {
                    nnz++;
                    A[i][j] = temp;
                } else {
                    A[i][j] = 0;
                }
            }

        for (j = L2_old; j < L2; j++)
            x[i] = ((double) rand() / (double) RAND_MAX);

        for (i = L1_old; i < L1; i++) {
            y[i] = ((double) rand() / (double) RAND_MAX);
            for (j = 0; j < L2; j++) {
                temp = ((double) rand() / (double) RAND_MAX);
                if (temp <= metaOfSparsity) {
                    nnz++;
                    A[i][j] = temp;
                } else {
                    A[i][j] = 0;
                }
            }
        }

        sparsity = (double) nnz / (L1 * L2);

        start = clock();
        simple_axpy_d_bench(y, A, x, L1, L2);
        end = clock();

        runtime = (double) (end - start) / CLOCKS_PER_SEC;

        // DEBUG: see the partial results
//                 printf("It took %g s for (%llu, %llu) sparsity of %g and mean of %g us per element\n",
//                         runtime, L1, L2, sparsity, runtime/(L1*L2)*1e6);

        if ((runtime > lowerTime && runtime <= higherTime) || running || (L1 * L2 >= maxElement)) {
            L1_old = L1;
            L2_old = L2;
            running = 1;

            timePerElement[haverun] = runtime / (L1 * L2);
            timePerElement[repetitions] = timePerElement[repetitions]
                    + timePerElement[haverun] / repetitions;

            haverun++;
            printf("Simple axpy for double (%llu, %llu) and sparsity of %g %d/%d\n",
                    L1, L2, sparsity, haverun, repetitions);
            if (haverun == repetitions)
                stayIn = 0;
        } else if (runtime > higherTime) {
            L1_old = L1;
            L1 = L1 * sqrt((lowerTime + higherTime) / (2 * runtime));

            L2_old = L2;
            L2 = L2 * sqrt((lowerTime + higherTime) / (2 * runtime));

        } else {
            L1_old = L1;
            L1 = L1 * sqrt((lowerTime + higherTime) / (2 * runtime));

            L2_old = L2;
            L2 = L2 * sqrt((lowerTime + higherTime) / (2 * runtime));
        }
    }

    // L1, L2, asked percentage of sparse, obtained percentage of sparse, repetitions, values
    fprintf(file, "Simple axpy: %llu %llu %g %g %d",
            L1, L2, metaOfSparsity, sparsity, repetitions);

    for (i = 0; i < repetitions; i++)
        fprintf(file, " %g", timePerElement[i]);

    fprintf(file, ";\n");

    printf("Simple axpy done: mean run time per total number of element %g us.\n",
            timePerElement[repetitions]*1e6);



    return 0;
}

/*
 * simple_axpy: 
 */
unsigned int simple_axpy_d_bench(double *y, double **A, double *x,
        long long unsigned int L1, long long unsigned int L2) {

    // DEBUG: show that we started solving
    // printf("Solving simple_axpy_d\n");

    for (long long unsigned int i = 0; i < L1; i++)
        for (long long unsigned int j = 0; j < L2; j++) {
            for (long long unsigned int k = 0; k < L2; k++)
                y[i] = y[i] + A[i][k] * x[k];
        }

    // DEBUG: show that we finalize solving
    // printf("Done solving simple_axpy_d\n");


    return 0;
}