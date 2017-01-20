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
 * File:   libtlmpennes1dsolver.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on January 18, 2017.
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
 * This contains implementations for solving the Pennes' equation in 1D with the
 * TLM that is independent of specific libraries.
 *
 */

#include <stdlib.h>

#include "libtlmpennes1dsolver.h"

/*
 * testInputTLMPennes1D: tests the inputs of the simulation
 */
unsigned int testInputTLMPennes1D(struct dataForSimulation* input) {
    unsigned int errorTLMnumber = 0;
    unsigned int Element1DDefined = 0;

    // checking the elements defined in the mesh
    // and issuing warning if necessary.
    for (int i = 0; i < 100; i++)
        if (input->mesh.quantityOfSpecificElement[i] != 0) {
            switch (i) {
                case 1: // 2 nodes line
                    Element1DDefined = 1;
                    break;
                case 2: // 3 nodes triangle
                    fprintf(stderr, "\nWARNING: This mesh has the 3 nodes triangle element "
                            "but the called solver is for 1D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 3: // 4 nodes quadrangle
                    fprintf(stderr, "\nWARNING: This mesh has the 4 nodes quadrangle element "
                            "but the called solver is for 1D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 4: // 4 nodes tetrahedron
                    fprintf(stderr, "\nWARNING: This mesh has the 4 nodes tetrahedron element "
                            "but the called solver is for 1D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 5: // 8 nodes hexahedron
                    fprintf(stderr, "\nWARNING: This mesh has the 8 nodes hexahedron element "
                            "but the called solver is for 1D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 6: // 6 nodes prism
                    fprintf(stderr, "\nWARNING: This mesh has the 6 nodes prism element "
                            "but the called solver is for 1D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 7: // 5 nodes pyramid
                    fprintf(stderr, "\nWARNING: This mesh has the 5 nodes pyramid element "
                            "but the called solver is for 1D problems. These elements will not "
                            "be considered in the calculation.\n\n");

                    break;
                case 15: // 1 node point
                    // I do nothing here. The one node point is welcome. It can be
                    // use for inputing sources or boundary conditions. They are 
                    // future implementations

                    break;
            }
        }

    if (Element1DDefined != 1) {
        errorTLMnumber = 6602;
    }

    // TODO what more should I test?

    return errorTLMnumber;
}

