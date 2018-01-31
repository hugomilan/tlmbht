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
    mesh->nameOfOutputFile = malloc( (strlen("--case") + 1)*sizeof(char) );
    strcpy(mesh->nameOfOutputFile, "--case");
    // I'm initiating the variable nameOfOutputFile with the default value.
    

    mesh->scale = (double*) malloc(sizeof (double)*3);

    mesh->scale[0] = 1;
    mesh->scale[1] = 1;
    mesh->scale[2] = 1;

    // initializing the flags as zero.
    mesh->inputNameDefined = 0;
    mesh->inputFormatDefined = 0;
    return 0;
}

/*
 * terminateMeshVariable: terminate the mesh variable used for configuration
 */
unsigned int terminateMeshVariable(struct MeshConfig *mesh) {
    free(mesh->nameOfInputFile);
    mesh->nameOfInputFile = NULL;

    free(mesh->nameOfOutputFile);
    mesh->nameOfOutputFile = NULL;

    free(mesh->scale);
    mesh->scale = NULL;

    return 0;
}

/*
 * printfMeshConfig: prints the mesh configurations that were read
 */
void printfMeshConfig(struct MeshConfig* input) {

    printf("Input data for the Mesh configuration:\n");

    printfNameOfInputFile(input->nameOfInputFile);
    printfInputFormatOutput(&input->inputF, input->nameOfOutputFile);

    printfScale(input->scale);

}

/*
 * printfNameOfInputFile: prints the name of the input file for the mesh
 */
void printfNameOfInputFile(char *input) {
    printf("The name of the input file was: %s\n", input);
}

/*
 * printfInputFormatOutput: prints the name of the input format for the mesh
 */
void printfInputFormatOutput(enum inputFormat *input, char *outputName) {
    printf("The format of the input file was ");

    switch (*input) {
        case GMSH:
            printf("gmsh (.msh, MeshFormat = 2.2), which will be converted to "
                    "tlmbht native with the following name: %s.tbn", outputName);
            break;
        case TLMTBN:
            printf("native tlmbht native (.tbn) mesh format");
            break;
        default:
            printf("Unknown mesh format");
            break;
    }

    printf(".\n");

}

/*
 * printfScale: prints the scale
 */
void printfScale(double *scale) {
    printf("The mesh will be scaled by [%lf, %lf, %lf]\n", scale[0], scale[1], scale[2]);

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
        meshInput->inputNameDefined = 1;

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

    } else if (compareCaseInsensitive(input, "output name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);
        
        // add one for the terminating character '\0'
        meshInput->nameOfOutputFile = realloc(meshInput->nameOfOutputFile, (strlen(input) + 1)*sizeof (char) );
        strcpy(meshInput->nameOfOutputFile, input);


    } else if (compareCaseInsensitive(input, "scale") == 0) {
        char **tempChar;
        int tempInt[] = {200, 200, 200};
        if ((errorTLMnumber = readVectorDoubleLengthThreeInputs(input, meshInput->scale, tempChar, tempInt)) != 0)
            return errorTLMnumber;
        // printf("Scale %f, %f, %f\n", meshInput->scale[0], meshInput->scale[1], meshInput->scale[2]);


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
unsigned int testInputMesh(struct MeshConfig *mesh, char * nameCase) {
    unsigned int errorFound = 0;

    if (mesh->inputNameDefined == 0) {
        sendErrorCodeAndMessage(1875, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }

    // Now I will adjust the nameOfOutputFile
    if (compareCaseInsensitive(mesh->nameOfOutputFile, "--case") == 0) {
        mesh->nameOfOutputFile = realloc(mesh->nameOfOutputFile,
                sizeof (char)*(strlen(nameCase) + 1));

        strcpy(mesh->nameOfOutputFile, nameCase);

    } else if (compareCaseInsensitive(mesh->nameOfOutputFile, "--mesh") == 0) {
        mesh->nameOfOutputFile = (char *) realloc(mesh->nameOfOutputFile,
                sizeof (char)*(strlen(mesh->nameOfInputFile) + 1));

        strcpy(mesh->nameOfOutputFile, mesh->nameOfInputFile);

    }

    return errorFound;
}