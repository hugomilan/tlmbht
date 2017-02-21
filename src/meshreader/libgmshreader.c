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
 * File:   libgmshreader.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 25, 2016.
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
 * this function converts gmsh (.msh, MeshFormat = 2.2) to the format .tbn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgmshreader.h"

#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"
#include "libtbnreader.h"

/*
 * gmshReader: reads the gmsh input file and save it as .tbn.
 * I implemented a scale variable (which is configurable through the case.tlm)
 * that can be used to scale the mesh, if required
 */
unsigned int gmshReader(struct MeshConfig * input, struct tlmInternalMesh * output) {

    unsigned int errorTLMnumber = 0, lineNumber = 0, elementCode, numberOfTags;
    long unsigned int tagNumber;
    struct dataGmsh2_2 inputGmsh;

    char * pline = NULL, *lineOriginal = NULL, *codeForReading = NULL;
    long lenLine = 0;
    enum gmsh2_2Config ConfigPoint = NOTHING_GMSH;
    FILE *pfile, *pfileWrite;
    char* nameOfFile = (char*) malloc(strlen(input->nameOfInputFile) + 5);
    // 1 - .
    // 2 - m
    // 3 - s
    // 4 - h
    // 5 - termination character "\0"
    strcpy(nameOfFile, input->nameOfInputFile);
    strcat(nameOfFile, ".msh");

    printf("Opening the file %s\n", nameOfFile);

    //We couldn't find the input file
    if ((pfile = fopen(nameOfFile, "r")) == NULL) {
        sendErrorCodeAndMessage(764, nameOfFile, NULL, NULL, NULL);
        free(nameOfFile);
        return 764;
    }

    if ((errorTLMnumber = initiateDataGmsh2_2(&inputGmsh)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        free(nameOfFile);
        return errorTLMnumber;
    }

    // read the entire gmsh file until we found the end-of-file character
        while (getlineTlmbht(&pline, &lenLine, pfile) != 1) {
        // I start the loop checking the variable for error.
        if (errorTLMnumber != 0 && errorTLMnumber != 9999 && errorTLMnumber != 9998
                && (errorTLMnumber < 7652 || errorTLMnumber > 7658)) {
            break; // get out of the while loop

        } else if (errorTLMnumber == 9999) {
            // this is not an error. This is just how I make the flag to indicate
            // some configuration was just finished 
            ConfigPoint = NOTHING_GMSH;

        } else if (errorTLMnumber >= 7652 && errorTLMnumber <= 7658) {
            // this is not an error, just an warning
            sendErrorCodeAndMessage(errorTLMnumber, &lineNumber, lineOriginal, NULL, NULL);
        }


        lineNumber++; //DEBUG: Count up of the number of lines

        // DEBUG: Shown the line number and content
        // printf("Line %04u: %s", lineNumber, pline);
        // DEBUG: Shown the line number, quantity of characters in the line and content
        // printf("Line %04u (%u): %s", lineNumber, strlen(pline), pline); 

        // I will copy the pline as a matter to show it for troubleshooting purposes.
        // + 1 for the end-of-line character
        lineOriginal = (char *) realloc(lineOriginal, strlen(pline) + 1);
        strcpy(lineOriginal, pline);

        // It returns 0 if it got some useful content on that line.
        // it returns 9998 if it got nothing useful
        // if there's nothing useful or an error, we should go to the next loop iteration.
        if ((errorTLMnumber = getUsefulContent(pline)) != 0)
            continue; // go to next iteration

        switch (ConfigPoint) {
            case NOTHING_GMSH:
                if ((errorTLMnumber = setMeshGmshConfigTo(pline, &ConfigPoint)) != 0)
                    continue; // go to the next loop if we found an error

                break;

            case MESH_FORMAT:
                // if this is true, then we finished reading the mesh format section
                if (compareCaseInsensitive(pline, "$EndMeshFormat") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }


                if (inputGmsh.MeshFormat.MeshFormatDefined == 0) {
                    sscanf(pline, "%f %d %d", &inputGmsh.MeshFormat.version,
                            &inputGmsh.MeshFormat.fileType,
                            &inputGmsh.MeshFormat.dataSize);
                    inputGmsh.MeshFormat.MeshFormatDefined = 1;
                    // we only read the mesh format once. If we find ourselves reading
                    // it more than once, my assumption is that it is an error
                } else {
                    errorTLMnumber = 7410;
                    continue;
                }


                break;
            case NODES:
                // testing that the mesh format information as read
                if (inputGmsh.MeshFormat.MeshFormatDefined == 0) {
                    errorTLMnumber = 7411;
                    continue;
                }

                if (compareCaseInsensitive(pline, "$EndNodes") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }

                // reading the number of nodes
                if (inputGmsh.numberOfNode == 0) {
                    sscanf(pline, "%llu", &inputGmsh.numberOfNode);
                    inputGmsh.nodes = (struct node *)
                            realloc(inputGmsh.nodes, sizeof (struct node)*
                            inputGmsh.numberOfNode);

                    // if the algorithm cannot allocate the variable, we will issue
                    // an error and let the user knows
                    if (inputGmsh.nodes == NULL) {
                        errorTLMnumber = 7510;
                        continue;
                    }

                    // reading the nodes
                } else {
                    // in this case, == is equivalent to >= because, to be greater,
                    // it would be required to be equal first. Note that the variable
                    // inputGmsh.numberOfNodeReads is add one at the end
                    if (inputGmsh.numberOfNodeReads == inputGmsh.numberOfNode) {
                        printf("More nodes were read than the number of nodes provided by gmsh input\n");
                        printf("%llu vs. %llu\n", inputGmsh.numberOfNodeReads + 1, inputGmsh.numberOfNode);
                        errorTLMnumber = 6516;
                        continue;
                    }
                    // number of the node (not saved), and positions in x, y, and z
                    sscanf(pline, "%*u %lf %lf %lf",
                            &inputGmsh.nodes[inputGmsh.numberOfNodeReads].x,
                            &inputGmsh.nodes[inputGmsh.numberOfNodeReads].y,
                            &inputGmsh.nodes[inputGmsh.numberOfNodeReads].z);

                    // Applying the scale factor
                    inputGmsh.nodes[inputGmsh.numberOfNodeReads].x =
                            inputGmsh.nodes[inputGmsh.numberOfNodeReads].x * input->scale[0];
                    inputGmsh.nodes[inputGmsh.numberOfNodeReads].y =
                            inputGmsh.nodes[inputGmsh.numberOfNodeReads].y * input->scale[1];
                    inputGmsh.nodes[inputGmsh.numberOfNodeReads].z =
                            inputGmsh.nodes[inputGmsh.numberOfNodeReads].z * input->scale[2];

                    inputGmsh.numberOfNodeReads++;
                }
                break;

            case ELEMENTS:
                // testing that the mesh format information as read
                if (inputGmsh.MeshFormat.MeshFormatDefined == 0) {
                    errorTLMnumber = 7412;
                    continue;
                }

                if (compareCaseInsensitive(pline, "$EndElements") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }

                // reading the number of elements
                if (inputGmsh.numberOfElement == 0) {
                    sscanf(pline, "%llu", &inputGmsh.numberOfElement);
                    inputGmsh.saveElement = (struct elementAny *)
                            realloc(inputGmsh.saveElement, sizeof (struct elementAny)*
                            inputGmsh.numberOfElement);

                    // if the algorithm cannot allocate the variable, we will issue
                    // an error and let the user knows
                    if (inputGmsh.saveElement == NULL) {
                        errorTLMnumber = 7511;
                        continue;
                    }

                    // reading the elements
                } else {
                    // in this case, == is equivalent to >= because, to be greater,
                    // it would be required to be equal first. Note that the variable
                    // inputGmsh.numberOfElementReads is add one at the end
                    if (inputGmsh.numberOfElementReads == inputGmsh.numberOfElement) {
                        printf("More elements were read than the number of elements provided by gmsh input\n");
                        printf("%llu vs. %llu\n", inputGmsh.numberOfElementReads + 1, inputGmsh.numberOfElement);
                        errorTLMnumber = 6517;
                        continue;
                    }
                    // the line of the element contains:
                    // elementNumber elementCode numberOfTagsOfThisElement Tags... Nodes...
                    //
                    // The default sequency of tags is 1st the number of the physical
                    // entity to which the element belongs (this is the only
                    // tag read by the software), 2nd the number of elementary
                    // geometrical entity to which the element belongs, 3rd the number of mesh partitions to which
                    // the element belongs, and 4th and so on are the partition ids. Negative
                    // partition ids indicate ghost cells.
                    //
                    // This code requires at least the first two tags. We will issue
                    // an error in case we don't see it.
                    sscanf(pline, "%*u %u %u %lu", &elementCode, &numberOfTags, &tagNumber);

                    if (numberOfTags < 2) {
                        errorTLMnumber = 6515;
                        continue;
                    }

                    // The ones I will not read
                    codeForReading = (char *) realloc(codeForReading,
                            sizeof (char)*(3 + numberOfTags)*4 + 1);
                    // elementNumber elementCode numberOfTagsOfThisElement Tags1 Tag2 Nodes...
                    strcpy(codeForReading, "%*u %*u %*u %*u %*u ");

                    // I'm not reading the others tag numbers (if any)
                    for (int i = 0; i < numberOfTags - 2; i++)
                        strcat(codeForReading, "%*u ");
                    // quantity of characters for the codeForReading string:
                    // (3 + numberOfTags)*4 = Quantity of unsigned ints not read
                    // elementNumber, elementCode, numberOfTags, Tags...
                    // 4 = '%*u '
                    // 1 = '/0'
                    // 5 * numberOfNodes. 5 = '%llu '

                    switch (elementCode) {
                        case 1: // 2 nodes line
                            codeForReading = (char *) realloc(codeForReading,
                                    sizeof (char)*(3 + numberOfTags)*4 + 1 + 5 * 2);

                            strcat(codeForReading, "%llu %llu");

                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].elementCode = 1;
                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].tag = tagNumber;

                            sscanf(pline, codeForReading, &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N1,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N2);

                            break;
                        case 2: // 3 nodes triangle
                            codeForReading = (char *) realloc(codeForReading,
                                    sizeof (char)*(3 + numberOfTags)*4 + 1 + 5 * 3);

                            strcat(codeForReading, "%llu %llu %llu");

                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].elementCode = 2;
                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].tag = tagNumber;

                            sscanf(pline, codeForReading, &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N1,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N2,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N3);

                            break;
                        case 3: // 4 nodes quadrangle
                            codeForReading = (char *) realloc(codeForReading,
                                    sizeof (char)*(3 + numberOfTags)*4 + 1 + 5 * 3);

                            strcat(codeForReading, "%llu %llu %llu %llu");

                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].elementCode = 3;
                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].tag = tagNumber;

                            sscanf(pline, codeForReading, &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N1,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N2,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N3,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N4);
                            
                            break;
                        case 4: // 4 nodes tetrahedron
                            codeForReading = (char *) realloc(codeForReading,
                                    sizeof (char)*(3 + numberOfTags)*4 + 1 + 5 * 4);

                            strcat(codeForReading, "%llu %llu %llu %llu");

                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].elementCode = 4;
                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].tag = tagNumber;

                            sscanf(pline, codeForReading, &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N1,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N2,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N3,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N4);

                            break;
                        case 5: // 8 nodes hexahedron
                            codeForReading = (char *) realloc(codeForReading,
                                    sizeof (char)*(3 + numberOfTags)*4 + 1 + 5 * 8);

                            strcat(codeForReading, "%llu %llu %llu %llu %llu %llu %llu %llu");

                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].elementCode = 5;
                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].tag = tagNumber;

                            sscanf(pline, codeForReading, &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N1,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N2,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N3,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N4,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N5,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N6,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N7,
                                    &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N8);
                            
                            break;
                        case 6: // 6 nodes prism
                            errorTLMnumber = 6512;
                            continue;
                            break;
                        case 7: // 5 nodes pyramid
                            errorTLMnumber = 6513;
                            continue;
                            break;
                        case 15: // 1 node point
                            codeForReading = (char *) realloc(codeForReading,
                                    sizeof (char)*(3 + numberOfTags)*4 + 1 + 5);

                            strcat(codeForReading, "%llu");

                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].elementCode = 15;
                            inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].tag = tagNumber;

                            sscanf(pline, codeForReading, &inputGmsh.saveElement
                                    [inputGmsh.numberOfElementReads].N1);

                            break;
                        default:
                            errorTLMnumber = 6514;
                            continue;
                            break;
                    }

                    inputGmsh.quantityOfSpecificElement[elementCode] =
                            inputGmsh.quantityOfSpecificElement[elementCode] + 1;
                    inputGmsh.numberOfElementReads++;
                }
                break;

            case PHYSICAL_NAMES:
                if (compareCaseInsensitive(pline, "$EndPhysicalNames") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }
                errorTLMnumber = 7652;
                break;

            case PERIODIC:
                if (compareCaseInsensitive(pline, "$EndPeriodic") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }
                errorTLMnumber = 7653;
                break;

            case NODE_DATA:
                if (compareCaseInsensitive(pline, "$EndNodeData") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }
                errorTLMnumber = 7654;
                break;

            case ELEMENT_DATA:
                if (compareCaseInsensitive(pline, "$EndElementData") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }
                errorTLMnumber = 7655;
                break;

            case ELEMENT_NODE_DATA:
                if (compareCaseInsensitive(pline, "$EndElementNodeData") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }
                errorTLMnumber = 7656;
                break;

            case INTERPOLATION_SCHEME:
                if (compareCaseInsensitive(pline, "$EndInterpolationScheme") == 0) {
                    errorTLMnumber = 9999;
                    continue;
                }
                errorTLMnumber = 7657;
                break;

            case UNRECOGNIZED_HEADER:
                if ((strncmp(pline, "$", 1) == 0)) {
                    errorTLMnumber = 9999;
                    continue;
                }

                errorTLMnumber = 7658;
                break;

                // this is not expected to happen
            default:
                errorTLMnumber = 7659;
                break;
        }
    }

    // converting the TLM codes from the safe values to zero if no error was detected
    // and sending an error message if it happened
    if (errorTLMnumber >= 7652 && errorTLMnumber <= 7658) {
        // this is not an error, just an warning
        sendErrorCodeAndMessage(errorTLMnumber, &lineNumber, lineOriginal, NULL, NULL);
        errorTLMnumber = 0;
    } else if (errorTLMnumber == 9999 || errorTLMnumber == 9998) {
        errorTLMnumber = 0;
    }

    // in case of an error, send the error message
    if (errorTLMnumber != 0)
        sendErrorCodeAndMessage(errorTLMnumber, &lineNumber, lineOriginal, NULL, NULL);

    // freeing and closing file
    free(pline);
    pline = NULL;
    free(lineOriginal);
    lineOriginal = NULL;
    free(codeForReading);
    codeForReading = NULL;
    if (fclose(pfile) == EOF) {
        sendErrorCodeAndMessage(345, NULL, NULL, NULL, NULL);
        errorTLMnumber = 345;
    }

    // in case of an error, terminating the Gmsh data and returning
    if (errorTLMnumber != 0 && errorTLMnumber != 9999 && errorTLMnumber != 9998
            && (errorTLMnumber < 7652 || errorTLMnumber > 7658)) {
        // terminating the Gmsh data and showing error message if so
        if ((errorTLMnumber = terminateDataGmsh2_2(&inputGmsh)) != 0)
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);

        return errorTLMnumber;
    }


    printf("The file %s was successfully read.\n", nameOfFile);

    // changing the name of file for saving the content read
    nameOfFile = (char*) realloc(nameOfFile, sizeof (char)*(strlen(input->nameOfOutputFile) + 5));
    // 4 (extension) + 1 ('\0')
    strcpy(nameOfFile, input->nameOfOutputFile);
    strcat(nameOfFile, ".tbn");

    // Now it is writing mode. If there is a file with the same name, I'm sorry,
    // it will be replaced
    if ((pfileWrite = fopen(nameOfFile, "w")) == NULL) {
        // error opening the file
        sendErrorCodeAndMessage(764, nameOfFile, NULL, NULL, NULL);
        errorTLMnumber = 764;
    }
    // at this point, all the elements and nodes were read. Now it is time to
    // convert them.
    // I will first try to read the internal mesh data to the memory and save the 
    // data into the file at the same time. If we cannot do this, I will try to 
    // deallocate the data, save the data into the file, and them read the file again.
    if (errorTLMnumber == 0 &&
            (errorTLMnumber = readAndWriteGmsh(output, &inputGmsh, pfileWrite)) != 0) {
        // 9999 = no memory. Try the other approach
        // trying to save the file, deallocate from the memory, and then read
        if (errorTLMnumber != 9999 ||
                (errorTLMnumber = writeThenReadGmsh(output, &inputGmsh, pfileWrite, input)) != 0)
            sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }


    // closing and freeing the files
    // First check if it is != than NULL because it can be already closed in the
    // functions above    
    if (pfileWrite != NULL && (fclose(pfileWrite) == EOF)) {
        sendErrorCodeAndMessage(345, NULL, NULL, NULL, NULL);
        errorTLMnumber = 345;
    }

    free(nameOfFile);
    nameOfFile = NULL;

    if ((errorTLMnumber = terminateDataGmsh2_2(&inputGmsh)) != 0)
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);

    return errorTLMnumber;
}

