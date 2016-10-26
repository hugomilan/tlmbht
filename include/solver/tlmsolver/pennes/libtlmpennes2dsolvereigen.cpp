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
 * File:   libtlmpennes2dsolvereigen.cpp
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
 * 
 *
 */

#include "libtlmpennes2dsolvereigen.h"
#include <stdlib.h>

// this library has some basic functions
#include "libtlmpennes2dsolver.h"

/*
 * solverTLMPennes2DEigen: 
 */
unsigned int solverTLMPennes2DEigen(struct dataForSimulation* input) {
    unsigned int errorTLMnumber = 0;
    struct calculationTLMEigen calcs;
    struct boundaryData *boundaries;
    struct connectionLeveln intersections;
    FILE *f;

    printf("Solver called: TLM; Pennes equation in 2D; Eigen library; ");
    
    printTypeTemporalSimulation(input);
    printf("\n");
    
    

    // test if the inputs for TLM Pennes 2D are ok
    if (errorTLMnumber == 0 &&
            (errorTLMnumber = testInputTLMPennes2D(input)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }

    // initiate the variables
    if (errorTLMnumber == 0 &&
            (errorTLMnumber = initiateVariablesTLMPennes(input, &calcs,
            &(calcs.numbers), &boundaries, &intersections, &f)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }

    // get the matrices values
    if (errorTLMnumber == 0 &&
            (errorTLMnumber = calculateMatricesPennesEigen(input, &calcs, &boundaries, &intersections, f)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }

    // Solve the problem
    if (errorTLMnumber == 0){
        // in time domain
           if (input->simulationInput.Solv == DYNAMIC){
               errorTLMnumber = tlmSolveMatricesTimeDomainEigen(&calcs, input, f);
           } else if (input->simulationInput.Solv == STEADY){
                errorTLMnumber = tlmSolveMatricesSteadyStateEigen(&calcs, input, f);
           }
           if (errorTLMnumber != 0){
               sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
           }
    }

    // finish the calculations
    if ((errorTLMnumber = terminateVariablesTLMEigen(input, &calcs, &boundaries,
            &intersections, &f)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }

    printf("Exiting the solver: TLM; Pennes equation in 2D; ");
    printTypeTemporalSimulation(input);
    printf("\n");

    return errorTLMnumber;
}
