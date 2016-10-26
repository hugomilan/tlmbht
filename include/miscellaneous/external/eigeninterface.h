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
 * File:   eigeninterface.h
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

#ifndef EIGENINTERFACE_H
#define EIGENINTERFACE_H

#endif /* EIGENINTERFACE_H */
#include <stdio.h>

unsigned int cpp_eigen_saxpy_benchmark(long long unsigned int, long long unsigned int,
        int, double *, double, double, FILE *, double, long long unsigned int);

unsigned int cpp_eigen_saxpy_sparse_benchmark(long long unsigned int, long long unsigned int,
        int, double *, double, double, FILE *, double, long long unsigned int);

#ifdef __cplusplus
extern "C" {
#endif

    unsigned int call_from_c_eigen_saxpy_benchmark(long long unsigned int, long long unsigned int,
            int, double *, double, double, FILE *, double, long long unsigned int);

    unsigned int call_from_c_eigen_saxpy_sparse_benchmark(long long unsigned int, long long unsigned int,
            int, double *, double, double, FILE *, double, long long unsigned int);

#ifdef __cplusplus
}
#endif