/*
 * setMeshGmshConfigTo: compare the string of the input to standardized texts.
 * The comparison is case insensitive.
 */
unsigned int setMeshGmshConfigTo(char *input, enum gmsh2_2Config * config) {
    if (compareCaseInsensitive(input, "$MeshFormat") == 0) {
        *config = MESH_FORMAT;
    } else if (compareCaseInsensitive(input, "$Nodes") == 0) {
        *config = NODES;
    } else if (compareCaseInsensitive(input, "$Elements") == 0) {
        *config = ELEMENTS;
    } else if (compareCaseInsensitive(input, "$PhysicalNames") == 0) {
        *config = PHYSICAL_NAMES;
    } else if (compareCaseInsensitive(input, "$Periodic") == 0) {
        *config = PERIODIC;
    } else if (compareCaseInsensitive(input, "$NodeData") == 0) {
        *config = NODE_DATA;
    } else if (compareCaseInsensitive(input, "$ElementData") == 0) {
        *config = ELEMENT_DATA;
    } else if (compareCaseInsensitive(input, "$ElementNodeData") == 0) {
        *config = ELEMENT_NODE_DATA;
    } else if (compareCaseInsensitive(input, "$InterpolationScheme") == 0) {
        *config = INTERPOLATION_SCHEME;
    } else if ((strncmp(input, "$", 1) == 0)) {
        *config = UNRECOGNIZED_HEADER;
    } else {
        return 7659;
    }



    return 0;
}

