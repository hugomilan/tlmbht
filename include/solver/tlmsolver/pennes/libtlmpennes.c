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
 * File:   libtlmpennes.c
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
 * This contains implementations for solving the Pennes' equation with the
 * TLM that is independent of specific libraries and the dimension of the simulation
 *
 */

#include "libtlmpennes.h"
#include <string.h>
#include <stdlib.h>


// Maybe this function can be independent of the physics (Pennes, EM, CFD, etc) being solved.

/*
 * initiateVariablesTLMPennes: allocate in memory and initiate the variables used
 * for the TLM simulation. The matrices variable, which is dependent on the library
 * being used, is inputed as void. Then, the function can get any type. The matrices
 * variable is, then, send to the specific allocate function.
 */
unsigned int initiateVariablesTLMPennes(struct dataForSimulation *input,
        void * matrices, struct TLMnumbers *numbers, struct boundaryData** boundaries,
        struct connectionLeveln *intersections, FILE **file) {
    clock_t begin = clock();
    if (input->timingMode == 1) printf("\n\nInitiating variables...\n");

    unsigned int errorTLMnumber;
    // the level of the intersection variable
    unsigned int level;
    // variable that contains the initial allocation for each level of the intersection variable
    long long unsigned int *allocateForEachLevel;
    // the values inserted for the allocateForEachLevel variable are arbitrary.

    // #TODO: these are approximated calculations to initiate the boundary variable.
    clock_t begin_con = clock();
    switch (input->simulationInput.dimen) {
        case ONE:
            level = 1;

            allocateForEachLevel = (long long unsigned int *) malloc(sizeof (long long unsigned int)*(level + 1));

            allocateForEachLevel[0] = input->mesh.numberOfNode; // line. Level 1. 1 point
            allocateForEachLevel[1] = 3; // level of the ports. Level 0

            break;
        case TWO:
            level = 2;

            allocateForEachLevel = (long long unsigned int *) malloc(sizeof (long long unsigned int)*(level + 1));

            // triangle and quadrangle. Level 2. 2 points
            allocateForEachLevel[0] = 1 + (input->mesh.quantityOfSpecificElement[2] +
                    input->mesh.quantityOfSpecificElement[3]) / 2;

            // line. Level 1. 1 point
            allocateForEachLevel[1] = 1 + input->mesh.quantityOfSpecificElement[1];
            allocateForEachLevel[2] = 3; // level of the ports. Level 0
            
            // this is an alternative way. With this, we allocate much less memory.
            // I've to find a better alternative to balance memory/time.
            allocateForEachLevel[0] = 1;
            allocateForEachLevel[1] = 1;

            break;
        case THREE:
            if (input->mesh.quantityOfSpecificElement[5] ||
                    input->mesh.quantityOfSpecificElement[6] ||
                    input->mesh.quantityOfSpecificElement[7]){
                level = 4;
            } else {
                level = 3;
            }
            
            
            allocateForEachLevel = (long long unsigned int *) malloc(sizeof (long long unsigned int)*(level + 1));
            int leveln = 0;
            if (level == 4){
                // Hexahedron, prism and pyramid. Level 4. 4 points
                allocateForEachLevel[leveln] = 1 + (input->mesh.quantityOfSpecificElement[5] +
                        input->mesh.quantityOfSpecificElement[6] +
                        input->mesh.quantityOfSpecificElement[7]) / 3;
                leveln++;
            }
            
            

            // Tetrahedron, hexahedron, prism and pyramid. Level 3. 3 points
            allocateForEachLevel[leveln] = 1 + (input->mesh.quantityOfSpecificElement[4] +
                    input->mesh.quantityOfSpecificElement[5] +
                    input->mesh.quantityOfSpecificElement[6] +
                    input->mesh.quantityOfSpecificElement[7]) / 8;
            leveln++;

            // Triangles and line. Level 2. 2 points
            allocateForEachLevel[leveln] = 1 + (input->mesh.quantityOfSpecificElement[2] +
                    input->mesh.quantityOfSpecificElement[3]) / 2;
            leveln++;
            // Level 3. 1 point
            allocateForEachLevel[leveln] = 1 + (input->mesh.quantityOfSpecificElement[2] +
                    input->mesh.quantityOfSpecificElement[3]) / 8;
            leveln++;
            // level of the port
            allocateForEachLevel[leveln] = 3; // level of the ports. Level 0
            
            // DEBUG: see what was calculated
            //printf("%lld, %lld, %lld, %lld\n", allocateForEachLevel[0],allocateForEachLevel[1],
            //        allocateForEachLevel[2],allocateForEachLevel[3]);
            
            // this is an alternative way. With this, we allocate much less memory.
            // I've to find a better alternative to balance memory/time.
            for (int i2 = 0; i2 < level; i2++){
                allocateForEachLevel[i2] = 1;
            }

            break;
    }


    // DEBUG: show where we are
    //    printf("Initiating the connection variable.\n");
    // initiate the variable that has the connections.
    if ((errorTLMnumber = initiate_connectionLeveln(intersections, level, allocateForEachLevel)) != 0) {
        return errorTLMnumber;
    }
    free(allocateForEachLevel);
    allocateForEachLevel = NULL;
    clock_t end_con = clock();

    if (input->timingMode == 1) {
        double time_spent_con = (double) (end_con - begin_con) / CLOCKS_PER_SEC;
        printf("Time to initiate connections %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_con * 1e3, time_spent_con, time_spent_con / 60.0, time_spent_con / (60 * 60));
    }


    // DEBUG: testing if the levels were written correctly
    //    printf("Some tests %u, %u, %u, %u, %u\n", intersections->level, intersections->innerLevel[5].level,
    //            intersections->innerLevel[8].level, intersections->innerLevel[2].innerLevel[6].level,
    //            intersections->innerLevel[2].innerLevel[2].level);

    // DEBUG: show where we are
    //    printf("Getting the TLM numbers.\n");

    clock_t begin_get = clock();
    // get the numbers, the connections, and wrap them
    if ((errorTLMnumber = getTLMnumbers(input, numbers, intersections)) != 0) {
        return errorTLMnumber;
    }

    clock_t end_get = clock();

    // DEBUG: shows the numbers we got
    //    printf("Number of nodes %llu, number of ports %llu, number of outputs %llu\n",
    //            matrices->numbers.Nodes, matrices->numbers.Ports, matrices->numbers.Output);

    clock_t begin_bound = clock();
    if ((errorTLMnumber = initiateBoundaryTypeAndDataPennes(boundaries, input)) != 0) {
        return errorTLMnumber;
    }
    clock_t end_bound = clock();

    // Now it is writing mode. If there is a file with the same name, I'm sorry,
    // it will be replaced
    char* nameOfFile = (char*) malloc(strlen(input->meshInput.nameOfInputFile) + 5);
    strcpy(nameOfFile, input->meshInput.nameOfInputFile);
    strcat(nameOfFile, ".tmo");


    if ((*file = fopen(nameOfFile, "w")) == NULL) {
        // error opening the file
        // sendErrorCodeAndMessage(764, nameOfFile, NULL, NULL, NULL);
        return 764;
    }
    free(nameOfFile);




    clock_t begin_mat = clock();
    // initiating the matrices
    switch (input->simulationInput.libraryForCalculation) {
        case EIGEN:
            // quantities to reserve in the sparse matrices used in the implementation
            // using the Eigen library
            ; // trick to overcome C limitation that would not allow me to 
            // define the variable below
            unsigned int quantityToReserve[] = {4, 2}; // Initial value 

            // if we have ... I increase the number to reserve. The numbers
            // to reserve are the square of the number of ports 

            // Triangle elements
            if (input->mesh.quantityOfSpecificElement[2]) {
                quantityToReserve[0] = 9;
                quantityToReserve[1] = 9;
            }

            // quadrangle or tetrahedron elements
            if (input->mesh.quantityOfSpecificElement[3] != 0 ||
                    input->mesh.quantityOfSpecificElement[4] != 0) {
                quantityToReserve[0] = 16;
                quantityToReserve[1] = 16;
            }

            // pyramid elements
            if (input->mesh.quantityOfSpecificElement[7] != 0) {
                quantityToReserve[0] = 25;
                quantityToReserve[1] = 25;
            }

            // prism elements
            if (input->mesh.quantityOfSpecificElement[6] != 0) {
                quantityToReserve[0] = 36;
                quantityToReserve[1] = 36;
            }

            // hexahedron elements
            if (input->mesh.quantityOfSpecificElement[5] != 0) {
                quantityToReserve[0] = 64;
                quantityToReserve[1] = 64;
            }

            if (errorTLMnumber = call_from_c_initiate_matrices_calculationTLMEigen(matrices,
                    quantityToReserve) != 0) {
                return errorTLMnumber;
            }
            break;
    }
    clock_t end_mat = clock();


    clock_t end = clock();

    if (input->timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

        double time_spent_get = (double) (end_get - begin_get) / CLOCKS_PER_SEC;
        double time_spent_bound = (double) (end_bound - begin_bound) / CLOCKS_PER_SEC;
        double time_spent_mat = (double) (end_mat - begin_mat) / CLOCKS_PER_SEC;

        printf("Total time to initiate TLM numbers %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_get * 1e3, time_spent_get, time_spent_get / 60.0, time_spent_get / (60 * 60));


        printf("Time to initiate the boundaries %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_bound * 1e3, time_spent_bound, time_spent_bound / 60.0, time_spent_bound / (60 * 60));

        printf("Time to initiate the matrices %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_mat * 1e3, time_spent_mat, time_spent_mat / 60.0, time_spent_mat / (60 * 60));

        printf("Total time to initiate the variables %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }


    return 0;
}

/*
 * initiateBoundaryTypeAndDataPennes: allocate memory for the boundary variables and
 * allocate their values
 */
unsigned int initiateBoundaryTypeAndDataPennes(struct boundaryData **input,
        const struct dataForSimulation * inputData) {

    *input = (struct boundaryData*) malloc(sizeof (struct boundaryData)*
            inputData->quantityOfBoundariesRead);

    if (*input == NULL) {
        sendErrorCodeAndMessage(8707, NULL, NULL, NULL, NULL);
        return 8707;
    }

    for (unsigned int i = 0; i < inputData->quantityOfBoundariesRead; i++) {
        // input[i].boundaries->boundaryType = 
        // 0 - Adiabatic
        // 1 - temperature
        // 2 - heat flux
        // 3 - convection heat transfer (Newton's cooling law)
        // 4 - radiation heat transfer

        // If the boundary is adiabatic or temperature, it can be defined only once.
        // Otherwise, in one boundary we can have more than one effect. For instance,
        // we can have heat flux, convection, and radiation heat transfer.

        // adiabatic case
        if (inputData->boundaryInput[i].adiabaticDefined == 1) {
            (*input)[i].quantityOfBoundaries = 1;
            (*input)[i].boundaries = (struct boundaryTypeAndData*)
                    malloc(sizeof (struct boundaryTypeAndData));
            if ((*input)[i].boundaries == NULL) {
                sendErrorCodeAndMessage(8708, NULL, NULL, NULL, NULL);
                return 8708;
            }

            (*input)[i].boundaries[0].boundaryType = 0; // if type equals to zero, it is adiabatic
            (*input)[i].boundaries[0].boundaryData = NULL; // we don't have any data for adiabatic
            continue;
        }

        // temperature case
        if (inputData->boundaryInput[i].temperatureDefined == 1) {
            (*input)[i].quantityOfBoundaries = 1;
            (*input)[i].boundaries = (struct boundaryTypeAndData*)
                    malloc(sizeof (struct boundaryTypeAndData));
            if ((*input)[i].boundaries == NULL) {
                sendErrorCodeAndMessage(8709, NULL, NULL, NULL, NULL);
                return 8709;
            }

            (*input)[i].boundaries[0].boundaryType = 1;
            (*input)[i].boundaries[0].boundaryData = (double*) malloc(sizeof (double));
            (*input)[i].boundaries[0].boundaryData[0] = inputData->boundaryInput[i].temperatureBoundary;
            continue;
        }

        // starts by defining these variables. We can have more than one effect
        // on the boundary. For instance, we can have heat flux, convection and
        // radiation heat transfer. That's why we need to define the quantityOfBoundaries
        // variable
        (*input)[i].quantityOfBoundaries = 0;
        (*input)[i].boundaries = NULL;

        // heat flux case
        if (inputData->boundaryInput[i].heatFluxDefined == 1) {
            (*input)[i].quantityOfBoundaries++;
            (*input)[i].boundaries = (struct boundaryTypeAndData*)
                    realloc((*input)[i].boundaries,
                    sizeof (struct boundaryTypeAndData)*(*input)[i].quantityOfBoundaries);

            if ((*input)[i].boundaries == NULL) {
                sendErrorCodeAndMessage(8710, NULL, NULL, NULL, NULL);
                return 8710;
            }

            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryType = 2;
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData = (double*) malloc(sizeof (double));
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[0] = inputData->boundaryInput[i].heatFluxBoundary; // heat flux value
        }

        // convection case
        if (inputData->boundaryInput[i].convectionDefined == 1) {
            (*input)[i].quantityOfBoundaries++;
            (*input)[i].boundaries = (struct boundaryTypeAndData*)
                    realloc((*input)[i].boundaries,
                    sizeof (struct boundaryTypeAndData)*(*input)[i].quantityOfBoundaries);

            if ((*input)[i].boundaries == NULL) {
                sendErrorCodeAndMessage(8711, NULL, NULL, NULL, NULL);
                return 8711;
            }

            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryType = 3;
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData = (double*) malloc(sizeof (double)*2);
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[0] = inputData->boundaryInput[i].ConvectionTemperature; // temperature for the convection effect
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[1] = inputData->boundaryInput[i].ConvectionCoefficient; // convective heat transfer coefficient
        }

        // radiation case
        if (inputData->boundaryInput[i].radiationDefined == 1) {
            (*input)[i].quantityOfBoundaries++;
            (*input)[i].boundaries = (struct boundaryTypeAndData*)
                    realloc((*input)[i].boundaries,
                    sizeof (struct boundaryTypeAndData)*(*input)[i].quantityOfBoundaries);

            if ((*input)[i].boundaries == NULL) {
                sendErrorCodeAndMessage(8712, NULL, NULL, NULL, NULL);
                return 8712;
            }

            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryType = 4;
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData = (double*) malloc(sizeof (double)*2);
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[0] = inputData->boundaryInput[i].RadiationTemperature; // temperature for the radiation effect
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[1] = inputData->boundaryInput[i].RadiationEmissivity; // radiation emissivity coefficient
        }
    }


    return 0;
}