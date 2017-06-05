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
 * File:   libtlmpennes3dsolver.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on October 2, 2016.
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
 * two-dimensions. See libtlmpennes3dsolver.c for more details.
 *
 */

#ifndef LIBTLMPENNES3DSOLVER_H
#define LIBTLMPENNES3DSOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../configs/libconfig.h"


    unsigned int testInputTLMPennes3D(struct dataForSimulation*);



    // the terminate function is general and can be found at the libtlmsolver.c
    // terminateBoundaryTypeAndDataPennes

#ifdef __cplusplus
}
#endif

#endif /* LIBTLMPENNES3DSOLVER_H */

