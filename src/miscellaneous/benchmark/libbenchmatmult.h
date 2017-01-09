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

#include "../liberrorcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../external/eigeninterface.h"

    unsigned int benchmarkSAXPY(long long unsigned int, long long unsigned int,
        int, double, double, int, FILE *, long long unsigned int);
    
    
    unsigned int test_simple_axpy_d(long long unsigned int, long long unsigned int,
        int, double *, double, double, FILE *, double, long long unsigned int);
    
    unsigned int simple_axpy_d_bench(double *, double **, double *,
        long long unsigned int, long long unsigned int);


#ifdef __cplusplus
}
#endif

#endif /* LIBBENCHMATMULT_H */

