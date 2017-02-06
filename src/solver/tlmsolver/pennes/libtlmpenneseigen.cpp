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
 * File:   libtlmpenneseigen.cpp
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
 * 
 *
 */


#include <stdlib.h>
#include <time.h>

#include "libtlmpenneseigen.h"

#include "libtlmpennes.h"
#include "libtlmpennes1dsolver.h"
#include "libtlmpennes2dsolver.h"
#include "libtlmpennes3dsolver.h"
#include "../../../miscellaneous/liberrorcode.h"
#include "../../../miscellaneous/libwritetofiletlmbht.h"

/*
 * solverTLMPennesEigenGeneral: Solves the 'diffusion', 'hyperbolic diffusion',
 * 'heat', 'hyperbolic heat', 'pennes', and 'hyperbolic pennes' equations
 */
unsigned int solverTLMPennesEigenGeneral(struct dataForSimulation* input, int id, void** generalMatrix) {
    unsigned int errorTLMnumber = 0;
    struct calculationTLMEigen calcs;
    struct boundaryData *boundaries;
    struct connectionLeveln intersections;
    clock_t end_solve, begin_solve, end_matrices, begin_matrices, end_initiate, begin_initiate;

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Testing the input...\n");
    }

    switch (input->equationInput[id].dimen) {
        case ONE:
            // tests if the inputs for TLM Pennes 1D is ok
            if ((errorTLMnumber = testInputTLMPennes1D(input)) != 0) {
                sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
                return errorTLMnumber;
            }
            break;

        case TWO:
            // tests if the inputs for TLM Pennes 2D is ok
            if ((errorTLMnumber = testInputTLMPennes2D(input)) != 0) {
                sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
                return errorTLMnumber;
            }
            break;

        case THREE:
            // tests if the inputs for TLM Pennes 3D is ok
            if ((errorTLMnumber = testInputTLMPennes3D(input)) != 0) {
                sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
                return errorTLMnumber;
            }
            break;
    }

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Done testing the input.\n");
    }


    // initiate the variables
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Allocating and initiating the variables...\n");
    }
    begin_initiate = clock();
    if ((errorTLMnumber = initiateVariablesTLMPennes(input, &calcs,
            &(calcs.numbers), &boundaries, &intersections, id)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        // If I find an error, I will redirect the code to the cleaning part
        goto cleaning_part;
    }
    end_initiate = clock();
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("\nDone allocating and initiating the variables.\n");
    }

    if (input->simulationInput.timingMode == 1) {
        double time_spent_initiate = (double) (end_initiate - begin_initiate) / CLOCKS_PER_SEC;
        printf("Total time to allocate and initiate the variables %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent_initiate * 1e3, time_spent_initiate, time_spent_initiate / 60.0, time_spent_initiate / (60 * 60));
    }

    // get the matrices values. I verify if errorTLMnumber = 0 because if it is 
    // not I wanna get to the terminate part of the code to deallocate the allocated
    // variables
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Calculating the matrices...\n");
    }
    begin_matrices = clock();
    if ((errorTLMnumber = calculateMatricesPennesEigen(input, &calcs, &boundaries, &intersections, id)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        // If I find an error, I will redirect the code to the cleaning part
        goto cleaning_part;
    }
    end_matrices = clock();
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("\nDone calculating the matrices\n");
    }
    if (input->simulationInput.timingMode == 1) {
        double time_spent_matrices = (double) (end_matrices - begin_matrices) / CLOCKS_PER_SEC;
        printf("Total time to calculate the matrices %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent_matrices * 1e3, time_spent_matrices, time_spent_matrices / 60.0, time_spent_matrices / (60 * 60));
    }

    // Solve the problem
    if (input->equationInput[id].solveDirectly == 1) {
        if (input->simulationInput.printAdditionalMode == 1) {
            printf("Solving this equation directly\n");
        }
        begin_solve = clock();
        // in time domain
        if (input->equationInput[id].Solv == DYNAMIC) {
            errorTLMnumber = tlmSolveMatricesTimeDomainEigen(&calcs, input, id);
        } else if (input->equationInput[id].Solv == STEADY) {
            errorTLMnumber = tlmSolveMatricesSteadyStateEigen(&calcs, input, id);
        }
        if (errorTLMnumber != 0) {
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
            // If I find an error, I will redirect the code to the cleaning part
            goto cleaning_part;
        }

        end_solve = clock();

        // this equation does not need to be solved anymore. It was solved
        input->equationInput[id].needToSolve = 0;

        if (input->simulationInput.printAdditionalMode == 1) {
            printf("Done solving this equation directly\n");
        }

        if (input->simulationInput.timingMode == 1) {
            double time_spent_solve = (double) (end_solve - begin_solve) / CLOCKS_PER_SEC;
            printf("Total time to solve the matrices %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_solve * 1e3, time_spent_solve, time_spent_solve / 60.0, time_spent_solve / (60 * 60));
        }
    }

    // I deallocate the variables if I found an error during the processing, i.e.,
    // errorTLMnubmer != 0 OR if I don't need to solve this equation anymore needToSolve == 0
    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Deallocating (cleaning) the unnecessary variables\n");
    }
    // if errorTLMnumber != 0, it does a full deallocation (except for the input
    // variable, which is not changed here). If needToSolve = 0, it does a full
    // deallocation. If needToSolve = 1, it does a partial deallocation (does not
    // deallocate calcs)
cleaning_part:
    if ((errorTLMnumber = terminateVariablesTLMEigen(input, &calcs, &boundaries,
            &intersections, input->equationInput[id].needToSolve, errorTLMnumber, id)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Done deallocating (cleaning) the unnecessary variables\n");
    }



    if (input->simulationInput.printAdditionalMode == 1) {
        printf("Exiting the TLM solver using the Eigen library\n");
    }

    *generalMatrix = (void *) &calcs;

    return errorTLMnumber;
}

/*
 * calculateMatricesEigen: Calculate the matrixes M and E, and the supporting matrices
 * 
 */
unsigned int calculateMatricesPennesEigen(struct dataForSimulation *input,
        struct calculationTLMEigen *matrices, struct boundaryData** boundaries,
        struct connectionLeveln *intersections, int id) {

    unsigned int errorTLMnumber;

    // calculating the matrices for material line elements
    if (input->simulationInput.verboseMode == 1) {
        printf("Quantity of material line nodes found: %llu\n", matrices->numbers.MaterialElements[1]);
        if (input->equationInput[id].dimen != ONE) {
            printf("Material line nodes are not considered outside the one-dimensional problems. They will not be considered here.\n");
        }
    }
    if (matrices->numbers.MaterialElements[1] && input->equationInput[id].dimen == ONE) {
        if (input->simulationInput.verboseMode == 1) {
            printf("Calculating the matrix for the line nodes...\n");
        }
        clock_t begin_Line = clock();
        if ((errorTLMnumber = MaterialLinePennesEigen(input, matrices, id)) != 0) {
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
            return errorTLMnumber;
        }
        clock_t end_Line = clock();
        if (input->simulationInput.verboseMode == 1) {
            printf("Done calculating the matrix for the line nodes.\n");
            if (input->simulationInput.timingMode == 1) {
                double time_spent_Line = (double) (end_Line - begin_Line) / CLOCKS_PER_SEC;
                printf("Time to calculate the matrices for the line nodes %g ms (or %g s, or %g min, or %g hours).\n",
                        time_spent_Line * 1e3, time_spent_Line, time_spent_Line / 60.0, time_spent_Line / (60 * 60));
            }
        }
    }



    // calculating the matrices for material triangle elements
    if (input->simulationInput.verboseMode == 1) {
        printf("Quantity of material triangle nodes found: %llu\n", matrices->numbers.MaterialElements[2]);
        if (input->equationInput[id].dimen != TWO) {
            printf("Material triangle nodes are not considered outside the two-dimensional problems. They will not be considered here.\n");
        }
    }
    if (matrices->numbers.MaterialElements[2] && input->equationInput[id].dimen == TWO) {
        if (input->simulationInput.verboseMode == 1) {
            printf("Calculating the matrix for the triangle nodes...\n");
        }
        clock_t begin_Triangle = clock();
        if ((errorTLMnumber = MaterialTrianglePennesEigen(input, matrices, id)) != 0) {
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
            return errorTLMnumber;
        }
        clock_t end_Triangle = clock();
        if (input->simulationInput.verboseMode == 1) {
            printf("Done calculating the matrix for the triangle nodes.\n");
            if (input->simulationInput.timingMode == 1) {
                double time_spent_Triangle = (double) (end_Triangle - begin_Triangle) / CLOCKS_PER_SEC;
                printf("Time to calculate the matrices for the triangle nodes %g ms (or %g s, or %g min, or %g hours).\n",
                        time_spent_Triangle * 1e3, time_spent_Triangle, time_spent_Triangle / 60.0, time_spent_Triangle / (60 * 60));
            }
        }
    }

    // calculating the matrices for material tetrahedron elements
    if (input->simulationInput.verboseMode == 1) {
        printf("Quantity of material tetrahedron nodes found: %llu\n", matrices->numbers.MaterialElements[4]);
        if (input->equationInput[id].dimen != THREE) {
            printf("Material tetrahedron nodes are not considered outside the three-dimensional problems. They will not be considered here.\n");
        }
    }
    if (matrices->numbers.MaterialElements[4] && input->equationInput[id].dimen == THREE) {
        if (input->simulationInput.verboseMode == 1) {
            printf("Calculating the matrix for the tetrahedron nodes...\n");
        }
        clock_t begin_Tetrahedron = clock();
        if ((errorTLMnumber = MaterialTetrahedronPennesEigen(input, matrices, id)) != 0) {
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
            return errorTLMnumber;
        }
        clock_t end_Tetrahedron = clock();
        if (input->simulationInput.verboseMode == 1) {
            printf("Done calculating the matrix for the tetrahedron nodes.\n");
            if (input->simulationInput.timingMode == 1) {
                double time_spent_Tetrahedron = (double) (end_Tetrahedron - begin_Tetrahedron) / CLOCKS_PER_SEC;
                printf("Time to calculate the matrices for the tetrahedral nodes %g ms (or %g s, or %g min, or %g hours).\n",
                        time_spent_Tetrahedron * 1e3, time_spent_Tetrahedron, time_spent_Tetrahedron / 60.0, time_spent_Tetrahedron / (60 * 60));
            }
        }
    }


    // finalizing the matrices with the connection variables
    if (input->simulationInput.verboseMode == 1) {
        printf("Including connections and boundaries...\n");
    }
    clock_t begin_CB = clock();
    if ((errorTLMnumber = connectionsAndBoundariesPennesEigen(matrices, *boundaries,
            intersections, input, id)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    clock_t end_CB = clock();
    if (input->simulationInput.verboseMode == 1) {
        printf("Done including connections and boundaries.\n");
        if (input->simulationInput.timingMode == 1) {
            double time_spent_CB = (double) (end_CB - begin_CB) / CLOCKS_PER_SEC;
            printf("Time to include connections and boundaries %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_CB * 1e3, time_spent_CB, time_spent_CB / 60.0, time_spent_CB / (60 * 60));
        }
    }


    // writing data to the output file
    if (input->simulationInput.verboseMode == 1) {
        printf("Writing data to the output file...\n");
    }
    clock_t begin_wr = clock();
    if ((errorTLMnumber = writeFirstDataToFile(input, matrices, id)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    clock_t end_wr = clock();
    if (input->simulationInput.verboseMode == 1) {
        printf("Done writing data to the output file...\n");
        if (input->simulationInput.timingMode == 1) {
            double time_spent_wr = (double) (end_wr - begin_wr) / CLOCKS_PER_SEC;
            printf("Time to write data to file %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_wr * 1e3, time_spent_wr, time_spent_wr / 60.0, time_spent_wr / (60 * 60));
        }
    }


    if (input->equationInput[id].Solv == STEADY) {
        if (input->simulationInput.verboseMode == 1) {
            printf("Initiating the steady-state matrices...\n");
        }
        clock_t begin_ss = clock();
        if ((errorTLMnumber = tlmInitializeSteadyMatrices(matrices)) == 0) {
            return errorTLMnumber;
        }
        clock_t end_ss = clock();
        if (input->simulationInput.verboseMode == 1) {
            printf("Done initiating the steady-state matrices.\n");
            if (input->simulationInput.timingMode == 1) {
                double time_spent_SteadyState = (double) (end_ss - begin_ss) / CLOCKS_PER_SEC;
                printf("Time to calculate the steady-state matrices %g ms (or %g s, or %g min, or %g hours).\n",
                        time_spent_SteadyState * 1e3, time_spent_SteadyState, time_spent_SteadyState / 60.0, time_spent_SteadyState / (60 * 60));
            }
        }
    }

    if (input->simulationInput.verboseMode == 1) {
        printf("Terminating some variables...\n");
    }
    clock_t begin_tr = clock();
    if ((errorTLMnumber = terminateSomeVariablesEigen(input, matrices, boundaries, intersections, id)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    clock_t end_tr = clock();
    if (input->simulationInput.verboseMode == 1) {
        printf("Done terminating some variables...\n");
        if (input->simulationInput.timingMode == 1) {
            double time_spent_tr = (double) (end_tr - begin_tr) / CLOCKS_PER_SEC;
            printf("Time to terminate some variables %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_tr * 1e3, time_spent_tr, time_spent_tr / 60.0, time_spent_tr / (60 * 60));
        }
    }

    return 0;
}

/*
 * MaterialLineEigen: 
 * Line as a material was not implemented yet.
 */
unsigned int MaterialLinePennesEigen(struct dataForSimulation *input, struct calculationTLMEigen *matrices, int id) {
    unsigned int errorTLMnumber;
    unsigned long long numbersNodeAndPort[] = {0, 0};
    // 0 - number of node
    // 1 - number of port 0
    unsigned int j2;
    double Cd, Z[1], Cs, Zs, R[1], G, Is, Zhat, tau[1], taus;

    // line nomenclature
    //
    //   vertex 1  \ 
    //              \
    //               \    
    //                \
    //                 \  vertex 2
    //
    // lines can be boundary conditions and materials.
    // 1D: material; maybe boundary conditions (?)
    // 2D: boundary conditions (treated elsewhere); maybe materials (?)
    // 3D: maybe materials (?); maybe boundary conditions (?)
    //
    // element code 1: 2 nodes line.
    double *tempVar = NULL;
    tempVar = (double *) realloc(tempVar, sizeof (double)*4);
    // 0 - length of port 1 and 2. They are equal to (nodes_length)/2.
    // 1 - Lines' center x
    // 2 - Lines' center y
    // 3 - Lines' center z

    for (unsigned long long i = 0; i < input->mesh.quantityOfSpecificElement[1]; i++) {
        // i: element level
        // j: boundary type level
        // k: different tag numbers for the same boundary type
        for (unsigned int j = 0; j < input->equationInput[id].numberOfMaterials; j++)
            for (unsigned int k = 0; k < input->materialInput[ input->equationInput[id].materialNumbers[j] ].quantityOfNumberInput; k++)
                if (input->mesh.elements.Line[i].tag == input->materialInput[ input->equationInput[id].materialNumbers[j] ].numberInput[k]) {
                    j2 = input->equationInput[id].materialNumbers[j];
                    // I will use goto to get out of these two inner 'for' loops. Bare in mind
                    // that I'm just getting out the loops. You can also think that 
                    // I'm going to the next 'i', that is, incrementing i by 1 and going
                    // to the next value of i (if any).

                    // here I switch the code accordingly to what type of node I have
                    
                    // this is the simplest of all. Also called parabolic equation.
                    // Used for 'diffusion', 'heat', and 'pennes'
                    if (input->materialInput[j2].generalized_relaxationTime == 0) {
                        // this has two nodes that connect with others elements

                        getRealNodeAndPort_fromAbstractNode(1, // element code
                                i, // element number
                                matrices->numbers.abstractPortsToReal,
                                numbersNodeAndPort);

                        // the -1 is necessary because the C indexing starts at zero
                        // and my number of node starts at 1
                        getGeometricalVariablesTLMline(&input->mesh.nodes[input->mesh.elements.Line[i].N1 - 1],
                                &input->mesh.nodes[input->mesh.elements.Line[i].N2 - 1], tempVar);

                        matrices->L[numbersNodeAndPort[1] + 0] = 1; // this length is defined as 1 for 1D nodes
                        matrices->L[numbersNodeAndPort[1] + 1] = 1; // this length is defined as 1 for 1D nodes

                        matrices->deltal[numbersNodeAndPort[1] + 0] = tempVar[0]; // length of port 1
                        matrices->deltal[numbersNodeAndPort[1] + 1] = tempVar[0]; // length of port 2

                        // generalization of Cd
                        Cd = input->materialInput[j2].generalized_coefficient_b;

                        // generalization of R
                        R[0] = tempVar[0] / input->materialInput[j2].generalized_diffusionCoeff;

                        // generalization of G
                        G = 2 * tempVar[0] * input->materialInput[j2].generalized_sink_a;

                        // generalization of Is
                        Is = 2 * tempVar[0] * input->materialInput[j2].generalized_source;

                        // Z = dt/(2*Cd*deltal). Manually validated
                        Z[0] = input->equationInput[id].timeStep / (tempVar[0] * 2 * Cd);

                        // all the impedances
                        matrices->Z[numbersNodeAndPort[1] + 0] = Z[0];
                        matrices->Z[numbersNodeAndPort[1] + 1] = Z[0];

                        // all the resistances
                        matrices->R[numbersNodeAndPort[1] + 0] = R[0];
                        matrices->R[numbersNodeAndPort[1] + 1] = R[0];

                        // Manually validated
                        Zhat = Z[0] / (2 + Z[0] * G);

                        // Manually validated
                        tau[0] = 2 * Zhat / Z[0];
                        // this is actually matrix S.
                        // M = C*S
                        // this matrix is M[line][column].
                        matrices->M.insert(numbersNodeAndPort[1] + 0,
                                numbersNodeAndPort[1] + 0) = tau[0] - 1;
                        matrices->M.insert(numbersNodeAndPort[1] + 0,
                                numbersNodeAndPort[1] + 1) = tau[0];

                        matrices->M.insert(numbersNodeAndPort[1] + 1,
                                numbersNodeAndPort[1] + 0) = tau[0];
                        matrices->M.insert(numbersNodeAndPort[1] + 1,
                                numbersNodeAndPort[1] + 1) = tau[0] - 1;

                        // matrix tau
                        matrices->tau.insert(numbersNodeAndPort[0],
                                numbersNodeAndPort[1] + 0) = tau[0];
                        matrices->tau.insert(numbersNodeAndPort[0],
                                numbersNodeAndPort[1] + 1) = tau[0];

                        // this is actually matrix ZIS.
                        // E = C*ZIS + B
                        matrices->E(numbersNodeAndPort[1] + 0) = Zhat*Is;
                        matrices->E(numbersNodeAndPort[1] + 1) = Zhat*Is;

                        // matrix E_output
                        matrices->E_output(numbersNodeAndPort[0]) = Zhat*Is;

                        matrices->Points_output[numbersNodeAndPort[0]].x = tempVar[1];
                        matrices->Points_output[numbersNodeAndPort[0]].y = tempVar[2];
                        matrices->Points_output[numbersNodeAndPort[0]].z = tempVar[3];


                        // initial value
                        // Vi0 = (Ti - ZIS)/sum(tau)
                        matrices->Vi(numbersNodeAndPort[1] + 0) = (input->materialInput[j2].generalized_initialScalar
                                - matrices->E(numbersNodeAndPort[1] + 0)) / (2 * tau[0]);
                        matrices->Vi(numbersNodeAndPort[1] + 1) = matrices->Vi(numbersNodeAndPort[1] + 0);
                        
                        
                        
                        
                        
                        // this is for the wave models. Also called hyperbolic equation.
                        // Used for 'hyperbolic diffusion', 'hyperbolic heat', and 
                        // 'hyperbolic pennes'
                    } else {
                        // this has three nodes. Two nodes connect with others elements
                        // and one extra node is used to model the relaxation time
                        
                        getRealNodeAndPort_fromAbstractNode(1, // element code
                                i, // element number
                                matrices->numbers.abstractPortsToReal,
                                numbersNodeAndPort);

                        // the -1 is necessary because the C indexing starts at zero
                        // and my number of node starts at 1
                        getGeometricalVariablesTLMline(&input->mesh.nodes[input->mesh.elements.Line[i].N1 - 1],
                                &input->mesh.nodes[input->mesh.elements.Line[i].N2 - 1], tempVar);

                        matrices->L[numbersNodeAndPort[1] + 0] = 1; // this length is defined as 1 for 1D nodes
                        matrices->L[numbersNodeAndPort[1] + 1] = 1; // this length is defined as 1 for 1D nodes
                        matrices->L[numbersNodeAndPort[1] + 2] = 1; // this length is defined as 1 for 1D nodes

                        matrices->deltal[numbersNodeAndPort[1] + 0] = tempVar[0]; // length of port 1
                        matrices->deltal[numbersNodeAndPort[1] + 1] = tempVar[0]; // length of port 2
                        matrices->deltal[numbersNodeAndPort[1] + 2] = 1; // length of stub is not defined

                        // generalization of Cd
                        Cd = input->materialInput[j2].generalized_diffusionCoeff*
                                input->equationInput[id].timeStep*input->equationInput[id].timeStep/
                                (4*tempVar[0]*tempVar[0]*input->materialInput[j2].generalized_relaxationTime);
                        
                        // generalization of Cs
                        Cs = (input->materialInput[j2].generalized_coefficient_b - Cd)*2*tempVar[0];
                        
                        // generalization of R
                        R[0] = tempVar[0] / input->materialInput[j2].generalized_diffusionCoeff;

                        // generalization of G
                        G = 2 * tempVar[0] * input->materialInput[j2].generalized_sink_a;

                        // generalization of Is
                        Is = 2 * tempVar[0] * input->materialInput[j2].generalized_source;

                        // Z = dt/(2*Cd*deltal). Manually validated
                        Z[0] = input->equationInput[id].timeStep / (tempVar[0] * 2 * Cd);
                        
                        // Zs = dt/(2*Cs*deltal)
                        if (Cs > 0){
                            Zs = input->equationInput[id].timeStep / (tempVar[0] * 2 * Cs);
                        } else if (Cs < 0){
                            Zs = -input->equationInput[id].timeStep / (tempVar[0] * 2 * Cs);
                            // this can only be Cs = 0, which means that we don't need stub
                        } else {
                            Zs = -1; // this is my flag that tells me that I don't need stub
                        }
                        

                        // all the impedances
                        matrices->Z[numbersNodeAndPort[1] + 0] = Z[0];
                        matrices->Z[numbersNodeAndPort[1] + 1] = Z[0];
                        // this is the stub impedance
                        matrices->Z[numbersNodeAndPort[1] + 2] = Zs;

                        // all the resistances
                        matrices->R[numbersNodeAndPort[1] + 0] = R[0];
                        matrices->R[numbersNodeAndPort[1] + 1] = R[0];
                        matrices->R[numbersNodeAndPort[1] + 2] = 1; // this is not defined

                        // Calculating Zhat
                        // In this case I don't have Zs
                        if (Zs == -1){
                            Zhat = Z[0] / (2 + Z[0] * G);
                            taus = 0;
                            
                            // this is the case that I need Zs
                        } else {
                            Zhat = 1/(G + 1/Zs + 2/Z);
                            taus = 2 * Zhat / Zs;
                        }
                        

                        // Manually validated
                        tau[0] = 2 * Zhat / Z[0];
                        
                        // this is actually matrix S.
                        // M = C*S
                        // this matrix is M[line][column].
                        matrices->M.insert(numbersNodeAndPort[1] + 0,
                                numbersNodeAndPort[1] + 0) = tau[0] - 1;
                        matrices->M.insert(numbersNodeAndPort[1] + 0,
                                numbersNodeAndPort[1] + 1) = tau[0];
                        matrices->M.insert(numbersNodeAndPort[1] + 0,
                                numbersNodeAndPort[1] + 2) = taus;

                        matrices->M.insert(numbersNodeAndPort[1] + 1,
                                numbersNodeAndPort[1] + 0) = tau[0];
                        matrices->M.insert(numbersNodeAndPort[1] + 1,
                                numbersNodeAndPort[1] + 1) = tau[0] - 1;
                        matrices->M.insert(numbersNodeAndPort[1] + 1,
                                numbersNodeAndPort[1] + 2) = taus;
                        
                        matrices->M.insert(numbersNodeAndPort[1] + 2,
                                numbersNodeAndPort[1] + 0) = tau[0];
                        matrices->M.insert(numbersNodeAndPort[1] + 2,
                                numbersNodeAndPort[1] + 1) = tau[0];
                        matrices->M.insert(numbersNodeAndPort[1] + 2,
                                numbersNodeAndPort[1] + 2) = taus - 1;

                        // matrix tau
                        matrices->tau.insert(numbersNodeAndPort[0],
                                numbersNodeAndPort[1] + 0) = tau[0];
                        matrices->tau.insert(numbersNodeAndPort[0],
                                numbersNodeAndPort[1] + 1) = tau[0];
                        matrices->tau.insert(numbersNodeAndPort[0],
                                numbersNodeAndPort[1] + 2) = taus;

                        // this is actually matrix ZIS.
                        // E = C*ZIS + B
                        matrices->E(numbersNodeAndPort[1] + 0) = Zhat*Is;
                        matrices->E(numbersNodeAndPort[1] + 1) = Zhat*Is;
                        matrices->E(numbersNodeAndPort[1] + 2) = Zhat*Is;

                        // matrix E_output
                        matrices->E_output(numbersNodeAndPort[0]) = Zhat*Is;

                        matrices->Points_output[numbersNodeAndPort[0]].x = tempVar[1];
                        matrices->Points_output[numbersNodeAndPort[0]].y = tempVar[2];
                        matrices->Points_output[numbersNodeAndPort[0]].z = tempVar[3];


                        // initial value
                        // Vi0 = (Ti - ZIS)/sum(tau)
                        matrices->Vi(numbersNodeAndPort[1] + 0) = (input->materialInput[j2].generalized_initialScalar
                                - matrices->E(numbersNodeAndPort[1] + 0)) / (2 * tau[0]);
                        matrices->Vi(numbersNodeAndPort[1] + 1) = matrices->Vi(numbersNodeAndPort[1] + 0);
                        
                    }

                    goto end_for_j_and_for_k_line;
                }
end_for_j_and_for_k_line:
        ;
    }
    return 0;
}

/*
 * MaterialTriangleEigen: Calculates the parameters to set the triangle as material
 * or as boundary in a 2D simulation.
 * Triangle as a boundary was not implemented yet.
 */
unsigned int MaterialTrianglePennesEigen(struct dataForSimulation *input, struct calculationTLMEigen *matrices, int id) {
    unsigned int errorTLMnumber;
    unsigned long long numbersNodeAndPort[] = {0, 0};
    // 0 - number of node
    // 1 - number of port 0
    unsigned int j2;
    double Cd, Z[3], R[3], G, Is, Zhat, tau[3];



    // triangular nomenclature.
    //              vertex 1
    //              /\
    //     face 1  /  \   face 2
    //            /    \
    //           /      \
    // vertex 2 /________\ vertex 3
    //            face 3

    // triangles can be boundary conditions and mediums.
    // 1D: Not defined;
    // 2D: Mediums; Maybe boundary conditions (?)
    // 3D: Boundary Condition (treated elsewhere).
    //
    // element code 3: 3 nodes triangle.
    double *tempVar = NULL;
    tempVar = (double *) realloc(tempVar, sizeof (double)*10);
    // 0 - length of port 1 (from center of triangle to center of face 1)
    // 1 - length of port 2 (from center of triangle to center of face 2)
    // 2 - length of port 3 (from center of triangle to center of face 3)
    // 3 - triangular area
    // 4 - length of face 1
    // 5 - length of face 2
    // 6 - length of face 3
    // 7 - Triangles' center x
    // 8 - Triangles' center y
    // 9 - Triangles' center z

    for (unsigned long long i = 0; i < input->mesh.quantityOfSpecificElement[2]; i++) {
        // i: element level
        // j: boundary type level
        // k: different tag numbers for the same boundary type
        for (unsigned int j = 0; j < input->equationInput[id].numberOfMaterials; j++)
            for (unsigned int k = 0; k < input->materialInput[ input->equationInput[id].materialNumbers[j] ].quantityOfNumberInput; k++)
                if (input->mesh.elements.Triangle[i].tag == input->materialInput[ input->equationInput[id].materialNumbers[j] ].numberInput[k]) {
                    j2 = input->equationInput[id].materialNumbers[j];
                    // I will use goto to get out of these two inner 'for' loops. Bare in mind
                    // that I'm just getting out the loops. You can also think that 
                    // I'm going to the next 'i', that is, incrementing i by 1 and going
                    // to the next value of i (if any).

                    getRealNodeAndPort_fromAbstractNode(2, // element code
                            i, // element number
                            matrices->numbers.abstractPortsToReal,
                            numbersNodeAndPort);

                    // the -1 is necessary because the C indexing starts at zero
                    // and my number of node starts at 1
                    getGeometricalVariablesTLMtriangle(&input->mesh.nodes[input->mesh.elements.Triangle[i].N1 - 1],
                            &input->mesh.nodes[input->mesh.elements.Triangle[i].N2 - 1],
                            &input->mesh.nodes[input->mesh.elements.Triangle[i].N3 - 1], tempVar);

                    matrices->L[numbersNodeAndPort[1] + 0] = tempVar[4]; // length of face 1
                    matrices->L[numbersNodeAndPort[1] + 1] = tempVar[5]; // length of face 2
                    matrices->L[numbersNodeAndPort[1] + 2] = tempVar[6]; // length of face 3

                    matrices->deltal[numbersNodeAndPort[1] + 0] = tempVar[0]; // length of port 1
                    matrices->deltal[numbersNodeAndPort[1] + 1] = tempVar[1]; // length of port 2
                    matrices->deltal[numbersNodeAndPort[1] + 2] = tempVar[2]; // length of port 3

                    // generalization of Cd
                    Cd = tempVar[3] * input->materialInput[j2].generalized_coefficient_b /
                            (tempVar[0] + tempVar[1] + tempVar[2]);

                    // generalization of R
                    R[0] = tempVar[0] / (input->materialInput[j2].generalized_diffusionCoeff * tempVar[4]);
                    R[1] = tempVar[1] / (input->materialInput[j2].generalized_diffusionCoeff * tempVar[5]);
                    R[2] = tempVar[2] / (input->materialInput[j2].generalized_diffusionCoeff * tempVar[6]);

                    // generalization of G
                    G = tempVar[3] * input->materialInput[j2].generalized_sink_a;

                    // generalization of Is
                    Is = tempVar[3] * input->materialInput[j2].generalized_source;

                    // Z = dt/(2*Cd*deltal). Manually validated
                    Z[0] = input->equationInput[id].timeStep / (2 * Cd);
                    Z[1] = Z[0] / tempVar[1];
                    Z[2] = Z[0] / tempVar[2];
                    Z[0] = Z[0] / tempVar[0];

                    // all the impedances
                    matrices->Z[numbersNodeAndPort[1] + 0] = Z[0];
                    matrices->Z[numbersNodeAndPort[1] + 1] = Z[1];
                    matrices->Z[numbersNodeAndPort[1] + 2] = Z[2];

                    // all the resistances
                    matrices->R[numbersNodeAndPort[1] + 0] = R[0];
                    matrices->R[numbersNodeAndPort[1] + 1] = R[1];
                    matrices->R[numbersNodeAndPort[1] + 2] = R[2];

                    // Manually validated
                    Zhat = Z[0] * Z[1] * Z[2] / (Z[0] * Z[1] + Z[0] * Z[2] + Z[1] * Z[2]
                            + Z[0] * Z[1] * Z[2] * G);

                    // Manually validated
                    tau[0] = 2 * Zhat / Z[0];
                    tau[1] = 2 * Zhat / Z[1];
                    tau[2] = 2 * Zhat / Z[2];
                    // this is actually matrix S.
                    // M = C*S
                    // this matrix is M[line][column].
                    matrices->M.insert(numbersNodeAndPort[1] + 0,
                            numbersNodeAndPort[1] + 0) = tau[0] - 1;
                    matrices->M.insert(numbersNodeAndPort[1] + 0,
                            numbersNodeAndPort[1] + 1) = tau[1];
                    matrices->M.insert(numbersNodeAndPort[1] + 0,
                            numbersNodeAndPort[1] + 2) = tau[2];

                    matrices->M.insert(numbersNodeAndPort[1] + 1,
                            numbersNodeAndPort[1] + 0) = tau[0];
                    matrices->M.insert(numbersNodeAndPort[1] + 1,
                            numbersNodeAndPort[1] + 1) = tau[1] - 1;
                    matrices->M.insert(numbersNodeAndPort[1] + 1,
                            numbersNodeAndPort[1] + 2) = tau[2];

                    matrices->M.insert(numbersNodeAndPort[1] + 2,
                            numbersNodeAndPort[1] + 0) = tau[0];
                    matrices->M.insert(numbersNodeAndPort[1] + 2,
                            numbersNodeAndPort[1] + 1) = tau[1];
                    matrices->M.insert(numbersNodeAndPort[1] + 2,
                            numbersNodeAndPort[1] + 2) = tau[2] - 1;

                    // matrix tau
                    matrices->tau.insert(numbersNodeAndPort[0],
                            numbersNodeAndPort[1] + 0) = tau[0];
                    matrices->tau.insert(numbersNodeAndPort[0],
                            numbersNodeAndPort[1] + 1) = tau[1];
                    matrices->tau.insert(numbersNodeAndPort[0],
                            numbersNodeAndPort[1] + 2) = tau[2];

                    // this is actually matrix ZIS.
                    // E = C*ZIS + B
                    matrices->E(numbersNodeAndPort[1] + 0) = Zhat*Is;
                    matrices->E(numbersNodeAndPort[1] + 1) = Zhat*Is;
                    matrices->E(numbersNodeAndPort[1] + 2) = Zhat*Is;

                    // matrix E_output
                    matrices->E_output(numbersNodeAndPort[0]) = Zhat*Is;

                    matrices->Points_output[numbersNodeAndPort[0]].x = tempVar[7];
                    matrices->Points_output[numbersNodeAndPort[0]].y = tempVar[8];
                    matrices->Points_output[numbersNodeAndPort[0]].z = tempVar[9];


                    // initial value
                    // Vi0 = (Ti - ZIS)/sum(tau)
                    matrices->Vi(numbersNodeAndPort[1] + 0) = (input->materialInput[j2].generalized_initialScalar
                            - matrices->E(numbersNodeAndPort[1] + 0)) / (tau[0] + tau[1] + tau[2]);
                    matrices->Vi(numbersNodeAndPort[1] + 1) = matrices->Vi(numbersNodeAndPort[1] + 0);
                    matrices->Vi(numbersNodeAndPort[1] + 2) = matrices->Vi(numbersNodeAndPort[1] + 0);

                    goto end_for_j_and_for_k_triangle;
                }
end_for_j_and_for_k_triangle:
        ;
    }
    return 0;
}

/*
 * MaterialTetrahedronPennesEigen: Calculates the parameters to set the tetrahedron
 * as material in a 2D simulation.
 * Tetrahedron as a boundary was not implemented yet.
 */
unsigned int MaterialTetrahedronPennesEigen(struct dataForSimulation *input, struct calculationTLMEigen *matrices, int id) {
    unsigned int errorTLMnumber;
    unsigned long long numbersNodeAndPort[] = {0, 0};
    // 0 - number of node
    // 1 - number of port 0
    unsigned int j2;
    double Cd, Z[4], R[4], G, Is, Zhat, tau[4];



    /* tetrahedral nomenclature.
     *                      vertex 1
     *                          __
     *                         /\ \_
     *                        /  \  \_
     *                       /    \   \_
     *                      /      \    \_  edge 3
     *                     /        \     \_
     *                    /          \      \_
     *                   /          ar\ea 2   \_
     *                  /              \        \_
     *         edge 1  /                \  area 3 \_
     *                /      area 1      \ edge 2   \_  vertex 4
     *               /                    \    _____//
     *              /                  ____\__/     /
     *             /     edge 5 ______/     \      / edge 6
     *            /      ______/             \    /
     *           /______/           area 4    \  /
     * vertex 2 //_____________________________\/ vertex 3
     *                      edge 4
     */
    // triangles can be boundary conditions and mediums. The boundary conditions
    // version was not implemented yet.   
    // element code 3: 3 nodes triangle. Used only for medium only. The boundary
    // condition version was not implemented yet
    double *tempVar = NULL;
    tempVar = (double *) realloc(tempVar, sizeof (double)*12);
    // 0 - length of port 1 (from center of tetrahedron to center of area 1)
    // 1 - length of port 2 (from center of tetrahedron to center of area 2)
    // 2 - length of port 3 (from center of tetrahedron to center of area 3)
    // 3 - length of port 4 (from center of tetrahedron to center of area 4)
    // 4 - area of triangle 1
    // 5 - area of triangle 2
    // 6 - area of triangle 3
    // 7 - area of triangle 4
    // 8 - volume of the tetrahedron
    // 9 - tetrahedron' center x
    // 10 - tetrahedron' center y
    // 11 - tetrahedron' center z
    for (unsigned long long i = 0; i < input->mesh.quantityOfSpecificElement[4]; i++) {
        // i: element level
        // j: material (of equation id) type level
        // k: different tag numbers for the same material type
        // j2: the material number from equation. Used to reduce the access to input->equationInput[id].materialNumbers[j]
        for (unsigned int j = 0; j < input->equationInput[id].numberOfMaterials; j++)
            for (unsigned int k = 0; k < input->materialInput[ input->equationInput[id].materialNumbers[j] ].quantityOfNumberInput; k++)
                if (input->mesh.elements.Tetrahedron[i].tag == input->materialInput[ input->equationInput[id].materialNumbers[j] ].numberInput[k]) {
                    j2 = input->equationInput[id].materialNumbers[j];
                    // I will use goto to get out of these two inner 'for' loops. Bare in mind
                    // that I'm just getting out the loops. You can also think that 
                    // I'm going to the next 'i', that is, incrementing i by 1 and going
                    // to the next value of i (if any).
                    getRealNodeAndPort_fromAbstractNode(4, // element code
                            i, // element number
                            matrices->numbers.abstractPortsToReal,
                            numbersNodeAndPort);

                    // the -1 is necessary because the C indexing starts at zero
                    // and my number of node starts at 1
                    getGeometricalVariablesTLMtetrahedron(&input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N1 - 1],
                            &input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N2 - 1],
                            &input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N3 - 1],
                            &input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N4 - 1], tempVar);

                    matrices->L[numbersNodeAndPort[1] + 0] = tempVar[4]; // area of face 1
                    matrices->L[numbersNodeAndPort[1] + 1] = tempVar[5]; // area of face 2
                    matrices->L[numbersNodeAndPort[1] + 2] = tempVar[6]; // area of face 3
                    matrices->L[numbersNodeAndPort[1] + 3] = tempVar[7]; // area of face 4

                    matrices->deltal[numbersNodeAndPort[1] + 0] = tempVar[0]; // length of port 1
                    matrices->deltal[numbersNodeAndPort[1] + 1] = tempVar[1]; // length of port 2
                    matrices->deltal[numbersNodeAndPort[1] + 2] = tempVar[2]; // length of port 3
                    matrices->deltal[numbersNodeAndPort[1] + 3] = tempVar[3]; // length of port 4

                    // generalization of Cd
                    Cd = tempVar[8] * input->materialInput[j2].generalized_coefficient_b /
                            (tempVar[0] + tempVar[1] + tempVar[2] + tempVar[3]);

                    // generalization of R
                    R[0] = tempVar[0] / (input->materialInput[j2].generalized_diffusionCoeff * tempVar[4]);
                    R[1] = tempVar[1] / (input->materialInput[j2].generalized_diffusionCoeff * tempVar[5]);
                    R[2] = tempVar[2] / (input->materialInput[j2].generalized_diffusionCoeff * tempVar[6]);
                    R[3] = tempVar[3] / (input->materialInput[j2].generalized_diffusionCoeff * tempVar[7]);

                    // generalization of G
                    G = tempVar[8] * input->materialInput[j2].generalized_sink_a;

                    // generalization of Is
                    Is = tempVar[8] * input->materialInput[j2].generalized_source;

                    // Z = dt/(2*C*deltal).
                    Z[0] = input->equationInput[id].timeStep / (2 * Cd);
                    Z[1] = Z[0] / tempVar[1];
                    Z[2] = Z[0] / tempVar[2];
                    Z[3] = Z[0] / tempVar[3];
                    Z[0] = Z[0] / tempVar[0];

                    // all the impedances
                    matrices->Z[numbersNodeAndPort[1] + 0] = Z[0];
                    matrices->Z[numbersNodeAndPort[1] + 1] = Z[1];
                    matrices->Z[numbersNodeAndPort[1] + 2] = Z[2];
                    matrices->Z[numbersNodeAndPort[1] + 3] = Z[3];

                    // all the resistances
                    matrices->R[numbersNodeAndPort[1] + 0] = R[0];
                    matrices->R[numbersNodeAndPort[1] + 1] = R[1];
                    matrices->R[numbersNodeAndPort[1] + 2] = R[2];
                    matrices->R[numbersNodeAndPort[1] + 3] = R[3];


                    // Manually validated
                    Zhat = Z[0] * Z[1] * Z[2] * Z[3] / (Z[0] * Z[1] * Z[2]
                            + Z[0] * Z[1] * Z[3] + Z[0] * Z[2] * Z[3]
                            + Z[1] * Z[2] * Z[3] + Z[0] * Z[1] * Z[2] * Z[3] * G);

                    // Manually validated
                    tau[0] = 2 * Zhat / Z[0];
                    tau[1] = 2 * Zhat / Z[1];
                    tau[2] = 2 * Zhat / Z[2];
                    tau[3] = 2 * Zhat / Z[3];

                    // this is actually matrix S.
                    // M = C*S
                    // this matrix is M[line][column].
                    matrices->M.insert(numbersNodeAndPort[1] + 0,
                            numbersNodeAndPort[1] + 0) = tau[0] - 1;
                    matrices->M.insert(numbersNodeAndPort[1] + 0,
                            numbersNodeAndPort[1] + 1) = tau[1];
                    matrices->M.insert(numbersNodeAndPort[1] + 0,
                            numbersNodeAndPort[1] + 2) = tau[2];
                    matrices->M.insert(numbersNodeAndPort[1] + 0,
                            numbersNodeAndPort[1] + 3) = tau[3];

                    matrices->M.insert(numbersNodeAndPort[1] + 1,
                            numbersNodeAndPort[1] + 0) = tau[0];
                    matrices->M.insert(numbersNodeAndPort[1] + 1,
                            numbersNodeAndPort[1] + 1) = tau[1] - 1;
                    matrices->M.insert(numbersNodeAndPort[1] + 1,
                            numbersNodeAndPort[1] + 2) = tau[2];
                    matrices->M.insert(numbersNodeAndPort[1] + 1,
                            numbersNodeAndPort[1] + 3) = tau[3];

                    matrices->M.insert(numbersNodeAndPort[1] + 2,
                            numbersNodeAndPort[1] + 0) = tau[0];
                    matrices->M.insert(numbersNodeAndPort[1] + 2,
                            numbersNodeAndPort[1] + 1) = tau[1];
                    matrices->M.insert(numbersNodeAndPort[1] + 2,
                            numbersNodeAndPort[1] + 2) = tau[2] - 1;
                    matrices->M.insert(numbersNodeAndPort[1] + 2,
                            numbersNodeAndPort[1] + 3) = tau[3];

                    matrices->M.insert(numbersNodeAndPort[1] + 3,
                            numbersNodeAndPort[1] + 0) = tau[0];
                    matrices->M.insert(numbersNodeAndPort[1] + 3,
                            numbersNodeAndPort[1] + 1) = tau[1];
                    matrices->M.insert(numbersNodeAndPort[1] + 3,
                            numbersNodeAndPort[1] + 2) = tau[2];
                    matrices->M.insert(numbersNodeAndPort[1] + 3,
                            numbersNodeAndPort[1] + 3) = tau[3] - 1;

                    // matrix tau
                    matrices->tau.insert(numbersNodeAndPort[0],
                            numbersNodeAndPort[1] + 0) = tau[0];
                    matrices->tau.insert(numbersNodeAndPort[0],
                            numbersNodeAndPort[1] + 1) = tau[1];
                    matrices->tau.insert(numbersNodeAndPort[0],
                            numbersNodeAndPort[1] + 2) = tau[2];
                    matrices->tau.insert(numbersNodeAndPort[0],
                            numbersNodeAndPort[1] + 3) = tau[3];

                    // this is actually matrix ZIS.
                    // E = C*ZIS + B
                    matrices->E(numbersNodeAndPort[1] + 0) = Zhat*Is;
                    matrices->E(numbersNodeAndPort[1] + 1) = Zhat*Is;
                    matrices->E(numbersNodeAndPort[1] + 2) = Zhat*Is;
                    matrices->E(numbersNodeAndPort[1] + 3) = Zhat*Is;

                    // matrix E_output
                    matrices->E_output(numbersNodeAndPort[0]) = Zhat*Is;

                    // the center of the tetrahedron
                    matrices->Points_output[numbersNodeAndPort[0]].x = tempVar[9];
                    matrices->Points_output[numbersNodeAndPort[0]].y = tempVar[10];
                    matrices->Points_output[numbersNodeAndPort[0]].z = tempVar[11];

                    // Vi0 = (Ti - ZIS)/sum(tau)
                    // generalization of the initial value
                    matrices->Vi(numbersNodeAndPort[1]) = (input->materialInput[j2].generalized_initialScalar
                            - matrices->E(numbersNodeAndPort[1])) / (tau[0] + tau[1] + tau[2] + tau[3]);
                    matrices->Vi(numbersNodeAndPort[1] + 1) = matrices->Vi(numbersNodeAndPort[1]);
                    matrices->Vi(numbersNodeAndPort[1] + 2) = matrices->Vi(numbersNodeAndPort[1]);
                    matrices->Vi(numbersNodeAndPort[1] + 3) = matrices->Vi(numbersNodeAndPort[1]);

                    goto end_for_j_and_for_k_triangle;
                }
end_for_j_and_for_k_triangle:
        ;
    }
    return 0;
}

/*
 * connectionsAndBoundariesPennesEigen: assumes that the intersection happens between two nodes
 * or between one node and a boundary, only. Intersections with three nodes would break
 * 
 * THIS FUNCTION MAY BE WELL SUITED FOR GOING AT THE LEVEL OF TLM AND NOT BEING SPECIFIC
 * FOR PENNES
 */
unsigned int connectionsAndBoundariesPennesEigen(struct calculationTLMEigen *matrices,
        struct boundaryData* boundaries, struct connectionLeveln *connection,
        struct dataForSimulation *input, int id) {

    unsigned errorTLMnumber = 0, approximationTB = 1, j;
    // approximationTB = 1; calculates TB using Vr
    // approximationTB = 2; calculates TB using Tc
    // tests showed that using Vr gives a better approximation than using Tc
    // I will try to keep both internally just for testing purposes

    unsigned long long i, j1, offset, j2, j3, k, offset_TB, offset_heat_flux;

    offset_TB = matrices->numbers.Nodes * input->equationInput[id].saveScalar;

    offset_heat_flux = offset_TB + matrices->numbers.Intersections * input->equationInput[id].saveScalarBetween;

    unsigned long long *startEnd = NULL;
    startEnd = (unsigned long long *) malloc(sizeof (unsigned long long)*8);
    startEnd[1] = 8;
    // 0 - number of ports saved
    // 1 - number of allocations
    // 2 - number of the port 0
    // 3 - number of the first port of the Node that contains port 0
    // 4 - number of the last port of the Node that contains  port 0
    // 5 - number of the port 1
    // ...

    int j1a;
    double *reflection, *transmission, *B, *transmission_out, *B_out,
            *transmission_out_flux, *B_out_flux;
    // reflection
    // 0: total reflection for port 1
    // 1: total reflection for port 2
    // ...

    // transmission
    // 0: port 1 to port 2
    // 1: port 1 to port 3
    // ...
    // startEnd[0]: port 2 to port 3
    // startEnd[0] + 1: port 2 to port 4
    // ...

    // B:
    // 0: value for port 1
    // 1: value for port 2
    // ...

    // transmission_out(_flux)
    // 0: effect of port 1 on TB (or q'')
    // 1: effect of port 2 on TB (or q'')
    // ...
    // n: effect of port n on TB (or q'')

    // B_out(_flux)
    // 0: constant effect of port 1 on TB (or q'')
    // 1: constant effect of port 2 on TB (or q'')
    // ...
    // n: constant effect of port n on TB (or q'')

    // I will know how many ports by reading startEnd[0]. I will know the length
    // of the vectors reflection and B by using (startEnd[1] - 2)/3
    // the length of transmission by (startEnd[1] - 2)/3*(startEnd[1] - 2)/3
    reflection = (double *) malloc(sizeof (double)*2);
    transmission = (double *) malloc(sizeof (double)*4);
    B = (double *) malloc(sizeof (double)*2);
    transmission_out = (double *) malloc(sizeof (double)*2);
    B_out = (double *) malloc(sizeof (double)*2);
    transmission_out_flux = (double *) malloc(sizeof (double)*2);
    B_out_flux = (double *) malloc(sizeof (double)*2);


    unsigned long long *portsNumbers;
    // this pointer will contain the number of the ports
    // 0: size of the pointer
    // 1: quantity of port numbers
    // 2: number of the ports 0
    // ...
    // n: number of port n - 2
    portsNumbers = (unsigned long long*) malloc(
            sizeof (unsigned long long)*7);
    // here I start getting the number of the ports
    portsNumbers[0] = 7;

    for (i = 0; i < connection->accumulatedIntersections[0]; i++) {
        // DEBUG: show where we are
        //                printf("Initial i% llu", i);
        getPortsOrPoints(connection, i, &portsNumbers);
        // DEBUG: show where we are
        //                 printf("\n");

        // if the quantity of ports is equal to 1, then, this is just
        // adiabatic boundary condition, which means that the reflection
        // coefficient is zero
        if (portsNumbers[1] == 1) {
            // number of ports saved
            startEnd[0] = 1;
            // number of port 1
            getRealPortNumber_fromAbstractPortNumber(portsNumbers[2],
                    matrices->numbers.abstractPortsToReal, startEnd + 2);

            // DEBUG: see the pair of points that are adiabatic
            // I'm seeing the pairs of points.
            // As in C, we start at zero and I start at 1, then, I need
            // to convert, summing 1, to see the real point number
            //            printf("Adiabatic boundary condition by lack. Test Points (%llu)\n", i + 1);

            reflection[0] = 1;
            transmission[0] = 0;
            B[0] = 0;

            if (input->equationInput[id].saveScalarBetween) {
                B_out[0] = 0;
                switch (approximationTB) {
                    case 1: // using Vr
                        transmission_out[0] = 2;
                        break;
                    case 2: // using Tc
                        transmission_out[0] = 1;
                        break;
                }
            }

            if (input->equationInput[id].saveFlux) {
                B_out_flux[0] = 0;
                transmission_out_flux[0] = 0;
            }

        } else {
            // testing if it is a boundary. If any of the ports allocated are equal to zero,
            // then, this has a boundary. If the quantity of ports is 2, then, this is just
            // a boundary condition that is not in contact with any material node. Then, I
            // will do nothing about it. Otherwise, I will work it out
            //
            // By assumption, if j = 0, then the port will be located at j = 2.
            // If j != 0, then the port will be located at j = 0.
            // the maximum value of quantityOfPorts is 3
            for (j = 2; j < (portsNumbers[1] + 2); j++) {
                if (portsNumbers[j] == 0) {
                    if (portsNumbers[1] > 2) {
                        // the number following the first zero, is the number of the boundary.
                        // The number of the boundary can be zero as well

                        // DEBUG: testing the number of the boundary
                        // printf("Test (%llu) number", i);
                        // printf(" number of the boundary %llu\n", portsNumbers[j + 3]);

                        // j1a is the position of the port. j is the position
                        // of the first zero. Assuming that we have only ONE port
                        // connected to this intersection, then, IF j == 2, the
                        // boundary was the first one to be included in the connection
                        // variable. Therefore, j1a is j + 2 ( +1 is the boundary
                        // number and +2 is the port number). ELSE, then j > 2,
                        // which means that the port is in the first position.                        
                        if (j == 2) j1a = j + 2;
                        else j1a = j - 1;

                        reflection[0] = 0;
                        transmission[0] = 0;
                        B[0] = 0;

                        transmission_out[0] = 0;
                        B_out[0] = 0;

                        // by design, the number of boundaries for adiabatic or temperature are only one.
                        switch (boundaries[portsNumbers[j + 1]].boundaries[0].boundaryType) {
                                // this is adiabatic boundary condition
                            case 0:
                                // number of ports saved
                                startEnd[0] = 1;
                                // number of port 0
                                getRealPortNumber_fromAbstractPortNumber(portsNumbers[j1a],
                                        matrices->numbers.abstractPortsToReal, startEnd + 2);

                                // DEBUG: see the pair of points that are adiabatic
                                // I'm seeing the pairs of points.
                                // As in C, we start at zero and I start at 1, then, I need
                                // to convert, summing 1, to see the real point number
                                //                                printf("Adiabatic boundary condition by choice.\n");

                                reflection[0] = 1;
                                //                                transmission[0] = 0; // was defined 0 above
                                //                                B[0] = 0; // was defined 0 above

                                if (input->equationInput[id].saveScalarBetween) {
                                    B_out[0] = 0;
                                    switch (approximationTB) {
                                        case 1: // using Vr
                                            transmission_out[0] = 2;
                                            break;
                                        case 2: // using Tc
                                            transmission_out[0] = 1;
                                            break;
                                    }
                                }

                                if (input->equationInput[id].saveFlux) {
                                    B_out_flux[0] = 0;
                                    transmission_out_flux[0] = 0;
                                }

                                break;

                                // this is temperature boundary condition
                            case 1:
                                // number of ports saved
                                startEnd[0] = 1;

                                getRealPortNumber_fromAbstractPortNumber(portsNumbers[j1a],
                                        matrices->numbers.abstractPortsToReal, startEnd + 2);

                                // DEBUG: see the pair of points that are adiabatic
                                // I'm seeing the pairs of points.
                                // As in C, we start at zero and I start at 1, then, I need
                                // to convert, summing 1, to see the real point number
                                //                                printf("Temperature boundary condition.\n");

                                reflection[0] = (matrices->R[startEnd[2]] - matrices->Z[startEnd[2]]) /
                                        (matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]);

                                //                                transmission[0] = 0; // was defined 0 above

                                B[0] = boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0]
                                        * matrices->Z[startEnd[2]] /
                                        (matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]);

                                // DEBUG: print R, Z and reflection
                                //                                        printf("R (%e) Z(%e) reflection (%e)\n",
                                //                                                matrices->R[startEnd[2]],
                                //                                                matrices->Z[startEnd[2]],
                                //                                                reflection[0]);

                                if (input->equationInput[id].saveScalarBetween) {
                                    B_out[0] = boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0];
                                    switch (approximationTB) {
                                        case 1: // using Vr
                                            transmission_out[0] = 0;
                                            break;
                                        case 2: // using Tc
                                            transmission_out[0] = 0;
                                            break;
                                    }
                                }

                                if (input->equationInput[id].saveFlux) {
                                    // from port 0 to outside
                                    transmission_out_flux[0] = 2 / (
                                            (matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]) *
                                            matrices->L[startEnd[2]]);


                                    B_out_flux[0] = -1 * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0] *
                                            transmission_out_flux[0] / 2;


                                }

                                break;


                                // heat flux boundary condition
                            case 2:

                                //                                printf("Heat flux boundary condition.\n");

                                // number of ports saved
                                startEnd[0] = 1;
                                // number of port 0
                                getRealPortNumber_fromAbstractPortNumber(portsNumbers[j1a],
                                        matrices->numbers.abstractPortsToReal, startEnd + 2);

                                reflection[0] = 1;

                                B[0] = boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0] // q''
                                        * matrices->Z[startEnd[2]]
                                        * matrices->L[startEnd[2]];


                                if (input->equationInput[id].saveScalarBetween) {
                                    switch (approximationTB) {
                                        case 1: // using Vr
                                            // effect of the heat flux on Tp
                                            B_out[0] = (matrices->Z[startEnd[2]] + matrices->R[startEnd[2]])
                                                    * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0]
                                                    * matrices->L[startEnd[2]];
                                            // effect of Vr on Tp
                                            transmission_out[0] = 2;
                                            break;
                                        case 2: // using Tc
                                            // effect of the heat flux on Tp
                                            B_out[0] = boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0] // q''
                                                    * matrices->R[startEnd[2]]
                                                    * matrices->L[startEnd[2]];
                                            // effect of Tc on Tp
                                            transmission_out[0] = 1;
                                            break;
                                    }
                                }

                                if (input->equationInput[id].saveFlux) {
                                    // from inside to outside. Positive values for 
                                    // heat flux input are towards inside, hence
                                    // that's why -1
                                    B_out_flux[0] = -1 * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0];

                                    transmission_out_flux[0] = 0;
                                }

                                break;

                                // convection heat flux boundary condition
                            case 3:
                                // number of ports saved
                                startEnd[0] = 1;
                                // number of port 0
                                getRealPortNumber_fromAbstractPortNumber(portsNumbers[j1a],
                                        matrices->numbers.abstractPortsToReal, startEnd + 2);

                                double Rc;

                                Rc = 1 / (boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[1]
                                        * matrices->L[startEnd[2]]);

                                reflection[0] = (Rc + matrices->R[startEnd[2]] - matrices->Z[startEnd[2]]) /
                                        (Rc + matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]);

                                B[0] = boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0] // Tc
                                        * matrices->Z[startEnd[2]] /
                                        (Rc + matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]);



                                if (input->equationInput[id].saveScalarBetween) {
                                    switch (approximationTB) {
                                        case 1: // using Vr
                                            B_out[0] = (matrices->Z[startEnd[2]] + matrices->R[startEnd[2]]) /
                                                    (matrices->Z[startEnd[2]] + matrices->R[startEnd[2]] + Rc)
                                                    * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0];

                                            transmission_out[0] = 2 * Rc /
                                                    (matrices->Z[startEnd[2]] + matrices->R[startEnd[2]] + Rc);
                                            break;
                                        case 2: // using Tc
                                            B_out[0] = matrices->R[startEnd[2]] /
                                                    (matrices->R[startEnd[2]] + Rc)
                                                    * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0];


                                            transmission_out[0] = Rc /
                                                    (matrices->R[startEnd[2]] + Rc);
                                            break;
                                    }
                                }

                                if (input->equationInput[id].saveFlux) {
                                    transmission_out_flux[0] = 2 / (
                                            (matrices->R[startEnd[2]] + matrices->Z[startEnd[2]] + Rc) *
                                            matrices->L[startEnd[2]]);

                                    B_out_flux[0] = -1 * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0] *
                                            transmission_out_flux[0] / 2;


                                }

                                break;

                                // radiation heat flux boundary condition
                            case 4:
                                //                                printf("Radiation boundary condition.\n");
                                // future implementation
                                break;
                        }
                        // get out of the for loop that verifies if it is a boundary
                        goto transmission_and_reflection;
                    }
                }
            }

            // if the for loop did not find a boundary condition, then,
            // my assumption is that this is between two ports
            // number of ports saved
            startEnd[0] = 2;
            // number of port 0
            getRealPortNumber_fromAbstractPortNumber(portsNumbers[2],
                    matrices->numbers.abstractPortsToReal, startEnd + 2);

            // number of port 1
            getRealPortNumber_fromAbstractPortNumber(portsNumbers[3],
                    matrices->numbers.abstractPortsToReal, startEnd + 5);

            // DEBUG: see the pair of points that are adiabatic
            // I'm seeing the pairs of points.
            // As in C, we start at zero and I start at 1, then, I need
            // to convert, summing 1, to see the real point number
            //                    printf("Between two ports (%llu, %llu). Test Points (%llu, %llu)\n",
            //                            startEnd[2], startEnd[5], i + 1, k + 1);

            reflection[0] = matrices->R[startEnd[2]] + matrices->Z[startEnd[2]] +
                    matrices->R[startEnd[5]] + matrices->Z[startEnd[5]];
            reflection[1] = 1 - 2 * matrices->Z[startEnd[5]] / reflection[0];
            reflection[0] = 1 - 2 * matrices->Z[startEnd[2]] / reflection[0];

            // effect of port 0 in port 1
            transmission[0] = 1 - reflection[1];
            // effect of port 1 in port 0
            transmission[1] = 1 - reflection[0];

            // DEBUG: print reflection and transmission coefficients
            //                    printf("reflection (%e, %e). Transmission (%e, %e)\n",
            //                            reflection[0], reflection[1], transmission[0], transmission[1]);

            B[0] = 0;
            B[1] = 0;

            if (input->equationInput[id].saveScalarBetween) {
                B_out[0] = 0;
                B_out[1] = 0;
                switch (approximationTB) {
                    case 1: // using Vr
                        transmission_out[0] = 2 * (matrices->R[startEnd[5]] + matrices->Z[startEnd[5]]) / (
                                matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]
                                + matrices->R[startEnd[5]] + matrices->Z[startEnd[5]]);

                        transmission_out[1] = 2 * (matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]) / (
                                matrices->R[startEnd[2]] + matrices->Z[startEnd[2]]
                                + matrices->R[startEnd[5]] + matrices->Z[startEnd[5]]);
                        break;
                    case 2: // using Tc
                        transmission_out[0] = (matrices->R[startEnd[5]]) / (
                                matrices->R[startEnd[2]] + matrices->R[startEnd[5]]);

                        transmission_out[1] = (matrices->R[startEnd[2]]) / (
                                matrices->R[startEnd[2]] + matrices->R[startEnd[5]]);
                        break;
                }
            }

            if (input->equationInput[id].saveFlux) {
                B_out_flux[0] = 0;
                B_out_flux[1] = 0;

                transmission_out_flux[0] = 2 / (
                        (matrices->R[startEnd[2]] + matrices->Z[startEnd[2]] +
                        matrices->R[startEnd[5]] + matrices->Z[startEnd[5]]) *
                        matrices->L[startEnd[2]]);

                transmission_out_flux[1] = -transmission_out_flux[0];
            }

        }



