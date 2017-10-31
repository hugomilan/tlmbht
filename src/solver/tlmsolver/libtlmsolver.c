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
 * File:   libtlmsolver.c
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
 * redirects the flow of the algorithm for choosing the adequate TLM solver.
 * Basic functions for working with TLM that are general to all TLM solvers.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "libtlmsolver.h"
#include "../../miscellaneous/liberrorcode.h"
#include "../../miscellaneous/libmiscellaneous.h"
#include "../../miscellaneous/liblinalg.h"

/*
 * solverTLM: redirects the flow of the algorithm to the adequate function call
 */
unsigned int solverTLM(struct dataForSimulation* input, int id, void** generalMatrix) {
    unsigned int errorTLMnumber = 0;

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("TLM Solver\n");
    }

    switch (input->equationInput[id].libraryForCalculation) {
        case EIGEN:
            errorTLMnumber = call_from_c_solverEigenTLM(input, id, generalMatrix);
            break;
        case CUDA:
            break;
    }
    return errorTLMnumber;
}

/*
 * solveTimeStepTLM: redirects the flow of the algorithm to the adequate function call
 */
unsigned int solveTimeStepTLM(struct dataForSimulation* input, int id, void** generalMatrix) {
    unsigned int errorTLMnumber = 0;

    if (input->simulationInput.printAdditionalMode == 1) {
        printf("\nCalculating the for equation group %04d\n", id + 1);
        printf("TLM Solver\n");
    }

    switch (input->equationInput[id].libraryForCalculation) {
        case EIGEN:
            //            errorTLMnumber = call_from_c_solverTimeStepEigenTLM(input, id, generalMatrix);
            break;
        case CUDA:
            break;
    }
    return errorTLMnumber;
}

/*
 * terminateBoundaryTypeAndData: deallocate the boundary variable from memory
 */
unsigned int terminateBoundaryTypeAndData(struct boundaryData **input,
        const struct dataForSimulation * inputData, int id) {

    // if the pointed variable is null, it has been deallocated already
    if ((*input) != NULL) {
        // go over all the boundaries numbers

        for (unsigned int i = 0; i < inputData->equationInput[id].numberOfBoundaries; i++) {
            // go over all quantity of boundaries for each boundary number
            for (unsigned int j = 0; j < (*input)[i].quantityOfBoundaries; j++) {
                // deallocating the memory for the boundary data
                free((*input)[i].boundaries[j].boundaryData);
                (*input)[i].boundaries[j].boundaryData = NULL;
            }
            // deallocating the memory for the boundary
            free((*input)[i].boundaries);
            (*input)[i].boundaries = NULL;
        }
        // deallocating the memory for the boundaryData variable
        free((*input));
        (*input) = NULL;
    }

    return 0;
}

/*
 * initiate_connectionLeveln: allocate in memory the variable
 * that will hold the connections.
 */
unsigned int initiate_connectionLeveln(struct connectionLeveln *con,
        unsigned int level, unsigned long long *allocateForEachLevel) {
    unsigned int errorTLMnumber = 0;

    con->level = level;
    con->quantitySaved = 0;
    con->quantityAllocated = allocateForEachLevel[0];
    con->accumulatedIntersections = NULL;
    // DEBUG: show allocate for each
    //        printf("Allocate for each %llu (%llu). Level %u. Pointer address %p \n",
    //                allocateForEachLevel[0], con->quantityAllocated, con->level, con);

    if ((con->portsOrPoints = (unsigned long long*) malloc(
            sizeof (unsigned long long)*con->quantityAllocated)) == NULL) {
        errorTLMnumber = 8698;
        sendErrorCodeAndMessage(errorTLMnumber, &level, &(con->quantityAllocated), NULL, NULL);
        return errorTLMnumber;
    }

    if (level > 0) {
        if ((con->innerLevel = (struct connectionLeveln*) malloc(
                sizeof (struct connectionLeveln)*con->quantityAllocated)) == NULL) {
            errorTLMnumber = 8699;
            sendErrorCodeAndMessage(errorTLMnumber, &level, &(con->quantityAllocated), NULL, NULL);
            return errorTLMnumber;
        }

        for (unsigned long long i = 0; i < con->quantityAllocated; i++) {
            if ((errorTLMnumber = initiate_connectionLeveln(
                    &(con->innerLevel[i]), level - 1, allocateForEachLevel + 1)) != 0)
                return errorTLMnumber;
        }
    } else {
        con->innerLevel = NULL;
    }
    return 0;
}

/*
 * reallocate_connectionLeveln: allocate more memory for connection variable
 * if we have reach the end of the buffer
 */
unsigned int reallocate_connectionLeveln(struct connectionLeveln *con, unsigned int extra) {
    unsigned int errorTLMnumber = 0;

    // I reallocate 20% + extra of the quantity already saved
    con->quantityAllocated = extra + (unsigned long long) (1.2 * con->quantitySaved);
    // DEBUG: Show how much are we reallocating
    //    printf(" reallocated to %llu", con->quantityAllocated);

    // reallocating the portsOrPoints variable
    if ((con->portsOrPoints = (unsigned long long*) realloc(con->portsOrPoints,
            sizeof (unsigned long long)*con->quantityAllocated)) == NULL) {
        errorTLMnumber = 8696;
        sendErrorCodeAndMessage(errorTLMnumber, &(con->level), &(con->quantityAllocated), NULL, NULL);
        return errorTLMnumber;
    }

    if (con->level > 0) {
        if ((con->innerLevel = (struct connectionLeveln*) realloc(con->innerLevel,
                sizeof (struct connectionLeveln)*con->quantityAllocated)) == NULL) {
            errorTLMnumber = 8695;
            sendErrorCodeAndMessage(errorTLMnumber, &(con->level), &(con->quantityAllocated), NULL, NULL);
            return errorTLMnumber;
        }

        unsigned long long *pointsToAllocate;

        pointsToAllocate = (unsigned long long*) malloc(sizeof (unsigned long long)*con->level);

        getquantityAllocated_connectionLeveln(&(con->innerLevel[0]), pointsToAllocate);

        // DEBUG: show how many points should we allocate
        //        printf(", sizes to allocate ");
        //        for (unsigned int k = 0; k < con->level; k++)
        //            printf(" %llu", pointsToAllocate[k]);

        for (unsigned long long i = con->quantitySaved; i < con->quantityAllocated; i++) {
            if ((errorTLMnumber = initiate_connectionLeveln(
                    &(con->innerLevel[i]), con->level - 1, pointsToAllocate)) != 0) {
                return errorTLMnumber;
            }

            // DEBUG: test the allocating
            //            printf(", for the allocation %llu, level %u, saved/allocated %llu/%llu",
            //                    i, con->innerLevel[i].level, con->innerLevel[i].quantitySaved,
            //                    con->innerLevel[i].quantityAllocated);
        }

        free(pointsToAllocate);
        pointsToAllocate = NULL;


    } else {
        con->innerLevel = NULL;
    }
    return 0;
}

/*
 * add_to_connectionLeveln: add a value in the variable that has the connections
 */
unsigned int add_to_connectionLeveln(struct connectionLeveln * con,
        unsigned long long* Points,
        unsigned int quantityOfPortsToAdd, unsigned long long* numberOfPortsToAdd) {
    unsigned int errorTLMnumber = 0, newInterception = 0;
    unsigned long long PointToSave = 0, i;
    // if the quantity of points is less than the level that we are, then, we
    // access an special point number, which is number zero (initial value). Point number zero
    // indicates the intersections that do not involve those levels.
    // Points[0] = quantity of points to save
    if (con->level > 0 && Points[0] >= con->level) {
        PointToSave = Points[Points[0] + 1 - con->level];
    }


    // here, I have to match the number of the points
    if (con->level > 0) {

        // did I find matching points? If I find a matching point, then, I call
        // this function recursively. If I did not find a matching point, then,
        // I execute the code after the for (i.e., on the goto)
        for (i = 0; i < con->quantitySaved; i++) {
            if (con->portsOrPoints[i] == PointToSave) {
                goto get_out_if;
            }
        }


        // I did not find matching points, then, I have to add this point to the buffer.
        // This means that this is a new intersection point

        // If I'm at the threshold of buffer overflow, I allocate more space for it
        if (con->quantitySaved == con->quantityAllocated) {
            if ((errorTLMnumber = reallocate_connectionLeveln(con, 1)) != 0) {
                return errorTLMnumber;
            }
        }

        i = con->quantitySaved;
        con->portsOrPoints[i] = PointToSave;

        con->quantitySaved++;
        // flag to indicate that this is a new interception point
        newInterception = 1;

get_out_if:

        errorTLMnumber = add_to_connectionLeveln(&(con->innerLevel[i]), Points,
                quantityOfPortsToAdd, numberOfPortsToAdd);


        // this is the level 0
    } else {
        // If I'm at the threshold of buffer overflow, I allocate more space for it
        if (con->quantityAllocated < (con->quantitySaved + quantityOfPortsToAdd)) {
            //            printf(", reallocating... ");
            if ((errorTLMnumber = reallocate_connectionLeveln(con, quantityOfPortsToAdd)) != 0)
                return errorTLMnumber;
        }
        
        // saving the ports' number
        for (i = con->quantitySaved; i < (con->quantitySaved + quantityOfPortsToAdd); i++) {
            con->portsOrPoints[i] = numberOfPortsToAdd[i - con->quantitySaved];
        }

        con->quantitySaved = con->quantitySaved + quantityOfPortsToAdd;
    }



    // return = 1 is my flag that this is a new interception.
    // I can get a new interception recursively with the errorTLMnumber = 1
    // or with newInterception = 1. I will only send this flag when
    // errorTLMnumber = 1 OR when newInterception = 1 && errorTLMnumber = 0
    if (errorTLMnumber == 1 || (newInterception == 1 && errorTLMnumber == 0)) {
        return 1;
    }

    return errorTLMnumber;
}

/*
 * wrap_size_connectionLeveln: deallocate in memory the variables
 * that were not used in the connections.
 */
unsigned int wrap_size_connectionLeveln(struct connectionLeveln *con) {
    unsigned int errorTLMnumber;
    unsigned long long i;

    // if quantity saved = 0, then I deallocate everything. Otherwise, only
    // deallocate what was not used
    if (con->quantitySaved != 0) {
        
        if ((con->portsOrPoints = (unsigned long long*) realloc(con->portsOrPoints,
                sizeof (unsigned long long)*con->quantitySaved)) == NULL) {
            errorTLMnumber = 8694;
            sendErrorCodeAndMessage(errorTLMnumber, &(con->level), &(con->quantitySaved), NULL, NULL);
            return errorTLMnumber;
        }


        // recursively terminating and reallocating
        if (con->level > 0) {
            for (i = 0; i < con->quantitySaved; i++) {
                wrap_size_connectionLeveln(&(con->innerLevel[i]));
            }

            // deallocating the positions that were not used
            for (i = con->quantitySaved; i < con->quantityAllocated; i++) {
                terminate_connectionLeveln(&(con->innerLevel[i]));

            }
            
            if ((con->innerLevel = (struct connectionLeveln*) realloc(con->innerLevel,
                    sizeof (struct connectionLeveln)*con->quantitySaved)) == NULL) {
                errorTLMnumber = 8693;
                sendErrorCodeAndMessage(errorTLMnumber, &(con->level), &(con->quantitySaved), NULL, NULL);
                return errorTLMnumber;
            }

            // now I get the quantity of accumulated elements.
            // First I allocate this variable
            if ((con->accumulatedIntersections = (unsigned long long*)
                    realloc(con->accumulatedIntersections,
                    sizeof (unsigned long long)*(con->quantitySaved + 1))) == NULL) {
                errorTLMnumber = 8697;
                sendErrorCodeAndMessage(errorTLMnumber, &(con->level), &(con->quantitySaved), NULL, NULL);
                return errorTLMnumber;
            }


            if (con->level == 1) {
                // at level 1, each position is equal to one intersection
                // therefore, I just allocate them as 1 and them put the position 0,
                // which contains the total for that level, equals to the quantitySaved.
                for (i = 0; i < con->quantitySaved; i++) {
                    con->accumulatedIntersections[i + 1] = i + 1;
                }
                con->accumulatedIntersections[0] = con->quantitySaved;


            } else {
                // for levels different than 1, I start position 0 with the value 0
                // and increment it with the values that I get
                con->accumulatedIntersections[0] = 0;
                for (i = 0; i < con->quantitySaved; i++) {
                    con->accumulatedIntersections[0] +=
                            con->innerLevel[i].accumulatedIntersections[0];

                    con->accumulatedIntersections[i + 1] =
                            con->accumulatedIntersections[0];
                }
            }
        }

        // now that we terminated the excess allocated, we reduce it to quantitySaved
        con->quantityAllocated = con->quantitySaved;
        
        // if there is nothing saved here, I just free it
    } else {

        // deallocating the portsOrPoints variable
        free(con->portsOrPoints);
        con->portsOrPoints = NULL;

        // deallocating the positions that were not used
        for (i = 0; i < con->quantityAllocated; i++) {
            terminate_connectionLeveln(&(con->innerLevel[i]));
        }

        free(con->innerLevel);
        con->innerLevel = NULL;

        free(con->accumulatedIntersections);
        con->accumulatedIntersections = NULL;
    }

    return 0;
}

/*
 * terminate_connectionLeveln: deallocate the variable that hold the connections.
 */
unsigned int terminate_connectionLeveln(struct connectionLeveln *con) {

    if (con->portsOrPoints != NULL) {
        if (con->level > 0)
            for (unsigned long long i = 0; i < con->quantityAllocated; i++) {
                terminate_connectionLeveln(con->innerLevel + i);
            }

        free(con->portsOrPoints);
        con->portsOrPoints = NULL;

        free(con->innerLevel);
        con->innerLevel = NULL;

        free(con->accumulatedIntersections);
        con->accumulatedIntersections = NULL;
    }


    return 0;
}

/*
 * getquantityAllocated_connectionLeveln: get the points allocated for all of
 * the innerLevel[0].
 */
unsigned int getquantityAllocated_connectionLeveln(struct connectionLeveln *con,
        unsigned long long *allocatedPoints) {

    allocatedPoints[0] = con->quantityAllocated;
    if (con->level > 0) {
        getquantityAllocated_connectionLeveln(&(con->innerLevel[0]), allocatedPoints + 1);
    }

    return 0;
}

/*
 * getPortsOrPoints: get the points or ports saved on the connection variable.
 * position = number of the intersection point that I want to get the number of the ports
 */
unsigned int getPortsOrPoints(struct connectionLeveln *con,
        unsigned long long position, unsigned long long **output) {
    unsigned int errorTLMnumber = 0;

    if (con->level == 0) {
        // I assume that output[0] has the size of this pointer
        // I also assume that output[1] has the quantity of saved variables
        if (((*output)[0] - 2) < con->quantitySaved) {
            // reallocate the output pointer with the size of quantitySaved
            // + 1 + 2 (position 0: size of output; position 1: size of the
            // saved variables) + 20% of the size of the quantitySaved
            (*output)[0] = 1 + 2 + (unsigned long long) (1.2 * con->quantitySaved);

            if (((*output) = (unsigned long long*) realloc((*output),
                    sizeof (unsigned long long)*(*output)[0])) == NULL) {
                errorTLMnumber = 8733;
                sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
                return errorTLMnumber;
            }
        }

        // saving the variables in the pointer output
        (*output)[1] = con->quantitySaved;
        for (unsigned long long i = 0; i < con->quantitySaved; i++) {
            (*output)[i + 2] = con->portsOrPoints[i];
        }
        return 0;
    }

    // going to an inner level if this is not the level 0
    for (unsigned long long i = 0; i < con->quantitySaved; i++) {
        // I will go to the level where position is at most equal to the 
        // highest number of that level
        if (position < con->accumulatedIntersections[i + 1]) {
            // if I'm going to a position greater than position 0, I need to 
            // wrap the value of position. I do this by removing the accumulated
            // value of the anterior position
            if (i > 0)
                position = position - con->accumulatedIntersections[i];
            
            return getPortsOrPoints(&(con->innerLevel[i]), position,
                    output);
        }
    }
}

/*
 * allocatePointsPort: receive the number of the points to which the connection
 * happens and them allocate the ports on those positions
 */