/*
 * initiateDataGmsh2_2: initiate the gmsh file with standard values
 */
unsigned int initiateDataGmsh2_2(struct dataGmsh2_2 *input) {
    input->MeshFormat.MeshFormatDefined = 0;

    // elements from 0 to 99
    input->quantityOfSpecificElement = (unsigned long long*)
            malloc(sizeof (unsigned long long)*100);
    for (int i = 0; i < 100; i++) {
        input->quantityOfSpecificElement[i] = 0;
    }
    input->saveElement = NULL;
    input->numberOfElementReads = 0;
    input->numberOfElement = 0;


    input->nodes = NULL;
    input->numberOfNode = 0;
    input->numberOfNodeReads = 0;
    return 0;
}

/*
 * terminateDataGmsh2_2: terminate the gmsh file
 */
unsigned int terminateDataGmsh2_2(struct dataGmsh2_2 *input) {

    free(input->quantityOfSpecificElement);
    input->quantityOfSpecificElement = NULL;
    free(input->saveElement);
    input->saveElement = NULL;
    free(input->nodes);
    input->nodes = NULL;

    return 0;
}

/*
 * deallocateOnlyTheNodeGmsh: This is an strategy to release partial memory
 * while doing the processing. This function will free only the nodes.
 */
unsigned int deallocateOnlyTheNodeGmsh(struct dataGmsh2_2 *input) {

    free(input->nodes);
    input->nodes = NULL;

    return 0;
}

