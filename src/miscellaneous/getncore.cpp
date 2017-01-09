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
 * File:   getncore.c
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
 * Deprecated library in favor of the builtin OpenMP functions.
 *
 */

#include "getncore.h"
#include <iostream>
#include <thread>
#include <omp.h>
 

void getncores(unsigned int *n) {
    *n = std::thread::hardware_concurrency();
}

extern "C"{
    void call_from_c_getncores(unsigned int *n){
        
        return getncores(n);
    }
}