unsigned int allocatePointsPort(unsigned long long *Points,
        struct connectionLeveln *input,
        unsigned int quantityOfPortsToAdd,
        unsigned long long *numberOfPortsToAdd) {

    unsigned int errorTLMnumber;

    // sorting the numbers
    sortOptmizedBubbleSort(Points);

    if ((errorTLMnumber = add_to_connectionLeveln(input, Points,
            quantityOfPortsToAdd, numberOfPortsToAdd)) != 0) {
        return errorTLMnumber;
    }

    return errorTLMnumber;
}

/*
 * getGeometricalVariablesTLMline: Receives the x, y, z from the nodes and
 * calculates the length of the ports. For the line, the length is defined as
 * (line size)/2.
 */
unsigned int getGeometricalVariablesTLMline(const struct node *N1,
        const struct node *N2, double *output) {
    // output variable:
    // 0 - length of port 1 and 2. They are equal to (nodes_length)/2.
    // 1 - Lines' center x
    // 2 - Lines' center y
    // 3 - Lines' center z

    // port 1: N1 and center
    // port 2: N2 and center

    // line nomenclature
    //
    //   vertex 1  \ 
    //              \
    //               \    
    //                \
    //                 \  vertex 2
    //

    // obtains the norm of the vector defined between the two nodes
    nodesNorm(N1, N2, &output[0]);
    output[0] = output[0] / 2;
    output[1] = (N1->x + N2->x) / 2; // center_x;
    output[2] = (N1->y + N2->y) / 2; // center_y;
    output[3] = (N1->z + N2->z) / 2; // center_z

    return 0;
}

/*
 * getGeometricalVariablesTLMtriangle: Receives the x, y, z from the nodes and
 * calculates the area of the triangle, the length of each triangular face,
 * and the distance from the center of the triangle to the middle of each
 * triangular face. This function was manually validated.
 */
unsigned int getGeometricalVariablesTLMtriangle(const struct node *N1,
        const struct node *N2, const struct node *N3, double *output) {
    // output variable:
    // 0 - length of port 1
    // 1 - length of port 2
    // 2 - length of port 3
    // 3 - triangular area
    // 4 - length of face 1
    // 5 - length of face 2
    // 6 - length of face 3
    // 7 - Triangles' center x
    // 8 - Triangles' center y
    // 9 - Triangles' center z

    // port 1: N1 and N2
    // port 2: N1 and N3
    // port 3: N2 and N3

    // triangular nomenclature.
    //              vertex 1
    //              /\
    //     face 1  /  \   face 2
    //            /    \
    //           /      \
    // vertex 2 /________\ vertex 3
    //            face 3


    double center[3];
    double deltaXl[3], deltaYl[3], deltaZl[3], deltal[3];

    center[0] = (N1->x + N2->x + N3->x) / 3;
    center[1] = (N1->y + N2->y + N3->y) / 3;
    center[2] = (N1->z + N2->z + N3->z) / 3;

    deltaXl[0] = (N1->x + N2->x) / 2 - center[0];
    deltaYl[0] = (N1->y + N2->y) / 2 - center[1];
    deltaZl[0] = (N1->z + N2->z) / 2 - center[2];
    deltal[0] = sqrt(deltaXl[0] * deltaXl[0] + deltaYl[0] * deltaYl[0] + deltaZl[0] * deltaZl[0]);

    deltaXl[1] = (N1->x + N3->x) / 2 - center[0];
    deltaYl[1] = (N1->y + N3->y) / 2 - center[1];
    deltaZl[1] = (N1->z + N3->z) / 2 - center[2];
    deltal[1] = sqrt(deltaXl[1] * deltaXl[1] + deltaYl[1] * deltaYl[1] + deltaZl[1] * deltaZl[1]);

    deltaXl[2] = (N2->x + N3->x) / 2 - center[0];
    deltaYl[2] = (N2->y + N3->y) / 2 - center[1];
    deltaZl[2] = (N2->z + N3->z) / 2 - center[2];
    deltal[2] = sqrt(deltaXl[2] * deltaXl[2] + deltaYl[2] * deltaYl[2] + deltaZl[2] * deltaZl[2]);


    output[0] = deltal[0];
    output[1] = deltal[1];
    output[2] = deltal[2];
    triangleArea(N1, N2, N3, &output[3]);
    nodesNorm(N1, N2, &output[4]);
    nodesNorm(N1, N3, &output[5]);
    nodesNorm(N2, N3, &output[6]);
    output[7] = center[0];
    output[8] = center[1];
    output[9] = center[2];

    return 0;
}

/*
 * getGeometricalVariablesTLMquadrangle: Receives the x, y, z from the nodes and
 * calculates the geometrical characteristics of the quadrangle.
 */
unsigned int getGeometricalVariablesTLMquadrangle(const struct node *N1,
        const struct node *N2, const struct node *N3, const struct node *N4, double *output) {
    // 0 - length of port 1 (from center of quadrangle to center of face 1)
    // 1 - length of port 2 (from center of quadrangle to center of face 2)
    // 2 - length of port 3 (from center of quadrangle to center of face 3)
    // 3 - length of port 4 (from center of quadrangle to center of face 4)
    // 4 - quadrangle area
    // 5 - length of face 1
    // 6 - length of face 2
    // 7 - length of face 3
    // 8 - length of face 4
    // 9 - Quadrangle's center x
    // 10 - Quadrangle's center y
    // 11 - Quadrangle's center z

    // port 1: N1 and N2
    // port 2: N2 and N3
    // port 3: N3 and N4
    // port 4: N1 and N4

    // triangular nomenclature.
    //                        face 1   
    //     vertex 1  _______________________ vertex 2
    //              /                      / 
    //             /                      / 
    //    face 4  /                      / face 2
    //           /                      /
    // vertex 4 /______________________/ vertex 3
    //                  face 3

    // lengths of the edges
    double center[3];
    double deltaXl[4], deltaYl[4], deltaZl[4], deltal[4];


    center[0] = (N1->x + N2->x + N3->x + N4->x) / 4;
    center[1] = (N1->y + N2->y + N3->y + N4->y) / 4;
    center[2] = (N1->z + N2->z + N3->z + N4->z) / 4;

    deltaXl[0] = (N1->x + N2->x) / 2 - center[0];
    deltaYl[0] = (N1->y + N2->y) / 2 - center[1];
    deltaZl[0] = (N1->z + N2->z) / 2 - center[2];
    deltal[0] = sqrt(deltaXl[0] * deltaXl[0] + deltaYl[0] * deltaYl[0] + deltaZl[0] * deltaZl[0]);

    deltaXl[1] = (N2->x + N3->x) / 2 - center[0];
    deltaYl[1] = (N2->y + N3->y) / 2 - center[1];
    deltaZl[1] = (N2->z + N3->z) / 2 - center[2];
    deltal[1] = sqrt(deltaXl[1] * deltaXl[1] + deltaYl[1] * deltaYl[1] + deltaZl[1] * deltaZl[1]);

    deltaXl[2] = (N3->x + N4->x) / 2 - center[0];
    deltaYl[2] = (N3->y + N4->y) / 2 - center[1];
    deltaZl[2] = (N3->z + N4->z) / 2 - center[2];
    deltal[2] = sqrt(deltaXl[2] * deltaXl[2] + deltaYl[2] * deltaYl[2] + deltaZl[2] * deltaZl[2]);

    deltaXl[3] = (N1->x + N4->x) / 2 - center[0];
    deltaYl[3] = (N1->y + N4->y) / 2 - center[1];
    deltaZl[3] = (N1->z + N4->z) / 2 - center[2];
    deltal[3] = sqrt(deltaXl[3] * deltaXl[3] + deltaYl[3] * deltaYl[3] + deltaZl[3] * deltaZl[3]);

    output[0] = deltal[0];
    output[1] = deltal[1];
    output[2] = deltal[2];
    output[3] = deltal[3];
    quadrangleArea(N1, N2, N3, N4, &output[4]);
    // maybe quadranglePlaneArea(N1, N2, N3, N4, &output[4]);
    nodesNorm(N1, N2, &output[5]);
    nodesNorm(N2, N3, &output[6]);
    nodesNorm(N3, N4, &output[7]);
    nodesNorm(N1, N4, &output[8]);
    output[9] = center[0];
    output[10] = center[1];
    output[11] = center[2];

    return 0;
}

/*
 * getGeometricalVariablesTLMtetrahedron: Receives the x, y, z from the nodes and
 * calculates the geometrical characteristics of the tetrahedron. Pointwise validated.
 */
unsigned int getGeometricalVariablesTLMtetrahedron(const struct node *N1,
        const struct node *N2, const struct node *N3, const struct node *N4, double *output) {
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

    // port 1: N1, N2 and N3
    // port 2: N1, N2 and N4
    // port 3: N1, N3 and N4
    // port 4: N2, N3 and N4

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
    // lengths of the edges
    double center[3], deltaXl[4], deltaYl[4], deltaZl[4], deltal[4];


    center[0] = (N1->x + N2->x + N3->x + N4->x) / 4;
    center[1] = (N1->y + N2->y + N3->y + N4->y) / 4;
    center[2] = (N1->z + N2->z + N3->z + N4->z) / 4;

    deltaXl[0] = (N1->x + N2->x + N3->x) / 3 - center[0];
    deltaYl[0] = (N1->y + N2->y + N3->y) / 3 - center[1];
    deltaZl[0] = (N1->z + N2->z + N3->z) / 3 - center[2];
    deltal[0] = sqrt(deltaXl[0] * deltaXl[0] + deltaYl[0] * deltaYl[0] + deltaZl[0] * deltaZl[0]);

    deltaXl[1] = (N1->x + N2->x + N4->x) / 3 - center[0];
    deltaYl[1] = (N1->y + N2->y + N4->y) / 3 - center[1];
    deltaZl[1] = (N1->z + N2->z + N4->z) / 3 - center[2];
    deltal[1] = sqrt(deltaXl[1] * deltaXl[1] + deltaYl[1] * deltaYl[1] + deltaZl[1] * deltaZl[1]);

    deltaXl[2] = (N1->x + N3->x + N4->x) / 3 - center[0];
    deltaYl[2] = (N1->y + N3->y + N4->y) / 3 - center[1];
    deltaZl[2] = (N1->z + N3->z + N4->z) / 3 - center[2];
    deltal[2] = sqrt(deltaXl[2] * deltaXl[2] + deltaYl[2] * deltaYl[2] + deltaZl[2] * deltaZl[2]);

    deltaXl[3] = (N2->x + N3->x + N4->x) / 3 - center[0];
    deltaYl[3] = (N2->y + N3->y + N4->y) / 3 - center[1];
    deltaZl[3] = (N2->z + N3->z + N4->z) / 3 - center[2];
    deltal[3] = sqrt(deltaXl[3] * deltaXl[3] + deltaYl[3] * deltaYl[3] + deltaZl[3] * deltaZl[3]);

    output[0] = deltal[0];
    output[1] = deltal[1];
    output[2] = deltal[2];
    output[3] = deltal[3];
    triangleArea(N1, N2, N3, &output[4]);
    triangleArea(N1, N2, N4, &output[5]);
    triangleArea(N1, N3, N4, &output[6]);
    triangleArea(N2, N3, N4, &output[7]);
    tetrahedronVolume(N1, N2, N3, N4, &output[8]);
    output[9] = center[0];
    output[10] = center[1];
    output[11] = center[2];

    return 0;
}

/*
 * getGeometricalVariablesTLMpyramid: Receives the x, y, z from the nodes and
 * calculates the geometrical characteristics of the pyramid. Pointwise validated.
 */
unsigned int getGeometricalVariablesTLMpyramid(const struct node *N1,
        const struct node *N2, const struct node *N3, const struct node *N4,
        const struct node *N5, double *output) {
    // 0 - length of port 1 (from center of pyramid to center of area 1)
    // 1 - length of port 2 (from center of pyramid to center of area 2)
    // 2 - length of port 3 (from center of pyramid to center of area 3)
    // 3 - length of port 4 (from center of pyramid to center of area 4)
    // 4 - length of port 5 (from center of pyramid to center of area 5)
    // 5 - area of quadrangle 1
    // 6 - area of triangle 2
    // 7 - area of triangle 3
    // 8 - area of triangle 4
    // 9 - area of triangle 5
    // 10 - volume of the pyramid
    // 11 - pyramid's center x
    // 12 - pyramid's center y
    // 13 - pyramid's center z

    // port 1: N1, N2, N3, and N4
    // port 2: N1, N2, and N5
    // port 3: N1, N4, and N5
    // port 4: N2, N3, and N5
    // port 5: N3, N4, and N5

    /* pyramid nomenclature.
     *                                  
     *         vertex 4  ________________________________ vertex 3
     *                  /\___    area 5        _________/|           
     *                 /3    \____  __________/         /         
     *                /           \/vertex 5   1       4|            
     *               /a         __/\__                 /   
     *              /e       __/      \__    a        a|  
     *             /r     __/            \__e        e/      
     *            /a   __/                 r\__      r|
     *           /  __/       area 2      a    \__  a/ 
     * vertex 1 /__/______________________________\__| vertex 2
     *      
     *                            
     */
    // lengths of the edges
    double center[3], deltaXl[5], deltaYl[5], deltaZl[5], deltal[5], temp;


    center[0] = (N1->x + N2->x + N3->x + N4->x + N5->x) / 5;
    center[1] = (N1->y + N2->y + N3->y + N4->y + N5->y) / 5;
    center[2] = (N1->z + N2->z + N3->z + N4->z + N5->z) / 5;

    deltaXl[0] = (N1->x + N2->x + N3->x + N4->x) / 4 - center[0];
    deltaYl[0] = (N1->y + N2->y + N3->y + N4->y) / 4 - center[1];
    deltaZl[0] = (N1->z + N2->z + N3->z + N4->z) / 4 - center[2];
    deltal[0] = sqrt(deltaXl[0] * deltaXl[0] + deltaYl[0] * deltaYl[0] + deltaZl[0] * deltaZl[0]);

    deltaXl[1] = (N1->x + N2->x + N5->x) / 3 - center[0];
    deltaYl[1] = (N1->y + N2->y + N5->y) / 3 - center[1];
    deltaZl[1] = (N1->z + N2->z + N5->z) / 3 - center[2];
    deltal[1] = sqrt(deltaXl[1] * deltaXl[1] + deltaYl[1] * deltaYl[1] + deltaZl[1] * deltaZl[1]);

    deltaXl[2] = (N1->x + N4->x + N5->x) / 3 - center[0];
    deltaYl[2] = (N1->y + N4->y + N5->y) / 3 - center[1];
    deltaZl[2] = (N1->z + N4->z + N5->z) / 3 - center[2];
    deltal[2] = sqrt(deltaXl[2] * deltaXl[2] + deltaYl[2] * deltaYl[2] + deltaZl[2] * deltaZl[2]);

    deltaXl[3] = (N2->x + N3->x + N5->x) / 3 - center[0];
    deltaYl[3] = (N2->y + N3->y + N5->y) / 3 - center[1];
    deltaZl[3] = (N2->z + N3->z + N5->z) / 3 - center[2];
    deltal[3] = sqrt(deltaXl[3] * deltaXl[3] + deltaYl[3] * deltaYl[3] + deltaZl[3] * deltaZl[3]);

    deltaXl[4] = (N3->x + N4->x + N5->x) / 3 - center[0];
    deltaYl[4] = (N3->y + N4->y + N5->y) / 3 - center[1];
    deltaZl[4] = (N3->z + N4->z + N5->z) / 3 - center[2];
    deltal[4] = sqrt(deltaXl[4] * deltaXl[4] + deltaYl[4] * deltaYl[4] + deltaZl[4] * deltaZl[4]);

    output[0] = deltal[0];
    output[1] = deltal[1];
    output[2] = deltal[2];
    output[3] = deltal[3];
    output[4] = deltal[4];
    quadrangleArea(N1, N2, N3, N4, &output[5]);
    triangleArea(N1, N2, N5, &output[6]);
    triangleArea(N1, N4, N5, &output[7]);
    triangleArea(N2, N3, N5, &output[8]);
    triangleArea(N3, N4, N5, &output[9]);
    tetrahedronVolume(N1, N2, N3, N5, &temp);
    tetrahedronVolume(N1, N3, N4, N5, &output[10]);
    output[10] = output[10] + temp;
    output[11] = center[0];
    output[12] = center[1];
    output[13] = center[2];

    return 0;
}

