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
 * File:   libtbnreader.c
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
 * Reads the .tbn mesh format and saves into the memory
 *
 */



#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libtbnreader.h"

#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"



/*
 * tbnReader: opens the input file and reads the data
 */
unsigned int tbnReader(struct MeshConfig * input, struct tlmInternalMesh * output) {

    unsigned int errorTLMnumber = 0, lineNumber = 0, startEndBrackets = 0,
            ElementsWereRead = 0, * ElementsToRead = NULL, ElementsBeingRead = 0;

    unsigned long long numberOfNodeReads = 0, numberOfElementsRead = 0;
    char * pline = NULL, *lineOriginal = NULL, *codeForReading = NULL;
    long lenLine = 0;
    enum tlmtbnConfig ConfigPoint = NOTHING_TBN;
    FILE *pfile;
    char* nameOfFile = (char*) malloc(strlen(input->nameOfInputFile) + 5);
    // 1 - .
    // 2 - t
    // 3 - b
    // 4 - n
    // 5 - termination character "\0"
    strcpy(nameOfFile, input->nameOfInputFile);
    strcat(nameOfFile, ".tbn");

    printf("Opening the file %s\n", nameOfFile);

    if ((pfile = fopen(nameOfFile, "r")) == NULL) {//We couldn't find the input file
        sendErrorCodeAndMessage(764, nameOfFile, NULL, NULL, NULL);
        free(nameOfFile);
        nameOfFile = NULL;
        return 764;
    }

    // initiating the mesh
    if ((errorTLMnumber = initiateTlmInternalMesh(output)) != 0) {
        free(nameOfFile);
        nameOfFile = NULL;
        return errorTLMnumber;
    }


    // reads until we find the end-of-file character
    while (getlineTlmbht(&pline, &lenLine, pfile) != 1) {
        // I start the loop checking the variable for error.
        if (errorTLMnumber != 0 && errorTLMnumber != 9999 && errorTLMnumber != 9998) {
            break;
        } else if (errorTLMnumber == 9999) {
            // this is not an error. This is just how I make the flag to indicate
            // some configuration was just finished reading
            ConfigPoint = NOTHING_TBN;
        }
        // DEBUG: Count up of the number of lines
        lineNumber++;

        // DEBUG: Shown the line number and content
        // printf("Line %04u: %s", lineNumber, pline);
        // DEBUG: Shown the line number, quantity of characters in the line and content
        // printf("Line %04u (%u): %s", lineNumber, strlen(pline), pline); 

        // I will copy the pline as a matter to show it for troubleshooting purposes.
        lineOriginal = (char *) realloc(lineOriginal, strlen(pline) + 1);
        strcpy(lineOriginal, pline);

        // DEBUG: Shown the line number and content using lineOriginal variable
        // printf("Original Line %04u: %s", lineNumber, lineOriginal);



        // It returns 0 if it got some useful content on that line.
        // it returns 9998 if it got nothing useful
        // if there's nothing useful or an error, we should go to the next loop iteration.
        if ((errorTLMnumber = getUsefulContent(pline)) != 0)
            continue; // go to next iteration

        switch (ConfigPoint) {
            case NOTHING_TBN:
                if ((errorTLMnumber = setMeshTbnConfigTo(pline, &ConfigPoint)) != 0)
                    continue; // go to the next iteration if we found an error

                break;

            case NODES_TBN:
                if (startEndBrackets == 0) {
                    if ((errorTLMnumber = testStartBrackets(pline)) != 0)
                        continue;

                    startEndBrackets = 1; //defined as the bracket started
                    continue;
                }

                if (pline[0] == '}' && startEndBrackets == 1) {
                    if ((errorTLMnumber = testEndBrackets(pline)) != 0)
                        continue;

                    startEndBrackets = 0; //defined as the bracket was finalized
                    errorTLMnumber = 9999; // that is my code to indicate that it was done.
                    continue;
                }

                if (output->numberOfNode == 0) {
                    sscanf(pline, "%llu", &output->numberOfNode);
                    if ((output->nodes = (struct node *)
                            realloc(output->nodes, sizeof (struct node)*
                            output->numberOfNode)) == NULL) {
                        // we couldn't allocate space for the nodes
                        errorTLMnumber = 5449;
                        continue;
                    }
                    continue;
                }

                if (numberOfNodeReads >= output->numberOfNode) {
                    printf("More nodes were read than the number of nodes provided by tbn input\n");
                    printf("%llu vs. %llu\n", numberOfNodeReads + 1, output->numberOfNode);
                    errorTLMnumber = 6516;
                    continue;
                }

                sscanf(pline, "%lf %lf %lf",
                        &output->nodes[numberOfNodeReads].x,
                        &output->nodes[numberOfNodeReads].y,
                        &output->nodes[numberOfNodeReads].z);

                // Applying the scale factor
                output->nodes[numberOfNodeReads].x =
                        output->nodes[numberOfNodeReads].x * input->scale[0];
                output->nodes[numberOfNodeReads].y =
                        output->nodes[numberOfNodeReads].y * input->scale[1];
                output->nodes[numberOfNodeReads].z =
                        output->nodes[numberOfNodeReads].z * input->scale[2];

                numberOfNodeReads++;
                break;

            case ELEMENTS_TBN:

                if (startEndBrackets == 0) {
                    if ((errorTLMnumber = testStartBrackets(pline)) != 0)
                        continue;

                    startEndBrackets = 1; //define as the bracket started
                    continue;
                }

                if (pline[0] == '}' && startEndBrackets == 1) {
                    if ((errorTLMnumber = testEndBrackets(pline)) != 0)
                        continue;

                    startEndBrackets = 0; //define as the bracket as finalized
                    errorTLMnumber = 9999; // that is my code to indicate that it was done.
                    continue;
                }



                if (ElementsWereRead == 0) {
                    sscanf(pline, "%u ", &output->quantityOfElementTypes);
                    codeForReading = (char *) realloc(codeForReading,
                            sizeof (char)*(4 + (3 + 5) * output->quantityOfElementTypes + 1));

                    ElementsToRead = (unsigned int*) realloc(ElementsToRead,
                            sizeof (unsigned int)*output->quantityOfElementTypes);

                    unsigned int elementCode;
                    unsigned long long quantityOfElements;

                    for (int j = 0; j < output->quantityOfElementTypes; j++) {
                        strcpy(codeForReading, "%*u ");

                        for (int j2 = 0; j2 < j; j2++)
                            strcat(codeForReading, "%*u %*u ");

                        strcat(codeForReading, "%u %llu ");

                        sscanf(pline, codeForReading,
                                &elementCode,
                                &quantityOfElements);

                        ElementsToRead[j] = elementCode;

                        output->quantityOfSpecificElement[elementCode] = quantityOfElements;

                        if ((errorTLMnumber = allocateTLMInternalMeshElementsOnly(output, &elementCode)) != 0) {
                            continue;
                        }
                    }

                    ElementsWereRead = 1;
                    continue;
                }

                // the line of the element contains:
                // Tag Nodes...
                switch (ElementsToRead[ElementsBeingRead]) {
                    case 1: // 2 nodes line
                        sscanf(pline, "%lu %llu %llu",
                                &output->elements.Line[numberOfElementsRead].tag,
                                &output->elements.Line[numberOfElementsRead].N1,
                                &output->elements.Line[numberOfElementsRead].N2);

                        break;
                    case 2: // 3 nodes triangle
                        sscanf(pline, "%lu %llu %llu %llu",
                                &output->elements.Triangle[numberOfElementsRead].tag,
                                &output->elements.Triangle[numberOfElementsRead].N1,
                                &output->elements.Triangle[numberOfElementsRead].N2,
                                &output->elements.Triangle[numberOfElementsRead].N3);

                        break;
                    case 3: // 4 nodes quadrangle
                        sscanf(pline, "%lu %llu %llu %llu %llu",
                                &output->elements.Quadrangle[numberOfElementsRead].tag,
                                &output->elements.Quadrangle[numberOfElementsRead].N1,
                                &output->elements.Quadrangle[numberOfElementsRead].N2,
                                &output->elements.Quadrangle[numberOfElementsRead].N3,
                                &output->elements.Quadrangle[numberOfElementsRead].N4);
                        break;
                    case 4: // 4 nodes tetrahedron
                        sscanf(pline, "%lu %llu %llu %llu %llu",
                                &output->elements.Tetrahedron[numberOfElementsRead].tag,
                                &output->elements.Tetrahedron[numberOfElementsRead].N1,
                                &output->elements.Tetrahedron[numberOfElementsRead].N2,
                                &output->elements.Tetrahedron[numberOfElementsRead].N3,
                                &output->elements.Tetrahedron[numberOfElementsRead].N4);

                        break;
                    case 5: // 8 nodes hexahedron
                        sscanf(pline, "%lu %llu %llu %llu %llu %llu %llu %llu %llu",
                                &output->elements.Hexahedron[numberOfElementsRead].tag,
                                &output->elements.Hexahedron[numberOfElementsRead].N1,
                                &output->elements.Hexahedron[numberOfElementsRead].N2,
                                &output->elements.Hexahedron[numberOfElementsRead].N3,
                                &output->elements.Hexahedron[numberOfElementsRead].N4,
                                &output->elements.Hexahedron[numberOfElementsRead].N5,
                                &output->elements.Hexahedron[numberOfElementsRead].N6,
                                &output->elements.Hexahedron[numberOfElementsRead].N7,
                                &output->elements.Hexahedron[numberOfElementsRead].N8);
                        
                        break;
                    case 6: // 6 nodes prism
                        sscanf(pline, "%lu %llu %llu %llu %llu %llu %llu %llu %llu",
                                &output->elements.Prism[numberOfElementsRead].tag,
                                &output->elements.Prism[numberOfElementsRead].N1,
                                &output->elements.Prism[numberOfElementsRead].N2,
                                &output->elements.Prism[numberOfElementsRead].N3,
                                &output->elements.Prism[numberOfElementsRead].N4,
                                &output->elements.Prism[numberOfElementsRead].N5,
                                &output->elements.Prism[numberOfElementsRead].N6);
                        
                        break;
                    case 7: // 5 nodes pyramid
                        sscanf(pline, "%lu %llu %llu %llu %llu %llu %llu %llu %llu",
                                &output->elements.Pyramid[numberOfElementsRead].tag,
                                &output->elements.Pyramid[numberOfElementsRead].N1,
                                &output->elements.Pyramid[numberOfElementsRead].N2,
                                &output->elements.Pyramid[numberOfElementsRead].N3,
                                &output->elements.Pyramid[numberOfElementsRead].N4,
                                &output->elements.Pyramid[numberOfElementsRead].N5);
                        break;
                    case 15: // 1 node point
                        sscanf(pline, "%lu %llu",
                                &output->elements.Point[numberOfElementsRead].tag,
                                &output->elements.Point[numberOfElementsRead].N1);

                        break;
                    default:
                        errorTLMnumber = 6514;
                        continue;
                        break;
                }

                if (++numberOfElementsRead >=
                        output->quantityOfSpecificElement[ElementsToRead[ElementsBeingRead]]) {
                    numberOfElementsRead = 0;
                    ElementsBeingRead++; // now I will read the next element
                }

                break;
            default:
                errorTLMnumber = 7659;
                break;
        }
    }

    // adjusting the errorTLMnumber variable
    if (errorTLMnumber == 9999 || errorTLMnumber == 9998)
        errorTLMnumber = 0;

    // sending the error message if we got an error
    if (errorTLMnumber != 0)
        sendErrorCodeAndMessage(errorTLMnumber, &lineNumber, lineOriginal, NULL, NULL);

    // testing the output variable
    if (errorTLMnumber != 0 && (errorTLMnumber = testTlmInternalMesh(output)) != 0)
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);

    if (errorTLMnumber == 0)
        printf("The file %s was successfully read.\n", nameOfFile);

    // freeing and closing
    free(pline);
    pline = NULL;
    free(lineOriginal);
    lineOriginal = NULL;
    free(codeForReading);
    codeForReading = NULL;
    free(ElementsToRead);
    ElementsToRead = NULL;
    free(nameOfFile);
    nameOfFile = NULL;
    fclose(pfile);
    pfile = NULL;

    return errorTLMnumber;
}

/*
 * setMeshTbnConfigTo: compare the string of the input to standardized texts.
 * The comparison is case insensitive.
 */
unsigned int setMeshTbnConfigTo(char * input, enum tlmtbnConfig * configInput) {
    if (compareCaseInsensitive(input, "nodes") == 0) {
        *configInput = NODES_TBN;

    } else if (compareCaseInsensitive(input, "elements") == 0) {
        *configInput = ELEMENTS_TBN;

    } else {
        return 1147;
    }
    return 0;
}

