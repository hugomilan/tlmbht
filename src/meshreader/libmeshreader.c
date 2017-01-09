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

#include <time.h>
#include "libmeshreader.h"

#include "libgmshreader.h"
#include "libtbnreader.h"
#include "../configs/libmeshconfig.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * meshReaderAndConverter: reads the mesh and convert it to .tbn, if required
 */
unsigned int meshReaderAndConverter(struct MeshConfig * meshInput,
        struct tlmInternalMesh * meshOutput, int timingMode) {
    clock_t begin = clock();
    unsigned errorTLMnumber;

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
    
    clock_t end = clock();

    if (timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("\n\nTime to read the mesh %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }

    return 0;
}