/*
 * getGeometricalVariablesTLMhexahedron: Receives the x, y, z from the nodes and
 * calculates the geometrical characteristics of the hexahedron. Pointwise validated.
 */
unsigned int getGeometricalVariablesTLMhexahedron(const struct node *N1,
        const struct node *N2, const struct node *N3, const struct node *N4, const struct node *N5,
        const struct node *N6, const struct node *N7, const struct node *N8, double *output) {
    // 0 - length of port 1 (from center of tetrahedron to center of area 1)
    // 1 - length of port 2 (from center of tetrahedron to center of area 2)
    // 2 - length of port 3 (from center of tetrahedron to center of area 3)
    // 3 - length of port 4 (from center of tetrahedron to center of area 4)
    // 4 - length of port 5 (from center of tetrahedron to center of area 5)
    // 5 - length of port 6 (from center of tetrahedron to center of area 6)
    // 6 - area of quadrangle 1
    // 7 - area of quadrangle 2
    // 8 - area of quadrangle 3
    // 9 - area of quadrangle 4
    // 10 - area of quadrangle 5
    // 11 - area of quadrangle 6
    // 12 - volume of the hexahedron
    // 13 - hexahedron's center x
    // 14 - hexahedron's center y
    // 15 - hexahedron's center z

    // port 1: N1, N2, N3, and N4
    // port 2: N1, N2, N5, and N6
    // port 3: N1, N4, N5, and N8
    // port 4: N2, N3, N6, and N8
    // port 5: N3, N4, N7, and N8
    // port 6: N5, N6, N7, and N8

    /* hexahedral nomenclature.
     * 
     * 
     *            vertex 4____________________________ vertex 3
     *                   /|                          /|
     *                  / .                         / |
     *                 /  .      5                 /  |
     *                /   |                       /   |
     *               /    .    a                 /    |
     *              /     .   e                 /     |
     *             /      |  r     area 1      /      |
     *            /       . a                 /       |
     *  vertex 8 /___________________________/vertex 7|
     *          |         | vertex 1         |        |
     *          |        , _.._.._.._.._.._..|_.._ .._| vertex 2
     *          |     3 ,                    |     4 /
     *          |      /   2                 |      /
     *          |   a ,                      |   a /
     *          |  e ,   a                   |  e /
     *          | r /   e       area 6       | r /
     *          |a ,   r                     |a /
     *          | ,   a                      | /
     *          |/___________________________|/
     *      vertex 5                        vertex 6
     * 
     */
    // lengths of the edges
    double center[3];
    double deltaXl[6], deltaYl[6], deltaZl[6], deltal[6];

    // nomenclature in Grandy (1997) and mine:
    // 0 -> 1
    // 1 -> 2
    // 2 -> 4
    // 3 -> 3
    // 4 -> 5
    // 5 -> 6
    // 6 -> 8
    // 7 -> 7


    center[0] = (N1->x + N2->x + N3->x + N4->x + N5->x + N6->x + N7->x + N8->x) / 8;
    center[1] = (N1->y + N2->y + N3->y + N4->y + N5->y + N6->y + N7->y + N8->y) / 8;
    center[2] = (N1->z + N2->z + N3->z + N4->z + N5->z + N6->z + N7->z + N8->z) / 8;

    deltaXl[0] = (N1->x + N2->x + N3->x + N4->x) / 4 - center[0];
    deltaYl[0] = (N1->y + N2->y + N3->y + N4->y) / 4 - center[1];
    deltaZl[0] = (N1->z + N2->z + N3->z + N4->z) / 4 - center[2];
    deltal[0] = sqrt(deltaXl[0] * deltaXl[0] + deltaYl[0] * deltaYl[0] + deltaZl[0] * deltaZl[0]);

    deltaXl[1] = (N1->x + N2->x + N5->x + N6->x) / 4 - center[0];
    deltaYl[1] = (N1->y + N2->y + N5->y + N6->y) / 4 - center[1];
    deltaZl[1] = (N1->z + N2->z + N5->z + N6->z) / 4 - center[2];
    deltal[1] = sqrt(deltaXl[1] * deltaXl[1] + deltaYl[1] * deltaYl[1] + deltaZl[1] * deltaZl[1]);

    deltaXl[2] = (N1->x + N4->x + N5->x + N8->x) / 4 - center[0];
    deltaYl[2] = (N1->y + N4->y + N5->y + N8->y) / 4 - center[1];
    deltaZl[2] = (N1->z + N4->z + N5->z + N8->z) / 4 - center[2];
    deltal[2] = sqrt(deltaXl[2] * deltaXl[2] + deltaYl[2] * deltaYl[2] + deltaZl[2] * deltaZl[2]);

    deltaXl[3] = (N2->x + N3->x + N6->x + N7->x) / 4 - center[0];
    deltaYl[3] = (N2->y + N3->y + N6->y + N7->y) / 4 - center[1];
    deltaZl[3] = (N2->z + N3->z + N6->z + N7->z) / 4 - center[2];
    deltal[3] = sqrt(deltaXl[3] * deltaXl[3] + deltaYl[3] * deltaYl[3] + deltaZl[3] * deltaZl[3]);

    deltaXl[4] = (N3->x + N4->x + N7->x + N8->x) / 4 - center[0];
    deltaYl[4] = (N3->y + N4->y + N7->y + N8->y) / 4 - center[1];
    deltaZl[4] = (N3->z + N4->z + N7->z + N8->z) / 4 - center[2];
    deltal[4] = sqrt(deltaXl[4] * deltaXl[4] + deltaYl[4] * deltaYl[4] + deltaZl[4] * deltaZl[4]);

    deltaXl[5] = (N5->x + N6->x + N7->x + N8->x) / 4 - center[0];
    deltaYl[5] = (N5->y + N6->y + N7->y + N8->y) / 4 - center[1];
    deltaZl[5] = (N5->z + N6->z + N7->z + N8->z) / 4 - center[2];
    deltal[5] = sqrt(deltaXl[5] * deltaXl[5] + deltaYl[5] * deltaYl[5] + deltaZl[5] * deltaZl[5]);

    output[0] = deltal[0];
    output[1] = deltal[1];
    output[2] = deltal[2];
    output[3] = deltal[3];
    output[4] = deltal[4];
    output[5] = deltal[5];
    quadrangleArea(N1, N2, N3, N4, &output[6]);
    quadrangleArea(N1, N2, N6, N5, &output[7]);
    quadrangleArea(N1, N4, N8, N5, &output[8]);
    quadrangleArea(N2, N3, N7, N6, &output[9]);
    quadrangleArea(N3, N4, N8, N7, &output[10]);
    quadrangleArea(N5, N6, N7, N8, &output[11]);
    hexahedronTHVolume(N1, N2, N3, N4, N5, N6, N7, N8, &output[12]);
    output[13] = center[0];
    output[14] = center[1];
    output[15] = center[2];
    return 0;
}

/*
 * getTLMnumbers: Get the TLM numbers. They are:
 * a) number of ports
 * b) number of nodes (sum of number of material elements)
 * c) number of outputs
 * d) number of intersections
 * e) number of boundary elements for each specific element
 * f) number of material elements for each specific element
 * g) number of undefined (not a boundary neither a material) elements for each specific element
 * 
 */