/*
 * deallocateOnlyTheElementGmsh: This is an strategy to release partial memory
 * while doing the processing. This function will free only the elements.
 */
unsigned int deallocateOnlyTheElementGmsh(struct dataGmsh2_2 *input) {

    free(input->quantityOfSpecificElement);
    input->quantityOfSpecificElement = NULL;
    free(input->saveElement);
    input->saveElement = NULL;

    return 0;
}

/*
 * readAndWriteGmsh: Try to allocate the data into the memory and to save into
 * the file at the same time.
 */
unsigned int readAndWriteGmsh(struct tlmInternalMesh * output,
        struct dataGmsh2_2 * input, FILE * saveFile) {
    unsigned long long i, *quantityOfElementTypesSaved;
    unsigned int errorTLMnumber;

    // allocating in the memory the variables for nodes and elements
    if ((errorTLMnumber = allocateTLMInternalMeshNodesAndElements(output, input->numberOfNode,
            input->quantityOfSpecificElement)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return 9999; // my code to say that we faced a problem of memory
    }

    // variable used for incrementing the quantity of elements saved.
    quantityOfElementTypesSaved = (unsigned long long*)
            malloc(sizeof (unsigned long long)*100);
    for (int i = 0; i < 100; i++) {
        quantityOfElementTypesSaved[i] = 0;
    }

    fprintf(saveFile, "Nodes\n");
    fprintf(saveFile, "{\n");
    fprintf(saveFile, "%llu\n", input->numberOfNode);
    // Nodes: saving and reading in the internal mesh
    for (i = 0; i < input->numberOfNode; i++) {
        fprintf(saveFile, "%.17g %.17g %.17g\n", input->nodes[i].x,
                input->nodes[i].y, input->nodes[i].z);

        output->nodes[i].x = input->nodes[i].x;
        output->nodes[i].y = input->nodes[i].y;
        output->nodes[i].z = input->nodes[i].z;
    }
    fprintf(saveFile, "}\n");

    // at this point, I can deallocate the Gmsh nodes
    if ((errorTLMnumber = deallocateOnlyTheNodeGmsh(input)) != 0)
        return errorTLMnumber;

    // before writing information about the element, I have to allocate them
    // into the output variable because I cannot know if the elements type
    // are going to be in order
    for (i = 0; i < input->numberOfElement; i++) {
        switch (input->saveElement[i].elementCode) {
            case 1: // 2 nodes line
                output->elements.Line[quantityOfElementTypesSaved[1]].tag =
                        input->saveElement[i].tag;
                output->elements.Line[quantityOfElementTypesSaved[1]].N1 =
                        input->saveElement[i].N1;
                output->elements.Line[quantityOfElementTypesSaved[1]].N2 =
                        input->saveElement[i].N2;

                quantityOfElementTypesSaved[1]++;

                break;
            case 2: // 3 nodes triangle
                output->elements.Triangle[quantityOfElementTypesSaved[2]].tag =
                        input->saveElement[i].tag;
                output->elements.Triangle[quantityOfElementTypesSaved[2]].N1 =
                        input->saveElement[i].N1;
                output->elements.Triangle[quantityOfElementTypesSaved[2]].N2 =
                        input->saveElement[i].N2;
                output->elements.Triangle[quantityOfElementTypesSaved[2]].N3 =
                        input->saveElement[i].N3;

                quantityOfElementTypesSaved[2]++;

                break;
            case 3: // 4 nodes quadrangle
                output->elements.Quadrangle[quantityOfElementTypesSaved[3]].tag =
                        input->saveElement[i].tag;
                output->elements.Quadrangle[quantityOfElementTypesSaved[3]].N1 =
                        input->saveElement[i].N1;
                output->elements.Quadrangle[quantityOfElementTypesSaved[3]].N2 =
                        input->saveElement[i].N2;
                output->elements.Quadrangle[quantityOfElementTypesSaved[3]].N3 =
                        input->saveElement[i].N3;
                output->elements.Quadrangle[quantityOfElementTypesSaved[3]].N4 =
                        input->saveElement[i].N4;

                quantityOfElementTypesSaved[3]++;

                break;
            case 4: // 4 nodes tetrahedron
                output->elements.Tetrahedron[quantityOfElementTypesSaved[4]].tag =
                        input->saveElement[i].tag;
                output->elements.Tetrahedron[quantityOfElementTypesSaved[4]].N1 =
                        input->saveElement[i].N1;
                output->elements.Tetrahedron[quantityOfElementTypesSaved[4]].N2 =
                        input->saveElement[i].N2;
                output->elements.Tetrahedron[quantityOfElementTypesSaved[4]].N3 =
                        input->saveElement[i].N3;
                output->elements.Tetrahedron[quantityOfElementTypesSaved[4]].N4 =
                        input->saveElement[i].N4;

                quantityOfElementTypesSaved[4]++;

                break;
            case 5: // 8 nodes hexahedron
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].tag =
                        input->saveElement[i].tag;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N1 =
                        input->saveElement[i].N1;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N2 =
                        input->saveElement[i].N2;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N3 =
                        input->saveElement[i].N3;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N4 =
                        input->saveElement[i].N4;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N5 =
                        input->saveElement[i].N5;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N6 =
                        input->saveElement[i].N6;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N7 =
                        input->saveElement[i].N7;
                output->elements.Hexahedron[quantityOfElementTypesSaved[5]].N8 =
                        input->saveElement[i].N8;

                quantityOfElementTypesSaved[5]++;

                break;
            case 6: // 6 nodes prism
                output->elements.Prism[quantityOfElementTypesSaved[6]].tag =
                        input->saveElement[i].tag;
                output->elements.Prism[quantityOfElementTypesSaved[6]].N1 =
                        input->saveElement[i].N1;
                output->elements.Prism[quantityOfElementTypesSaved[6]].N2 =
                        input->saveElement[i].N2;
                output->elements.Prism[quantityOfElementTypesSaved[6]].N3 =
                        input->saveElement[i].N3;
                output->elements.Prism[quantityOfElementTypesSaved[6]].N4 =
                        input->saveElement[i].N4;
                output->elements.Prism[quantityOfElementTypesSaved[6]].N5 =
                        input->saveElement[i].N5;
                output->elements.Prism[quantityOfElementTypesSaved[6]].N6 =
                        input->saveElement[i].N6;

                quantityOfElementTypesSaved[6]++;

                break;
            case 7: // 5 nodes pyramid
                output->elements.Pyramid[quantityOfElementTypesSaved[7]].tag =
                        input->saveElement[i].tag;
                output->elements.Pyramid[quantityOfElementTypesSaved[7]].N1 =
                        input->saveElement[i].N1;
                output->elements.Pyramid[quantityOfElementTypesSaved[7]].N2 =
                        input->saveElement[i].N2;
                output->elements.Pyramid[quantityOfElementTypesSaved[7]].N3 =
                        input->saveElement[i].N3;
                output->elements.Pyramid[quantityOfElementTypesSaved[7]].N4 =
                        input->saveElement[i].N4;
                output->elements.Pyramid[quantityOfElementTypesSaved[7]].N5 =
                        input->saveElement[i].N5;

                quantityOfElementTypesSaved[7]++;

                break;
            case 15: // 1 node point
                output->elements.Point[quantityOfElementTypesSaved[15]].tag =
                        input->saveElement[i].tag;
                output->elements.Point[quantityOfElementTypesSaved[15]].N1 =
                        input->saveElement[i].N1;

                quantityOfElementTypesSaved[15]++;

                break;
        }

    }

    // at this point, I can deallocate the elements
    if ((errorTLMnumber = deallocateOnlyTheElementGmsh(input)) != 0)
        return errorTLMnumber;

    // Now that I have the data into the output variable, I can go on and save the
    // data into the file
    if ((errorTLMnumber = writeElementToFileTlmFormat(output, saveFile)) != 0)
        return errorTLMnumber;

    return 0;
}

