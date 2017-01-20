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


#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "libtlmpennes.h"

#include "../src/miscellaneous/libwritetofiletlmbht.h"
#include "../../../miscellaneous/liberrorcode.h"


// Maybe this function can be independent of the physics (Pennes, EM, CFD, etc) being solved.

/*
 * initiateVariablesTLMPennes: allocate in memory and initiate the variables used
 * for the TLM simulation. The matrices variable, which is dependent on the library
 * being used, is inputed as void. Then, the function can get any type. The matrices
 * variable is, then, send to the specific allocate function.
 */
unsigned int initiateVariablesTLMPennes(struct dataForSimulation *input,
        void * matrices, struct TLMnumbers *numbers, struct boundaryData** boundaries,
        struct connectionLeveln *intersections, int id) {

    clock_t begin_con = clock();
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("\n\nInitiating the connection variable.\n");
    }

    unsigned int errorTLMnumber;

    // the level of the intersection variable
    unsigned int level;
    // variable that contains the initial allocation for each level of the intersection variable
    unsigned long long *allocateForEachLevel;
    // the values inserted for the allocateForEachLevel variable are arbitrary.

    // TODO these are approximated calculations to initiate the connection variable.
    switch (input->equationInput[id].dimen) {
        case ONE:
            level = 1;

            allocateForEachLevel = (unsigned long long *) malloc(sizeof (unsigned long long)*(level + 1));

            allocateForEachLevel[0] = input->mesh.numberOfNode; // line. Level 1. 1 point
            allocateForEachLevel[1] = 3; // level of the ports. Level 0

            break;
        case TWO:
            level = 2;

            allocateForEachLevel = (unsigned long long *) malloc(sizeof (unsigned long long)*(level + 1));

            // triangle and quadrangle. Level 2. 2 points
            allocateForEachLevel[0] = 1 + (input->mesh.quantityOfSpecificElement[2] +
                    input->mesh.quantityOfSpecificElement[3]) / 2;

            // line. Level 1. 1 point
            allocateForEachLevel[1] = 1 + input->mesh.quantityOfSpecificElement[1];
            allocateForEachLevel[2] = 3; // level of the ports. Level 0

            // this is an alternative way. I did run out of memory using the above approach.
            // With this, we allocate much less memory.
            // I've to find a better alternative to balance memory/time.
            allocateForEachLevel[0] = 1;
            allocateForEachLevel[1] = 1;

            break;
        case THREE:
            if (input->mesh.quantityOfSpecificElement[5] ||
                    input->mesh.quantityOfSpecificElement[6] ||
                    input->mesh.quantityOfSpecificElement[7]) {
                level = 4;
            } else {
                level = 3;
            }


            allocateForEachLevel = (unsigned long long *) malloc(sizeof (unsigned long long)*(level + 1));
            int leveln = 0;
            if (level == 4) {
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

            // this is an alternative way. I did run out of memory using the above approach.
            // With this, we allocate much less memory.
            // I've to find a better alternative to balance memory/time.
            for (int i2 = 0; i2 < level; i2++) {
                allocateForEachLevel[i2] = 1;
            }
            break;
    }

    // VERBOSE: see the preallocation
    if (input->simulationInput.verboseMode == 1) {
        printf("Pre-allocations for the connection variable with %u level(s): "
                "%llu", level, allocateForEachLevel[0]);
        for (int ilevel = 1; ilevel < level; ilevel++) {
            printf(", %llu", allocateForEachLevel[ilevel]);
        }
        printf("\n");
    }

    if ((errorTLMnumber = initiate_connectionLeveln(intersections, level, allocateForEachLevel)) != 0) {
        return errorTLMnumber;
    }
    free(allocateForEachLevel);
    allocateForEachLevel = NULL;
    
    clock_t end_con = clock();

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Done initiating the connection variable.\n");
    }

    if (input->simulationInput.timingMode == 1) {
        double time_spent_con = (double) (end_con - begin_con) / CLOCKS_PER_SEC;
        printf("Time to initiate connections %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_con * 1e3, time_spent_con, time_spent_con / 60.0, time_spent_con / (60 * 60));
    }


    // DEBUG: testing if the levels were written correctly
    //    printf("Some tests %u, %u, %u, %u, %u\n", intersections->level, intersections->innerLevel[5].level,
    //            intersections->innerLevel[8].level, intersections->innerLevel[2].innerLevel[6].level,
    //            intersections->innerLevel[2].innerLevel[2].level);


    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Getting the TLM numbers (number of ports, nodes outputs, etc.) and the connection variable...\n");
    }

    clock_t begin_get = clock();
    // get the numbers, the connections, and wrap them
    if ((errorTLMnumber = getTLMnumbers(input, numbers, intersections, id)) != 0) {
        return errorTLMnumber;
    }

    clock_t end_get = clock();

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Done getting the TLM numbers (number of ports, nodes outputs, etc.) and the connection variable.\n");
    }
    
    if (input->simulationInput.timingMode == 1) {
        double time_spent_get = (double) (end_get - begin_get) / CLOCKS_PER_SEC;
        printf("Time to initiate TLM numbers (number of ports, nodes outputs, etc.) and the connection variable %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_get * 1e3, time_spent_get, time_spent_get / 60.0, time_spent_get / (60 * 60));
    }

    // DEBUG: shows the numbers we got
    //    printf("Number of nodes %llu, number of ports %llu, number of outputs %llu\n",
    //            matrices->numbers.Nodes, matrices->numbers.Ports, matrices->numbers.Output);

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Initiating the boundary variable...\n");
    }
    clock_t begin_bound = clock();
    if ((errorTLMnumber = initiateBoundaryTypeAndDataPennes(boundaries, input, id)) != 0) {
        return errorTLMnumber;
    }
    clock_t end_bound = clock();
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Done initiating the boundary variable.\n");
    }
    
    if (input->simulationInput.timingMode == 1) {
        double time_spent_bound = (double) (end_bound - begin_bound) / CLOCKS_PER_SEC;
        printf("Time to initiate the boundaries %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_bound * 1e3, time_spent_bound, time_spent_bound / 60.0, time_spent_bound / (60 * 60));
    }
    
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Creating the output file...\n");
    }

    // creating the output file (or checking if it was already created)
    if (errorTLMnumber = creatOutputFile(input) != 0) {
        return errorTLMnumber;
    }
    
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Done creating the output file.\n");
        printf("Allocating and initiating the matrices...\n");
    }

    clock_t begin_mat = clock();
    // initiating the matrices
    switch (input->equationInput[id].libraryForCalculation) {
        case EIGEN:
            // quantities to reserve in the sparse matrices used in the implementation
            // using the Eigen library
            ; // trick to overcome C limitation that would not allow me to 
            // define the variable below
            unsigned int quantityToReserve[] = {4, 4}; // Initial value 

            // if we have what is defined below, I increase the number to reserve. The numbers
            // to reserve are the square of the number of ports .

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

        case CUDA:
            // future implementation
            break;
    }
    clock_t end_mat = clock();
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Done allocating and initiating the matrices.\n");
    }
    if (input->simulationInput.timingMode == 1) {
        double time_spent_mat = (double) (end_mat - begin_mat) / CLOCKS_PER_SEC;
        printf("Time to initiate the matrices %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_mat * 1e3, time_spent_mat, time_spent_mat / 60.0, time_spent_mat / (60 * 60));
    }


    return 0;
}