unsigned int getTLMnumbers(const struct dataForSimulation * input,
        struct TLMnumbers* numbers, struct connectionLeveln * intersections, int id) {

    if (input->simulationInput.verboseMode == 1) {
        printf("Initiating TLM numbers... \n");
    }
    unsigned int errorTLMnumber = 0, flag, flagStub, j, k, l, quantityOfPortsToAdd;
    // flag:
    // 0 - Undefined (not a boundary neither a material)
    // 1 - Boundary element
    // 2 - Material element

    // flagStub:
    // 0 - this element does not have stub
    // 1 - this element have stub

    unsigned long long temp[] = {0, 0}, i,
            points[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    // points[0] - number of points saved
    // points[1] - point 0
    // points[2] - point 1
    // points[3] - point 2
    // points[4] - point 3
    // points[5] - point 4
    // points[6] - point 5
    // points[7] - point 6
    // points[8] - point 7


    unsigned long tag;


    clock_t begin_iN = clock();
    // initiate the TLM numbers
    if ((errorTLMnumber = initiateTLMnumbers(numbers)) != 0)
        return errorTLMnumber;
    clock_t end_iN = clock();

    if (input->simulationInput.verboseMode == 1) {
        printf("Done initiating the TLM numbers\n");
        if (input->simulationInput.timingMode == 1) {
            double time_spent_iN = (double) (end_iN - begin_iN) / CLOCKS_PER_SEC;
            printf("Time to allocate the numbers %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_iN * 1e3, time_spent_iN, time_spent_iN / 60.0, time_spent_iN / (60 * 60));
        }
    }
    // initiate the variable that will be used to convert the abstract number of the port
    // to the real number of the port.
    // * the abstract number of the port is maximum number of port that would be used if
    // all the elements are boundary
    // * the real number of the port is the abstract number of the port minus the
    // number of the ports that were boundaries. I use the real number of ports to access
    // the positions in the matrices
    if (input->simulationInput.verboseMode == 1) {
        printf("Initiating the converter from abstract number to real number...\n");
    }
    clock_t begin_ar = clock();
    if ((errorTLMnumber = initiate_aPortToRealPort(input, &(numbers->abstractPortsToReal), id)) != 0) {
        return errorTLMnumber;
    }
    clock_t end_ar = clock();

    if (input->simulationInput.verboseMode == 1) {
        printf("Done initiating the converter from abstract number to real number.\n");

        if (input->simulationInput.timingMode == 1) {
            double time_spent_ar = (double) (end_ar - begin_ar) / CLOCKS_PER_SEC;

            printf("Time to allocate the converter from Abstract number to Real number %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_ar * 1e3, time_spent_ar, time_spent_ar / 60.0, time_spent_ar / (60 * 60));
        }
    }

    if (input->simulationInput.verboseMode == 1) {
        printf("Allocating and saving connection positions in memory...\n");
    }
    clock_t begin_al = clock();
    // this loop can be fully parallelized given that the accesses to the
    // functions below are synchronized. Maybe OpenMP would be a better approach
    // here given that this function is not expected to by very heavy. I think
    // that it would be best to parallelize the 'i' for-loop (but you better analyze it).
    // 1) allocatePointsPort()
    // 2) add_to_aPortToRealPort()
    for (l = 0; l < 100; l++) {
        // DEBUG: show the quantity of specific element
        //    printf("Element code %d, quantity %lld \n", l,
        //            input->mesh.quantityOfSpecificElement[l]);

        for (i = 0; i < input->mesh.quantityOfSpecificElement[l]; i++) {
            // l: element type level
            // i: element level
            // j: boundary and material type level
            // k: different tag numbers for the same boundary type

            // DEBUG: show the number of the element an its quantity
            // printf("Element code %d, elements %lld of %lld \n", l, i+1, 
            //        input->mesh.quantityOfSpecificElement[l]);

            flag = 0;
            flagStub = 0;

            // get the tag number of this element and use the inner two for-loops to
            // see if it is a material or a boundary
            switch (l) {
                case 1: // 2 nodes line
                    tag = input->mesh.elements.Line[i].tag;
                    break;
                case 2: // 3 nodes triangle
                    tag = input->mesh.elements.Triangle[i].tag;
                    break;
                case 3: // 4 nodes quadrangle
                    tag = input->mesh.elements.Quadrangle[i].tag;
                    break;
                case 4: // 4 nodes tetrahedron
                    tag = input->mesh.elements.Tetrahedron[i].tag;
                    break;
                case 5: // 8 nodes hexahedron
                    tag = input->mesh.elements.Hexahedron[i].tag;
                    break;
                case 6: // 6 nodes prism
                    tag = input->mesh.elements.Prism[i].tag;
                    break;
                case 7: // 5 nodes pyramid
                    tag = input->mesh.elements.Pyramid[i].tag;
                    break;
                case 15: // 1 node point
                    tag = input->mesh.elements.Point[i].tag;
                    break;
            }


            // I will use goto to get out of these two inner 'for' loops. Bare in mind
            // that I'm just getting out the loops. You can also think that 
            // I'm going to the next 'i', that is, incrementing i by 1 and going
            // to the next value of i (if any).
            for (j = 0; j < input->equationInput[id].numberOfBoundaries; j++)
                for (k = 0; k < input->boundaryInput[ input->equationInput[id].boundaryNumbers[j] ].quantityOfNumberInput; k++) {
                    // checking if this is a boundary
                    if (tag == input->boundaryInput[ input->equationInput[id].boundaryNumbers[j] ].numberInput[k]) {
                        flag = 1;
                        temp[0] = 0; // flag that indicates that this is a boundary
                        temp[1] = input->equationInput[id].boundaryNumbers[j]; // number of the boundary
                        quantityOfPortsToAdd = 2; // we add two ports
                        numbers->BoundaryElements[l]++; // increment the number of boundary elements
                        // DEBUG: show where we were
                        //                        printf("We were at Boundary (%u, %llu, %u, %u) total %llu\n",
                        //                                l, i, j, k, numbers->BoundaryElements[l]);

                        goto end_for_j_and_for_k_line;
                    }
                }

            // I will get here if the element was not a boundary
            for (j = 0; j < input->equationInput[id].numberOfMaterials; j++)
                for (k = 0; k < input->materialInput[ input->equationInput[id].materialNumbers[j] ].quantityOfNumberInput; k++) {

                    if (tag == input->materialInput[ input->equationInput[id].materialNumbers[j] ].numberInput[k]) {
                        flag = 2;
                        // I will probably have to modify this part when I code to solve for parameters that change
                        if (input->materialInput[ input->equationInput[id].materialNumbers[j] ].generalized_relaxationTime != 0) {
                            flagStub = 1;
                        }
                        quantityOfPortsToAdd = 1;
                        numbers->MaterialElements[l]++;
                        // DEBUG: show where we were
                        //                        printf("We were at Material (%u, %llu, %u, %u) total %llu\n",
                        //                                l, i, j, k, numbers->MaterialElements[l]);
                        goto end_for_j_and_for_k_line;
                    }
                }


end_for_j_and_for_k_line:
            // we only get here if the node was detected as boundary or material.
            // Otherwise, flag = 0;
            if (flag != 0) {
                switch (l) {
                    case 1: // 2 nodes line
                        switch (input->equationInput[id].dimen) {
                            case ONE:
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        2 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;

                                points[0] = 1; // number of points to add
                                points[1] = input->mesh.elements.Line[i].N1;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                if (flag == 2)
                                    temp[0]++;

                                points[1] = input->mesh.elements.Line[i].N2;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                break;
                            case TWO:
                                /* FALLTHRU */
                                // break;
                            case THREE:
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        2 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;

                                points[0] = 2; // number of points to add
                                points[1] = input->mesh.elements.Line[i].N1;
                                points[2] = input->mesh.elements.Line[i].N2;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                break;
                        }
                        break;
                    case 2: // 3 nodes triangle
                        switch (input->equationInput[id].dimen) {
                            case ONE:
                                // not defined?

                                break;
                            case TWO:
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        3 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;

                                points[0] = 2; // number of points to add
                                points[1] = input->mesh.elements.Triangle[i].N1;
                                points[2] = input->mesh.elements.Triangle[i].N2;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Triangle[i].N1;
                                points[2] = input->mesh.elements.Triangle[i].N3;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Triangle[i].N2;
                                points[2] = input->mesh.elements.Triangle[i].N3;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                break;
                            case THREE:
                                // the ports start at 1 because 0 is my flag
                                // to indicate that this is a boundary
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        3 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;

                                points[0] = 3; // number of points to add
                                points[1] = input->mesh.elements.Triangle[i].N1;
                                points[2] = input->mesh.elements.Triangle[i].N2;
                                points[3] = input->mesh.elements.Triangle[i].N3;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                break;
                        }

                        break;
                    case 3: // 4 nodes quadrangle
                        switch (input->equationInput[id].dimen) {
                            case ONE:
                                // not defined?

                                break;
                            case TWO:
                                // the ports start at 1 because 0 is my flag
                                // to indicate that this is a boundary
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        4 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;

                                points[0] = 2; // number of points to add
                                points[1] = input->mesh.elements.Quadrangle[i].N1;
                                points[2] = input->mesh.elements.Quadrangle[i].N2;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Quadrangle[i].N2;
                                points[2] = input->mesh.elements.Quadrangle[i].N3;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Quadrangle[i].N3;
                                points[2] = input->mesh.elements.Quadrangle[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Quadrangle[i].N1;
                                points[2] = input->mesh.elements.Quadrangle[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                break;
                            case THREE:
                                // the ports start at 1 because 0 is my flag
                                // to indicate that this is a boundary
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        4 * i;
                                // numbers->abstractPortsToReal[l].portsPerNode * i;

                                points[0] = 4; // number of ports to add
                                points[1] = input->mesh.elements.Quadrangle[i].N1;
                                points[2] = input->mesh.elements.Quadrangle[i].N2;
                                points[3] = input->mesh.elements.Quadrangle[i].N3;
                                points[4] = input->mesh.elements.Quadrangle[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                break;
                        }

                        break;
                    case 4: // 4 nodes tetrahedron
                        // I do the calculation depending on the dimension
                        switch (input->equationInput[id].dimen) {
                            case ONE:
                                // not defined?
                                break;
                            case TWO:
                                // not defined?
                                break;
                            case THREE:
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        4 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;
                                points[0] = 3; // number of ports to add
                                points[1] = input->mesh.elements.Tetrahedron[i].N1;
                                points[2] = input->mesh.elements.Tetrahedron[i].N2;
                                points[3] = input->mesh.elements.Tetrahedron[i].N3;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Tetrahedron[i].N1;
                                points[2] = input->mesh.elements.Tetrahedron[i].N2;
                                points[3] = input->mesh.elements.Tetrahedron[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Tetrahedron[i].N1;
                                points[2] = input->mesh.elements.Tetrahedron[i].N3;
                                points[3] = input->mesh.elements.Tetrahedron[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Tetrahedron[i].N2;
                                points[2] = input->mesh.elements.Tetrahedron[i].N3;
                                points[3] = input->mesh.elements.Tetrahedron[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }
                                break;
                        }

                        break;
                    case 5: // 8 nodes hexahedron
                        // I do the calculation depending on the dimension
                        switch (input->equationInput[id].dimen) {
                            case ONE:
                                // not defined?
                                break;
                            case TWO:
                                // not defined?
                                break;
                            case THREE:
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        6 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;
                                points[0] = 4; // number of points to add
                                points[1] = input->mesh.elements.Hexahedron[i].N1;
                                points[2] = input->mesh.elements.Hexahedron[i].N2;
                                points[3] = input->mesh.elements.Hexahedron[i].N3;
                                points[4] = input->mesh.elements.Hexahedron[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Hexahedron[i].N1;
                                points[2] = input->mesh.elements.Hexahedron[i].N2;
                                points[3] = input->mesh.elements.Hexahedron[i].N5;
                                points[4] = input->mesh.elements.Hexahedron[i].N6;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Hexahedron[i].N1;
                                points[2] = input->mesh.elements.Hexahedron[i].N4;
                                points[3] = input->mesh.elements.Hexahedron[i].N5;
                                points[4] = input->mesh.elements.Hexahedron[i].N8;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Hexahedron[i].N2;
                                points[2] = input->mesh.elements.Hexahedron[i].N3;
                                points[3] = input->mesh.elements.Hexahedron[i].N6;
                                points[4] = input->mesh.elements.Hexahedron[i].N7;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Hexahedron[i].N3;
                                points[2] = input->mesh.elements.Hexahedron[i].N4;
                                points[3] = input->mesh.elements.Hexahedron[i].N7;
                                points[4] = input->mesh.elements.Hexahedron[i].N8;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Hexahedron[i].N5;
                                points[2] = input->mesh.elements.Hexahedron[i].N6;
                                points[3] = input->mesh.elements.Hexahedron[i].N7;
                                points[4] = input->mesh.elements.Hexahedron[i].N8;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }
                                break;
                        }

                        break;
                    case 6: // 6 nodes prism
                        printf("Projection was not implemented yet\n");

                        break;
                    case 7: // 5 nodes pyramid
                        // I do the calculation depending on the dimension
                        switch (input->equationInput[id].dimen) {
                            case ONE:
                                // not defined?
                                break;
                            case TWO:
                                // not defined?
                                break;
                            case THREE:
                                // the ports start at 1 because 0 is my flag
                                // to indicate that this is a boundary
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        5 * i; // numbers->abstractPortsToReal[l].portsPerNode * i;
                                points[0] = 4; // number of points to add
                                points[1] = input->mesh.elements.Pyramid[i].N1;
                                points[2] = input->mesh.elements.Pyramid[i].N2;
                                points[3] = input->mesh.elements.Pyramid[i].N3;
                                points[4] = input->mesh.elements.Pyramid[i].N4;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[0] = 3; // number of points to add
                                points[1] = input->mesh.elements.Pyramid[i].N1;
                                points[2] = input->mesh.elements.Pyramid[i].N2;
                                points[3] = input->mesh.elements.Pyramid[i].N5;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Pyramid[i].N1;
                                points[2] = input->mesh.elements.Pyramid[i].N4;
                                points[3] = input->mesh.elements.Pyramid[i].N5;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Pyramid[i].N2;
                                points[2] = input->mesh.elements.Pyramid[i].N3;
                                points[3] = input->mesh.elements.Pyramid[i].N5;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }

                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                if (flag == 2)
                                    temp[0]++;
                                points[1] = input->mesh.elements.Pyramid[i].N3;
                                points[2] = input->mesh.elements.Pyramid[i].N4;
                                points[3] = input->mesh.elements.Pyramid[i].N5;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }
                                break;
                        }

                        break;
                    case 15: // 1 node point
                        // I do the calculation depending on the dimension
                        switch (input->equationInput[id].dimen) {
                            case ONE:
                                // a point is always a boundary. The question
                                // is how to include this boundary in the 2D and
                                // 3D calculations?
                                // 
                                // temp[0] is my offset that will give me the
                                // abstract number of the node at that interface
                                //
                                // I don't expect flag == 2 for the point.
                                if (flag == 2)
                                    temp[0] = numbers->abstractPortsToReal[l].previousMaximumAbstractPort +
                                        i; // numbers->abstractPortsToReal[l].portsPerNode * i;


                                points[0] = 1; // number of points to add
                                points[1] = input->mesh.elements.Point[i].N1;
                                if ((errorTLMnumber = allocatePointsPort(points, intersections,
                                        quantityOfPortsToAdd, temp)) != 0) {
                                    if (errorTLMnumber == 1) {
                                        numbers->Intersections++;
                                        errorTLMnumber = 0;
                                    } else {
                                        return errorTLMnumber;
                                    }
                                }
                                break;
                            case TWO:
                                // a point is always a boundary. The question
                                // is how to include this boundary in the 2D and
                                // 3D calculations?
                                /* FALLTHRU */
                            case THREE:
                                break;
                        }
                        break;
                }
            } else { //if (flag == 0) {
                numbers->NotDefinedElements[l]++;
            }

            if ((errorTLMnumber =
                    add_to_aPortToRealPort(flag, flagStub, j, &(numbers->abstractPortsToReal[l]))) != 0)
                // add_to_a[bstract]PortToRealPort. I add the number of the node
                return errorTLMnumber;
        }
    }

    clock_t end_al = clock();
    if (input->simulationInput.verboseMode == 1) {
        printf("Done allocating and saving connection positions in memory.\n");
        if (input->simulationInput.timingMode == 1) {
            double time_spent_al = (double) (end_al - begin_al) / CLOCKS_PER_SEC;

            printf("Time to allocate and save connection positions in memory %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_al * 1e3, time_spent_al, time_spent_al / 60.0, time_spent_al / (60 * 60));
        }
    }

    if (input->simulationInput.verboseMode == 1) {
        printf("Wrapping the TLM numbers...\n");
    }
    clock_t begin_wN = clock();
    // wrap the TLM numbers, taking away points that are not needed
    if ((errorTLMnumber = wrapTLMnumbers(input, numbers, id)) != 0)
        return errorTLMnumber;
    clock_t end_wN = clock();
    // if number of nodes is equal to zero, it did not find any material elements
    // associated with the tags provided
    if (numbers->Nodes == 0)
        return 6610;


    if (input->simulationInput.verboseMode == 1) {
        printf("Done wrapping the TLM numbers.\n");
        if (input->simulationInput.timingMode == 1) {
            double time_spent_wN = (double) (end_wN - begin_wN) / CLOCKS_PER_SEC;

            printf("Time to wrap the numbers %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_wN * 1e3, time_spent_wN, time_spent_wN / 60.0, time_spent_wN / (60 * 60));
        }
    }

    if (input->simulationInput.verboseMode == 1) {
        printf("Wrapping the connection variable...\n");
    }
    clock_t begin_wC = clock();
    // now that we have the connections, we can wrap the intersection variable
    if ((errorTLMnumber = wrap_size_connectionLeveln(intersections)) != 0)
        return errorTLMnumber;
    clock_t end_wC = clock();

    if (input->simulationInput.verboseMode == 1) {
        printf("Done wrapping connection variable.\n");
        if (input->simulationInput.timingMode == 1) {
            double time_spent_wC = (double) (end_wC - begin_wC) / CLOCKS_PER_SEC;

            printf("Time to wrap the connection %g ms (or %g s, or %g min, or %g hours).\n",
                    time_spent_wC * 1e3, time_spent_wC, time_spent_wC / 60.0, time_spent_wC / (60 * 60));
        }
    }

    return 0;
}

/*
 * initiateTLMnumbers: allocate and initiate the TLM numbers
 */
unsigned int initiateTLMnumbers(struct TLMnumbers* numbers) {
    numbers->Ports = 0;
    numbers->StubPorts = 0;
    numbers->Nodes = 0;
    numbers->Output = 0;
    numbers->Points_Output = 0;
    numbers->Intersections = 0;

    // initiate them as 0. I have a list of 100 elements type, that's why I 
    // allocate 100. But I don't use all of these positions. I might reduce it
    // to a more usable number in the future. However, for developmental purposes,
    // I will leave it as 100.
    if ((numbers->BoundaryElements = (unsigned long long *) calloc(100,
            sizeof (unsigned long long))) == NULL)
        return 8730;

    if ((numbers->MaterialElements = (unsigned long long *) calloc(100,
            sizeof (unsigned long long))) == NULL)
        return 8731;

    if ((numbers->NotDefinedElements = (unsigned long long *) calloc(100,
            sizeof (unsigned long long))) == NULL)
        return 8732;

    return 0;
}

/*
 * terminateTLMnumbers: deallocate the TLM numbers
 */
unsigned int terminateTLMnumbers(struct TLMnumbers* numbers) {
    free(numbers->BoundaryElements);
    numbers->BoundaryElements = NULL;

    free(numbers->MaterialElements);
    numbers->MaterialElements = NULL;

    free(numbers->NotDefinedElements);
    numbers->NotDefinedElements = NULL;

    terminate_aPortToRealPort(&(numbers->abstractPortsToReal));
}

/*
 * initiate_aPortToRealPort: allocate aPortToRealPort. This variable is used
 * to convert abstract ports to real ports. What are abstract ports? At the beginning
 * of the simulation I don't known which elements are going to be used as materials
 * or boundaries, neither if they will need stubs. Abstract ports consider all the 
 * possibilities (such as all elements are materials and all of them need stubs).
 * Then, we convert the abstract port number to the real port number to get the position
 * of the port in the matrix equations
 */
unsigned int initiate_aPortToRealPort(const struct dataForSimulation * input,
        struct aPortToRealPort** Ports, int id) {
    double percentage = 0, percentageStub = 0;
    // the values used for percentage and percentageStub are arbitrary
    unsigned int type = 0;

    if ((*Ports = (struct aPortToRealPort*) malloc(sizeof (struct aPortToRealPort)*100)) == NULL)
        return 8734;

    // allocating for each element code
    for (unsigned int i = 0; i < 100; i++) {
        switch (i) {
            case 1: // 2 nodes line
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // expected to be all material
                        percentage = 0.02; // allocate 2%
                        type = 2; // as boundary
                        break;
                    case TWO:
                        // expected to be all boundary
                        percentage = 0.02; // allocate 2%
                        type = 3; // as material
                        break;
                    case THREE:
                        // undefined?
                        percentage = 0.02; // allocate 2%
                        type = 3; // as material
                        break;
                }
                break;



            case 2: // 3 nodes triangle
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case TWO:
                        // expected to be all material
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case THREE:
                        // expected to be all boundary
                        percentage = 0.01; // allocate 1%
                        type = 3; // as material
                        break;
                }
                break;



            case 3: // 4 nodes quadrangle
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case TWO:
                        // expected to be all material
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case THREE:
                        // expected to be all boundary
                        percentage = 0.01; // allocate 1%
                        type = 3; // as material
                        break;
                }
                break;



            case 4: // 4 nodes tetrahedron
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case TWO:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case THREE:
                        // expected to be all material
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                }
                break;



            case 5: // 8 nodes hexahedron
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case TWO:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case THREE:
                        // expected to be all material
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                }
                break;



            case 6: // 6 nodes prism
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case TWO:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case THREE:
                        // expected to be all material
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                }
                break;



            case 7: // 5 nodes pyramid
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case TWO:
                        // undefined?
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                    case THREE:
                        // expected to be all material
                        percentage = 0.01; // allocate 1%
                        type = 2; // as boundary
                        break;
                }
                break;



            case 15: // 1 node point
                switch (input->equationInput[id].dimen) {
                    case ONE:
                        // expected to be all boundary
                        percentage = 0.02; // allocate 2%
                        type = 3; // as material
                        break;
                    case TWO:
                        // expected to be all boundary
                        percentage = 0.02; // allocate 2%
                        type = 3; // as material
                        break;
                    case THREE:
                        // expected to be all boundary
                        percentage = 0.02; // allocate 2%
                        type = 3; // as material
                        break;
                }
                break;
            default:
                percentage = 0;
                type = 0;
                break;
        }

        // get how many ports per node this element contains
        (*Ports)[i].portsPerNode = getNumberOfPortsGivenElement(i, input->equationInput[id].dimen);

        if (i == 0) {
            // the number of the node shall start at 0
            (*Ports)[i].previousMaximumAbstractNode = 0;

            // I've to start at 1 because 0 is my code to when an intersection
            // has a boundary
            (*Ports)[i].previousMaximumAbstractPort = 1;

        } else {
            // that's the part that is preventing this loop to be parallelized
            // maybe I can run it serially because it is not supposed to take
            // a long time to run. The other option is to run without knowing the
            // previous number of node, then, I run a serial loop to get it
            (*Ports)[i].previousMaximumAbstractNode =
                    input->mesh.quantityOfSpecificElement[i - 1] +
                    (*Ports)[i - 1].previousMaximumAbstractNode;

            (*Ports)[i].previousMaximumAbstractPort =
                    (*Ports)[i - 1].portsPerNode * input->mesh.quantityOfSpecificElement[i - 1] +
                    (*Ports)[i - 1].previousMaximumAbstractPort;
        }


        // setting the pointerType
        (*Ports)[i].pointerType = type;

        (*Ports)[i].quantitySaved = 0;
        // this 1 guarantees that we will have at least one position to save
        (*Ports)[i].quantityAllocated = 1 + percentage *
                input->mesh.quantityOfSpecificElement[i];

        if (((*Ports)[i].nodesNumbers = (unsigned long long*) malloc(
                sizeof (unsigned long long)*
                (*Ports)[i].quantityAllocated)) == NULL)
            return 8735;



        // setting the pointerTypeStub based on the equation we are going to solve
        switch (input->equationInput[id].typeS) {
            case HYPERBOLIC_DIFFUSION:
                /* FALLTHRU */
                // break;
            case HYPERBOLIC_HEAT:
                /* FALLTHRU */
                // break;
            case HYPERBOLIC_PENNES:
                // I start assuming that the majority of the elements DO have stub
                (*Ports)[i].pointerTypeStub = 2;
                percentageStub = 0.02; // allocate 2% in case we have elements
                // that DO NOT have stub
                break;
            case DIFFUSION:
                /* FALLTHRU */
                // break;
            case HEAT:
                /* FALLTHRU */
                // break;
            case PENNES:
                // I start assuming that the majority of the elements DO NOT have stub
                (*Ports)[i].pointerTypeStub = 3;
                percentageStub = 0.02; // allocate 2% in case we have elements
                // that DO have stub

                // this part will have to be modified when we consider parameters that change with time...
                // they will also add one stub
                break;
        }

        (*Ports)[i].quantitySavedStub = 0;
        // this 1 guarantees that we will have at least one position to save
        (*Ports)[i].quantityAllocatedStub = 1 + percentageStub *
                input->mesh.quantityOfSpecificElement[i];

        if (((*Ports)[i].nodesNumbersStub = (unsigned long long*) malloc(
                sizeof (unsigned long long)*
                (*Ports)[i].quantityAllocatedStub)) == NULL)
            return 8741;

    }


    return 0;
}

/*
 * reallocate_aPortToRealPort: reallocate aPortToRealPort
 */
unsigned int reallocate_aPortToRealPort(struct aPortToRealPort* Ports) {

    Ports->quantityAllocated = 5 + (unsigned long long) 1.2 * Ports->quantityAllocated;
    // add 20% of the existing plus 5 for each reallocation (in case 20% is < 1)
    // these values are arbitrary

    if ((Ports->nodesNumbers = (unsigned long long*) realloc(Ports->nodesNumbers,
            Ports->quantityAllocated * sizeof (unsigned long long))) == NULL)
        return 8736;


    return 0;
}

/*
 * reallocate_aPortToRealPortStub: reallocate aPortToRealPort for Stubs
 */
unsigned int reallocate_aPortToRealPortStub(struct aPortToRealPort* Ports) {

    Ports->quantityAllocatedStub = 5 + (unsigned long long) 1.5 * Ports->quantityAllocatedStub;
    // add 50% of the existing plus 5 for each reallocation (in case 50% is < 1)
    // these values are arbitrary

    if ((Ports->nodesNumbersStub = (unsigned long long*) realloc(Ports->nodesNumbersStub,
            Ports->quantityAllocatedStub * sizeof (unsigned long long))) == NULL)
        return 8740;


    return 0;
}

/*
 * add_to_aPortToRealPort: add the number of the port in the input variable
 */
unsigned int add_to_aPortToRealPort(unsigned int flag, unsigned int flagStub,
        unsigned long long number,
        struct aPortToRealPort* input) {
    // flag
    // 0 - undefined node
    // 1 - boundary node
    // 2 - material node

    // flagStub
    // 0 - does not have stub
    // 1 - have stub

    // pointerType
    // 0 - this element type is all boundary. We have nothing allocated. This is defined later.
    // 1 - this element type is all material. We have nothing allocated. This is defined later.
    // 2 - contains the number of the nodes that are boundary (and not defined)
    // 3 - contains the number of the nodes that are material

    // pointerTypeStub
    // 0 - this element type do not have stub. We have nothing allocated. This is defined later.
    // 1 - this element type have stub. We have nothing allocated. This is defined later.
    // 2 - contains the number of the nodes that do not have stub
    // 3 - contains the number of the nodes that have stub

    unsigned int errorTLMnumber;

    if ((flag == 0 || flag == 1) && input->pointerType != 2)
        goto addStub;
    //return 0; // I can't just exit. I have to treat the stub

    if (flag == 2 && (input->pointerType != 3))
        goto addStub;
    //return 0; // I can't just exit. I have to treat the stub

    if (input->quantitySaved == input->quantityAllocated) {
        if ((errorTLMnumber = reallocate_aPortToRealPort(
                input)) != 0)
            return errorTLMnumber;
    }

    input->nodesNumbers[input->quantitySaved] = number;

    input->quantitySaved++;

    // now for the stub
addStub:
    // I only add the stub number IF the element is material.
    if (flag != 2)
        return 0;

    if (flagStub == 0 && input->pointerTypeStub != 2)
        return 0;

    if (flagStub == 1 && input->pointerTypeStub != 3)
        return 0;

    if (input->quantitySavedStub == input->quantityAllocatedStub) {
        if ((errorTLMnumber = reallocate_aPortToRealPortStub(
                input)) != 0)
            return errorTLMnumber;
    }

    input->nodesNumbersStub[input->quantitySavedStub] = number;

    input->quantitySavedStub++;


    return 0;
}

/*
 * wrapTLMnumbers: remove the unnecessary allocated positions
 */
unsigned int wrapTLMnumbers(const struct dataForSimulation * input,
        struct TLMnumbers* numbers, int id) {
    unsigned int errorTLMnumber = 0;

    for (unsigned int i = 0; i < 100; i++) {
        // Nodes is initiated as zero. Here I'm accumulating the number of nodes
        numbers->Nodes = numbers->Nodes + numbers->MaterialElements[i];

        numbers->Ports = numbers->Ports +
                numbers->abstractPortsToReal[i].portsPerNode * numbers->MaterialElements[i];

        if (numbers->abstractPortsToReal[i].pointerTypeStub == 2) {
            // here the quantitySavedStub has the quantity of nodes that DO NOT have stub
            numbers->StubPorts = numbers->StubPorts +
                    (numbers->MaterialElements[i] -
                    numbers->abstractPortsToReal[i].quantitySavedStub);

        } else if (numbers->abstractPortsToReal[i].pointerTypeStub == 3) {
            // here the quantitySavedStub has the quantity of nodes that DO have stub
            numbers->StubPorts = numbers->StubPorts +
                    numbers->abstractPortsToReal[i].quantitySavedStub;
        }

        if (i == 0) {
            numbers->abstractPortsToReal[i].previousMaximumRealNode = 0;

            numbers->abstractPortsToReal[i].previousMaximumRealPort = 0;

            numbers->abstractPortsToReal[i].previousMaximumRealStubPort = 0;

        } else {
            numbers->abstractPortsToReal[i].previousMaximumRealNode =
                    numbers->abstractPortsToReal[i - 1].previousMaximumRealNode +
                    numbers->MaterialElements[i - 1];

            numbers->abstractPortsToReal[i].previousMaximumRealPort =
                    numbers->abstractPortsToReal[i - 1].previousMaximumRealPort +
                    numbers->abstractPortsToReal[i - 1].portsPerNode * numbers->MaterialElements[i - 1];

            // calculate the number of previous stubs based on the pointerTypeStub of the previous element
            switch (numbers->abstractPortsToReal[i - 1].pointerTypeStub) {
                case 0:
                    // all nodes of the element i-1 DO NOT have stub
                    numbers->abstractPortsToReal[i].previousMaximumRealStubPort =
                            numbers->abstractPortsToReal[i - 1].previousMaximumRealStubPort;
                    break;
                case 1:
                    // all nodes of the element i-1 DO have stub
                    numbers->abstractPortsToReal[i].previousMaximumRealStubPort =
                            numbers->abstractPortsToReal[i - 1].previousMaximumRealStubPort +
                            numbers->MaterialElements[i - 1];
                    break;
                case 2:
                    // here the quantitySavedStub has the quantity of nodes that DO NOT have stub
                    numbers->abstractPortsToReal[i].previousMaximumRealStubPort =
                            numbers->abstractPortsToReal[i - 1].previousMaximumRealStubPort +
                            (numbers->MaterialElements[i - 1] -
                            numbers->abstractPortsToReal[i - 1].quantitySavedStub);
                    break;
                case 3:
                    // here the quantitySavedStub has the quantity of nodes that DO have stub
                    numbers->abstractPortsToReal[i].previousMaximumRealStubPort =
                            numbers->abstractPortsToReal[i - 1].previousMaximumRealStubPort +
                            numbers->abstractPortsToReal[i - 1].quantitySavedStub;
                    break;
            }
        }

        // wrapping the nodesNumbers vector to see if it was all boundary/material
        // or a mixture of both
        if (numbers->MaterialElements[i] == 0) {
            // this element is all boundary
            numbers->abstractPortsToReal[i].pointerType = 0;
            numbers->abstractPortsToReal[i].quantitySaved = 0;

            free(numbers->abstractPortsToReal[i].nodesNumbers);
            numbers->abstractPortsToReal[i].nodesNumbers = NULL;

        } else if ((numbers->BoundaryElements[i] +
                numbers->NotDefinedElements[i]) == 0) {
            // this element is all material
            numbers->abstractPortsToReal[i].pointerType = 1;
            numbers->abstractPortsToReal[i].quantitySaved = 0;

            free(numbers->abstractPortsToReal[i].nodesNumbers);
            numbers->abstractPortsToReal[i].nodesNumbers = NULL;

        } else {
            // these elements are part boundary (and/or not defined) and part material

            // sorting the numbers saved from smallest to biggest
            qsort(numbers->abstractPortsToReal[i].nodesNumbers,
                    numbers->abstractPortsToReal[i].quantitySaved,
                    sizeof (unsigned long long),
                    compareLLU);

            // can we reduce the size of nodesNumbers by inverting the pointerType?
            // that is, if I saved more than half of the elements I will change the type
            // of the pointerType so that it will have less than half
            if (numbers->abstractPortsToReal[i].quantitySaved >
                    (numbers->MaterialElements[i] - numbers->abstractPortsToReal[i].quantitySaved)) {
                // changing the nodesNumberStub type. Inside this function I also
                // change the value of quantitySavedStub
                if ((errorTLMnumber = changeNodesNumbersType(
                        &numbers->abstractPortsToReal[i].nodesNumbers,
                        &numbers->abstractPortsToReal[i].quantitySaved,
                        numbers->MaterialElements[i])) != 0) {
                    return errorTLMnumber;
                }

                // here I change pointerTypeStub
                if (numbers->abstractPortsToReal[i].pointerType == 3) {
                    numbers->abstractPortsToReal[i].pointerType = 2;
                } else {
                    numbers->abstractPortsToReal[i].pointerType = 3;
                }


                // I'm already with the allocation that has the smallest length of nodesNumbersStub
            } else {
                // shortening the pointer to the quantity saved
                if ((numbers->abstractPortsToReal[i].nodesNumbers = (unsigned long long*)
                        realloc(numbers->abstractPortsToReal[i].nodesNumbers,
                        sizeof (unsigned long long)*
                        numbers->abstractPortsToReal[i].quantitySaved)) == NULL)
                    return 8738;
            }
        }

        numbers->abstractPortsToReal[i].quantityAllocated =
                numbers->abstractPortsToReal[i].quantitySaved;




        // wrapping the nodesNumbersStub vector to see if it was all with/without stub
        // or a mixture of both
        if (numbers->MaterialElements[i] == 0
                // this element is all boundary so we don't have stubs here
                || (numbers->abstractPortsToReal[i].pointerTypeStub == 3 &&
                numbers->abstractPortsToReal[i].quantitySavedStub == 0) ||
                // we were allocating the number of nodes that DO have stub and
                // we didn't find any stub. I don't need to include a check that
                // MaterialElements > 0 here because if if it zero it would get into
                // here anyways.
                (numbers->abstractPortsToReal[i].pointerTypeStub == 2 &&
                numbers->abstractPortsToReal[i].quantitySavedStub == numbers->MaterialElements[i])
                // we were allocating the number of nodes that DO NOT have stub and
                // we find that all the nodes of this material element have stub.
                ) {

            numbers->abstractPortsToReal[i].pointerTypeStub = 0;
            numbers->abstractPortsToReal[i].quantitySavedStub = 0;

            free(numbers->abstractPortsToReal[i].nodesNumbersStub);
            numbers->abstractPortsToReal[i].nodesNumbersStub = NULL;

        } else if ((numbers->abstractPortsToReal[i].pointerTypeStub == 3 && numbers->MaterialElements[i] == numbers->abstractPortsToReal[i].quantitySavedStub) ||
                // we were allocating the number of nodes that DO have stub and
                // we find that all the nodes of this material element have stub.
                (numbers->abstractPortsToReal[i].pointerTypeStub == 2 && numbers->abstractPortsToReal[i].quantitySavedStub == 0 && numbers->MaterialElements[i] > 0)) {
            // we were allocating the number of nodes that DO NOT have not stub and we 
            // didn't find any even though we found material nodes. So, we 
            // conclude that the elements of these nodes all have stub

            numbers->abstractPortsToReal[i].pointerTypeStub = 1;
            numbers->abstractPortsToReal[i].quantitySavedStub = 0;

            free(numbers->abstractPortsToReal[i].nodesNumbersStub);
            numbers->abstractPortsToReal[i].nodesNumbersStub = NULL;

        } else {
            // these elements are part with/without stub

            // sorting the numbers saved from smallest to biggest
            qsort(numbers->abstractPortsToReal[i].nodesNumbersStub,
                    numbers->abstractPortsToReal[i].quantitySavedStub,
                    sizeof (unsigned long long),
                    compareLLU);

            // can we reduce the size of nodesNumbersStub by inverting the pointerTypeStub?
            // that is, if I saved more than half of the elements I will change the type
            // of the pointerTypeStub so that it will have less than half
            if (numbers->abstractPortsToReal[i].quantitySavedStub >
                    (numbers->MaterialElements[i] - numbers->abstractPortsToReal[i].quantitySavedStub)) {
                // changing the nodesNumberStub type. Inside this function I also
                // change the value of quantitySavedStub
                if ((errorTLMnumber = changeNodesNumbersType(
                        &numbers->abstractPortsToReal[i].nodesNumbersStub,
                        &numbers->abstractPortsToReal[i].quantitySavedStub,
                        numbers->MaterialElements[i])) != 0) {
                    return errorTLMnumber;
                }

                // here I change pointerTypeStub
                if (numbers->abstractPortsToReal[i].pointerTypeStub == 3) {
                    numbers->abstractPortsToReal[i].pointerTypeStub = 2;
                } else {
                    numbers->abstractPortsToReal[i].pointerTypeStub = 3;
                }


                // I'm already with the allocation that has the smallest length of nodesNumbersStub
            } else {
                // shortening the pointer to the quantity saved
                if ((numbers->abstractPortsToReal[i].nodesNumbersStub = (unsigned long long*)
                        realloc(numbers->abstractPortsToReal[i].nodesNumbersStub,
                        sizeof (unsigned long long)*
                        numbers->abstractPortsToReal[i].quantitySavedStub)) == NULL)
                    return 8742;
            }
        }
        numbers->abstractPortsToReal[i].quantityAllocatedStub =
                numbers->abstractPortsToReal[i].quantitySavedStub;
    }


    // getting the number of outputs
    if (input->equationInput[id].saveScalar) {
        numbers->Output += numbers->Nodes;
        numbers->Points_Output += numbers->Nodes;
    }

    if (input->equationInput[id].saveScalarBetween) {
        numbers->Output += numbers->Intersections;
        numbers->Points_Output += numbers->Intersections;
    }

    if (input->equationInput[id].saveFlux) {
        numbers->Output += numbers->Intersections;
        numbers->Points_Output += numbers->Intersections; // this is for projections in x, y, z
        if (input->equationInput[id].saveScalarBetween == 0)
            numbers->Points_Output += numbers->Intersections; // this is for positions
    }

    return 0;
}

/*
 * changeNodesNumbersType: change the type of the nodesNumbers by changing what
 * is in nodesNumbers. For instance, if nodesNumbers have the number of the nodes
 * that DO have stub, it will change it to have the number of nodes that DO NOT have stub.
 */
unsigned int changeNodesNumbersType(unsigned long long **input,
        unsigned long long *size, unsigned long long maxNodeNumber) {
    unsigned long long i, j, k;
    unsigned int errorTLMnumber;

    // first I copy the content of input
    unsigned long long *input_copied;
    if ((input_copied = malloc(*size * sizeof (unsigned long long))) == NULL) {
        errorTLMnumber = 8743;
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }

    for (i = 0; i < *size; i++) {
        input_copied[i] = (*input)[i];
    }

    // now I realloc input so that it matches the new size
    if ((*input = realloc(*input, (maxNodeNumber - *size) * sizeof (unsigned long long))) == NULL) {
        errorTLMnumber = 8744;
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }

    // changing the content of input
    for (i = 0; i < input_copied[0]; i++) {
        (*input)[i] = i;
    }
    k = i;
    for (j = 0; j < *size; j++) {
        for (i = input_copied[j] + 1; i < input_copied[j + 1]; i++) {
            (*input)[k] = i;
            k++;
        }
    }
    *size = maxNodeNumber - *size;
    return 0;
}

/*
 * terminate_aPortToRealPort: deallocate aPortToRealPort
 */
unsigned int terminate_aPortToRealPort(struct aPortToRealPort** Ports) {

    if ((*Ports) != NULL) {
        for (unsigned int i = 0; i < 100; i++) {
            free((*Ports)[i].nodesNumbers);
            (*Ports)[i].nodesNumbers = NULL;

            free((*Ports)[i].nodesNumbersStub);
            (*Ports)[i].nodesNumbersStub = NULL;
        }

        free((*Ports));
        (*Ports) = NULL;
    }

    return 0;
}

/*
 * getNumberOfPortsGivenElement: return the number of the ports per element
 * given the dimension (1D, 2D, or 3D)
 */
unsigned int getNumberOfPortsGivenElement(unsigned int elementCode,
        enum dimSim dimension) {
    switch (elementCode) {
        case 1: // 2 nodes line
            switch (dimension) {
                case ONE:
                    return 2;

                case TWO:
                    return 1;

                case THREE:
                    // undefined?
                    return 1;
            }



        case 2: // 3 nodes triangle
            switch (dimension) {
                case ONE:
                    // undefined?
                    return 1;

                case TWO:
                    return 3;

                case THREE:
                    return 1;
            }


        case 3: // 4 nodes quadrangle
            switch (dimension) {
                case ONE:
                    // undefined?
                    return 1;

                case TWO:
                    return 4;

                case THREE:
                    return 1;
            }



        case 4: // 4 nodes tetrahedron
            switch (dimension) {
                case ONE:
                    // undefined?
                    return 1;

                case TWO:
                    // undefined?
                    return 1;

                case THREE:
                    return 4;
            }



        case 5: // 8 nodes hexahedron
            switch (dimension) {
                case ONE:
                    // undefined?
                    return 1;

                case TWO:
                    // undefined?
                    return 1;

                case THREE:
                    return 6;
            }



        case 6: // 6 nodes prism
            switch (dimension) {
                case ONE:
                    // undefined?
                    return 1;

                case TWO:
                    // undefined?
                    return 1;

                case THREE:
                    // this model was not made yet
                    return 5;
            }



        case 7: // 5 nodes pyramid
            switch (dimension) {
                case ONE:
                    // undefined?
                    return 1;

                case TWO:
                    // undefined?
                    return 1;

                case THREE:
                    return 5;
            }



        case 15: // 1 node point
            switch (dimension) {
                case ONE:
                    // undefined?
                    return 1;

                case TWO:
                    // undefined?
                    return 1;

                case THREE:
                    // undefined?
                    return 1;
            }

        default:
            return 0;
    }
}

/*
 * getRealPortNumber_fromAbstractPortNumber: receive the abstract number of the port
 * and return the real number of the port, the first port number of that node, and
 * the last port number of that node. This is used to make the connection process.
 * The abstract port comes from the connection variable and is the number of the 
 * port plus 1. The abstract port number assumes that all elements are material 
 * and that we don't have stubs
 */
void getRealPortNumber_fromAbstractPortNumber(
        unsigned long long abstractPortNumber, struct aPortToRealPort* Ports,
        unsigned long long *output) {
    // 0 - position of the Real port
    // 1 - first real port number of the node that contains the port at 0
    // 2 - last real port number of the node that contains the port at 0
    // 3 - position of the Real port with offset from stub
    // 4 - same as 3 + offset from stub
    // 5 - same as 4 + offset from stub and is the stub port number if this node has stub

    unsigned long long j, offset, numberOfNodeWithinElement;
    unsigned int i, offsetStub;

    // the element type of the abstractPortNumber is the type that has at most
    // the number of the abstract port number.
    for (i = 0; i < 99; i++) {
        // I will only see this if this element code has any real node allocated
        if ((Ports[i + 1].previousMaximumRealNode - Ports[i].previousMaximumRealNode) > 0 &&
                Ports[i + 1].previousMaximumAbstractPort > abstractPortNumber) {
            break;
        }
        // if I don't get into the 'if', then, the number of the element that has
        // this port number shall be the element 100.
    }

    // difference between the number of abstract ports and real ports
    offset = Ports[i].previousMaximumAbstractPort -
            Ports[i].previousMaximumRealPort; // this is the minimum value of the offset


    // this contains the number of the nodes that are boundary or undefined
    if (Ports[i].pointerType == 2) {
        for (j = 0; j < Ports[i].quantitySaved; j++) {
            if ((Ports[i].portsPerNode * (Ports[i].nodesNumbers[j] + 1) +
                    Ports[i].previousMaximumAbstractPort) > abstractPortNumber) {
                // when this happens, then, this node number does not influence
                // on the offset
                break;
            }
        }
        // j is the number of boundary nodes
        offset = offset + Ports[i].portsPerNode * (j);


        // this contains the number of the nodes that are material
    } else if (Ports[i].pointerType == 3) {
        for (j = 0; j < Ports[i].quantitySaved; j++) {
            if ((Ports[i].portsPerNode * (Ports[i].nodesNumbers[j] + 1) +
                    Ports[i].previousMaximumAbstractPort) > abstractPortNumber) {

                break;
            }
        }
        // j is the number of material nodes
        // to get the number of boundary nodes, I need to add Ports[i].nodesNumbers[j]
        offset = offset - Ports[i].portsPerNode * (j);

        // if j = quantitySaved, I have to reduce it because I cannot access the 
        // nodesNumber at j = quantitySaved
        if (j == Ports[i].quantitySaved) j--;

        offset = offset +
                Ports[i].portsPerNode * Ports[i].nodesNumbers[j];

    }

    // this is the real port number
    output[0] = (abstractPortNumber - offset);
    // this is the remainder of the division
    // the remainder will be zero if this is the first port number
    // and will be portsPerNode - 1 if this is the last port
    output[1] = (abstractPortNumber
            - Ports[i].previousMaximumAbstractPort) % Ports[i].portsPerNode;

    output[2] = output[0] + (Ports[i].portsPerNode - output[1]);

    output[1] = output[0] - output[1];

    // calculating the effects of stub offset on this node
    switch (Ports[i].pointerTypeStub) {
        case 0:
            // all nodes of the element DO NOT have stub
            output[3] = output[0] + Ports[i].previousMaximumRealStubPort;
            output[4] = output[1] + Ports[i].previousMaximumRealStubPort;
            output[5] = output[2] + Ports[i].previousMaximumRealStubPort;
            break;

            // all nodes of this element DO have stub
        case 1:
            // this is the number of THIS node (starting at zero).
            numberOfNodeWithinElement = (output[1] -
                    (Ports[i].previousMaximumRealPort - 1)) / Ports[i].portsPerNode;

            output[3] = output[0] + Ports[i].previousMaximumRealStubPort
                    + numberOfNodeWithinElement; // offset from the previous nodes of this element
            output[4] = output[1] + Ports[i].previousMaximumRealStubPort
                    + numberOfNodeWithinElement; // offset from the previous nodes of this element
            output[5] = output[2] + Ports[i].previousMaximumRealStubPort
                    + numberOfNodeWithinElement + 1; // offset from the previous nodes of this element + the stub of itself
            break;

            // this contains the nodes that DO NOT have stub
        case 2:
            // this is the number of THIS node (starting at zero).
            numberOfNodeWithinElement = (output[1] -
                    (Ports[i].previousMaximumRealPort - 1)) / Ports[i].portsPerNode;

            // how many stub nodes are there that have a number LESS than
            // this.
            for (j = 0; j < Ports[i].quantitySavedStub; j++) {
                if (Ports[i].nodesNumbers[j] >= numberOfNodeWithinElement) {
                    break;
                }
            }
            // if they are equal then THIS node does not have stub
            if (Ports[i].nodesNumbers[j] == numberOfNodeWithinElement) {
                offsetStub = 0;
            } else {
                offsetStub = 1;
            }
            output[3] = output[0] + Ports[i].previousMaximumRealStubPort
                    + (numberOfNodeWithinElement - j); // offset from the previous nodes of this element
            output[4] = output[1] + Ports[i].previousMaximumRealStubPort
                    + (numberOfNodeWithinElement - j); // offset from the previous nodes of this element
            output[5] = output[2] + Ports[i].previousMaximumRealStubPort
                    + (numberOfNodeWithinElement - j) // offset from the previous nodes of this element
                    + offsetStub; //  + the stub of itself
            break;

            // this contains the nodes that DO have stub
        case 3:
            // this is the number of THIS node (starting at zero).
            numberOfNodeWithinElement = (output[1] -
                    (Ports[i].previousMaximumRealPort - 1)) / Ports[i].portsPerNode;

            // how many nodes without stub are there that have a number LESS than
            // this.
            for (j = 0; j < Ports[i].quantitySavedStub; j++) {
                if (Ports[i].nodesNumbers[j] >= numberOfNodeWithinElement) {
                    break;
                }
            }
            // if they are equal then THIS node does have stub
            if (Ports[i].nodesNumbers[j] == numberOfNodeWithinElement) {
                offsetStub = 1;
            } else {
                offsetStub = 0;
            }
            output[3] = output[0] + Ports[i].previousMaximumRealStubPort
                    + j; // offset from the previous nodes of this element
            output[4] = output[1] + Ports[i].previousMaximumRealStubPort
                    + j; // offset from the previous nodes of this element
            output[5] = output[2] + Ports[i].previousMaximumRealStubPort
                    + j // offset from the previous nodes of this element
                    + offsetStub; //  + the stub of itself
            break;
    }


    return;
}

/*
 * getRealNodeAndPort_fromAbstractNode: return the real number of the node,
 * the real number of the first port of that node, and the real number of the 
 * first port of that node considering the offsets that stubs might do on the 
 * numbering.
 */
void getRealNodeAndPort_fromAbstractNode(unsigned int elementCode,
        unsigned long long abstractNodeNumber, struct aPortToRealPort* Ports,
        unsigned long long *output) {
    unsigned long long j;

    // abstractNodeNumber starts at 0 for every elementCode.
    // the previousMaximumRealNode contains the number of node of the previous element + 1
    // Hence, I can safely do abstractNodeNumber + previousMaximumRealNode.
    // Also note that previousMaximumRealPort has + 1 offset. What I mean by offset?
    // when abastractNodenumber = 0, previousMaximumRealNode = realNumber of this node
    // and previousMaximumRealPort = firstRealPortNumber of this node. I do -1
    // for the realPort number because it has an offset + 1.

    // all nodes of this boundary are material.
    if (Ports[elementCode].pointerType == 1) {
        output[0] = abstractNodeNumber + Ports[elementCode].previousMaximumRealNode;

        output[1] = Ports[elementCode].portsPerNode * abstractNodeNumber +
                Ports[elementCode].previousMaximumRealPort;

        // this contains the number of the nodes that are boundary or undefined
    } else if (Ports[elementCode].pointerType == 2) {
        for (j = 0; j < Ports[elementCode].quantitySaved; j++) {
            // I do a loop until I can find the boundary-element that has an abstract
            // number greater than the abstract number I'm asking to get the real numbers.
            // That is, the boundary-elements from 0 until j-1 will influence the calculated
            // real numbers that I'm asking
            if (Ports[elementCode].nodesNumbers[j] > abstractNodeNumber) {
                break;
            }
        }
        // j represents the quantity of nodes that are boundary and has a number less than the
        // abstractNodeNumber. Then, we remove these extras js from the real
        // node number (output[0])
        output[0] = abstractNodeNumber - j
                + Ports[elementCode].previousMaximumRealNode;

        output[1] = Ports[elementCode].portsPerNode * (abstractNodeNumber - j)
                + Ports[elementCode].previousMaximumRealPort;

        // this contains the number of the nodes that are material
    } else if (Ports[elementCode].pointerType == 3) {
        for (unsigned long long j = 0; j < Ports[elementCode].quantitySaved; j++) {
            // I do a loop until I can find the material-element that has an abstract
            // number equal or greater than the abstract number I'm asking to get
            // the real numbers. It is likely that this will end when they are
            // equal and so I should be able to change >= by ==.
            if (Ports[elementCode].nodesNumbers[j] >= abstractNodeNumber) {
                break;
            }
        }
        // j represents the position of the abstractNodeNumber in the vector
        // nodesNumbers. That is, j represents the number of nodes that are
        // material - 1.
        output[0] = j
                + Ports[elementCode].previousMaximumRealNode;

        output[1] = Ports[elementCode].portsPerNode * j
                + Ports[elementCode].previousMaximumRealPort;
    }

    // considering the effect of stub in the numbering

    // all nodes of this element type DO NOT have stub
    if (Ports[elementCode].pointerTypeStub == 0) {
        // number of realPorts + offset done by previous stubs
        output[2] = output[1] + Ports[elementCode].previousMaximumRealStubPort;

        // all nodes of this element type DO have stub
    } else if (Ports[elementCode].pointerTypeStub == 1) {
        // number of realPorts + offset done by previous stubs
        // + offset done by previous nodes that have stub
        output[2] = output[1] + Ports[elementCode].previousMaximumRealStubPort +
                abstractNodeNumber;

        // contains the number of the nodes that DO NOT have stub
    } else if (Ports[elementCode].pointerTypeStub == 2) {
        for (unsigned long long j = 0; j < Ports[elementCode].quantitySavedStub; j++) {
            // I do a loop until I can find the node without stub that has an abstract
            // number equal or greater than the abstract number I'm asking to get
            // the real numbers.
            if (Ports[elementCode].nodesNumbers[j] >= abstractNodeNumber) {
                break;
            }
        }
        // j represents the number of nodes without stub before abstractNodeNumber.
        // Then, to get the number of nodes with stub we do (abstractNodeNumber - j)
        output[2] = output[1] + Ports[elementCode].previousMaximumRealStubPort +
                (abstractNodeNumber - j);

        // contains the number of the nodes that DO have stub
    } else if (Ports[elementCode].pointerTypeStub == 3) {
        for (j = 0; j < Ports[elementCode].quantitySavedStub; j++) {
            // I do a loop until I can find the stubed-node that has an abstract
            // number equal or greater than the abstract number I'm asking to get the real numbers.
            // That is, the stubed-node from 0 until j-1 will influence the calculated
            // stub offset that I'm asking
            if (Ports[elementCode].nodesNumbers[j] >= abstractNodeNumber) {
                break;
            }
        }
        // j represents the quantity of nodes that have stub and has a number less than the
        // abstractNodeNumber. Then, we add these extras js 
        // 
        // number of realPorts + offset done by previous stubs
        // + offset done by previous nodes that have stub
        output[2] = output[1] + Ports[elementCode].previousMaximumRealStubPort +
                j;
    }


    return;
}

/*
 * getBetweenPointFromRealPortNumber: receive the real port number, finds
 * what element code it is, and return the position of this port
 */
unsigned int getBetweenPointFromRealPortNumber(struct aPortToRealPort *Ports,
        unsigned long long realPort, double *x, double *y, double *z,
        const struct dataForSimulation * input) {

    unsigned long long nodeNumber, portOrder;
    unsigned int i, errorTLMnumber;
    
    if ( (errorTLMnumber = 
             getNodeNumberAndPortOrderFromRealPortNumber(Ports, realPort, &i, &nodeNumber, &portOrder)) != 0){
         return errorTLMnumber;
     }

    switch (i) {
        case 1: // 2 nodes line
            getBetweenForLine(input, nodeNumber, portOrder, x, y, z);

            break;
        case 2: // 3 nodes triangle
            getBetweenForTriangle(input, nodeNumber, portOrder, x, y, z);

            break;
        case 3: // 4 nodes quadrangle
            getBetweenForQuadrangle(input, nodeNumber, portOrder, x, y, z);

            break;
        case 4: // 4 nodes tetrahedron
            getBetweenForTetrahedron(input, nodeNumber, portOrder, x, y, z);

            break;
        case 5: // 8 nodes hexahedron
            getBetweenForHexahedron(input, nodeNumber, portOrder, x, y, z);

            break;
        case 6: // 6 nodes prism
            printf("Get between prism was not implemented yet\n");

            break;
        case 7: // 5 nodes pyramid
            getBetweenForPyramid(input, nodeNumber, portOrder, x, y, z);

            break;
        case 15: // 1 node point
            printf("Get between point was not implemented yet\n");

            break;
        default:
            printf("Get between default was not implemented yet\n");

            return 0;
    }


    return 0;
}

/*
 * getBetweenForLine: return the position of the points
 * face.
 */
unsigned int getBetweenForLine(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Line[nodeNumber].N1 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Line[nodeNumber].N2 - 1;

            break;
    }

    *x = input->mesh.nodes[P1].x;
    *y = input->mesh.nodes[P1].y;
    *z = input->mesh.nodes[P1].z;

    return 0;
}