/*
 * writeThenReadGmsh: Write the Gmsh mesh read into the file then read it to
 * the internal mesh
 */
unsigned int writeThenReadGmsh(struct tlmInternalMesh * output,
        struct dataGmsh2_2 * inputGmsh, FILE * saveFile, struct MeshConfig * input) {
    unsigned int errorTLMnumber;

    fprintf(stderr, "WARNING: Not enough memory to open and save the mesh at the same time. "
            "We will save the mesh file, deallocate variables, and try to read the saved mesh.\n");
    // deallocating what was previous allocated (if any)
    if ((errorTLMnumber = terminateTlmInternalMesh(output)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }


    // only save the file, that is, do not save anything to output
    if ((errorTLMnumber = onlyWriteGmsh2_2(inputGmsh, saveFile)) != 0) {
        // error here. Treat it
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }


    // closing the file to be read by the functions below
    fclose(saveFile);
    saveFile = NULL;

    // now deallocate what we have read from Gmsh
    if ((errorTLMnumber = terminateDataGmsh2_2(inputGmsh)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    
    // I need to set the scale variable to 1. If don't do this, the mesh will be
    // scaled twice.
    double scale_temp[3];
    scale_temp[0] = input->scale[0];
    input->scale[0] = 1;
    scale_temp[1] = input->scale[1];
    input->scale[1] = 1;
    scale_temp[2] = input->scale[2];
    input->scale[2] = 1;

    // call the function that reads the tlm native format
    if ((errorTLMnumber = tbnReader(input, output)) != 0) {
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
        return errorTLMnumber;
    }
    
    // recovering the value of the scale
    input->scale[0] = scale_temp[0];
    input->scale[1] = scale_temp[1];
    input->scale[2] = scale_temp[2];

    fprintf(stderr, "WARNING: We were successful to save and read the mesh.");

    return 0;
}

/*
 * onlyWriteGmsh2_2: Write the nodes and elements to the file
 */
unsigned int onlyWriteGmsh2_2(struct dataGmsh2_2 * input, FILE * saveFile) {

    unsigned long long i, quantityOfElementSaved;
    unsigned int errorTLMnumber, quantityOfElementTypes = 0;

    fprintf(saveFile, "Nodes\n");
    fprintf(saveFile, "{\n");
    fprintf(saveFile, "%llu\n", input->numberOfNode);
    // I will first look for the nodes
    for (i = 0; i < input->numberOfNode; i++) {
        fprintf(saveFile, "%.17g %.17g %.17g\n", input->nodes[i].x,
                input->nodes[i].y, input->nodes[i].z);
    }
    fprintf(saveFile, "}\n");

    // at this point, I can deallocate the nodes
    if ((errorTLMnumber = deallocateOnlyTheNodeGmsh(input)) != 0)
        return errorTLMnumber;

    fprintf(saveFile, "Elements\n");
    fprintf(saveFile, "{\n");


    for (i = 0; i < 100; i++) {
        if (input->quantityOfSpecificElement[i] != 0)
            // Counting up how many elements were used
            quantityOfElementTypes++;
    }

    fprintf(saveFile, "%u ", quantityOfElementTypes);

    for (unsigned int j = 0; j < 100; j++) {
        if (input->quantityOfSpecificElement[j] != 0) // I go here only if it is non-zero
            // Then I will tell how many of each element is there on the file
            fprintf(saveFile, "%u %llu ", j, input->quantityOfSpecificElement[j]);
    }
    fprintf(saveFile, "\n"); // just go to the next line

    for (unsigned int j = 0; j < 100; j++) {
        i = 0;
        if (input->quantityOfSpecificElement[j] != 0)
            switch (j) {
                case 1: // 2 nodes line
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 1) {
                            fprintf(saveFile, "%lu %llu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1,
                                    input->saveElement[i].N2);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
                case 2: // 3 nodes triangle
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 2) {
                            fprintf(saveFile, "%lu %llu %llu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1,
                                    input->saveElement[i].N2,
                                    input->saveElement[i].N3);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
                case 3: // 4 nodes quadrangle
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 3) {
                            fprintf(saveFile, "%lu %llu %llu %llu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1,
                                    input->saveElement[i].N2,
                                    input->saveElement[i].N3,
                                    input->saveElement[i].N4);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
                case 4: // 4 nodes tetrahedron
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 4) {
                            fprintf(saveFile, "%lu %llu %llu %llu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1,
                                    input->saveElement[i].N2,
                                    input->saveElement[i].N3,
                                    input->saveElement[i].N4);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
                case 5: // 8 nodes hexahedron
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 5) {
                            fprintf(saveFile, "%lu %llu %llu %llu %llu %llu %llu %llu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1,
                                    input->saveElement[i].N2,
                                    input->saveElement[i].N3,
                                    input->saveElement[i].N4,
                                    input->saveElement[i].N5,
                                    input->saveElement[i].N6,
                                    input->saveElement[i].N7,
                                    input->saveElement[i].N8);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
                case 6: // 6 nodes prism
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 6) {
                            fprintf(saveFile, "%lu %llu %llu %llu %llu %llu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1,
                                    input->saveElement[i].N2,
                                    input->saveElement[i].N3,
                                    input->saveElement[i].N4,
                                    input->saveElement[i].N5,
                                    input->saveElement[i].N6);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
                case 7: // 5 nodes pyramid
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 7) {
                            fprintf(saveFile, "%lu %llu %llu %llu %llu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1,
                                    input->saveElement[i].N2,
                                    input->saveElement[i].N3,
                                    input->saveElement[i].N4,
                                    input->saveElement[i].N5);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
                case 15: // 1 node point
                    for (quantityOfElementSaved = 0; quantityOfElementSaved < input->quantityOfSpecificElement[j]; i++) {
                        if (input->saveElement[i].elementCode == 15) {
                            fprintf(saveFile, "%lu %llu\n",
                                    input->saveElement[i].tag,
                                    input->saveElement[i].N1);
                            quantityOfElementSaved++;
                        }
                    }
                    break;
            }
    }

    // at this point, I can deallocate the elements
    if ((errorTLMnumber = deallocateOnlyTheElementGmsh(input)) != 0)
        return errorTLMnumber;

    fprintf(saveFile, "}\n");


    return 0;
}

/*
 * testInputGmsh: tests the inputs. Future implementation
 */
unsigned int testInputGmsh(struct dataGmsh2_2* input) {
    return 0;
}