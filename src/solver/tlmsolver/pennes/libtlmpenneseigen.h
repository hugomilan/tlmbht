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
 * File:   libtlmpenneseigen.h
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
 * See libtlmpenneseigen.c for more details.
 *
 */

#ifndef LIBTLMPENNESEIGEN_H
#define LIBTLMPENNESEIGEN_H

#include "../libtlmsolvereigen.h"



unsigned int solverTLMPennesEigenGeneral(struct dataForSimulation*, int, void **);

unsigned int calculateMatricesPennesEigen(struct dataForSimulation *,
        struct calculationTLMEigen *, struct boundaryData**,
        struct connectionLeveln *, int);

unsigned int MaterialLinePennesEigen(struct dataForSimulation *,
        struct calculationTLMEigen *, int id);

unsigned int MaterialTrianglePennesEigen(struct dataForSimulation *, struct calculationTLMEigen *, int);

unsigned int MaterialTetrahedronPennesEigen(struct dataForSimulation *, struct calculationTLMEigen *, int);

unsigned int connectionsAndBoundariesPennesEigen(struct calculationTLMEigen *, struct boundaryData *,
        struct connectionLeveln *, struct dataForSimulation *, int id);

unsigned int calculateConnectionCoefficientsEigen(struct calculationTLMEigen *,
        struct boundaryData *, struct connectionAndBoundaryCoefficients *,
        struct dataForSimulation *, int);

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* LIBTLMPENNESEIGEN_H */