/*
 * getBetweenForTriangle: return the position of the middle of the triangular
 * face.
 */
unsigned int getBetweenForTriangle(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Triangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Triangle[nodeNumber].N2 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Triangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Triangle[nodeNumber].N3 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Triangle[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Triangle[nodeNumber].N3 - 1;

            break;
    }

    *x = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x) / 2;
    *y = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y) / 2;
    *z = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z) / 2;

    return 0;
}

/*
 * getBetweenForQuadrangle: return the position of the middle of the quadrangle
 * face.
 */
unsigned int getBetweenForQuadrangle(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N2 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N3 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N3 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N4 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N4 - 1;

            break;
    }

    *x = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x) / 2;
    *y = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y) / 2;
    *z = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z) / 2;

    return 0;
}

/*
 * getBetweenForTetrahedron: return the position of the middle of the triangle.
 */
unsigned int getBetweenForTetrahedron(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2, P3;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N3 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N4 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N4 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N4 - 1;

            break;
    }

    *x = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x) / 3;
    *y = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y) / 3;
    *z = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z) / 3;

    return 0;
}

/*
 * getBetweenForPyramid: return the position of the middle of the quadrangle/triangle.
 */
unsigned int getBetweenForPyramid(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2, P3, P4;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P4 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            *x = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x 
                    + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x) / 4;
            *y = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y 
                    + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y) / 4;
            *z = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z 
                    + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z) / 4;
            
            return 0;
        case 1:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
        case 4:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;
            
            break;
    }
            

    *x = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x) / 3;
    *y = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y) / 3;
    *z = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z) / 3;

    return 0;
}

