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

#include "libtlmpenneseigen.h"
#include <stdlib.h>

#include "../../../miscellaneous/liberrorcode.h"

/*
 * calculateMatricesEigen: Calculate the matrixes M and E, and the supporting matrices
 * 
 */
unsigned int calculateMatricesPennesEigen(struct dataForSimulation *input,
        struct calculationTLMEigen *matrices, struct boundaryData** boundaries,
        struct connectionLeveln *intersections, FILE *file) {

    if (input->timingMode == 1) {
        printf("\n\nCalculating matrices...\n");
    }

    unsigned int errorTLMnumber;
    clock_t begin = clock();

    clock_t begin_Line = clock();
    // getting the boundaries from the line as a boundary
    if (matrices->numbers.MaterialElements[1])
        if ((errorTLMnumber = MaterialLinePennesEigen(input, matrices)) != 0) {
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
            return errorTLMnumber;
        }
    clock_t end_Line = clock();

    clock_t begin_Triangle = clock();
    if (matrices->numbers.MaterialElements[2])
        if ((errorTLMnumber = MaterialTrianglePennesEigen(input, matrices)) != 0) {
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
            return errorTLMnumber;
        }
    clock_t end_Triangle = clock();
    
    clock_t begin_Tetrahedron = clock();
    if (matrices->numbers.MaterialElements[4])
        if ((errorTLMnumber = MaterialTetrahedronPennesEigen(input, matrices)) != 0) {
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
            return errorTLMnumber;
        }
    clock_t end_Tetrahedron = clock();
    
    // DEBUG: Show how many elements were found
//    printf("Line %llu, Triangle %llu, Tetrahedron %llu\n",
//            matrices->numbers.MaterialElements[1], matrices->numbers.MaterialElements[2],
//            matrices->numbers.MaterialElements[4]);

    // DEBUG: test the matrix M
    // long long unsigned int Node = 1;
    // printf("Scattering matrix for node %llu\n", Node);
    // for (unsigned int i = (Node - 1)*3; i < Node * 3; i++) {
    //     for (unsigned int j = (Node - 1)*3; j < Node * 3; j++) {
    //         printf(" %e ", calcs.M[i][j]);
    //     }
    //     printf("\n");
    // }

    clock_t begin_CB = clock();
    if ((errorTLMnumber = connectionsAndBoundariesPennesEigen(matrices, *boundaries,
            intersections, input)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    clock_t end_CB = clock();


    //    // DEBUG: tests if the data was written into the intersection variable
    //    long long unsigned int N1 = 9, N2 = 10;
    //    
    //    printf("Writing test inside solverTLMPennes2DDynamic (%llu, %llu) (%llu", N1, N2, intersections.connections[N1][N2].ports[0]);
    //     for (unsigned int i = 1; i < intersections.connections[N1][N2].quantityOfPorts; i++){
    //        printf(" and %llu",intersections.connections[N1][N2].ports[i]);
    //     }
    //     printf(") total (%u)\n", intersections.connections[N1][N2].quantityOfPorts);


    clock_t begin_wr = clock();
    if ((errorTLMnumber = writeFirstDataToFileEigen(matrices, input, file)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    clock_t end_wr = clock();
    
    clock_t begin_ss = clock();
    if (input->simulationInput.Solv == STEADY){
        if ( (errorTLMnumber = tlmInitializeSteadyMatrices(matrices)) == 0 ){
            return errorTLMnumber;
        }
    }
    clock_t end_ss = clock();


    // this timing appear first because I clear some variables later.
    if (input->timingMode == 1) {
        double time_spent_Line = (double) (end_Line - begin_Line) / CLOCKS_PER_SEC;
        double time_spent_Triangle = (double) (end_Triangle - begin_Triangle) / CLOCKS_PER_SEC;
        double time_spent_Tetrahedron = (double) (end_Tetrahedron - begin_Tetrahedron) / CLOCKS_PER_SEC;
        double time_spent_SteadyState = (double) (end_ss - begin_ss) / CLOCKS_PER_SEC;

        if (matrices->numbers.MaterialElements[1] != 0) // line
            printf("Time to calculate the matrices for the %llu line nodes %g ms (or %g s, or %g min, or %g hours).\n",
                matrices->numbers.MaterialElements[1], time_spent_Line * 1e3, time_spent_Line, time_spent_Line / 60.0, time_spent_Line / (60 * 60));

        if (matrices->numbers.MaterialElements[2] != 0) // triangle
            printf("Time to calculate the matrices for the %llu triangle nodes %g ms (or %g s, or %g min, or %g hours).\n",
                matrices->numbers.MaterialElements[2], time_spent_Triangle * 1e3, time_spent_Triangle, time_spent_Triangle / 60.0, time_spent_Triangle / (60 * 60));

        if (matrices->numbers.MaterialElements[4] != 0) // tetrahedron
            printf("Time to calculate the matrices for the %llu tetrahedral nodes %g ms (or %g s, or %g min, or %g hours).\n",
                matrices->numbers.MaterialElements[4], time_spent_Tetrahedron * 1e3, time_spent_Tetrahedron, time_spent_Tetrahedron / 60.0, time_spent_Tetrahedron / (60 * 60));
        
        if (input->simulationInput.Solv == STEADY)
            printf("Time to calculate the steady-state matrices %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_SteadyState * 1e3, time_spent_SteadyState, time_spent_SteadyState / 60.0, time_spent_SteadyState / (60 * 60));
    
    }

    clock_t begin_tr = clock();
    if ((errorTLMnumber = terminateSomeVariablesEigen(input, matrices, boundaries, intersections)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    clock_t end_tr = clock();

    clock_t end = clock();


    if (input->timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        double time_spent_CB = (double) (end_CB - begin_CB) / CLOCKS_PER_SEC;
        double time_spent_wr = (double) (end_wr - begin_wr) / CLOCKS_PER_SEC;
        double time_spent_tr = (double) (end_tr - begin_tr) / CLOCKS_PER_SEC;

        printf("Time to calculate for connections and boundaries %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_CB * 1e3, time_spent_CB, time_spent_CB / 60.0, time_spent_CB / (60 * 60));

        printf("Time to write data to file %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_wr * 1e3, time_spent_wr, time_spent_wr / 60.0, time_spent_wr / (60 * 60));

        printf("Time to deallocate some variables %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent_tr * 1e3, time_spent_tr, time_spent_tr / 60.0, time_spent_tr / (60 * 60));

        printf("Total time to calculate the matrices %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }
    return 0;
}

/*
 * MaterialLineEigen: 
 * Line as a material was not implemented yet.
 */
unsigned int MaterialLinePennesEigen(struct dataForSimulation *input, struct calculationTLMEigen *matrices) {
    unsigned int errorTLMnumber;
    // line nomenclature
    //
    //   vertex 1  \
    //              \
    //               \    face
    //                \
    //                 \  vertex 2
    // element code 1: 2 nodes line. Used only for boundary

    // contains the values of two ports
    long long unsigned int temp[] = {0, 0};
    for (long long unsigned int i = 0; i < input->mesh.quantityOfSpecificElement[1]; i++) {
        // i: element level
        // j: boundary type level
        // k: different tag numbers for the same boundary type
        for (unsigned int j = 0; j < input->quantityOfMaterialsRead; j++)
            for (unsigned int k = 0; k < input->materialInput[j].quantityOfNumberInput; k++)
                if (input->mesh.elements.Line[i].tag == input->materialInput[j].numberInput[k]) {
                    // I will use goto to get out of these two inner 'for' loops. Bare in mind
                    // that I'm just getting out the loops. You can also think that 
                    // I'm going to the next 'i', that is, incrementing i by 1 and going
                    // to the next value of i (if any).

                    printf("ERROR: Line element as material. This is not supposed to happen.\n");

                    // DEBUG: show where we were
                    // printf("We were at (%llu, %u, %u)\n", i, j, k);

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
unsigned int MaterialTrianglePennesEigen(struct dataForSimulation *input, struct calculationTLMEigen *matrices) {
    unsigned int errorTLMnumber;
    long long unsigned int numbersNodeAndPort[] = {0, 0, 0};
    // 0 - number of node
    // 1 - number of port 0
    // 2 - number of output. // future implementation



    // triangular nomenclature.
    //              vertex 1
    //              /\
    //     face 1  /  \   face 2
    //            /    \
    //           /      \
    // vertex 2 /________\ vertex 3
    //            face 3

    // triangles can be boundary conditions and mediums. The boundary conditions
    // version was not implemented yet.   
    // element code 3: 3 nodes triangle. Used only for medium only. The boundary
    // condition version was not implemented yet
    double *tempVar = NULL;
    tempVar = (double *) realloc(tempVar, sizeof (double)*13);
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
    // 10 - perpendicular length of port 1 
    // 11 - perpendicular length of port 2 
    // 12 - perpendicular length of port 3 
    
    for (long long unsigned int i = 0; i < input->mesh.quantityOfSpecificElement[2]; i++) {
        // i: element level
        // j: boundary type level
        // k: different tag numbers for the same boundary type
        for (unsigned int j = 0; j < input->quantityOfMaterialsRead; j++)
            for (unsigned int k = 0; k < input->materialInput[j].quantityOfNumberInput; k++)
                if (input->mesh.elements.Triangle[i].tag == input->materialInput[j].numberInput[k]) {
                    // I will use goto to get out of these two inner 'for' loops. Bare in mind
                    // that I'm just getting out the loops. You can also think that 
                    // I'm going to the next 'i', that is, incrementing i by 1 and going
                    // to the next value of i (if any).


                    // DEBUG: show where we are
                    //                    printf("Triangular element number %llu, total number of triangles"
                    //                            " %llu, points numbers (%llu, %llu, %llu)\n", i + 1,
                    //                            input->mesh.quantityOfSpecificElement[2], input->mesh.elements.Triangle[i].N1,
                    //                            input->mesh.elements.Triangle[i].N2, input->mesh.elements.Triangle[i].N3);

                    // the ports numbering for tlmbht matlab 0.1 is different from
                    // what was used for this algorithm
                    // In matlab, I numbered the ports as:
                    // port 1_old: between point 2 and 3
                    // port 2_old: between point 1 and 3
                    // port 3_old: between point 1 and 2
                    // 
                    // Here, the numbers are:
                    // port 1: between point 1 and 2
                    // port 2: between point 1 and 3
                    // port 3: between point 2 and 3
                    // 
                    // Therefore:
                    // port 1_old: port 3
                    // port 2_old: port 2
                    // port 3_old: port 1
                    //
                    // to compare between these two algorithms, I managed to change
                    // the port numbering in some DEBUG code below.

                    getRealNodeAndPort_fromAbstractNode(2, // element code
                            i, // element number
                            matrices->numbers.abstractPortsToReal,
                            numbersNodeAndPort);

                    //                    countingNumberOfPorts = countingNumberOfPorts + 1; // port 1


                    // DEBUG: (1 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts + 2;

                    // DEBUG: show where we are
                    //                    printf("Points 1 and 2 (port %llu)\n", countingNumberOfPorts);


                    //                    if ((errorTLMnumber = allocatePointsPort(input->mesh.elements.Triangle[i].N1,
                    //                            input->mesh.elements.Triangle[i].N2, intersections,
                    //                            1, &countingNumberOfPorts)) != 0) {
                    //                        return errorTLMnumber;
                    //                    }



                    //                    countingNumberOfPorts = countingNumberOfPorts + 1; // port 2


                    // DEBUG: (2 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts - 2;

                    // DEBUG: show where we are
                    //                    printf("Points 1 and 3 (port %llu)\n", countingNumberOfPorts);


                    //                    if ((errorTLMnumber = allocatePointsPort(input->mesh.elements.Triangle[i].N1,
                    //                            input->mesh.elements.Triangle[i].N3, intersections,
                    //                            1, &countingNumberOfPorts)) != 0) {
                    //                        return errorTLMnumber;
                    //                    }


                    //                    countingNumberOfPorts = countingNumberOfPorts + 1; // port 3


                    // DEBUG: (3 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts - 2;

                    // DEBUG: show where we are
                    //                    printf("Points 2 and 3 (port %llu)\n", countingNumberOfPorts);


                    //                    if ((errorTLMnumber = allocatePointsPort(input->mesh.elements.Triangle[i].N2,
                    //                            input->mesh.elements.Triangle[i].N3, intersections,
                    //                            1, &countingNumberOfPorts)) != 0) {
                    //                        return errorTLMnumber;
                    //                    }

                    // DEBUG: (4 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts + 2;

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

                    // DEBUG: print hte tempVar
                    //                    printf("tempVar variable (%e, %e, %e, %e, %e, %e, %e)\n",
                    //                            tempVar[0], tempVar[1], tempVar[2], tempVar[3], tempVar[4],
                    //                            tempVar[5], tempVar[6]);

                    double Cap, Z[3], R[3], G, Is, Zhat, tau[3];

                    // C = A*ro*cp/sum(deltal). Manually validated
                    Cap = tempVar[3] * input->materialInput[j].matDensity *
                            input->materialInput[j].matSpecificHeat / (
                            tempVar[0] + tempVar[1] + tempVar[2]);

                    // Z = dt/(2*C*deltal). Manually validated
                    Z[0] = input->simulationInput.timeStep / (2 * Cap);
                    Z[1] = Z[0] / tempVar[1];
                    Z[2] = Z[0] / tempVar[2];
                    Z[0] = Z[0] / tempVar[0];

                    // all the impedances
                    matrices->Z[numbersNodeAndPort[1] + 0] = Z[0];
                    matrices->Z[numbersNodeAndPort[1] + 1] = Z[1];
                    matrices->Z[numbersNodeAndPort[1] + 2] = Z[2];


                    // R = deltal/(k*L).
                    R[0] = tempVar[0] / (input->materialInput[j].matThermalConductivity * tempVar[4]);
                    R[1] = tempVar[1] / (input->materialInput[j].matThermalConductivity * tempVar[5]);
                    R[2] = tempVar[2] / (input->materialInput[j].matThermalConductivity * tempVar[6]);
                    // R = deltal*cos(theta)/(k*L). Manually validated. This approach didn't improve the
                    // for the tetrahedron node. That's why I'm keeping the original one
//                    R[0] = tempVar[10] / (input->materialInput[j].matThermalConductivity * tempVar[4]);
//                    R[1] = tempVar[11] / (input->materialInput[j].matThermalConductivity * tempVar[5]);
//                    R[2] = tempVar[12] / (input->materialInput[j].matThermalConductivity * tempVar[6]);

                    // all the resistances
                    matrices->R[numbersNodeAndPort[1] + 0] = R[0];
                    matrices->R[numbersNodeAndPort[1] + 1] = R[1];
                    matrices->R[numbersNodeAndPort[1] + 2] = R[2];

                    // G = A*wb*pb*cb. Manually validated
                    G = tempVar[3] * input->materialInput[j].bloodPerfusion
                            * input->materialInput[j].bloodDensity
                            * input->materialInput[j].bloodSpecificHeat;


                    // Is = A*(Qmet + wb*pb*cb*Tb). Manually validated
                    Is = tempVar[3]*(input->materialInput[j].internalHeatGeneration
                            + input->materialInput[j].bloodPerfusion
                            * input->materialInput[j].bloodDensity
                            * input->materialInput[j].bloodSpecificHeat
                            * input->materialInput[j].bloodTemperature);

                    // Manually validated
                    Zhat = Z[0] * Z[1] * Z[2] / (Z[0] * Z[1] + Z[0] * Z[2] + Z[1] * Z[2]
                            + Z[0] * Z[1] * Z[2] * G);

                    // Manually validated
                    tau[0] = 2 * Zhat / Z[0];
                    tau[1] = 2 * Zhat / Z[1];
                    tau[2] = 2 * Zhat / Z[2];

                    // DEBUG: validate Cap, Zs, Rs, G, Is, Zhat, taus
                    // printf("Cap %f, Z0 %e, Z1 %e, Z2 %e, R0 %e, R1 %e, R2 %e\n"
                    //        "G %e, Is %e, Zhat %e, tau0 %e, tau1 %e, tau2 %e\n",
                    //        Cap, Z[0], Z[1], Z[2], R[0], R[1], R[2], G, Is, Zhat,
                    //        tau[0], tau[1], tau[2]);


                    // S( nPortsAcu:nPortsAcuEnd, nPortsAcu:nPortsAcuEnd ) = ...
                    //  [ ( tau(1) - 1 ),     tau(2),             tau(3); ...
                    //  tau(1),         ( tau(2) - 1 ),       tau(3); ...
                    //  tau(1),           tau(2),           ( tau(3) - 1 )];

                    // this is actually matrix S.
                    // M = C*S
                    // this matrix is M[line][column]. May be, it would worth to do
                    // M[column][line] in order to use some LAPACK package.
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
                    matrices->Vi(numbersNodeAndPort[1] + 0) = (input->materialInput[j].initialTemperature
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
unsigned int MaterialTetrahedronPennesEigen(struct dataForSimulation *input, struct calculationTLMEigen *matrices) {
    unsigned int errorTLMnumber;
    long long unsigned int numbersNodeAndPort[] = {0, 0, 0};
    // 0 - number of node
    // 1 - number of port 0
    // 2 - number of output. // future implementation



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
    tempVar = (double *) realloc(tempVar, sizeof (double)*16);
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
    // 12 - perpendicular length of port 1
    // 13 - perpendicular length of port 2
    // 14 - perpendicular length of port 3
    // 15 - perpendicular length of port 4 
    for (long long unsigned int i = 0; i < input->mesh.quantityOfSpecificElement[4]; i++) {
        // i: element level
        // j: boundary type level
        // k: different tag numbers for the same boundary type
        for (unsigned int j = 0; j < input->quantityOfMaterialsRead; j++)
            for (unsigned int k = 0; k < input->materialInput[j].quantityOfNumberInput; k++)
                if (input->mesh.elements.Tetrahedron[i].tag == input->materialInput[j].numberInput[k]) {
                    // I will use goto to get out of these two inner 'for' loops. Bare in mind
                    // that I'm just getting out the loops. You can also think that 
                    // I'm going to the next 'i', that is, incrementing i by 1 and going
                    // to the next value of i (if any).


                    // DEBUG: show where we are
                    //                    printf("Triangular element number %llu, total number of triangles"
                    //                            " %llu, points numbers (%llu, %llu, %llu)\n", i + 1,
                    //                            input->mesh.quantityOfSpecificElement[2], input->mesh.elements.Triangle[i].N1,
                    //                            input->mesh.elements.Triangle[i].N2, input->mesh.elements.Triangle[i].N3);

                    // the ports numbering for tlmbht matlab 0.1 is different from
                    // what was used for this algorithm
                    // In matlab, I numbered the ports as:
                    // port 1_old: between point 2 and 3
                    // port 2_old: between point 1 and 3
                    // port 3_old: between point 1 and 2
                    // 
                    // Here, the numbers are:
                    // port 1: between point 1 and 2
                    // port 2: between point 1 and 3
                    // port 3: between point 2 and 3
                    // 
                    // Therefore:
                    // port 1_old: port 3
                    // port 2_old: port 2
                    // port 3_old: port 1
                    //
                    // to compare between these two algorithms, I managed to change
                    // the port numbering in some DEBUG code below.

                    getRealNodeAndPort_fromAbstractNode(4, // element code
                            i, // element number
                            matrices->numbers.abstractPortsToReal,
                            numbersNodeAndPort);

                    //                    countingNumberOfPorts = countingNumberOfPorts + 1; // port 1


                    // DEBUG: (1 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts + 2;

                    // DEBUG: show where we are
                    //                    printf("Points 1 and 2 (port %llu)\n", countingNumberOfPorts);


                    //                    if ((errorTLMnumber = allocatePointsPort(input->mesh.elements.Triangle[i].N1,
                    //                            input->mesh.elements.Triangle[i].N2, intersections,
                    //                            1, &countingNumberOfPorts)) != 0) {
                    //                        return errorTLMnumber;
                    //                    }



                    //                    countingNumberOfPorts = countingNumberOfPorts + 1; // port 2


                    // DEBUG: (2 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts - 2;

                    // DEBUG: show where we are
                    //                    printf("Points 1 and 3 (port %llu)\n", countingNumberOfPorts);


                    //                    if ((errorTLMnumber = allocatePointsPort(input->mesh.elements.Triangle[i].N1,
                    //                            input->mesh.elements.Triangle[i].N3, intersections,
                    //                            1, &countingNumberOfPorts)) != 0) {
                    //                        return errorTLMnumber;
                    //                    }


                    //                    countingNumberOfPorts = countingNumberOfPorts + 1; // port 3


                    // DEBUG: (3 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts - 2;

                    // DEBUG: show where we are
                    //                    printf("Points 2 and 3 (port %llu)\n", countingNumberOfPorts);


                    //                    if ((errorTLMnumber = allocatePointsPort(input->mesh.elements.Triangle[i].N2,
                    //                            input->mesh.elements.Triangle[i].N3, intersections,
                    //                            1, &countingNumberOfPorts)) != 0) {
                    //                        return errorTLMnumber;
                    //                    }

                    // DEBUG: (4 of 4) make the port number as tlmbht matlab 0.1
                    //                    countingNumberOfPorts = countingNumberOfPorts + 2;

                    // the -1 is necessary because the C indexing starts at zero
                    // and my number of node starts at 1                    
                    getGeometricalVariablesTLMtetrahedron(&input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N1 - 1],
                            &input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N2 - 1],
                            &input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N3 - 1],
                            &input->mesh.nodes[input->mesh.elements.Tetrahedron[i].N4 - 1],tempVar);

                    matrices->L[numbersNodeAndPort[1] + 0] = tempVar[4]; // area of face 1
                    matrices->L[numbersNodeAndPort[1] + 1] = tempVar[5]; // area of face 2
                    matrices->L[numbersNodeAndPort[1] + 2] = tempVar[6]; // area of face 3
                    matrices->L[numbersNodeAndPort[1] + 3] = tempVar[7]; // area of face 4

                    matrices->deltal[numbersNodeAndPort[1] + 0] = tempVar[0]; // length of port 1
                    matrices->deltal[numbersNodeAndPort[1] + 1] = tempVar[1]; // length of port 2
                    matrices->deltal[numbersNodeAndPort[1] + 2] = tempVar[2]; // length of port 3
                    matrices->deltal[numbersNodeAndPort[1] + 3] = tempVar[3]; // length of port 4

                    // DEBUG: print hte tempVar
                    //                    printf("tempVar variable (%e, %e, %e, %e, %e, %e, %e)\n",
                    //                            tempVar[0], tempVar[1], tempVar[2], tempVar[3], tempVar[4],
                    //                            tempVar[5], tempVar[6]);

                    double Cap, Z[4], R[4], G, Is, Zhat, tau[4];

                    // C = A*ro*cp/sum(deltal). Manually validated
                    Cap = tempVar[8] * input->materialInput[j].matDensity *
                            input->materialInput[j].matSpecificHeat / (
                            tempVar[0] + tempVar[1] + tempVar[2] + tempVar[3]);

                    // Z = dt/(2*C*deltal). Manually validated
                    Z[0] = input->simulationInput.timeStep / (2 * Cap);
                    Z[1] = Z[0] / tempVar[1];
                    Z[2] = Z[0] / tempVar[2];
                    Z[3] = Z[0] / tempVar[3];
                    Z[0] = Z[0] / tempVar[0];

                    // all the impedances
                    matrices->Z[numbersNodeAndPort[1] + 0] = Z[0];
                    matrices->Z[numbersNodeAndPort[1] + 1] = Z[1];
                    matrices->Z[numbersNodeAndPort[1] + 2] = Z[2];
                    matrices->Z[numbersNodeAndPort[1] + 3] = Z[3];


                    // R = deltal/(k*L). Manually validated
                    R[0] = tempVar[0] / (input->materialInput[j].matThermalConductivity * tempVar[4]);
                    R[1] = tempVar[1] / (input->materialInput[j].matThermalConductivity * tempVar[5]);
                    R[2] = tempVar[2] / (input->materialInput[j].matThermalConductivity * tempVar[6]);
                    R[3] = tempVar[3] / (input->materialInput[j].matThermalConductivity * tempVar[7]);
                    // R = deltal*cos(theta)/(k*L). It didn't improve.
//                    R[0] = tempVar[12] / (input->materialInput[j].matThermalConductivity * tempVar[4]);
//                    R[1] = tempVar[13] / (input->materialInput[j].matThermalConductivity * tempVar[5]);
//                    R[2] = tempVar[14] / (input->materialInput[j].matThermalConductivity * tempVar[6]);
//                    R[3] = tempVar[15] / (input->materialInput[j].matThermalConductivity * tempVar[7]);

                    // all the resistances
                    matrices->R[numbersNodeAndPort[1] + 0] = R[0];
                    matrices->R[numbersNodeAndPort[1] + 1] = R[1];
                    matrices->R[numbersNodeAndPort[1] + 2] = R[2];
                    matrices->R[numbersNodeAndPort[1] + 3] = R[3];

                    // G = Vt*wb*pb*cb. Manually validated
                    G = tempVar[8] * input->materialInput[j].bloodPerfusion
                            * input->materialInput[j].bloodDensity
                            * input->materialInput[j].bloodSpecificHeat;


                    // Is = Vt*(Qmet + wb*pb*cb*Tb). Manually validated
                    Is = tempVar[8]*(input->materialInput[j].internalHeatGeneration
                            + input->materialInput[j].bloodPerfusion
                            * input->materialInput[j].bloodDensity
                            * input->materialInput[j].bloodSpecificHeat
                            * input->materialInput[j].bloodTemperature);

                    // Manually validated
                    Zhat = Z[0] * Z[1] * Z[2] * Z[3] / (Z[0] * Z[1] * Z[2] 
                            + Z[0] * Z[1] * Z[3] + Z[0] * Z[2] * Z[3]
                            + Z[1] * Z[2] * Z[3] + Z[0] * Z[1] * Z[2] * Z[3] * G);

                    // Manually validated
                    tau[0] = 2 * Zhat / Z[0];
                    tau[1] = 2 * Zhat / Z[1];
                    tau[2] = 2 * Zhat / Z[2];
                    tau[3] = 2 * Zhat / Z[3];

                    // DEBUG: validate Cap, Zs, Rs, G, Is, Zhat, taus
                    // printf("Cap %f, Z0 %e, Z1 %e, Z2 %e, R0 %e, R1 %e, R2 %e\n"
                    //        "G %e, Is %e, Zhat %e, tau0 %e, tau1 %e, tau2 %e\n",
                    //        Cap, Z[0], Z[1], Z[2], R[0], R[1], R[2], G, Is, Zhat,
                    //        tau[0], tau[1], tau[2]);


                    // S( nPortsAcu:nPortsAcuEnd, nPortsAcu:nPortsAcuEnd ) = ...
                    //  [ ( tau(1) - 1 ),     tau(2),             tau(3); ...
                    //  tau(1),         ( tau(2) - 1 ),       tau(3); ...
                    //  tau(1),           tau(2),           ( tau(3) - 1 )];

                    // this is actually matrix S.
                    // M = C*S
                    // this matrix is M[line][column]. May be, it would worth to do
                    // M[column][line] in order to use some LAPACK package.
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

                    matrices->Points_output[numbersNodeAndPort[0]].x = tempVar[9];
                    matrices->Points_output[numbersNodeAndPort[0]].y = tempVar[10];
                    matrices->Points_output[numbersNodeAndPort[0]].z = tempVar[11];


                    // initial value
                    // Vi0 = (Ti - ZIS)/sum(tau)
                    matrices->Vi(numbersNodeAndPort[1]) = (input->materialInput[j].initialTemperature
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
 * connectionsAndBoundaries: assumes that the intersection happens between two nodes
 * or between one node and a boundary, only. Intersections with three nodes would break
 * 
 * THIS FUNCTION MAYBE WELL SUITED FOR GOING AT THE LEVEL OF TLM AND NOT BEING SPECIFIC
 * FOR PENNES
 */
unsigned int connectionsAndBoundariesPennesEigen(struct calculationTLMEigen *matrices,
        struct boundaryData* boundaries, struct connectionLeveln *connection,
        struct dataForSimulation *input) {

    // DEBUG: get the number of non-zeros
    //    printf("Number of non-zeros %llu.\n", matrices->M.nonZeros());

    unsigned int errorTLMnumber = 0, approximationTB = 1;
    // approximationTB = 1; calculates TB using Vr
    // approximationTB = 2; calculates TB using Tc
    // tests showed that using Vr gives a better approximation than using Tc
    // I will try to keep both internally just for testing purposes

    long long unsigned int i, j1, offset, j2, j3, k, offset_TB, offset_heat_flux;

    offset_TB = matrices->numbers.Nodes * input->simulationInput.saveTemperature;

    offset_heat_flux = offset_TB + matrices->numbers.Intersections * input->simulationInput.saveTemperatureBetween;

    //    offset_heat_flux = offset_TB
    //            + matrices->numbers.Intersections*input->simulationInput.saveTemperatureBetween;

    long long unsigned int *startEnd = NULL;
    startEnd = (long long unsigned int *) malloc(sizeof (long long unsigned int)*8);
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
    // 0 - total reflection for port 1
    // 1 - total reflection for port 2
    // ...

    // transmission
    // 0 - port 1 to port 2
    // 1 - port 1 to port 3
    // ...
    // startEnd[0] - port 2 to port 3
    // startEnd[0] + 1 - port 2 to port 4
    // ...

    // B:
    // 0 - value for port 1
    // 1 - value for port 2
    // ...

    // transmission_out(_flux)
    // 0 - effect of port 1 on TB (or q'')
    // 1 - effect of port 2 on TB (or q'')
    // ...
    // n - effect of port n on TB (or q'')

    // B_out(_flux)
    // 0 - constant effect of port 1 on TB (or q'')
    // 1 - constant effect of port 2 on TB (or q'')
    // ...
    // n - constant effect of port n on TB (or q'')

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

    // this pointer will contain the number of the ports
    // 0 - size of the pointer
    // 1 - quantity of port numbers
    // 2... - number of the ports
    long long unsigned int *portsNumbers;
    portsNumbers = (long long unsigned int*) malloc(
            sizeof (long long unsigned int)*7);
    // here I start getting the number of the ports
    portsNumbers[0] = 7;

    for (i = 0; i < connection->accumulatedIntersections[0]; i++) {
        // DEBUG: show where we are
        //                printf("Initial i% llu", i);
        // pointwise validation showed that this algorithm is taking the same 
        // connections as the matlab/octave code. The number of the ports are
        // different because I changed the port numbering in this version
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

            if (input->simulationInput.saveTemperatureBetween) {
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

            if (input->simulationInput.saveHeatFlux) {
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
            for (unsigned j = 2; j < (portsNumbers[1] + 2); j++) {
                if (portsNumbers[j] == 0) {
                    if (portsNumbers[1] > 2) {
                        // the number following the first zero, is the number of the boundary

                        // DEBUG: testing the number of the boundary
                        // printf("Test (%llu) number", i);
                        // printf(" number of the boundary %llu\n", portsNumbers[j + 3]);

                        // j1a is the position of the port
                        if ((j + 2) > (portsNumbers[1] + 2)) j1a = j - 1;
                        else j1a = j + 2;

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

                                if (input->simulationInput.saveTemperatureBetween) {
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

                                if (input->simulationInput.saveHeatFlux) {
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

                                if (input->simulationInput.saveTemperatureBetween) {
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

                                if (input->simulationInput.saveHeatFlux) {
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


                                if (input->simulationInput.saveTemperatureBetween) {
                                    switch (approximationTB) {
                                        case 1: // using Vr
                                            B_out[0] = (matrices->Z[startEnd[2]] + matrices->R[startEnd[2]])
                                                    * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0]
                                                    * matrices->L[startEnd[2]];

                                            transmission_out[0] = 2;
                                            break;
                                        case 2: // using Tc
                                            B_out[0] = matrices->Z[startEnd[2]]
                                                    * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0]
                                                    * matrices->L[startEnd[2]];

                                            transmission_out[0] = 1;
                                            break;
                                    }
                                }

                                if (input->simulationInput.saveHeatFlux) {
                                    // from inside to outside. Positive values for 
                                    // heat flux input are towards inside, hence
                                    // why -1
                                    B_out_flux[0] = -1 * boundaries[portsNumbers[j + 1]].boundaries[0].boundaryData[0];

                                    transmission_out_flux[0] = 0;
                                }

                                break;

                                // convection heat flux boundary condition
                            case 3:

                                //                                printf("Convection boundary condition.\n");

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



                                if (input->simulationInput.saveTemperatureBetween) {
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

                                if (input->simulationInput.saveHeatFlux) {
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

            if (input->simulationInput.saveTemperatureBetween) {
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

            if (input->simulationInput.saveHeatFlux) {
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
        ;
        // DEBUG: testing the multiplication algorithm
        //                        printf("Testing the multiplication algorithm (%llu, %llu, %llu)\n",
        //                                i, startEnd[2], startEnd[5]);
        // first we have to put the transmission effects
        for (j1 = 0; j1 < startEnd[0]; j1++) {
            offset = j1 * startEnd[0];

            if (j1 == 0 && (input->simulationInput.saveTemperatureBetween ||
                    input->simulationInput.saveHeatFlux)) {
                getBetweenPointFromRealPortNumber(matrices->numbers.abstractPortsToReal,
                        startEnd[2],
                        &(matrices->Points_output[offset_TB + i].x),
                        &(matrices->Points_output[offset_TB + i].y),
                        &(matrices->Points_output[offset_TB + i].z),
                        input);

                if (input->simulationInput.saveHeatFlux) {
                    getProjectionFromRealPortNumber(matrices->numbers.abstractPortsToReal,
                            startEnd[2],
                            &(matrices->Points_output[offset_heat_flux + i].x),
                            &(matrices->Points_output[offset_heat_flux + i].y),
                            &(matrices->Points_output[offset_heat_flux + i].z),
                            input);

                }
            }

            if (input->simulationInput.saveTemperatureBetween) {
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



            if (input->simulationInput.saveHeatFlux) {
                matrices->E_output(offset_heat_flux + i) = matrices->E_output(offset_heat_flux + i)
                        + transmission_out_flux[j1] * matrices->E(startEnd[j1 * 3 + 2])
                        + B_out_flux[j1];

                // DEBUG: tests the calculation of heat flux
                // printf("%llu: B_out_Flux %g, transmission_out_flux %g, E_output %g, Taus",
                //         j1, B_out_flux[j1], transmission_out_flux[j1], matrices->E_output(offset_heat_flux + i));

                for (j2 = startEnd[j1 * 3 + 3]; j2 < startEnd[j1 * 3 + 4]; j2++) {
                    matrices->tau.insert(offset_heat_flux + i, j2) =
                            transmission_out_flux[j1] * matrices->M.coeff(startEnd[j1 * 3 + 2], j2);

                    //     printf(" %g", matrices->tau.coeff(offset_heat_flux + i, j2));
                }
                // printf("\n");

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
    //    matrices->M.prune();
    matrices->tau.makeCompressed();
    //    matrices->tau.prune();

    // DEBUG: get the number of non-zeros
    //    printf("Number of non-zeros %llu.\n", matrices->M.nonZeros());
    //                 

    // DEBUG: test the output of the matrix M. The matrix M was validated against
    // the same matrix calculated in matlab
    //    long long unsigned int next = 1;
    //    long long unsigned int line_begin = 0, line_end = 120; //line_begin = (next - 1)*12, line_end = next*12;
    //    long long unsigned int column_begin = 0, column_end = 120;
    //        
    //            printf("Matrix M\n");
    //            for (i = line_begin; i < line_end; i++) {
    //    //            printf("%llu:", i);
    //                for (k = column_begin; k < column_end; k++)
    //                    printf(" %e, ", matrices->M.coeff(i, k));
    //                printf("\n");
    //            }

    //        printf("Matrix E\n");
    //        for (k = line_begin; k < line_end; k++)
    //            printf("%llu: %e\n", k, matrices->E(k));

    //    
    //     DEBUG: testing the matrix multiplication by summing all the terms
    //            double temp1 = 0, temp2 = 0;
    //                while(1); // trick to get all the prints
    //            for (i = 0; i < 120; i++){
    //                for (k = 0; k < 120; k++)
    //                    temp1 = temp1 + matrices->M[i][k];
    //                temp2 = temp2 + matrices->E[i];
    //            }
    //            printf("Sum of M (%e), sum of E (%e)\n", temp1, temp2);

    // DEBUG: show numbers
    //        printf("Number of intersections %llu, Number of Nodes %llu, Number of Ports %llu\n",
    //                matrices->numberOfIntersections, matrices->numberOfNodes,
    //                matrices->numberOfPorts);

    return 0;
}