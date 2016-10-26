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
 * File:   libmeshconfig.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 24, 2016.
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
 * Contains the functions for reading from the input file and 
 * configuring the mesh inputs.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libmeshconfig.h"
#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * initiateMeshVariable: initialize the mesh variable used for configuration
 * with standard values
 */
unsigned int initiateMeshVariable(struct MeshConfig *mesh) {
    mesh->inputF = GMSH;
    mesh->nameOfInputFile = NULL;

    // initializing the flags as zero.
    mesh->nameDefined = 0;
    mesh->inputFormatDefined = 0;
    return 0;
}

/*
 * terminateMeshVariable: terminate the mesh variable used for configuration
 */
unsigned int terminateMeshVariable(struct MeshConfig *mesh) {
    free(mesh->nameOfInputFile);
    mesh->nameOfInputFile = NULL;
    return 0;
}

/*
 * printfMeshConfig: prints the mesh configurations that were read
 */
void printfMeshConfig(struct MeshConfig* input) {

    printf("Input data for the Mesh configuration:\n");

    printfNameOfInputFile(input->nameOfInputFile);
    printfinputFormat(&input->inputF);

}

/*
 * printfNameOfInputFile: prints the name of the input file for the mesh
 */
void printfNameOfInputFile(char *input) {
    printf("The name of the input file was: %s\n", input);
}

/*
 * printfinputFormat: prints the name of the input format for the mesh
 */
void printfinputFormat(enum inputFormat *input) {
    printf("The format of the input file was ");

    switch (*input) {
        case GMSH:
            printf("gmsh (.msh, MeshFormat = 2.2), which will be converted to "
                    "tlmbht native (.tbn)");
            break;
        case TLMTBN:
            printf("natiave tlmbht native (.tbn) mesh format");
            break;
        default:
            printf("Unknown mesh format");
            break;
    }

    printf(".\n");

}

/*
 * setConfigurationMesh: reads the line and configures the required simulation parameter
 */
unsigned int setConfigurationMesh(char * input, struct MeshConfig * meshInput, int * startEndBrackets) {
    unsigned int errorTLMnumber;

    // DEBUG: shows that got here
    // printf("Inside MESH\n");
    if (*startEndBrackets == 0) {

        if ((errorTLMnumber = testStartBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 1; //defined as the bracket started

    } else if (compareCaseInsensitive(input, "file name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        meshInput->nameOfInputFile = (char*) realloc(meshInput->nameOfInputFile, strlen(input) + 1);

        strcpy(meshInput->nameOfInputFile, input);
        meshInput->nameDefined = 1;

    } else if (compareCaseInsensitive(input, "input format") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%s", input);
        if (compareCaseInsensitive(input, "gmsh") == 0) {
            meshInput->inputF = GMSH;
        } else if (compareCaseInsensitive(input, "tlmtbn") == 0) {
            meshInput->inputF = TLMTBN;
        } else {
            return 3879;
        }

        // flag that the input format was defined
        meshInput->inputFormatDefined = 1;

    } else if (input[0] == '}' && *startEndBrackets == 1) {
        // DEBUG: closing the brackets
        // printf("inside the closure\n");
        if ((errorTLMnumber = testEndBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 0; //defined as the bracket was finalized
        return 9999; // that is my code to indicate that it was done.
    } else {
        return 1147;
    }



    return 0;
}

/*
 * testInputMesh: tests if all the required inputs were read
 */
unsigned int testInputMesh(struct MeshConfig *mesh) {
    unsigned int errorFound = 0;
    if (mesh->nameDefined == 0) {
        sendErrorCodeAndMessage(1875, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }
    return errorFound;
}