/*
 * initiateBoundaryTypeAndDataPennes: allocate memory for the boundary variables and
 * allocate their values
 */
unsigned int initiateBoundaryTypeAndDataPennes(struct boundaryData **input,
        const struct dataForSimulation * inputData, int id) {

    *input = (struct boundaryData*) malloc(sizeof (struct boundaryData)*
            inputData->equationInput[id].numberOfBoundaries);
    

    if (*input == NULL) {
        sendErrorCodeAndMessage(8707, NULL, NULL, NULL, NULL);
        return 8707;
    }

    for (unsigned int i = 0; i < inputData->equationInput[id].numberOfBoundaries; i++) {
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
        if (inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].adiabaticDefined == 1) {
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

        // scalar case
        if (inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_scalarDefined == 1) {
            (*input)[i].quantityOfBoundaries = 1;
            (*input)[i].boundaries = (struct boundaryTypeAndData*)
                    malloc(sizeof (struct boundaryTypeAndData));
            if ((*input)[i].boundaries == NULL) {
                sendErrorCodeAndMessage(8709, NULL, NULL, NULL, NULL);
                return 8709;
            }

            (*input)[i].boundaries[0].boundaryType = 1;
            (*input)[i].boundaries[0].boundaryData = (double*) malloc(sizeof (double));
            (*input)[i].boundaries[0].boundaryData[0] = inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_scalar;
            continue;
        }

        // starts by defining these variables. We can have more than one effect
        // on the boundary. For instance, we can have heat flux, convection and
        // radiation heat transfer. That's why we need to define the quantityOfBoundaries
        // variable
        (*input)[i].quantityOfBoundaries = 0;
        (*input)[i].boundaries = NULL;

        // heat flux case
        if (inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_fluxDefined == 1) {
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
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[0] = inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_flux; // heat flux value
        }

        // convection case
        if (inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_convectionDefined == 1) {
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
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[0] = inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_convectionScalar; // temperature for the convection effect
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[1] = inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_convectionCoefficient; // convective heat transfer coefficient
        }

        // radiation case
        if (inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_radiationDefined == 1) {
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
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[0] = inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_radiationScalar; // temperature for the radiation effect
            (*input)[i].boundaries[(*input)[i].quantityOfBoundaries - 1].boundaryData[1] = inputData->boundaryInput[ inputData->equationInput[id].boundaryNumbers[i] ].generalized_radiationCoefficient; // radiation emissivity coefficient
        }
    }


    return 0;
}