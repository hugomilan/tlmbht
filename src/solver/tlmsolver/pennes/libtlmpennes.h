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
 * File:   libtlmpennes.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 15, 2016.
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
 * See libtlmpennes.c for more details.
 *
 */

#ifndef LIBTLMPENNES_H
#define LIBTLMPENNES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../libtlmsolver.h"
#include "../../../configs/libconfig.h"

    unsigned int initiateVariablesTLMPennes(struct dataForSimulation *,
            void *, struct TLMnumbers *, struct boundaryData**,
            struct connectionLeveln *, int id);

    unsigned int initiateBoundaryTypeAndDataPennes(struct boundaryData **, const struct dataForSimulation *,
            int);



#ifdef __cplusplus
}
#endif

#endif /* LIBTLMPENNES_H */

