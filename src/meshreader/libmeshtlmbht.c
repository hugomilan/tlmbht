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
 * File:   libmeshtlmbht.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on July 1, 2016.
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

#include "libmeshtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * initiateTlmInternalMesh: initialize the internal mesh variable. This variable
 * will be used as the mesh. The mesh being read will be used to change this variable.
 */
unsigned int initiateTlmInternalMesh(struct tlmInternalMesh *input) {
    input->nodes = NULL;
    input->numberOfNode = 0;

    // allocating NULL
    input->elements.Line = NULL;
    input->elements.Triangle = NULL;
    input->elements.Quadrangle = NULL;
    input->elements.Tetrahedron = NULL;
    input->elements.Hexahedron = NULL;
    input->elements.Prism = NULL;
    input->elements.Pyramid = NULL;
    input->elements.Point = NULL;
    // elements from 0 to 99
    input->quantityOfSpecificElement = (unsigned long long*)
            malloc(sizeof (unsigned long long)*100);
    for (int i = 0; i < 100; i++) {
        input->quantityOfSpecificElement[i] = 0;
    }
    input->quantityOfElementTypes = 0;


    return 0;
}

/*
 * terminateTlmInternalMesh: terminate the internal mesh variable.
 */
unsigned int terminateTlmInternalMesh(struct tlmInternalMesh *input) {
    // Terminating the Nodes
    free(input->nodes);
    input->nodes = NULL;

    // Terminating the Elements
    free(input->elements.Line);
    input->elements.Line = NULL;
    free(input->elements.Triangle);
    input->elements.Triangle = NULL;
    free(input->elements.Quadrangle);
    input->elements.Quadrangle = NULL;
    free(input->elements.Tetrahedron);
    input->elements.Tetrahedron = NULL;
    free(input->elements.Hexahedron);
    input->elements.Hexahedron = NULL;
    free(input->elements.Prism);
    input->elements.Prism = NULL;
    free(input->elements.Pyramid);
    input->elements.Pyramid = NULL;
    free(input->elements.Point);
    input->elements.Point = NULL;

    free(input->quantityOfSpecificElement);
    input->quantityOfSpecificElement = NULL;

    return 0;
}

/*
 * allocateTLMInternalMeshNodesAndElements: We allocate in the memory variables
 * for the nodes and element of the internal mesh, which will be used in the solving process
 */
unsigned int allocateTLMInternalMeshNodesAndElements(struct tlmInternalMesh *input,
        unsigned long long int numberOfNodes, unsigned long long *quantityOfSpecificElement) {

    unsigned int errorTLMnumber;

    // allocating the memory for the nodes
    if ((input->nodes = (struct node*) realloc(input->nodes, sizeof (struct node)*
            numberOfNodes)) == NULL)
        return 5499; // unable to allocate the memory for the nodes

    // allocating the memory for the elements
    for (int i = 0; i < 100; i++) {
        input->quantityOfSpecificElement[i] = quantityOfSpecificElement[i];

        if (input->quantityOfSpecificElement[i] != 0) { // I go here only if it is non-zero
            input->quantityOfElementTypes++;
            if ((errorTLMnumber = allocateTLMInternalMeshElementsOnly(input, &i)) != 0) {
                return errorTLMnumber; // unable to allocate the memory for the elements
            }
        }
    }

    input->numberOfNode = numberOfNodes;
    return 0;
}

/*
 * allocateTLMInternalMeshElementsOnly: We allocate in the memory variables
 * for the element of the internal mesh, which will be used in the solving process
 */
