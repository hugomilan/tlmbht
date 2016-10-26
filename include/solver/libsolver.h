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
 * File:   libsolver.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on July 2, 2016.
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
 * contains declaration of functions, structures, and enumerations for function
 * calls that are common to all the solvers. See libsolver.c for more details.
 *
 */

#ifndef LIBSOLVER_H
#define LIBSOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../configs/libconfig.h"

    struct Point {
        double x;
        double y;
        double z;
    };
    
    //Check if the simulation is time-domain and print it.
    void printTypeTemporalSimulation(struct dataForSimulation*);

#ifdef __cplusplus
}
#endif

#endif /* LIBSOLVER_H */