transmission_and_reflection:
        ; // trick to write something after the goto statement
        for (j1 = 0; j1 < startEnd[0]; j1++) {
            offset = j1 * startEnd[0];

            if (j1 == 0 && (input->equationInput[id].saveScalarBetween ||
                    input->equationInput[id].saveFlux)) {
                // I'm getting the positions of the points "in between" two nodes. I can
                // calculate the temperature on these points.
                getBetweenPointFromRealPortNumber(matrices->numbers.abstractPortsToReal,
                        startEnd[2],
                        &(matrices->Points_output[offset_TB + i].x),
                        &(matrices->Points_output[offset_TB + i].y),
                        &(matrices->Points_output[offset_TB + i].z),
                        input);

                if (input->equationInput[id].saveFlux) {
                    // I should answer why do I need this...
                    getProjectionFromRealPortNumber(matrices->numbers.abstractPortsToReal,
                            startEnd[2],
                            &(matrices->Points_output[offset_heat_flux + i].x),
                            &(matrices->Points_output[offset_heat_flux + i].y),
                            &(matrices->Points_output[offset_heat_flux + i].z),
                            input);
                    //                    printf("The output outside: %f\n", matrices->Points_output[offset_heat_flux + i].x*matrices->Points_output[offset_heat_flux + i].x + matrices->Points_output[offset_heat_flux + i].y*matrices->Points_output[offset_heat_flux + i].y + matrices->Points_output[offset_heat_flux + i].z*matrices->Points_output[offset_heat_flux + i].z);

                }
            }

            if (input->equationInput[id].saveScalarBetween) {
                matrices->E_output(offset_TB + i) = matrices->E_output(offset_TB + i)
                        + transmission_out[j1] * matrices->E(startEnd[j1 * 3 + 2])
                        + B_out[j1];

                switch (approximationTB) {
                    case 1: // using Vr
                        // transmission effects of port j1 into TB
                        for (j2 = startEnd[j1 * 3 + 3]; j2 < startEnd[j1 * 3 + 4]; j2++) {
                            matrices->tau.insert(offset_TB + i, j2) =
                                    transmission_out[j1] * matrices->M.coeff(startEnd[j1 * 3 + 2], j2);
                        }
                        break;
                    case 2: // using Tc
                        for (j2 = startEnd[j1 * 3 + 3]; j2 < startEnd[j1 * 3 + 4]; j2++) {

                            if (startEnd[j1 * 3 + 2] == j2) {
                                if (j2 == startEnd[j1 * 3 + 3]) {
                                    matrices->tau.insert(offset_TB + i, j2) =
                                            transmission_out[j1] * matrices->M.coeff(j2 + 1, j2);
                                } else {
                                    matrices->tau.insert(offset_TB + i, j2) =
                                            transmission_out[j1] * matrices->M.coeff(j2 - 1, j2);
                                }
                            } else {
                                matrices->tau.insert(offset_TB + i, j2) =
                                        transmission_out[j1] * matrices->M.coeff(startEnd[j1 * 3 + 2], j2);
                            }

                        }
                        break;
                }
            }



            if (input->equationInput[id].saveFlux) {
                matrices->E_output(offset_heat_flux + i) = matrices->E_output(offset_heat_flux + i)
                        + transmission_out_flux[j1] * matrices->E(startEnd[j1 * 3 + 2])
                        + B_out_flux[j1];

                for (j2 = startEnd[j1 * 3 + 3]; j2 < startEnd[j1 * 3 + 4]; j2++) {
                    matrices->tau.insert(offset_heat_flux + i, j2) =
                            transmission_out_flux[j1] * matrices->M.coeff(startEnd[j1 * 3 + 2], j2);
                }
            }


            // going to port 2, port 3...
            // startEnd[j3*3 + 2] - 1 - number of port j3 - 1
            // startEnd[j1*3 + 2] - 1 - number of port j1 - 1
            for (j3 = j1 + 1; j3 < startEnd[0]; j3++) {
                // transmission effects of port j1 into port j3
                for (j2 = startEnd[j1 * 3 + 3]; j2 < startEnd[j1 * 3 + 4]; j2++) {
                    matrices->M.insert(startEnd[j3 * 3 + 2], j2) =
                            transmission[offset] * matrices->M.coeff(startEnd[j1 * 3 + 2], j2);
                }

                // transmission effects of port j3 into port j1
                for (j2 = startEnd[j3 * 3 + 3]; j2 < startEnd[j3 * 3 + 4]; j2++) {
                    matrices->M.insert(startEnd[j1 * 3 + 2], j2) =
                            transmission[offset + (startEnd[0] - 1) * j3] * matrices->M.coeff(startEnd[j3 * 3 + 2], j2);
                }

                // transmission effects of port j3 into port j1
                B[j1] = B[j1] +
                        transmission[offset + (startEnd[0] - 1) * j3] * matrices->E(startEnd[j3 * 3 + 2]);
                // transmission effects of port j1 into port j3
                B[j3] = B[j3] +
                        transmission[offset] * matrices->E(startEnd[j1 * 3 + 2]);
                offset++;
            }
        }

        // reflection effects
        for (j1 = 0; j1 < startEnd[0]; j1++) {
            for (j2 = startEnd[j1 * 3 + 3]; j2 < startEnd[j1 * 3 + 4]; j2++) {
                matrices->M.coeffRef(startEnd[j1 * 3 + 2], j2) =
                        reflection[j1] * matrices->M.coeff(startEnd[j1 * 3 + 2], j2);
            }
            matrices->E(startEnd[j1 * 3 + 2]) =
                    reflection[j1] * matrices->E(startEnd[j1 * 3 + 2])
                    + B[j1];
        }
    }

    matrices->M.makeCompressed();
    matrices->tau.makeCompressed();

    free(startEnd);
    startEnd = NULL;

    return 0;
}