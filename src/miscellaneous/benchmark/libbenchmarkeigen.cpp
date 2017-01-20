/*
 * TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
 * 
 * Copyright (C) 2015 to 2017 by Cornell University. All Rights Reserved.
 * 
 * Written by Hugo Fernando Maia Milan.
 * 
 * Free for educational, research and non-profit purposes.
 * Refer to the license file for details.
 */

/* 
 * File:   libbenchmarkeigen.cpp
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


#include <omp.h>
#include <time.h>

#include "libbenchmarkeigen.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace Eigen;

unsigned int cpp_eigen_saxpy_benchmark(unsigned long long L1, unsigned long long L2,
        int repetitions, double *timePerElement, double lowerTime, double higherTime,
        FILE *file, double metaOfSparsity, long long unsigned maxElement) {
    //    printf("The lower %g and higher %g\n", lowerTime, higherTime);
    MatrixXd A;
    VectorXd y, x;
    unsigned long long L1_old = 0, L2_old = 0; // old values for L1 and L2
    unsigned long long i, j, nnz = 0; // nnz = number of non-zeros
    int stayIn = 1, haverun = 0, running = 0, havefill = 0;
    double sparsity;

    clock_t start, end;
    double runtime;

    // pseud-random number generator
    srand((unsigned) time(NULL));
    //    printf("Number of processors %ld of %ld\n", sysconf (_SC_NPROCESSORS_CONF), sysconf (_SC_NPROCESSORS_ONLN));

    //    omp_set_num_threads(8);

    printf("Running matrix multiplication with dense matrix in Eigen (Eigen saxpy)\n");
    //    printf("Maximum number of threads %d\n", omp_get_max_threads());
    //    printf("Number of threads being used %d\n", Eigen::nbThreads());
    while (stayIn) {
        // DEBUG: See that it is allocating
        //         printf("Allocating the pointer with sizes (%llu, %llu).\n", L1, L2);
        A.resize(L1, L2);
        y.resize(L1);
        x.resize(L2);

        start = clock();
        x = A * x + y;
        end = clock();

        runtime = (double) (end - start) / CLOCKS_PER_SEC;

        // DEBUG: see the partial results
        //         printf("It took %g s for (%llu, %llu) and mean of %g us per element\n",
        //                 runtime, L1, L2, runtime/(L1*L2)*1e6);

        if ((runtime > lowerTime && runtime <= higherTime) || running || (L1 * L2 >= maxElement)) {
            running = 1;
            L1_old = L1;
            L2_old = L2;

            if (havefill == 0) {
                A = MatrixXd::Random(L1, L2);
                y = VectorXd::Random(L1);
                x = VectorXd::Random(L2);
                havefill = 1;
                continue;
            }

            timePerElement[haverun] = runtime / (L1 * L2);
            timePerElement[repetitions] = timePerElement[repetitions]
                    + timePerElement[haverun] / repetitions;

            haverun++;
            printf("Eigen saxpy for double (%llu, %llu) %d/%d\n",
                    L1, L2, haverun, repetitions);
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
    fprintf(file, "Eigen saxpy: %llu %llu %g %g %d",
            L1, L2, metaOfSparsity, sparsity, repetitions);

    for (i = 0; i < repetitions; i++)
        fprintf(file, " %g", timePerElement[i]);

    fprintf(file, ";\n");

    printf("Eigen saxpy done: mean run time per total number of element %g us.\n",
            timePerElement[repetitions]*1e6);



    return 0;
}

unsigned int cpp_eigen_saxpy_sparse_benchmark(unsigned long long L1, unsigned long long L2,
        int repetitions, double *timePerElement, double lowerTime, double higherTime,
        FILE *file, double metaOfSparsity, long long unsigned maxElement) {
    SparseMatrix<double, RowMajor, long long> A(L1, L2);
    A.reserve(VectorXi::Constant(L1, 6)); // reserving 6 non-zero elements per row

    printf("Inside the sparse guy\n");

    return 0;
}



extern "C" {

    unsigned int call_from_c_eigen_saxpy_benchmark(unsigned long long L1, unsigned long long L2,
            int repetitions, double *timePerElement, double lowerTime, double higherTime,
            FILE *file, double metaOfSparsity, unsigned long long maxElement) {

        return cpp_eigen_saxpy_benchmark(L1, L2, repetitions, timePerElement,
                lowerTime, higherTime, file, metaOfSparsity, maxElement);
    }

    unsigned int call_from_c_eigen_saxpy_sparse_benchmark(unsigned long long L1, unsigned long long L2,
            int repetitions, double *timePerElement, double lowerTime, double higherTime,
            FILE *file, double metaOfSparsity, unsigned long long maxElement) {

        return cpp_eigen_saxpy_sparse_benchmark(L1, L2, repetitions, timePerElement,
                lowerTime, higherTime, file, metaOfSparsity, maxElement);
    }
}