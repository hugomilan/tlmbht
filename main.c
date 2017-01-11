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
 * File:   main.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 24, 2016.
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
 * Main file for the TLMBHT Version 0.2.0
 * We use Semantic Versioning 2.0.0 http://semver.org/
 * 
 * This function receives a script file as input and performs the operations requested
 * by the script file.
 * 
 * In this software, the functions will return zero when they work correctly.
 * 
 */


/*
 * Acknowledgment: Brazilian National Counsel of Technological and Scientific
 * Development (Proc. 203312/2014-7) for scholarship to HFMM (2015-2016).
 */


/*
 * TODO (hugomilan#1#): List of possible improvements
 * 
 * 1) Implement for Heat Transfer, Electromagnetism, and Computational Fluid Dynamics.
 * 2) Implement to 1D, 2D and 3D.
 * 3) Use different types of solution methods, i.e., TLM, FDTD, and FEM.
 * 4) implement the source header
 * 5) Create a debug mode
 * 6) Think about generalizing across different libraries/dimensions the following functions
 *      a) connectionsAndBoundariesPennes2DEigen()
 *      b) MaterialTrianglePennes2DEigen()
 *      c) MaterialLinePennes2DEigen()
 *      d) calculateMatricesEigen()
 *      e) struct calculationTLMEigen (the variables that can be included across platforms,
 *          i.e., Z, R, L, deltal, Points_output, numbers)
 * 7) output of:
 *      a) temperature between nodes
 *      b) heat flux and direction ?
 * 8) solve for steady-state
 * 9) test different libraries
 * 10) test correction methods for the TLM
 * 11) more than 2 ports for connection
 * 12) radiation boundary condition
 * 13) test if input is numeric.
 * 14) test if the number of the material was set more than once for different materials
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <omp.h>

#include "src/configs/libconfig.h"
#include "src/miscellaneous/liberrorcode.h"
#include "src/miscellaneous/libmiscellaneous.h"
#include "src/meshreader/libmeshreader.h"
#include "src/solver/libsolvercalls.h"
#include "src/miscellaneous/benchmark/libbenchmark.h"
#include "src/miscellaneous/getncore.h"

/*
 * Declaring functions used here in main
 */
unsigned int initializeAll(struct dataForSimulation *);
unsigned int terminateAll(struct dataForSimulation *);

/*
 * main: opens and reads the input file
 */

int main(int argc, char *argv[]) {
    clock_t begin = clock();
    struct dataForSimulation newDataConfig;
    unsigned int tlmErrorCode;
    printf("TLMBHT 0.2.0\n");
    
    // initializing all the variables
    if ((tlmErrorCode = initializeAll(&newDataConfig)) != 0) {
        sendErrorCodeAndMessage(12, NULL, NULL, NULL, NULL);
        return tlmErrorCode;
    }

    // Opening the input file
    if ((tlmErrorCode = testAndReadInputFile(argc, argv, &newDataConfig)) != 0) {
        terminateAll(&newDataConfig);
        return tlmErrorCode;
    }

    printf("The input file was read.\n");
    
    if (newDataConfig.runningBenchmark == 1){
        if ((tlmErrorCode = runBenchmark()) != 0) {
            terminateAll(&newDataConfig);
            return tlmErrorCode;
        }
    }

    // Will the simulation be run?
    if (newDataConfig.runningSimulation == 1) {
        if (newDataConfig.simulationInput.printAdditional == 1)
            printfAllInputData(&newDataConfig);

        printf("Reading the mesh file.\n");
        // Reading and Converting the input mesh if required
        if ((tlmErrorCode = meshReaderAndConverter(&newDataConfig.meshInput,
                &newDataConfig.mesh, newDataConfig.timingMode)) != 0) {
            terminateAll(&newDataConfig);
            return tlmErrorCode;
        }

        if (newDataConfig.simulationInput.printAdditional == 1)
            printfAllMeshData(&newDataConfig.mesh);

        printf("The mesh file was successfully read.\n");

        printf("Starting the solver.\n");
        // Solving
        if ((tlmErrorCode = solve(&newDataConfig)) != 0) {
            terminateAll(&newDataConfig);
            return tlmErrorCode;
        }
    }


    //closing the input data for configuration
    if ((tlmErrorCode = terminateAll(&newDataConfig)) != 0)
        return tlmErrorCode;
    
    clock_t end = clock();

    if (newDataConfig.timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("\n\nTotal execution time %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }

    return 0;
}

/*
 * initializeAll: initialize all the required variables.
 */
unsigned int initializeAll(struct dataForSimulation *input) {
    unsigned int errorTLMnumber;

    // initializing the configuration variables
    if ((errorTLMnumber = initiateAllConfigurationVarialbes(input)) != 0)
        return errorTLMnumber;

    // initializing the mesh variable.
    if ((errorTLMnumber = initiateTlmInternalMesh(&input->mesh)) != 0)
        return errorTLMnumber;

    return 0;
}

/*
 * terminateAll: Terminate the variables. This is, free the ones that were allocated.
 * As standard, I free the variables and then assigned them the value NULL. I do this
 * so that, in case the variable is accessed afterwards, the software won't crash.
 */
unsigned int terminateAll(struct dataForSimulation *input) {
    unsigned int errorTLMnumber;

    // terminating the configuration variables
    if ((errorTLMnumber = terminateAllConfigurationVarialbes(input)) != 0)
        return errorTLMnumber;

    // terminating the internal mesh variables
    if ((errorTLMnumber = terminateTlmInternalMesh(&input->mesh)) != 0)
        return errorTLMnumber;

    return 0;
}