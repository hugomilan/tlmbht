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
 * File:   libinterfaceceigen.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 9, 2016.
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
 * contains declaration of functions, structures, and enumerations for making the
 * interface between solvers that use the Eigen library that is C++ and the
 * calls that are in C. See libinterfaceceigen.c for more details.
 *
 */

#ifndef LIBINTERFACECEIGEN_H
#define LIBINTERFACECEIGEN_H

#ifdef __cplusplus
extern "C" {
#endif


#include "../configs/libconfig.h"
    unsigned int call_from_c_solverTLMPennes2DEigen(struct dataForSimulation*);
    unsigned int call_from_c_solverTLMPennes3DEigen(struct dataForSimulation*);
    unsigned int call_from_c_initiate_matrices_calculationTLMEigen(void *, unsigned int*);

#ifdef __cplusplus
}
#endif

#endif /* LIBINTERFACECEIGEN_H */

