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
 * File:   libsolver.c
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
 * Purposed to be a library that would have function calls that are common
 * to all the solvers.
 *
 */

#include "libsolver.h"

// future implementation

void printTypeTemporalSimulation(struct dataForSimulation *input){
    if (input->simulationInput.Solv == DYNAMIC){
        printf("Time-domain.\n");
    } else {
        printf("Steady-State.\n");
        if (input->simulationInput.timeStepDefined != 1)
            input->simulationInput.timeStep = 1;
    }
    
}