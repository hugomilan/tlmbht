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
 * File:   libtlmpennes3dsolvereigen.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on October 6, 2016.
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
 * transmission-line modeling method as the solver of the Pennes equation.
 * See libtlmpennes3dsolvereigen.c for more details.
 *
 */

#ifndef LIBTLMPENNES3DSOLVEREIGEN_H
#define LIBTLMPENNES3DSOLVEREIGEN_H


#include "libtlmpenneseigen.h"

unsigned int solverTLMPennes3DEigen(struct dataForSimulation*);

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* LIBTLMPENNES3DSOLVEREIGEN_H */

