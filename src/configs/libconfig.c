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
 * File:   libconfig.c
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
 * Contains the configuration code and redirects the flow of the algorithm 
 * accordingly to what to configure
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libconfig.h"

#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * testAndReadInputFile: Tests if the input file exists and call the function
 * responsible for reading it.
 */
unsigned int testAndReadInputFile(int nInput, char* pArgs[], struct dataForSimulation *newDataForSimu) {

    unsigned int tlmErrorCode = 0, inputPosition, inputPositionDefined = 0, showHelp = 0, showVersion = 0;
    FILE *pfile;
    // checking the inputs. I don't check input 0 because this is
    // the function being called
    for (unsigned int i = 1; i < nInput; i++) {
        // if we know where the input position is and we are looking at it,
        // we just keep on going.
        if (inputPositionDefined == 1 && inputPosition == i) {
            continue;
        }
        // verifying the input
        if (pArgs[i][0] == '-') {
            // the input starts with '-' ?
            if (pArgs[i][1] == '-') {
                // the input starts with "--"
                if (compareCaseInsensitive(pArgs[i], "--verbose") == 0) {
                    newDataForSimu->simulationInput.verboseMode = 1;
                } else if (compareCaseInsensitive(pArgs[i], "--timing") == 0) {
                    newDataForSimu->simulationInput.timingMode = 1;
                } else if (compareCaseInsensitive(pArgs[i], "--help") == 0) {
                    showHelp = 1;
                } else if (compareCaseInsensitive(pArgs[i], "--version") == 0) {
                    showVersion = 1;
                } else if (compareCaseInsensitive(pArgs[i], "--input") == 0) {
                    if (inputPositionDefined == 1) {
                        tlmErrorCode = 317;
                        sendErrorCodeAndMessage(tlmErrorCode, &i, pArgs[i], &inputPosition, pArgs[inputPosition]);
                        break;
                    }
                    inputPosition = i + 1;
                    inputPositionDefined = 1;
                } else {
                    // this is an unknown input command
                    tlmErrorCode = 316;
                    sendErrorCodeAndMessage(tlmErrorCode, pArgs[i], NULL, NULL, NULL);
                    break;
                }
            } else {
                for (int j = 1; j < strlen(pArgs[i]); j++) {
                    // the input starts with '-'
                    if (compareCaseInsensitive(pArgs[i], "e") == 0) {
                        newDataForSimu->simulationInput.verboseMode = 1;
                    } else if (compareCaseInsensitive(pArgs[i], "t") == 0) {
                        newDataForSimu->simulationInput.timingMode = 1;
                    } else if (compareCaseInsensitive(pArgs[i], "h") == 0) {
                        showHelp = 1;
                    } else if (compareCaseInsensitive(pArgs[i], "v") == 0) {
                        showVersion = 1;
                    } else if (compareCaseInsensitive(pArgs[i], "i") == 0) {
                        if (inputPositionDefined == 1) {
                            tlmErrorCode = 317;
                            sendErrorCodeAndMessage(tlmErrorCode, &i, pArgs[i], &inputPosition, pArgs[inputPosition]);
                            break;
                        }
                        inputPosition = i + 1;
                        inputPositionDefined = 1;
                    } else {
                        // this is an unknown input command
                        tlmErrorCode = 316;
                        sendErrorCodeAndMessage(tlmErrorCode, pArgs[i], NULL, NULL, NULL);
                        break;
                    }
                }
            }
        } else if (i == 1) {
            // it didn't start with '-' so I'm assuming this is the input
            inputPosition = i;
            inputPositionDefined = 1;

        } else {
            // some text after the first input that does not start with '-'. 
            // I'm not sure what it is
            tlmErrorCode = 316;
            sendErrorCodeAndMessage(tlmErrorCode, pArgs[i], NULL, NULL, NULL);
            break;
        }
    }

    if (tlmErrorCode != 0) {
        // we had an error while reading the inputs. Let me through the help
        // BLACK_BOX_PRINT_HELP
        return tlmErrorCode;
    }

    // displaying the options we got from the inputs
    if (newDataForSimu->simulationInput.verboseMode == 1)
        printf("Verbose mode on\n");
    if (newDataForSimu->simulationInput.timingMode == 1)
        printf("Timing mode on\n");
    if (showHelp == 1)
        printf("HELP\n");
    if (showVersion == 1)
        printf("Version\n");

    // BLACK_BOX_PRINT_HELP
    // useful link http://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char
    // useful code xxd -i a | sed 's/\([0-9a-f]\)$/\0, 0x00/'
    //            char *pline = NULL;
    //            pfile = fopen("case.tlm", "r");
    //            size_t lenLine = 0;
    //
    //            // readings line-by-line until we get to the end-of-file character
    //            while (getline(&pline, &lenLine, f) != EOF) {
    //                printf("%s", pline); 
    //            }



    // checking if the input file position was defined. If not, them the case.tlm
    // file should exist in the same folder where the call is being made
    if (inputPositionDefined == 0) {
        pfile = fopen("case.tlm", "r");
        if (pfile == NULL) {
            sendErrorCodeAndMessage(315, NULL, NULL, NULL, NULL);
            return 315;
        }

        // saving the name of the input file
        newDataForSimu->simulationInput.nameOfInputFile = (char *) malloc((strlen("case.tlm") + 1) * sizeof (char));
        // 9 = strlen("case.tlm") + '\0'
        strcpy(newDataForSimu->simulationInput.nameOfInputFile, "case.tlm");

        printf("Reading case.tlm\n");


        // We received an input
    } else {
        pfile = fopen(pArgs[inputPosition], "r");

        //We couldn't find the input file
        if (pfile == NULL) {
            sendErrorCodeAndMessage(764, pArgs[1], NULL, NULL, NULL);
            return 764;
        }

        // saving the name of the input file
        newDataForSimu->simulationInput.nameOfInputFile = (char *) malloc((strlen(pArgs[inputPosition]) + 1) * sizeof (char));
        // 9 = strlen(pArgs[inputPosition]) + '\0'
        strcpy(newDataForSimu->simulationInput.nameOfInputFile, pArgs[inputPosition]);

        printf("Reading the input file: %s\n", pArgs[inputPosition]);
    }

    // saving the name of the input file without the last point (file extension)
    getBeforePoint(newDataForSimu->simulationInput.nameOfInputFile);

    clock_t begin = clock();

    // we could open the input. Now let's read it
    if ((tlmErrorCode = readFileTLM(pfile, newDataForSimu)) != 0) {
        return tlmErrorCode;
    }

    printf("The input file was read.\n");



    // close the file
    if (fclose(pfile) == EOF) {
        sendErrorCodeAndMessage(345, NULL, NULL, NULL, NULL);
        return 345;
    }

    return 0;
}