/*
 * getBetweenForHexahedron: return the position of the middle of the quadrangle
 */
unsigned int getBetweenForHexahedron(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2, P3, P4;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;

            break;
        case 4:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;

            break;
        case 5:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;

            break;
    }

    *x = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x
            + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x) / 4.0;
    *y = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y
            + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y) / 4.0;
    *z = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z
            + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z) / 4.0;

    return 0;
}

/*
 * getProjectionFromRealPortNumber: return the unitary vector going outside the 
 * geometrical element from the port given.
 */
unsigned int getProjectionFromRealPortNumber(struct aPortToRealPort *Ports,
        unsigned long long realPort, double *x, double *y, double *z,
        const struct dataForSimulation * input) {
    
    unsigned long long nodeNumber, portOrder;
    unsigned int i, errorTLMnumber;
    
     if ( (errorTLMnumber = 
             getNodeNumberAndPortOrderFromRealPortNumber(Ports, realPort, &i, &nodeNumber, &portOrder)) != 0){
         return errorTLMnumber;
     }


    switch (i) {
        case 1: // 2 nodes line
            getOutsideProjectionLine(input, nodeNumber, portOrder, x, y, z);

            break;
        case 2: // 3 nodes triangle
            getOutsideProjectionTriangle(input, nodeNumber, portOrder, x, y, z);

            break;
        case 3: // 4 nodes quadrangle
            getOutsideProjectionQuadrangle(input, nodeNumber, portOrder, x, y, z);

            break;
        case 4: // 4 nodes tetrahedron
            getOutsideProjectionTetrahedron(input, nodeNumber, portOrder, x, y, z);

            break;
        case 5: // 8 nodes hexahedron
            getOutsideProjectionHexahedron(input, nodeNumber, portOrder, x, y, z);

            break;
        case 6: // 6 nodes prism
            printf("Get projection was not implemented yet\n");

            break;
        case 7: // 5 nodes pyramid
            getOutsideProjectionPyramid(input, nodeNumber, portOrder, x, y, z);

            break;
        case 15: // 1 node point
            printf("Get projection was not implemented yet\n");

            break;
        default:
            return 0;
    }


    return 0;
}

