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
 * File:   libmeshreader.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 26, 2016.
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
 * Verifies if the mesh file exists and convert it if necessary.
 *
 */

#include "libmeshreader.h"

#include "libgmshreader.h"
#include "libtbnreader.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * meshReaderAndConverter: reads the mesh and convert it to .tbn, if required
 */
unsigned int meshReaderAndConverter(struct MeshConfig * meshInput,
        struct tlmInternalMesh * meshOutput) {
    unsigned errorTLMnumber;
    
    printf("The mesh to be read will be scaled by [%lf, %lf, %lf]\n", meshInput->scale[0],
            meshInput->scale[1], meshInput->scale[2]);

    switch (meshInput->inputF) {
        case GMSH: // Reading the gmsh input
            if ((errorTLMnumber = gmshReader(meshInput, meshOutput)) != 0) {
                return errorTLMnumber;
            }

            break;
        case TLMTBN: // Reading the .tbn input
            if ((errorTLMnumber = tbnReader(meshInput, meshOutput)) != 0) {
                return errorTLMnumber;
            }

            break;
        default: // this is not expected
            sendErrorCodeAndMessage(6599, NULL, NULL, NULL, NULL);
            break;
    }

    return 0;
}




