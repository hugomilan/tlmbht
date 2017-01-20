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
 * File:   libtlmpennes2dsolver.c
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
 * This contains implementations for solving the Pennes' equation in 3D with the
 * TLM that is independent of specific libraries.
 *
 */

#include <stdlib.h>

#include "libtlmpennes3dsolver.h"


/*
 * testInputTLMPennes3D: tests the inputs of the simulation
 */
unsigned int testInputTLMPennes3D(struct dataForSimulation* input) {
    unsigned int errorTLMnumber = 0;
    unsigned int Element3DDefined = 0;

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
                    // I don't check anything here. This is a boundary
                    // the triangle as an element is future implementation
                    break;
                case 3: // 4 nodes quadrangle
                    // I don't check anything here. This is a boundary
                    // the quadrangle as an element is future implementation
                    break;
                case 4: // 4 nodes tetrahedron
                    Element3DDefined = 1;
                    break;
                case 5: // 8 nodes hexahedron
                    Element3DDefined = 1;
                    break;
                case 6: // 6 nodes prism
                    Element3DDefined = 1;
                    break;
                case 7: // 5 nodes pyramid
                    Element3DDefined = 1;
                    break;
                case 15: // 1 node point
                    // I do nothing here. The one node point is welcome. It can be
                    // use for inputing sources or boundary conditions. They are 
                    // future implementations

                    break;
            }
        }

    if (Element3DDefined != 1) {
        errorTLMnumber = 6601;
    }

    // TODO what more should I test?

    return errorTLMnumber;
}

