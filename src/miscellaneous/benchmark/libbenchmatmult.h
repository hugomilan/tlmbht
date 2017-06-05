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
 * File:   libbenchmatmult.h
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

#ifndef LIBBENCHMATMULT_H
#define LIBBENCHMATMULT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

    unsigned int benchmarkSAXPY(unsigned long long, unsigned long long,
            int, double, double, int, FILE *, unsigned long long);


    unsigned int test_simple_axpy_d(unsigned long long, unsigned long long,
            int, double *, double, double, FILE *, double, unsigned long long);

    unsigned int simple_axpy_d_bench(double *, double **, double *,
            unsigned long long, unsigned long long);


#ifdef __cplusplus
}
#endif

#endif /* LIBBENCHMATMULT_H */