unsigned int allocateTLMInternalMeshElementsOnly(struct tlmInternalMesh *input, const int *i) {
    switch (*i) {
        case 1: // 2 nodes line
            if ((input->elements.Line =
                    realloc(input->elements.Line, sizeof (struct elementLine)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5501; // this means that the computer does not have memory for this.
            break;

        case 2: // 3 nodes triangle
            if ((input->elements.Triangle =
                    realloc(input->elements.Triangle, sizeof (struct elementTriangle)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5502; // this means that the computer does not have memory for this.
            break;

        case 3: // 4 nodes quadrangle
            if ((input->elements.Quadrangle =
                    realloc(input->elements.Quadrangle, sizeof (struct elementQuadrangle)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5503; // this means that the computer does not have memory for this.
            break;

        case 4: // 4 nodes tetrahedron
            if ((input->elements.Tetrahedron =
                    realloc(input->elements.Tetrahedron, sizeof (struct elementTetrahedron)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5504; // this means that the computer does not have memory for this.
            break;

        case 5: // 8 nodes hexahedron
            if ((input->elements.Hexahedron =
                    realloc(input->elements.Hexahedron, sizeof (struct elementHexahedron)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5505; // this means that the computer does not have memory for this.
            break;

        case 6: // 6 nodes prism
            if ((input->elements.Prism =
                    realloc(input->elements.Prism, sizeof (struct elementPrism)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5506; // this means that the computer does not have memory for this.
            break;

        case 7: // 5 nodes pyramid
            if ((input->elements.Pyramid =
                    realloc(input->elements.Pyramid, sizeof (struct elementPyramid)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5507; // this means that the computer does not have memory for this.
            break;

        case 15: // 1 node point
            if ((input->elements.Point =
                    realloc(input->elements.Point, sizeof (struct elementPoint)*
                    input->quantityOfSpecificElement[*i])) == NULL)
                return 5515; // this means that the computer does not have memory for this.
            break;
    }
    return 0;
}

/*
 * writeElementToFileTlmFormat: write the elements data to the file
 */
unsigned int writeElementToFileTlmFormat(struct tlmInternalMesh *input, FILE *saveFile) {
    unsigned long long i;

    fprintf(saveFile, "Elements\n");
    fprintf(saveFile, "{\n");
    // the next line: Total#ElementTypes #ElementType1 #Element1 #ElementType2 #Element2 ...


    // how many elements types this mesh has
    fprintf(saveFile, "%u ", input->quantityOfElementTypes);

    for (unsigned int j = 0; j < 100; j++) {
        if (input->quantityOfSpecificElement[j] != 0) // I go here only if it is non-zero
            // Then I will tell how many of each element is there on the file
            fprintf(saveFile, "%u %llu ", j, input->quantityOfSpecificElement[j]);
    }
    fprintf(saveFile, "\n"); // just go to the next line

    // 1 - 2 nodes line
    for (i = 0; i < input->quantityOfSpecificElement[1]; i++) {
        fprintf(saveFile, "%lu %llu %llu\n",
                input->elements.Line[i].tag,
                input->elements.Line[i].N1,
                input->elements.Line[i].N2);
    }
    // 2 - 3 nodes triangle
    for (i = 0; i < input->quantityOfSpecificElement[2]; i++) {
        fprintf(saveFile, "%lu %llu %llu %llu\n",
                input->elements.Triangle[i].tag,
                input->elements.Triangle[i].N1,
                input->elements.Triangle[i].N2,
                input->elements.Triangle[i].N3);
    }

    // 3 - 4 nodes quadrangle
    for (i = 0; i < input->quantityOfSpecificElement[3]; i++) {
        fprintf(saveFile, "%lu %llu %llu %llu %llu\n",
                input->elements.Quadrangle[i].tag,
                input->elements.Quadrangle[i].N1,
                input->elements.Quadrangle[i].N2,
                input->elements.Quadrangle[i].N3,
                input->elements.Quadrangle[i].N4);
    }

    // 4 - 4 nodes tetrahedron
    for (i = 0; i < input->quantityOfSpecificElement[4]; i++) {
        fprintf(saveFile, "%lu %llu %llu %llu %llu\n",
                input->elements.Tetrahedron[i].tag,
                input->elements.Tetrahedron[i].N1,
                input->elements.Tetrahedron[i].N2,
                input->elements.Tetrahedron[i].N3,
                input->elements.Tetrahedron[i].N4);
    }

    // 5 - 8 nodes hexahedron
    for (i = 0; i < input->quantityOfSpecificElement[5]; i++) {
        fprintf(saveFile, "%lu %llu %llu %llu %llu %llu %llu %llu %llu\n",
                input->elements.Hexahedron[i].tag,
                input->elements.Hexahedron[i].N1,
                input->elements.Hexahedron[i].N2,
                input->elements.Hexahedron[i].N3,
                input->elements.Hexahedron[i].N4,
                input->elements.Hexahedron[i].N5,
                input->elements.Hexahedron[i].N6,
                input->elements.Hexahedron[i].N7,
                input->elements.Hexahedron[i].N8);
    }

    // 6 - 6 nodes prism
    for (i = 0; i < input->quantityOfSpecificElement[6]; i++) {
        fprintf(saveFile, "%lu %llu %llu %llu %llu %llu %llu\n",
                input->elements.Prism[i].tag,
                input->elements.Prism[i].N1,
                input->elements.Prism[i].N2,
                input->elements.Prism[i].N3,
                input->elements.Prism[i].N4,
                input->elements.Prism[i].N5,
                input->elements.Prism[i].N6);
    }

    // 7 - 5 nodes pyramid
    for (i = 0; i < input->quantityOfSpecificElement[7]; i++) {
        fprintf(saveFile, "%lu %llu %llu %llu %llu %llu\n",
                input->elements.Pyramid[i].tag,
                input->elements.Pyramid[i].N1,
                input->elements.Pyramid[i].N2,
                input->elements.Pyramid[i].N3,
                input->elements.Pyramid[i].N4,
                input->elements.Pyramid[i].N5);
    }

    // 15 - 1 node point
    for (i = 0; i < input->quantityOfSpecificElement[15]; i++) {
        fprintf(saveFile, "%lu %llu\n",
                input->elements.Point[i].tag,
                input->elements.Point[i].N1);
    }
    fprintf(saveFile, "}\n");

    return 0;
}

/*
 * testTlmInternalMesh: tests if the mesh was properly read
 */
unsigned int testTlmInternalMesh(struct tlmInternalMesh * input) {
    unsigned int errorTLMnumber = 0;
    if (input->numberOfNode == 0) {
        errorTLMnumber = 2314;
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }

    if (input->quantityOfElementTypes == 0) {
        errorTLMnumber = 2315;
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }
    return errorTLMnumber;
}

/*
 * printfAllMeshData: prints the mesh information
 */
void printfAllMeshData(struct tlmInternalMesh *input) {
    printf("\nData read from the mesh:\n");
    printMeshTlmbhtNodesInformation(input);
    printMeshTlmbhtElementsInformation(input);
    printf("\n");
}

/*
 * printMeshTlmbhtNodesInformation: prints the nodes information
 */
void printMeshTlmbhtNodesInformation(struct tlmInternalMesh *input) {
    printf("Number of nodes read: %llu\n", input->numberOfNode);
}

/*
 * printMeshTlmbhtElementsInformation: prints the elements information
 */
void printMeshTlmbhtElementsInformation(struct tlmInternalMesh *input) {
    unsigned long long totalNumberOfElements = 0;

    for (int i = 0; i < 100; i++)
        totalNumberOfElements = totalNumberOfElements
            + input->quantityOfSpecificElement[i];

    printf("Total number of elements read: %llu.\n", totalNumberOfElements);

    for (int i = 0; i < 100; i++)
        if (input->quantityOfSpecificElement[i] != 0)
            switch (i) {
                case 1: // 2 nodes line
                    printf("Number of elements 2 nodes line: %llu.\n",
                            input->quantityOfSpecificElement[i]);
                    break;
                case 2: // 3 nodes triangle
                    printf("Number of elements 3 nodes triangle: %llu.\n",
                            input->quantityOfSpecificElement[i]);
                    break;
                case 3: // 4 nodes quadrangle
                    printf("Number of elements 4 nodes quadrangle: %llu.\n",
                            input->quantityOfSpecificElement[i]);

                    break;
                case 4: // 4 nodes tetrahedron
                    printf("Number of elements 4 nodes tetrahedron: %llu.\n",
                            input->quantityOfSpecificElement[i]);

                    break;
                case 5: // 8 nodes hexahedron
                    printf("Number of elements 8 nodes hexahedron: %llu.\n",
                            input->quantityOfSpecificElement[i]);

                    break;
                case 6: // 6 nodes prism
                    printf("Number of elements 6 nodes prism: %llu.\n",
                            input->quantityOfSpecificElement[i]);

                    break;
                case 7: // 5 nodes pyramid
                    printf("Number of elements 5 nodes pyramid: %llu.\n",
                            input->quantityOfSpecificElement[i]);

                    break;
                case 15: // 1 node point
                    printf("Number of elements 1 node point: %llu.\n",
                            input->quantityOfSpecificElement[i]);

                    break;
            }
}