/*
 * readFileTLM: reads the input file and redirects the flow of the algorithm
 * to the configuration to be made
 */
unsigned int readFileTLM(FILE * f, struct dataForSimulation *newDataForSimu) {
    unsigned int lineNumber = 0, errorTLMnumber = 0;
    char *pline = NULL, *lineOriginal = NULL;
    long lenLine = 0;


    int startEndBrackets = 0; // 0 - not started or finalized; 1 - started;
    enum configuringInside ConfigPoint = NOTHING;

    // reading line-by-line until we get to the end-of-file character
    int lineInt = 1;
        while (getlineTlmbht(&pline, &lenLine, f) != 1) {
        // I start the loop checking the variable for error.
        if (errorTLMnumber != 0 && errorTLMnumber != 9999 && errorTLMnumber != 9998) {
            // get out of the while loop
            break;
        } else if (errorTLMnumber == 9999) {
            // this is not an error. This is just how I make the flag to indicate
            // that the reading of some configuration was just finished
            switch (ConfigPoint) {

                    // this is not expected to happen
                case NOTHING:
                    errorTLMnumber = 200;
                    break;

                    // I don't allow more than one header for the simulation configuration
                    // characteristics.
                    // It can change in the future, if required.
                case SIMULATION:
                    newDataForSimu->simulationRead++;
                    if (newDataForSimu->simulationRead > 1)
                        errorTLMnumber = 201;
                    break;

                    // I don't allow more than one header for the mesh configuration
                    // characteristics.
                    // It can change in the future, if required.
                case MESH:
                    newDataForSimu->meshRead++;
                    if (newDataForSimu->meshRead > 1)
                        errorTLMnumber = 202;
                    break;

                    // Reallocate the variable that has the data for the equations
                    // with one extra space.
                case EQUATION:
                    newDataForSimu->quantityOfEquationsRead++;
                    newDataForSimu->equationInput = (struct Equation*)
                            realloc(newDataForSimu->equationInput,
                            sizeof (struct Equation)*
                            (newDataForSimu->quantityOfEquationsRead + 1));
                    break;

                    // Reallocate the variable that has the data for the materials
                    // with one extra space.
                case MATERIAL:
                    newDataForSimu->quantityOfMaterialsRead++;
                    newDataForSimu->materialInput = (struct MaterialConfig*)
                            realloc(newDataForSimu->materialInput,
                            sizeof (struct MaterialConfig)*
                            (newDataForSimu->quantityOfMaterialsRead + 1));
                    break;

                    // Reallocate the variable that has the data for the boundaries
                    // with one extra space.
                case BOUNDARY:
                    newDataForSimu->quantityOfBoundariesRead++;
                    newDataForSimu->boundaryInput = (struct BoundaryConfig*)
                            realloc(newDataForSimu->boundaryInput,
                            sizeof (struct BoundaryConfig)*
                            (newDataForSimu->quantityOfBoundariesRead + 1));
                    break;

                    // Reallocate the variable that has the data for the sources
                    // with one extra space.
                    //////// future implementation //////
                case SOURCES:
                    newDataForSimu->quantityOfSourcesRead++;
                    newDataForSimu->sourceInput = (struct SourceConfig*)
                            realloc(newDataForSimu->sourceInput,
                            sizeof (struct SourceConfig)*
                            (newDataForSimu->quantityOfSourcesRead + 1));
                    break;

                    // this is not expected to happen
                default:
                    errorTLMnumber = 206;
                    break;
            }

            // if an error was found, we stop the algorithm here.
            if (errorTLMnumber != 9999) {
                break; // get out of the while loop
            }

            // change to nothing being configured, so that the algorithm can look for
            // another configuration header
            ConfigPoint = NOTHING;
        }
        //DEBUG: Count up of the number of lines. Also used in the function that displays the error
        lineNumber++;



        if (newDataForSimu->simulationInput.verboseMode == 1) {
            // VERBOSE: Shown the line number, quantity of characters in the line and content
            printf("Line %04u (%lu): %s", lineNumber, strlen(pline), pline);
        }



        // I will copy the content of pline to show it for troubleshooting purposes.
        lineOriginal = (char *) realloc(lineOriginal, strlen(pline) + 1);
        strcpy(lineOriginal, pline);



        // It returns 0 if it got some useful content on that line.
        // it returns 9998 if it got nothing useful
        // if there's nothing useful or an error, we should go to the next loop iteration.
        if ((errorTLMnumber = getUsefulContent(pline)) != 0)
            continue; // go to next iteration


        if (newDataForSimu->simulationInput.verboseMode == 1) {
            // DEBUG: what comes out the useful test. Shown the line number, quantity of characters in the line and content
            printf("Useful content from line %04u (%lu): %s\n", lineNumber, strlen(pline), pline);
        }


        switch (ConfigPoint) {
                // is ConfigPoint NOTHING? If so, we should check the line to find out
                // what should we configure next
            case NOTHING:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring NOTHING\n");
                }

                // checking if we need to change what we are configuring now
                if ((errorTLMnumber = setConfigurationTo(pline, &ConfigPoint)) != 0)
                    continue; // go to the next iteration if we found an error

                break;

            case SIMULATION:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring SIMULATION\n");
                }

                // call the function responsible for configuring the simulation parameters
                if ((errorTLMnumber = setConfigurationSimu(pline, &newDataForSimu->simulationInput, &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error

                break;

            case MESH:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring MESH\n");
                }

                // call the function responsible for configuring the mesh parameters
                if ((errorTLMnumber = setConfigurationMesh(pline, &newDataForSimu->meshInput, &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 
                break;

            case EQUATION:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring EQUATION\n");
                }

                // call the function responsible for configuring the material parameters
                if ((errorTLMnumber = setConfigurationEquation(pline,
                        &newDataForSimu->equationInput[newDataForSimu->quantityOfEquationsRead],
                        &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 
                break;

            case MATERIAL:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring MATERIAL\n");
                }

                // call the function responsible for configuring the material parameters
                if ((errorTLMnumber = setConfigurationMaterial(pline,
                        &newDataForSimu->materialInput[newDataForSimu->quantityOfMaterialsRead],
                        &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 
                break;

            case BOUNDARY:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring BOUNDARY\n");
                }

                // call the function responsible for configuring the boundary parameters
                if ((errorTLMnumber = setConfigurationBoundary(pline,
                        &newDataForSimu->boundaryInput[newDataForSimu->quantityOfBoundariesRead],
                        &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 

                break;
            case SOURCES:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring SOURCES\n");
                }

                // call the function responsible for configuring the source parameters
                if ((errorTLMnumber = setConfigurationSource(pline,
                        &newDataForSimu->sourceInput[newDataForSimu->quantityOfSourcesRead],
                        &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 

                break;

                // this is not expected to happen
            default:
                errorTLMnumber = 8;
                sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
                break;
        }
    }

    // in case of error, sending the error message
    if (errorTLMnumber != 0 && errorTLMnumber != 9999 && errorTLMnumber != 9998)
        sendErrorCodeAndMessage(errorTLMnumber, &lineNumber, lineOriginal, NULL, NULL);

    // freeing. Closing of 'f' is done in testAndReadInputFile
    free(pline);
    pline = NULL;
    free(lineOriginal);
    lineOriginal = NULL;

    // in case of error, returning the output
    if (errorTLMnumber != 0 && errorTLMnumber != 9999 && errorTLMnumber != 9998)
        return errorTLMnumber;


    return testAllConfigurationVarialbes(newDataForSimu);
}

/*
 * setConfigurationTo: compare the string of the input to standardized texts.
 * The comparison is case insensitive.
 */
unsigned int setConfigurationTo(char *input, enum configuringInside * config) {
    if (compareCaseInsensitive(input, "simulation") == 0) {
        *config = SIMULATION;
    } else if (compareCaseInsensitive(input, "mesh") == 0) {
        *config = MESH;
    } else if (compareCaseInsensitive(input, "equation") == 0) {
        *config = EQUATION;
    } else if (compareCaseInsensitive(input, "material") == 0) {
        *config = MATERIAL;
    } else if (compareCaseInsensitive(input, "boundary") == 0) {
        *config = BOUNDARY;
    } else if (compareCaseInsensitive(input, "sources") == 0) {
        *config = SOURCES;
    } else {
        // what we found out is not equal to nothing above
        return 365;
    }

    return 0;
}

/*
 * initiateAllConfigurationVarialbes: initialize all the configuration variables
 */
unsigned int initiateAllConfigurationVarialbes(struct dataForSimulation *input) {
    int errorTLMnumber;

    // initializing variables
    input->simulationRead = 0;
    input->runningSimulation = 1;
    input->runningBenchmark = 0;
    input->meshRead = 0;
    input->quantityOfEquationsRead = 0;
    input->quantityOfMaterialsRead = 0;
    input->quantityOfBoundariesRead = 0;
    input->quantityOfSourcesRead = 0;
    
    // initializing who am I
    input->myName = malloc( ( strlen("tlmbht") + 1 )*sizeof(char) );
    strcpy(input->myName, "tlmbht");
    input->myVersion = malloc( ( strlen("0.2.0-alpha") + 1 )*sizeof(char) );
    strcpy(input->myVersion, "0.2.0-alpha");

    // initializing simulation variables used for configuration
    if ((errorTLMnumber = initiateSimulationVariable(&input->simulationInput)) != 0)
        return errorTLMnumber;

    // initializing mesh variables used for configuration
    if ((errorTLMnumber = initiateMeshVariable(&input->meshInput)) != 0)
        return errorTLMnumber;

    // we can have more than one equationInput. Here we initialize the first
    input->equationInput = (struct Equation*) malloc(sizeof (struct Equation));
    // initializing the equation variables used for configuration.
    if ((errorTLMnumber = initiateEquationVariable(&input->equationInput[0])) != 0)
        return errorTLMnumber;


    // we can have more than one materialInput. Here we initialize the first
    input->materialInput = (struct MaterialConfig*) malloc(sizeof (struct MaterialConfig));
    // initializing the materials variables used for configuration.
    if ((errorTLMnumber = intializeMaterialConfig(&input->materialInput[0])) != 0)
        return errorTLMnumber;

    // we can have more than one boundaryInput. Here we initialize the first
    input->boundaryInput = (struct BoundaryConfig*) malloc(sizeof (struct BoundaryConfig));
    // initializing the boundary variables used for configuration.
    if ((errorTLMnumber = initializeBoundaryConfig(&input->boundaryInput[0])) != 0)
        return errorTLMnumber;

    // we can have more than one sourceInput. Here we initialize the first
    input->sourceInput = (struct SourceConfig*) malloc(sizeof (struct SourceConfig));
    // initializing the source variables used for configuration.
    if ((errorTLMnumber = initializeSourceConfig(&input->sourceInput[0])) != 0)
        return errorTLMnumber;

    return 0;
}

/*
 * terminateAllConfigurationVarialbes: terminate all the configuration variables
 */
unsigned int terminateAllConfigurationVarialbes(struct dataForSimulation *input) {
    unsigned int errorTLMnumber;
    int i;
    
    free(input->myName);
    input->myName = NULL;
    free(input->myVersion);
    input->myVersion = NULL;

    // terminating the simulation variables used for configuration
    if ((errorTLMnumber = terminateSimulationVariable(&input->simulationInput)) != 0)
        return errorTLMnumber;

    // terminating the mesh variables used for configuration
    if ((errorTLMnumber = terminateMeshVariable(&input->meshInput)) != 0)
        return errorTLMnumber;

    // terminating the equation variables when the quantity of equations read was
    // greater than zero. If it was zero, we won't get this loop initialized.
    for (i = 0; i < input->quantityOfEquationsRead; i++)
        if ((errorTLMnumber = terminateEquationConfig(&input->equationInput[i])) != 0)
            return errorTLMnumber;
    // this is the case that none equation was read. In this case, we only have to 
    // terminate the equationInput[0].
    if (input->quantityOfEquationsRead == 0)
        if ((errorTLMnumber = terminateEquationConfig(&input->equationInput[0])) != 0)
            return errorTLMnumber;
    // now, the equationInput itself is being freed.
    free(input->equationInput);
    input->equationInput = NULL;

    // terminating the materials variables when the quantity of materials read was
    // greater than zero. If it was zero, we won't get this loop initialized.
    for (i = 0; i < input->quantityOfMaterialsRead; i++)
        if ((errorTLMnumber = terminateMaterialConfig(&input->materialInput[i])) != 0)
            return errorTLMnumber;
    // this is the case that no one material was read. In this case, we only have to 
    // terminate the materialInput[0].
    if (input->quantityOfMaterialsRead == 0)
        if ((errorTLMnumber = terminateMaterialConfig(&input->materialInput[0])) != 0)
            return errorTLMnumber;
    // now, the materialInput itself is being freed.
    free(input->materialInput);
    input->materialInput = NULL;

    // terminating the boundary variables when the quantity of boundaries read was
    // greater than zero. If it was zero, we won't get this loop initialized.
    for (i = 0; i < input->quantityOfBoundariesRead; i++)
        if ((errorTLMnumber = terminateBoundaryConfig(&input->boundaryInput[i])) != 0)
            return errorTLMnumber;
    // this is the case that no one boundary was read. In this case, we only have to 
    // terminate the boundaryInput[0].
    if (input->quantityOfBoundariesRead == 0)
        if ((errorTLMnumber = terminateBoundaryConfig(&input->boundaryInput[0])) != 0)
            return errorTLMnumber;
    // now, the boundaryInput itself is being freed.
    free(input->boundaryInput);
    input->boundaryInput = NULL;



    // terminating the source variables when the quantity of sources read was
    // greater than zero. If it was zero, we won't get this loop initialized.
    for (i = 0; i < input->quantityOfSourcesRead; i++)
        if ((errorTLMnumber = terminateSourceConfig(&input->sourceInput[i])) != 0)
            return errorTLMnumber;
    // this is the case that no one source was read. In this case, we only have to 
    // terminate the sourceInput[0].
    if (input->quantityOfSourcesRead == 0)
        if ((errorTLMnumber = terminateSourceConfig(&input->sourceInput[0])) != 0)
            return errorTLMnumber;
    // now, the sourceInput itself is being freed.
    free(input->sourceInput);
    input->sourceInput = NULL;

    return 0;
}

/*
 * printfAllInputData: prints the input data read by the algorithm from the input file
 */
void printfAllInputData(struct dataForSimulation * input) {
    
    printfMyNameAndVersion(input);

    printf("Information read from inputs:\n");
    // information from the simulation header
    printfAllSimuData(&input->simulationInput);
    printf("\n");

    // information from the mesh header
    printfMeshConfig(&input->meshInput);
    printf("\n");

    // information from the equation headers
    printf("Input data for the Equation configuration:\n");
    for (int i = 0; i < input->quantityOfEquationsRead; i++) {
        printf("Equation group %04d:\n", i + 1);
        printfAllEquaData(&input->equationInput[i]);
        printf("\n");
    }

    // information from the material headers
    printf("Input data for the Materials configuration:\n");
    for (int i = 0; i < input->quantityOfMaterialsRead; i++) {
        printf("Materials group %04d:\n", i + 1);
        printfMatConfig(&input->materialInput[i],
                &input->equationInput[input->materialInput[i].equationNumber]);
        printf("\n");
    }

    // information from the boundary headers
    printf("Input data for the Boundaries configuration:\n");
    for (int i = 0; i < input->quantityOfBoundariesRead; i++) {
        printf("Boundaries group %04d:\n", i + 1);
        printfBoundConfig(&input->boundaryInput[i],
                &input->equationInput[input->boundaryInput[i].equationNumber]);
        printf("\n");
    }

    // future implementation
    // information from the source headers
//    printf("Input data for the Sources configuration:\n");
//    for (int i = 0; i < input->quantityOfSourcesRead; i++) {
//        printf("Sources group %04d:\n", i + 1);
//        printfSourceConfig(&input->sourceInput[i]);
//        printf("\n");
//    }

}

/*
 * printfMyNameAndVersion: prints the name and version of this software
 */
void printfMyNameAndVersion(struct dataForSimulation * input) {
    printf("%s %s\n", input->myName, input->myVersion);
}


/*
 * testAllConfigurationVarialbes: tests if all the configuration variables
 * were adequately read
 */
unsigned int testAllConfigurationVarialbes(struct dataForSimulation *input) {
    unsigned int errorFound = 0;
    int i, j;

    // tests some variables that dictates if we are going to run the simulation or not. FUTURE IMPLEMENTATION
    if ((errorFound = testRunSimulation(input)) != 0) {
        // if errorFound = 9999, then, we are not going to run a simulation and,
        // hence, I don't need to care about the other variables
        if (errorFound == 9999) {
            input->runningSimulation = 0;
            return 0;
        }
    }

    // testing for the Simulation configuration field. 
    //
    // We did not read the simulation configurations
    if (input->simulationRead == 0) {
        sendErrorCodeAndMessage(208, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }

    // testing the simulation configurations input
    if (testInputSimu(&input->simulationInput, input->meshInput.nameOfInputFile) != 0) {
        sendErrorCodeAndMessage(1870, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }

    // testing for the Mesh configuration field
    //
    // we did not read the mesh configurations
    if (input->meshRead == 0) {
        sendErrorCodeAndMessage(210, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }

    // testing the mesh configurations input
    if (testInputMesh(&input->meshInput, input->simulationInput.nameOfInputFile) != 0) {
        sendErrorCodeAndMessage(1871, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }

    // testing for the Equation configuration field
    //
    // we did not read the material configurations
    if (input->quantityOfEquationsRead == 0) {
        sendErrorCodeAndMessage(212, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }
    // testing the material configurations read.
    for (i = 1; i <= input->quantityOfEquationsRead; i++) {

        // allocating the pointer that will have the number of the materials
        input->equationInput[i - 1].materialNumbers = (int *) malloc(sizeof (int)*input->quantityOfMaterialsRead);

        // we make j from 0 until quantityOfMaterialsRead - 1.
        for (j = 0; j < input->quantityOfMaterialsRead; j++) {
            if (compareCaseInsensitive(input->materialInput[j].equationName, input->equationInput[i - 1].equationName) == 0) {

                input->materialInput[j].equationNumber = i - 1;

                input->equationInput[i - 1].materialNumbers[input->equationInput[i - 1].numberOfMaterials] = j;
                input->equationInput[i - 1].numberOfMaterials++;
            }
        }

        input->equationInput[i - 1].materialNumbers = (int *) realloc(input->equationInput[i - 1].materialNumbers, sizeof (int)*input->equationInput[i - 1].numberOfMaterials);

        if (input->equationInput[i - 1].numberOfMaterials == 0) {
            printf("WARNING: No material was found associated with this equation. It"
                    "will be ignored during the calculation.");
        }

        // allocating the pointer that will have the number of the boundaries
        input->equationInput[i - 1].boundaryNumbers = (int *) malloc(sizeof (int)*input->quantityOfBoundariesRead);

        // we make j from 0 until quantityOfBoundariesRead - 1.
        for (j = 0; j < input->quantityOfBoundariesRead; j++) {
            if (compareCaseInsensitive(input->boundaryInput[j].equationName, input->equationInput[i - 1].equationName) == 0) {

                input->boundaryInput[j].equationNumber = i - 1;

                input->equationInput[i - 1].boundaryNumbers[input->equationInput[i - 1].numberOfBoundaries] = j;
                input->equationInput[i - 1].numberOfBoundaries++;
            }
        }

        input->equationInput[i - 1].boundaryNumbers = (int *) realloc(input->equationInput[i - 1].boundaryNumbers, sizeof (int)*input->equationInput[i - 1].numberOfBoundaries);


        if (testInputEquation(&input->equationInput[i - 1], i) != 0) {
            sendErrorCodeAndMessage(1876, &i, NULL, NULL, NULL);
            errorFound = 1;
        }

        // checking if there are equations with the same name
        for (j = i + 1; j <= input->quantityOfEquationsRead; j++) {
            if (compareCaseInsensitive(input->equationInput[j - 1].equationName, input->equationInput[i - 1].equationName) == 0) {
                sendErrorCodeAndMessage(1877, &i, input->equationInput[i - 1].equationName, &j, input->equationInput[j - 1].equationName);
                errorFound = 1;
            }
        }
    }

    // testing for the Materials configuration field
    //

    // we did not read the material configurations
    if (input->quantityOfMaterialsRead == 0) {
        sendErrorCodeAndMessage(211, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }

    // testing the material configurations read.
    for (i = 1; i <= input->quantityOfMaterialsRead; i++) {

        // we check if the name of the equation was defined
        if (input->materialInput[i - 1].equationNameDefined == 0) {
            sendErrorCodeAndMessage(4443, &i, NULL, NULL, NULL);
            errorFound = 1;
            // if the name of the equation was not defined, we move to the next material number
            continue;
        }

        // We check if the name of the equation defined in the Material was found in the Equation configuration
        if (input->materialInput[i - 1].equationNumber == -1) {
            sendErrorCodeAndMessage(4442, &i, input->materialInput[i - 1].equationName, NULL, NULL);
            errorFound = 1;
            continue;
        }

        if (testInputMaterial(&input->materialInput[i - 1],
                &input->equationInput[input->materialInput[i - 1].equationNumber], i) != 0) {
            sendErrorCodeAndMessage(1872, &i, NULL, NULL, NULL);
            errorFound = 1;
        }
    }

    // testing for the Boundary configuration field
    //
    // There is no error in not reading any boundary header. In this case, the
    // algorithm will use some standard boundary's type.
    //
    // testing the boundary configurations read.
    for (i = 1; i <= input->quantityOfBoundariesRead; i++) {

        // We check if the name of the equation defined in the Material are found in the Equation configuration
        if (input->boundaryInput[i - 1].equationNameDefined == 0) {
            sendErrorCodeAndMessage(4443, &i, NULL, NULL, NULL);
            errorFound = 1;
            // if we did not find matching equation names we move to the next material number
            continue;
        }

        // We check if the name of the equation defined in the Boundary was found in the Equation configuration
        if (input->boundaryInput[i - 1].equationNumber == -1) {
            sendErrorCodeAndMessage(4442, &i, input->boundaryInput[i - 1].equationName, NULL, NULL);
            errorFound = 1;
            continue;
        }

        if (testInputBoundary(&input->boundaryInput[i - 1],
                &input->equationInput[input->boundaryInput[i - 1].equationNumber], i,
                &input->simulationInput) != 0) {
            sendErrorCodeAndMessage(1873, &i, NULL, NULL, NULL);
            errorFound = 1;
        }
    }

    // future implementation    
    // testing for the Source configuration field
    //
    // There is no error in not reading any source header. In this case, the
    // algorithm will have any extra source inputs.
    //
    // testing the source configurations read.
    for (i = 1; i <= input->quantityOfSourcesRead; i++) {
        if (testInputSource(&input->sourceInput[i - 1], i) != 0) {
            sendErrorCodeAndMessage(1874, &i, NULL, NULL, NULL);
            errorFound = 1;
        }
    }

    return errorFound;
}

/*
 * testRunSimulation: tests if we are going to run the simulation or if we are
 * doing some testings in the algorithm
 */
unsigned int testRunSimulation(struct dataForSimulation *input) {
    unsigned int errorTLMnumber = 0;
    switch (input->simulationInput.Benchmark) {
        case RUN:
            /* FALLTHRU */
        case RUN_FAST:
            printfBench(&input->simulationInput.Benchmark);
            errorTLMnumber = 9999;
            /* FALLTHRU */
        case RUN_AND_SOLVE:
            /* FALLTHRU */
        case RUN_FAST_AND_SOLVE:
            input->runningBenchmark = 1;
            break;
    }

    return errorTLMnumber;
}
