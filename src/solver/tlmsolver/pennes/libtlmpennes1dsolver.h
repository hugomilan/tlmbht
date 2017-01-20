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
 * File:   libpennes1dsolver.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on January 18, 2017.
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
 * contains declaration of functions, structures, and enumerations for using the
 * transmission-line modeling method as the solver of the Pennes equation in 
 * one-dimension. See libtlmpennes1dsolver.c for more details.
 *
 */

#ifndef LIBPENNES1DSOLVER_H
#define LIBPENNES1DSOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../configs/libconfig.h"

    unsigned int testInputTLMPennes1D(struct dataForSimulation*);


#ifdef __cplusplus
}
#endif

#endif /* LIBPENNES1DSOLVER_H */


