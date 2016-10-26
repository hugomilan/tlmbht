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
 * File:   libtlmpennes2dsolver.c
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
 * This contains implementations for solving the Pennes' equation in 2D with the
 * TLM that is independent of specific libraries.
 *
 */

/*
 * TODO (hugomilan#1#): List of possible improvements
 * 
 * 1) Think through about what should be tested in testInputTLMPennes2D().
 */

#include "libtlmpennes2dsolver.h"
#include <stdlib.h>
#include <string.h>

/*
 * testInputTLMPennes2D: tests the inputs of the simulation
 */
unsigned int testInputTLMPennes2D(struct dataForSimulation* input) {
    unsigned int errorTLMnumber = 0;
    unsigned int Element2DDefined = 0;

    // checking the elements defined in the mesh
    // and issuing warning if necessary.
    for (int i = 0; i < 100; i++)
        if (input->mesh.quantityOfSpecificElement[i] != 0) {
            switch (i) {
                case 1: // 2 nodes line
                    // I don't check anything here. This is a boundary
                    // the line as an element is future implementation
                    break;
                case 2: // 3 nodes triangle
                    Element2DDefined = 1;
                    break;
                case 3: // 4 nodes quadrangle
                    // Element2DDefined = 1; // Future implementation
                    break;
                case 4: // 4 nodes tetrahedron
                    fprintf(stderr, "\nWARNING: This mesh has the 4 nodes tetrahedron element "
                            "but the called solver is for 2D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 5: // 8 nodes hexahedron
                    fprintf(stderr, "\nWARNING: This mesh has the 8 nodes hexahedron element "
                            "but the called solver is for 2D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 6: // 6 nodes prism
                    fprintf(stderr, "\nWARNING: This mesh has the 6 nodes prism element "
                            "but the called solver is for 2D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 7: // 5 nodes pyramid
                    fprintf(stderr, "\nWARNING: This mesh has the 5 nodes pyramid element "
                            "but the called solver is for 2D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 15: // 1 node point
                    // I do nothing here. The one node point is welcome. It can be
                    // use for inputing sources or boundary conditions. They are 
                    // future implementations

                    break;
            }
        }

    if (Element2DDefined != 1) {
        errorTLMnumber = 6600;
    }

    return errorTLMnumber;
}

