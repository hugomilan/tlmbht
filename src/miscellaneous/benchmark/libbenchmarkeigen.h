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
 * File:   libbenchmarkeigen.h
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

#ifndef LIBBENCHMARKEIGEN_H
#define LIBBENCHMARKEIGEN_H

#include <stdio.h>

unsigned int cpp_eigen_saxpy_benchmark(unsigned long long, unsigned long long,
        int, double *, double, double, FILE *, double, unsigned long long);

unsigned int cpp_eigen_saxpy_sparse_benchmark(unsigned long long, unsigned long long,
        int, double *, double, double, FILE *, double, unsigned long long);

#ifdef __cplusplus
extern "C" {
#endif

    unsigned int call_from_c_eigen_saxpy_benchmark(unsigned long long, unsigned long long,
            int, double *, double, double, FILE *, double, unsigned long long);

    unsigned int call_from_c_eigen_saxpy_sparse_benchmark(unsigned long long, unsigned long long,
            int, double *, double, double, FILE *, double, unsigned long long);

#ifdef __cplusplus
}
#endif

#endif /* LIBBENCHMARKEIGEN_H */