/*
 * getOutsideProjectionLine: return the unitary vector going outside the 
 * line element from the port given.
 */
unsigned int getOutsideProjectionLine(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Line[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Line[nodeNumber].N2 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Line[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Line[nodeNumber].N1 - 1;

            break;
    }

    double Lx, Ly, Lz, L;

    // this vector points on the direction of the triangular face. I wanna
    // a vector that is perpendicular to it
    // This vector has to be parallel to the area vector and must be going
    // inward the triangle.
    Lx = input->mesh.nodes[P1].x - input->mesh.nodes[P2].x;
    Ly = input->mesh.nodes[P1].y - input->mesh.nodes[P2].y;
    Lz = input->mesh.nodes[P1].z - input->mesh.nodes[P2].z;
    L = sqrt(Lx * Lx + Ly * Ly + Lz * Lz);

    *x = Lx / L;
    *y = Ly / L;
    *z = Lz / L;


    return 0;
}

/*
 * getOutsideProjectionTriangle: return the unitary vector going outside the 
 * triangle element from the port given.
 */
unsigned int getOutsideProjectionTriangle(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2, P3;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Triangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Triangle[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Triangle[nodeNumber].N3 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Triangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Triangle[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Triangle[nodeNumber].N2 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Triangle[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Triangle[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Triangle[nodeNumber].N1 - 1;

            break;
    }

    double Lx[2], Ly[2], Lz[2], L, lx, ly, lz, l, A, B, a, b;

    // this vector points on the direction of the triangular face. I wanna
    // a vector that is perpendicular to it
    // This vector has to be parallel to the area vector and must be going
    // inward the triangle.
    Lx[0] = input->mesh.nodes[P1].x - input->mesh.nodes[P2].x;
    Ly[0] = input->mesh.nodes[P1].y - input->mesh.nodes[P2].y;
    Lz[0] = input->mesh.nodes[P1].z - input->mesh.nodes[P2].z;
    // LT_x = a*Ly[0] + b*Lz[0]
    // LT_y = - a*Lx[0]
    // LT_z = - b*Lx[0]
    // Now I have to find 'a' and 'b'

    lx = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x) / 2 -
            (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x) / 3;

    ly = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y) / 2 -
            (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y) / 3;

    lz = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z) / 2 -
            (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z) / 3;



    // Here, I satisfy the requirement that the vector LT is perpendicular to
    // the area vector
    if (input->mesh.nodes[P1].z != 0 || input->mesh.nodes[P2].z != 0 || input->mesh.nodes[P3].z != 0) {
        Lx[1] = input->mesh.nodes[P1].x - input->mesh.nodes[P3].x;
        Ly[1] = input->mesh.nodes[P1].y - input->mesh.nodes[P3].y;
        Lz[1] = input->mesh.nodes[P1].z - input->mesh.nodes[P3].z;

        // first definition to make the equation-writing easier
        A = Ly[0]*(Ly[0] * Lz[1] - Lz[0] * Ly[1]) - Lx[0]*(Lz[0] * Lx[1] - Lx[0] * Lz[1]);

        B = -Lz[0]*(Ly[0] * Lz[1] - Lz[0] * Ly[1]) + Lx[0]*(Lx[0] * Ly[1] - Ly[0] * Lx[1]);

        if (B == 0 && A == 0) { // this is not expected to happen
            // the values of 'a' and 'b' don't matter. I can choose anything
            // to satisfy the requirement that the vector will be perpendicular 
            // to the area vector

            if ((lx * Lz[0] - Lx[0] * lz) == 0 && (lx * Ly[0] - Lx[0] * ly) == 0) {
                // the values of 'a' and 'b' don't influence because the vectors
                // are parallel
                l = sqrt(lx * lx + ly * ly + lz * lz);
                *x = lx / l;
                *y = ly / l;
                *z = lz / l;

                return 0;

            } else if ((lx * Lz[0] - Lx[0] * lz) == 0) {
                // the value of 'a' doesn't influence
                a = 1;
                b = 1;
                if ((lx * Lz[0] - Lx[0] * lz) < 0) {
                    b = -1;
                }
            } else if ((lx * Ly[0] - Lx[0] * ly) == 0) {
                // the value of 'b' doesn't influence
                a = 1;
                b = 1;
                if ((lx * Ly[0] - Lx[0] * ly) < 0) {
                    a = -1;
                }
            } else {
                // the value of both 'a' and 'b' influence
                b = 1;
                a = -0.9 * b * (lx * Lz[0] - Lx[0] * lz) / (lx * Ly[0] - Lx[0] * ly);
            }

        } else if (A == 0) {
            // the value of 'a' will not influence the perpendicularity with
            // the area vector, however, b = 0 to ensure that
            b = 0;
            a = 1;

            if ((lx * Ly[0] - Lx[0] * ly) < 0) {
                a = -1;
            }

        } else if (B == 0) {
            // the value of 'b' will not influence the perpendicularity with
            // the area vector, however, a = 0 to ensure that
            a = 0;
            b = 1;

            if ((lx * Lz[0] - Lx[0] * lz) < 0) {
                b = -1;
            }

        } else {
            // here we have to have a balance between 'a' and 'b' to ensure the
            // perpendicularity with the area vector
            b = 1;
            B = B / A; // then a = b*B
            if ((B * lx * Ly[0] + lx * Lz[0] - B * Lx[0] * ly - Lx[0] * lz) < 0) {
                b = -1;
            }

            a = b*B;
        }

    } else {
        // if all the z's are zero, then the area vector simply points towards z.
        // In this case, b = 0 is the requirement and 'a' can assume any value
        b = 0;
        a = 1;

        if ((lx * Ly[0] - Lx[0] * ly) < 0) {
            a = -1;
        }
    }

    // recycling variables lx, ly, lz
    lx = a * Ly[0] + b * Lz[0];
    ly = -a * Lx[0];
    lz = -b * Lx[0];
    l = sqrt(lx * lx + ly * ly + lz * lz);
    *x = lx / l;
    *y = ly / l;
    *z = lz / l;


    return 0;
}

/*
 * getOutsideProjectionQuadrangle: return the unitary vector going outside the 
 * quadrangle element from the port given.
 */
unsigned int getOutsideProjectionQuadrangle(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    unsigned long long P1, P2, P3, P4;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Quadrangle[nodeNumber].N4 - 1;
            P4 = input->mesh.elements.Quadrangle[nodeNumber].N3 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Quadrangle[nodeNumber].N1 - 1;
            P4 = input->mesh.elements.Quadrangle[nodeNumber].N4 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N3 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Quadrangle[nodeNumber].N2 - 1;
            P4 = input->mesh.elements.Quadrangle[nodeNumber].N1 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Quadrangle[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Quadrangle[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Quadrangle[nodeNumber].N2 - 1;
            P4 = input->mesh.elements.Quadrangle[nodeNumber].N3 - 1;

            break;
    }

    double Lx[2], Ly[2], Lz[2], L, lx, ly, lz, l, A, B, a, b;


    // DEBUG: this is from the center of the face to the center of the triangle

    // this vector points on the direction of the triangular face. I wanna
    // a vector that is perpendicular to it
    // This vector has to be parallel to the area vector and must be going
    // inward the triangle.
    Lx[0] = input->mesh.nodes[P1].x - input->mesh.nodes[P2].x;
    Ly[0] = input->mesh.nodes[P1].y - input->mesh.nodes[P2].y;
    Lz[0] = input->mesh.nodes[P1].z - input->mesh.nodes[P2].z;
    // LT_x = a*Ly[0] + b*Lz[0]
    // LT_y = - a*Lx[0]
    // LT_z = - b*Lx[0]
    // Now I have to find 'a' and 'b'

    lx = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x) / 2 -
            (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x) / 4;

    ly = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y) / 2 -
            (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y) / 4;

    lz = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z) / 2 -
            (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z) / 4;



    // Here, I satisfy the requirement that the vector LT is perpendicular to
    // the area vector
    if (input->mesh.nodes[P1].z != 0 || input->mesh.nodes[P2].z != 0 || input->mesh.nodes[P3].z != 0) {
        Lx[1] = input->mesh.nodes[P1].x - input->mesh.nodes[P3].x;
        Ly[1] = input->mesh.nodes[P1].y - input->mesh.nodes[P3].y;
        Lz[1] = input->mesh.nodes[P1].z - input->mesh.nodes[P3].z;

        // first definition to make the equation-writing easier
        A = Ly[0]*(Ly[0] * Lz[1] - Lz[0] * Ly[1]) - Lx[0]*(Lz[0] * Lx[1] - Lx[0] * Lz[1]);

        B = -Lz[0]*(Ly[0] * Lz[1] - Lz[0] * Ly[1]) + Lx[0]*(Lx[0] * Ly[1] - Ly[0] * Lx[1]);

        if (B == 0 && A == 0) { // this is not expected to happen
            // the values of 'a' and 'b' don't matter. I can choose anything
            // to satisfy the requirement that the vector will be perpendicular 
            // to the area vector

            if ((lx * Lz[0] - Lx[0] * lz) == 0 && (lx * Ly[0] - Lx[0] * ly) == 0) {
                // the values of 'a' and 'b' don't influence because the vectors
                // are parallel
                l = sqrt(lx * lx + ly * ly + lz * lz);
                *x = lx / l;
                *y = ly / l;
                *z = lz / l;

                return 0;

            } else if ((lx * Lz[0] - Lx[0] * lz) == 0) {
                // the value of 'a' doesn't influence
                a = 1;
                b = 1;
                if ((lx * Lz[0] - Lx[0] * lz) < 0) {
                    b = -1;
                }
            } else if ((lx * Ly[0] - Lx[0] * ly) == 0) {
                // the value of 'b' doesn't influence
                a = 1;
                b = 1;
                if ((lx * Ly[0] - Lx[0] * ly) < 0) {
                    a = -1;
                }
            } else {
                // the value of both 'a' and 'b' influence
                b = 1;
                a = -0.9 * b * (lx * Lz[0] - Lx[0] * lz) / (lx * Ly[0] - Lx[0] * ly);
            }

        } else if (A == 0) {
            // the value of 'a' will not influence the perpendicularity with
            // the area vector, however, b = 0 to ensure that
            b = 0;
            a = 1;

            if ((lx * Ly[0] - Lx[0] * ly) < 0) {
                a = -1;
            }

        } else if (B == 0) {
            // the value of 'b' will not influence the perpendicularity with
            // the area vector, however, a = 0 to ensure that
            a = 0;
            b = 1;

            if ((lx * Lz[0] - Lx[0] * lz) < 0) {
                b = -1;
            }

        } else {
            // here we have to have a balance between 'a' and 'b' to ensure the
            // perpendicularity with the area vector
            b = 1;
            B = B / A; // then a = b*B
            if ((B * lx * Ly[0] + lx * Lz[0] - B * Lx[0] * ly - Lx[0] * lz) < 0) {
                b = -1;
            }

            a = b*B;
        }

    } else {
        // if all the z's are zero, then the area vector simply points towards z.
        // In this case, b = 0 is the requirement and 'a' can assume any value
        b = 0;
        a = 1;

        if ((lx * Ly[0] - Lx[0] * ly) < 0) {
            a = -1;
        }
    }

    // recycling variables lx, ly, lz
    lx = a * Ly[0] + b * Lz[0];
    ly = -a * Lx[0];
    lz = -b * Lx[0];
    l = sqrt(lx * lx + ly * ly + lz * lz);
    *x = lx / l;
    *y = ly / l;
    *z = lz / l;


    return 0;
}

/*
 * getOutsideProjectionTetrahedron: return the unitary vector going outside the 
 * tetrahedron element from the port given.
 */
unsigned int getOutsideProjectionTetrahedron(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

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
    unsigned long long P1, P2, P3, P4;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N3 - 1;
            P4 = input->mesh.elements.Tetrahedron[nodeNumber].N4 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N4 - 1;
            P4 = input->mesh.elements.Tetrahedron[nodeNumber].N3 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N4 - 1;
            P4 = input->mesh.elements.Tetrahedron[nodeNumber].N2 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Tetrahedron[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Tetrahedron[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Tetrahedron[nodeNumber].N4 - 1;
            P4 = input->mesh.elements.Tetrahedron[nodeNumber].N1 - 1;

            break;
    }

    // lengths of the edges
    double deltaXL[2], deltaYL[2], deltaZL[2];
    double area, areaX, areaY, areaZ;
    double deltaXl, deltaYl, deltaZl;

    deltaXL[0] = input->mesh.nodes[P1].x - input->mesh.nodes[P2].x;
    deltaYL[0] = input->mesh.nodes[P1].y - input->mesh.nodes[P2].y;
    deltaZL[0] = input->mesh.nodes[P1].z - input->mesh.nodes[P2].z;

    deltaXL[1] = input->mesh.nodes[P1].x - input->mesh.nodes[P3].x;
    deltaYL[1] = input->mesh.nodes[P1].y - input->mesh.nodes[P3].y;
    deltaZL[1] = input->mesh.nodes[P1].z - input->mesh.nodes[P3].z;

    // vector of the triangle (P1, P2, P3)
    areaX = deltaYL[0] * deltaZL[1] - deltaYL[1] * deltaZL[0];
    areaY = deltaXL[1] * deltaZL[0] - deltaXL[0] * deltaZL[1];
    areaZ = deltaXL[0] * deltaYL[1] - deltaXL[1] * deltaYL[0];
    area = sqrt(areaX * areaX + areaY * areaY + areaZ * areaZ); // Twice the real area of the triangle. That is, this is the area of a parallelogram

    // vector from the center of the tetrahedron towards the center of the triangle
    deltaXl = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x) / 3
            - (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x) / 4;

    deltaYl = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y) / 3
            - (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y) / 4;

    deltaZl = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z) / 3
            - (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z) / 4;

    if (deltaXl * areaX + deltaYl * areaY + deltaZl * areaZ < 0) {
        *x = -areaX / area;
        *y = -areaY / area;
        *z = -areaZ / area;
    } else {
        *x = areaX / area;
        *y = areaY / area;
        *z = areaZ / area;
    }
    return 0;
}

/*
 * getOutsideProjectionPyramid: return the unitary vector going outside the 
 * pyramid element from the port given.
 */
unsigned int getOutsideProjectionPyramid(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {

    /* pyramid nomenclature.
     *                                  
     *         vertex 4  ________________________________ vertex 3
     *                  /\___    area 5        _________/|           
     *                 /3    \____  __________/         /         
     *                /           \/vertex 5   1       4|            
     *               /a         __/\__                 /   
     *              /e       __/      \__    a        a|  
     *             /r     __/            \__e        e/      
     *            /a   __/                 r\__      r|
     *           /  __/       area 2      a    \__  a/ 
     * vertex 1 /__/______________________________\__| vertex 2
     *      
     *                            
     */
    unsigned long long P1, P2, P3, P4, P5;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P4 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            P5 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P4 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            P5 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P4 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P5 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            P4 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P5 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
        case 4:
            P1 = input->mesh.elements.Pyramid[nodeNumber].N3 - 1;
            P2 = input->mesh.elements.Pyramid[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Pyramid[nodeNumber].N1 - 1;
            P4 = input->mesh.elements.Pyramid[nodeNumber].N2 - 1;
            P5 = input->mesh.elements.Pyramid[nodeNumber].N5 - 1;

            break;
    }

    // lengths of the edges
    double deltaXL[2], deltaYL[2], deltaZL[2];
    double area, areaX, areaY, areaZ;
    double deltaXl, deltaYl, deltaZl;

    deltaXL[0] = input->mesh.nodes[P1].x - input->mesh.nodes[P2].x;
    deltaYL[0] = input->mesh.nodes[P1].y - input->mesh.nodes[P2].y;
    deltaZL[0] = input->mesh.nodes[P1].z - input->mesh.nodes[P2].z;

    // quadrangle
    if (portOrder == 0) {
        // BASED ON MY ASSUMPTION THAT THE POINTS OF THE QUADRANGLE FORM A PLAN
        deltaXL[1] = input->mesh.nodes[P1].x - input->mesh.nodes[P4].x;
        deltaYL[1] = input->mesh.nodes[P1].y - input->mesh.nodes[P4].y;
        deltaZL[1] = input->mesh.nodes[P1].z - input->mesh.nodes[P4].z;

        // triangles
    } else {
        deltaXL[1] = input->mesh.nodes[P1].x - input->mesh.nodes[P5].x;
        deltaYL[1] = input->mesh.nodes[P1].y - input->mesh.nodes[P5].y;
        deltaZL[1] = input->mesh.nodes[P1].z - input->mesh.nodes[P5].z;
    }


    // vector of the triangle|quadrangle (P1, P2, P3|P5)
    areaX = deltaYL[0] * deltaZL[1] - deltaYL[1] * deltaZL[0];
    areaY = deltaXL[1] * deltaZL[0] - deltaXL[0] * deltaZL[1];
    areaZ = deltaXL[0] * deltaYL[1] - deltaXL[1] * deltaYL[0];
    area = sqrt(areaX * areaX + areaY * areaY + areaZ * areaZ); // Twice the real area of the triangle. That is, this is the area of a parallelogram

    // vector from the center of the pyramid towards the center of the triangle|quadrangle
    if (portOrder == 0) {
        deltaXl = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x) / 4
                - (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x + input->mesh.nodes[P5].x) / 5;

        deltaYl = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y) / 4
                - (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y + input->mesh.nodes[P5].y) / 5;

        deltaZl = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z) / 4
                - (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z + input->mesh.nodes[P5].z) / 5;

    } else {
        deltaXl = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P5].x) / 3
                - (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x + input->mesh.nodes[P5].x) / 5;

        deltaYl = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P5].y) / 3
                - (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y + input->mesh.nodes[P5].y) / 5;

        deltaZl = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P5].z) / 3
                - (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z + input->mesh.nodes[P5].z) / 5;
    }


    if (deltaXl * areaX + deltaYl * areaY + deltaZl * areaZ < 0) {
        *x = -areaX / area;
        *y = -areaY / area;
        *z = -areaZ / area;
    } else {
        *x = areaX / area;
        *y = areaY / area;
        *z = areaZ / area;
    }
    return 0;
}

/*
 * getOutsideProjectionHexahedron: return the unitary vector going outside the 
 * hexahedron element from the port given. I'M ASSUMING THAT THE QUADRANGLES
 * THAT FORM THE FACE OF THE HEXAHEDRON FORM AN UNIQUE PLAN. Then, any three
 * points of the quadrangle define the plan.
 */
unsigned int getOutsideProjectionHexahedron(const struct dataForSimulation * input,
        const unsigned long long nodeNumber, const unsigned long long portOrder,
        double *x, double *y, double *z) {


    /* hexahedral nomenclature.
     * 
     * 
     *            vertex 4____________________________ vertex 3
     *                   /|                          /|
     *                  / .                         / |
     *                 /  .      5                 /  |
     *                /   |                       /   |
     *               /    .    a                 /    |
     *              /     .   e                 /     |
     *             /      |  r     area 1      /      |
     *            /       . a                 /       |
     *  vertex 8 /___________________________/vertex 7|
     *          |         | vertex 1         |        |
     *          |        , _.._.._.._.._.._..|_.._ .._| vertex 2
     *          |     3 ,                    |     4 /
     *          |      /   2                 |      /
     *          |   a ,                      |   a /
     *          |  e ,   a                   |  e /
     *          | r /   e       area 6       | r /
     *          |a ,   r                     |a /
     *          | ,   a                      | /
     *          |/___________________________|/
     *      vertex 5                        vertex 6
     * 
     */
    unsigned long long P1, P2, P3, P4, P5, P6, P7, P8;

    switch (portOrder) {
        case 0:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;
            P5 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;
            P6 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;
            P7 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;
            P8 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;

            break;
        case 1:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;
            P5 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P6 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;
            P7 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;
            P8 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;

            break;
        case 2:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;
            P5 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P6 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P7 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;
            P8 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;

            break;
        case 3:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;
            P5 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P6 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;
            P7 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;
            P8 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;

            break;
        case 4:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;
            P5 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P6 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P7 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;
            P8 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;

            break;
        case 5:
            P1 = input->mesh.elements.Hexahedron[nodeNumber].N5 - 1;
            P2 = input->mesh.elements.Hexahedron[nodeNumber].N6 - 1;
            P3 = input->mesh.elements.Hexahedron[nodeNumber].N7 - 1;
            P4 = input->mesh.elements.Hexahedron[nodeNumber].N8 - 1;
            P5 = input->mesh.elements.Hexahedron[nodeNumber].N1 - 1;
            P6 = input->mesh.elements.Hexahedron[nodeNumber].N2 - 1;
            P7 = input->mesh.elements.Hexahedron[nodeNumber].N3 - 1;
            P8 = input->mesh.elements.Hexahedron[nodeNumber].N4 - 1;

            break;
    }

    // BASED ON MY ASSUMPTION THAT THE POINTS OF THE QUADRANGLE FORM A PLAN
    // lengths of the edges
    double deltaXL[2], deltaYL[2], deltaZL[2];
    double area, areaX, areaY, areaZ;
    double deltaXl, deltaYl, deltaZl;

    deltaXL[0] = input->mesh.nodes[P1].x - input->mesh.nodes[P2].x;
    deltaYL[0] = input->mesh.nodes[P1].y - input->mesh.nodes[P2].y;
    deltaZL[0] = input->mesh.nodes[P1].z - input->mesh.nodes[P2].z;

    deltaXL[1] = input->mesh.nodes[P1].x - input->mesh.nodes[P4].x;
    deltaYL[1] = input->mesh.nodes[P1].y - input->mesh.nodes[P4].y;
    deltaZL[1] = input->mesh.nodes[P1].z - input->mesh.nodes[P4].z;

    // vector of the triangle (P1, P2, P4). Based on my assumption that the four points
    // of the quadrangle are in the same plan, this is the vector perpendicular to the
    // quadrangle plane.
    areaX = deltaYL[0] * deltaZL[1] - deltaYL[1] * deltaZL[0];
    areaY = deltaXL[1] * deltaZL[0] - deltaXL[0] * deltaZL[1];
    areaZ = deltaXL[0] * deltaYL[1] - deltaXL[1] * deltaYL[0];
    area = sqrt(areaX * areaX + areaY * areaY + areaZ * areaZ); // Twice the real area of the triangle. That is, this is the area of a parallelogram

    // vector from the center of the Hexahedron towards the center of the triangle
    deltaXl = (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x) / 4
            - (input->mesh.nodes[P1].x + input->mesh.nodes[P2].x + input->mesh.nodes[P3].x + input->mesh.nodes[P4].x
            + input->mesh.nodes[P5].x + input->mesh.nodes[P6].x + input->mesh.nodes[P7].x + input->mesh.nodes[P8].x) / 8;

    deltaYl = (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y) / 4
            - (input->mesh.nodes[P1].y + input->mesh.nodes[P2].y + input->mesh.nodes[P3].y + input->mesh.nodes[P4].y
            + input->mesh.nodes[P5].y + input->mesh.nodes[P6].y + input->mesh.nodes[P7].y + input->mesh.nodes[P8].y) / 8;

    deltaZl = (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z) / 4
            - (input->mesh.nodes[P1].z + input->mesh.nodes[P2].z + input->mesh.nodes[P3].z + input->mesh.nodes[P4].z
            + input->mesh.nodes[P5].z + input->mesh.nodes[P6].z + input->mesh.nodes[P7].z + input->mesh.nodes[P8].z) / 8;

    if (deltaXl * areaX + deltaYl * areaY + deltaZl * areaZ < 0) {
        *x = -areaX / area;
        *y = -areaY / area;
        *z = -areaZ / area;
    } else {
        *x = areaX / area;
        *y = areaY / area;
        *z = areaZ / area;
    }
    return 0;
}


/*
 * getNodeNumberAndPortOrderFromRealPortNumber: return the number of the node and
 * the order of the port given the real number of the node.
 */
unsigned int getNodeNumberAndPortOrderFromRealPortNumber(struct aPortToRealPort *Ports,
        unsigned long long realPort, unsigned int *i_out, unsigned long long *nodeNumber,
        unsigned long long *portOrder) {
    
    unsigned int i;
    // the element type of the realPort is the type that has at most
    // the number of the real port number.
    for (i = 0; i < 99; i++) {
        // I will only see this if this element code has any real node allocated
        if ((Ports[i + 1].previousMaximumRealPort - Ports[i].previousMaximumRealPort) > 0 &&
                Ports[i + 1].previousMaximumRealPort > realPort) {

            break;
        }
        // if I don't get into the 'if', then, the number of the element that has
        // this port number shall be the element 100.
    }

    *nodeNumber = (unsigned long long) ((realPort
            - Ports[i].previousMaximumRealPort) / Ports[i].portsPerNode);

    // 0, 1, 2, ... , or n-1
    *portOrder = (realPort - Ports[i].previousMaximumRealPort) % Ports[i].portsPerNode;
    
    // easy way of getting i out.
    *i_out = i;
    return 0;
}

/*
 * initiateConnectionAndBoundaryCoefficients: initiate the variable connectionAndBoundaryCoefficients.
 * This variable is used during the connection & boundary process
 */
unsigned int initiateConnectionAndBoundaryCoefficients(struct connectionAndBoundaryCoefficients *input) {
    // I will know how many ports by reading startEnd[0]. I might end up reallocating
    // these vectors in future implementations.
    input->reflection = (double *) malloc(sizeof (double)*2);
    input->transmission = (double *) malloc(sizeof (double)*4);
    input->B = (double *) malloc(sizeof (double)*2);
    input->transmission_out = (double *) malloc(sizeof (double)*2);
    input->B_out = (double *) malloc(sizeof (double)*2);
    input->transmission_out_flux = (double *) malloc(sizeof (double)*2);
    input->B_out_flux = (double *) malloc(sizeof (double)*2);


    input->startEnd = (unsigned long long *) malloc(sizeof (unsigned long long)*14);
    input->startEnd[1] = 14;

    input->distanceBetweenPorts = 6;
    input->offsetRealPort = 2;
    input->offsetStubPort = 5;
    input->offsetStubPortFirst = 6;
    input->offsetStubPortLast = 7;
    input->offsetRealPort0 = 2;
    input->offsetStubPort0 = 5;
    input->offsetRealPort1 = 8;
    input->offsetStubPort1 = 11;

    input->portsNumbers = (unsigned long long*) malloc(
            sizeof (unsigned long long)*7);
    input->portsNumbers[0] = 7;

    // I will do only one check in to see if everything was allocated because they
    // use such small amount of memory spaces that it shouldn't be a problem
    if (input->reflection == NULL || input->transmission == NULL || input->B == NULL ||
            input->transmission_out == NULL || input->B_out == NULL ||
            input->transmission_out_flux == NULL || input->B_out_flux == NULL ||
            input->startEnd == NULL || input->portsNumbers == NULL) {
        return 8745;
    }

    return 0;
}

/*
 * terminateConnectionAndBoundaryCoefficients: terminate the variable connectionAndBoundaryCoefficients.
 * This variable is used during the connection & boundary process
 */
unsigned int terminateConnectionAndBoundaryCoefficients(struct connectionAndBoundaryCoefficients *input) {
    free(input->reflection);
    input->reflection = NULL;

    free(input->transmission);
    input->transmission = NULL;

    free(input->B);
    input->B = NULL;

    free(input->transmission_out);
    input->transmission_out = NULL;

    free(input->B_out);
    input->B_out = NULL;

    free(input->transmission_out_flux);
    input->transmission_out_flux = NULL;

    free(input->B_out_flux);
    input->B_out_flux = NULL;

    free(input->startEnd);
    input->startEnd = NULL;

    free(input->portsNumbers);
    input->portsNumbers = NULL;

    return 0;
}