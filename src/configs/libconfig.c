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
        newDataForSimu->simulationInput.printAdditionalMode = 1;
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
    enum configName ConfigPoint = NOTHING;
    int previousReadingCode = 0;

    // reading line-by-line until we get to the end-of-file character
    while (getlineTlmbht(&pline, &lenLine, f) != 1) {
        // I start the loop checking the variable for error.
        if (errorTLMnumber != 0 && errorTLMnumber != 9999 && errorTLMnumber != 9998) {
            // get out of the while loop
            break;
        }
        //DEBUG: Count up of the number of lines. Also used in the function that displays the error
        lineNumber++;



        if (newDataForSimu->simulationInput.verboseMode == 1) {
            // VERBOSE: Shown the line number, quantity of characters in the line and content
            printf("Line %04u (%lu): %s", lineNumber, (unsigned long) strlen(pline), pline);
        }



        // I will copy the content of pline to show it for troubleshooting purposes.
        lineOriginal = (char *) realloc(lineOriginal, strlen(pline) + 1);
        strcpy(lineOriginal, pline);



        // It returns 0 if it got some useful content on that line.
        // it returns 9998 if it got nothing useful
        // if there's nothing useful or an error, we should go to the next loop iteration.
        if ((errorTLMnumber = getUsefulContent(pline)) != 0){
            
            if (errorTLMnumber == 9998 && lenLine == -1){
                // we didn't get anything useful and this is the last line, so we break
                // the while loop
                break;
            }
            else {
                continue; // go to next iteration
            }
                
        }
            


        if (newDataForSimu->simulationInput.verboseMode == 1) {
            // DEBUG: what comes out the useful test. Shown the line number, quantity of characters in the line and content
            printf("Useful content from line %04u (%lu): %s\n", lineNumber, (unsigned long) strlen(pline), pline);
        }


        switch (ConfigPoint) {
                // is ConfigPoint NOTHING? If so, we should check the line to find out
                // what should we configure next
            case NOTHING:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Looking for Headers...\n");
                }

                // checking if we need to change what we are configuring now
                if ((errorTLMnumber = setConfigurationTo(pline, &ConfigPoint)) != 0)
                    continue; // go to the next iteration if we found an error

                switch (ConfigPoint) {
                    case SIMULATION:
                        if (newDataForSimu->simulationInput.verboseMode == 1) {
                            // VERBOSE: Shown what is being configured
                            printf("Changing to configure SIMULATION\n");
                        }
                        break;
                    case MESH:
                        if (newDataForSimu->simulationInput.verboseMode == 1) {
                            // VERBOSE: Shown what is being configured
                            printf("Changing to configure MESH\n");
                        }
                        break;
                    case EQUATION:
                        if (newDataForSimu->simulationInput.verboseMode == 1) {
                            // VERBOSE: Shown what is being configured
                            printf("Changing to configure EQUATION\n");
                        }
                        break;
                    case MATERIAL:
                        if (newDataForSimu->simulationInput.verboseMode == 1) {
                            // VERBOSE: Shown what is being configured
                            printf("Changing to configure MATERIAL\n");
                        }
                        break;
                    case BOUNDARY:
                        if (newDataForSimu->simulationInput.verboseMode == 1) {
                            // VERBOSE: Shown what is being configured
                            printf("Changing to configure BOUNDARY\n");
                        }
                        break;
                    case SOURCE:
                        if (newDataForSimu->simulationInput.verboseMode == 1) {
                            // VERBOSE: Shown what is being configured
                            printf("Changing to configure SOURCE\n");
                        }
                        break;
                    case FUNCTION:
                        if (newDataForSimu->simulationInput.verboseMode == 1) {
                            // VERBOSE: Shown what is being configured
                            printf("Changing to configure FUNCTION\n");
                        }
                        break;
                        // this is not expected to happen
                    default:
                        errorTLMnumber = 205;
                        break;
                }
                
                break;

            case SIMULATION:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring SIMULATION\n");
                }

                // call the function responsible for configuring the simulation parameters
                errorTLMnumber = setConfigurationSimu(pline, &newDataForSimu->simulationInput, &startEndBrackets);

                break;

            case MESH:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring MESH\n");
                }

                // call the function responsible for configuring the mesh parameters
                errorTLMnumber = setConfigurationMesh(pline, &newDataForSimu->meshInput, &startEndBrackets);
                break;

            case EQUATION:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring EQUATION\n");
                }

                // call the function responsible for configuring the material parameters
                errorTLMnumber = setConfigurationEquation(pline,
                        &newDataForSimu->equationInput[newDataForSimu->quantityOfEquationsRead],
                        &startEndBrackets);
                break;

            case MATERIAL:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring MATERIAL\n");
                }

                // call the function responsible for configuring the material parameters
                errorTLMnumber = setConfigurationMaterial(pline,
                        &newDataForSimu->materialInput[newDataForSimu->quantityOfMaterialsRead],
                        &startEndBrackets);
                break;

            case BOUNDARY:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring BOUNDARY\n");
                }

                // call the function responsible for configuring the boundary parameters
                errorTLMnumber = setConfigurationBoundary(pline,
                        &newDataForSimu->boundaryInput[newDataForSimu->quantityOfBoundariesRead],
                        &startEndBrackets);

                break;
            case SOURCE:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring SOURCE\n");
                }

                // call the function responsible for configuring the source parameters
                errorTLMnumber = setConfigurationSource(pline,
                        &newDataForSimu->sourceInput[newDataForSimu->quantityOfSourcesRead],
                        &startEndBrackets);

                break;
            case FUNCTION:
                if (newDataForSimu->simulationInput.verboseMode == 1) {
                    // VERBOSE: Shown what is being configured
                    printf("Configuring FUNCTION\n");
                }

                // call the function responsible for configuring the source parameters
                errorTLMnumber = setConfigurationFunction(pline,
                        &newDataForSimu->functionInput[newDataForSimu->quantityOfFunctionsRead],
                        &startEndBrackets);

                break;

                // this is not expected to happen
            default:
                errorTLMnumber = 8;
                sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
                break;
        }
        
        // Here I check if I the reading of the header as finalized and I move to the next
        // header.
        if (errorTLMnumber == 9999) {
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
                    
                    // initiating the new equation
                    previousReadingCode = errorTLMnumber;
                    if ((errorTLMnumber = 
                            initiateEquationVariable(&newDataForSimu->equationInput[
                            newDataForSimu->quantityOfEquationsRead])) != 0)
                        return errorTLMnumber;
                    errorTLMnumber = previousReadingCode;
                    break;

                    // Reallocate the variable that has the data for the materials
                    // with one extra space.
                case MATERIAL:
                    newDataForSimu->quantityOfMaterialsRead++;
                    newDataForSimu->materialInput = (struct MaterialConfig*)
                            realloc(newDataForSimu->materialInput,
                            sizeof (struct MaterialConfig)*
                            (newDataForSimu->quantityOfMaterialsRead + 1));
                    // initiating the new material
                    previousReadingCode = errorTLMnumber;
                    if ((errorTLMnumber = 
                            intializeMaterialConfig(&newDataForSimu->materialInput[
                            newDataForSimu->quantityOfMaterialsRead])) != 0)
                        return errorTLMnumber;
                    errorTLMnumber = previousReadingCode;
                    break;

                    // Reallocate the variable that has the data for the boundaries
                    // with one extra space.
                case BOUNDARY:
                    newDataForSimu->quantityOfBoundariesRead++;
                    newDataForSimu->boundaryInput = (struct BoundaryConfig*)
                            realloc(newDataForSimu->boundaryInput,
                            sizeof (struct BoundaryConfig)*
                            (newDataForSimu->quantityOfBoundariesRead + 1));
                    // initiating the new boundary
                    previousReadingCode = errorTLMnumber;
                    if ((errorTLMnumber = 
                            initializeBoundaryConfig(&newDataForSimu->boundaryInput[
                            newDataForSimu->quantityOfBoundariesRead])) != 0)
                        return errorTLMnumber;
                    errorTLMnumber = previousReadingCode;
                    break;

                    // Reallocate the variable that has the data for the sources
                    // with one extra space.
                    //////// future implementation //////
                case SOURCE:
                    newDataForSimu->quantityOfSourcesRead++;
                    newDataForSimu->sourceInput = (struct SourceConfig*)
                            realloc(newDataForSimu->sourceInput,
                            sizeof (struct SourceConfig)*
                            (newDataForSimu->quantityOfSourcesRead + 1));
                    // initiating the new boundary
                    previousReadingCode = errorTLMnumber;
                    if ((errorTLMnumber = 
                            initializeSourceConfig(&newDataForSimu->sourceInput[
                            newDataForSimu->quantityOfSourcesRead])) != 0)
                        return errorTLMnumber;
                    errorTLMnumber = previousReadingCode;
                    break;
                    
                    // Reallocate the variable that has the data for the functions
                    // with one extra space.
                case FUNCTION:
                    newDataForSimu->quantityOfFunctionsRead++;
                    newDataForSimu->functionInput = (struct FunctionConfig*)
                            realloc(newDataForSimu->functionInput,
                            sizeof (struct FunctionConfig)*
                            (newDataForSimu->quantityOfFunctionsRead + 1));
                    // initiating the new boundary
                    previousReadingCode = errorTLMnumber;
                    if ((errorTLMnumber = 
                            initializeFunctionConfig(&newDataForSimu->functionInput[
                            newDataForSimu->quantityOfFunctionsRead])) != 0)
                        return errorTLMnumber;
                    errorTLMnumber = previousReadingCode;
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
        
        // this is the flag that indicates the last line was read and it had content.
        // Hence, the content was analyzed first and now we can go.
        if (lenLine == -1){
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
unsigned int setConfigurationTo(char *input, enum configName * config) {
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
        *config = SOURCE;
    } else if (compareCaseInsensitive(input, "function") == 0) {
        *config = FUNCTION;
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
    input->quantityOfFunctionsRead = 0;
    input->numberOfAllocationsForVariables = 50;
    input->numberOfVariablesRead = 0;
    input->variableTable = (struct variableID*) malloc(input->numberOfAllocationsForVariables * sizeof (struct variableID));
    // initializing the variablesID
    for (int i = 0; i < input->numberOfAllocationsForVariables; i++){
        if ((errorTLMnumber = initializeVariableID(input->variableTable[i])) != 0)
            return errorTLMnumber;
    }

    // initializing who am I
    input->myName = malloc((strlen("tlmbht") + 1) * sizeof (char));
    strcpy(input->myName, "tlmbht");
    input->myVersion = malloc((strlen("0.2.0-beta") + 1) * sizeof (char));
    strcpy(input->myVersion, "0.2.0-beta");

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
    
    // we can have more than one functionInput. Here we initialize the first
    input->functionInput = (struct FunctionConfig*) malloc(sizeof (struct FunctionConfig));
    // initializing the source variables used for configuration.
    if ((errorTLMnumber = initializeFunctionConfig(&input->functionInput[0])) != 0)
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
    
    
    // terminating the source variables when the quantity of sources read was
    // greater than zero. If it was zero, we won't get this loop initialized.
    for (i = 0; i < input->quantityOfFunctionsRead; i++)
        if ((errorTLMnumber = terminateFunctionConfig(&input->functionInput[i])) != 0)
            return errorTLMnumber;
    // this is the case that no one function was read. In this case, we only have to 
    // terminate the functionInput[0].
    if (input->quantityOfFunctionsRead == 0)
        if ((errorTLMnumber = terminateFunctionConfig(&input->functionInput[0])) != 0)
            return errorTLMnumber;
    // now, the functionInput itself is being freed.
    free(input->functionInput);
    input->functionInput = NULL;

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
    
    printf("Input data for the Function configuration:\n");
    for (int i = 0; i < input->quantityOfFunctionsRead; i++) {
        printf("Function group %04d:\n", i + 1);
        printfFuncConfig(&input->functionInput[i]);
        printf("\n");
    }

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
    
    // TODO: check if the variables' names are unique

    // tests some variables that dictates if we are going to run the simulation or not. FUTURE IMPLEMENTATION
    if ((errorFound = testRunSimulation(input)) != 0) {
        // if errorFound = 9999, then, we are not going to run a simulation and,
        // hence, I don't need to care about the other variables
        if (errorFound == 9999) {
            input->runningSimulation = 0;
            return 0;
        }
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
    // we did not read the equation configurations
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
                &input->equationInput[input->materialInput[i - 1].equationNumber], i,
                input->functionInput, input->quantityOfFunctionsRead) != 0) {
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
                &input->simulationInput,
                input->functionInput, input->quantityOfFunctionsRead) != 0) {
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
    
    
    // testing for the Function configuration field
    //
    // There is no error in not reading any function header.
    //
    // testing the function configurations read.
    for (i = 1; i <= input->quantityOfFunctionsRead; i++) {
        if (testInputFunction(input, i) != 0) {
            sendErrorCodeAndMessage(1878, &i, NULL, NULL, NULL);
            errorFound = 1;
        }
    }

    return errorFound;
}




/*
 * testInputFunction: tests if the function inputs are all OK
 */
unsigned int testInputFunction(struct dataForSimulation *input, int id) {
    unsigned int errorTLMnumber = 0;
    char *stringSearch = NULL, *stringSubstitute = NULL, *stringPointer = NULL;
    int offset = 0;
    

    FILE *function;
    char filename[] = "function00.c";
    if (id > 9) {
        filename[8] = (char) ((int) id / 10) + '0';
        filename[9] = (char) id % 10 + '0';
    } else {
        filename[9] = (char) id + '0';
    }
    function = fopen(filename, "w");

    // creating the file
    // standard libraries
    fprintf(function, "%s", input->functionInput[id - 1].filePart1);
    // libraries from the user
    for (int i = 0; i < input->functionInput[id - 1].quantityOfLibrariesInput; i++){
        fprintf(function, "#include %s\n", input->functionInput[id - 1].library[i]);
    }
    fprintf(function, "%s", input->functionInput[id - 1].filePart2);
    
        
    // allocating the variableID
    input->functionInput[id - 1].inputVariables = (struct variableID*) malloc(
            sizeof(struct variableID)*input->functionInput[id - 1].numberOfVariables );
    
    for (int i = 0; i < input->functionInput[id - 1].numberOfVariables; i++){
        
        if( (errorTLMnumber = createVariableNamesTable(
                             input->functionInput[id - 1].inputVariablesName[i],
                             input, &input->functionInput[id - 1].inputVariables[i])) != 0){
            return errorTLMnumber;
        }
        
        
        
        // DEBUG: show the name of the function
//                printf("Variable name '%s', field name '%s', field ID %d, field variable ID %d, field location %d\n",
//                        input->functionInput[id - 1].inputVariables[i].name,
//                        input->functionInput[id - 1].inputVariables[i].fieldName,
//                        input->functionInput[id - 1].inputVariables[i].fieldId,
//                        input->functionInput[id - 1].inputVariables[i].fieldVariableId,
//                        input->functionInput[id - 1].inputVariables[i].fieldLocation);
        
        
        stringSearch = realloc(stringSearch, strlen(input->functionInput[id - 1].inputVariables[i].name)
                                           + strlen(input->functionInput[id - 1].inputVariables[i].fieldName)
                                           + 3);
        strcpy(stringSearch, input->functionInput[id - 1].inputVariables[i].fieldName);
        stringSearch[strlen(input->functionInput[id - 1].inputVariables[i].fieldName)] = ':';
        stringSearch[strlen(input->functionInput[id - 1].inputVariables[i].fieldName) + 1] = ':';
        strcpy(stringSearch + strlen(input->functionInput[id - 1].inputVariables[i].fieldName) + 2, input->functionInput[id - 1].inputVariables[i].name);
        
        
        stringSubstitute = realloc(stringSubstitute, strlen(input->functionInput[id - 1].inputVariables[i].name)
                                           + strlen(input->functionInput[id - 1].inputVariables[i].fieldName)
                                           + 3);
        strcpy(stringSubstitute, input->functionInput[id - 1].inputVariables[i].fieldName);
        stringSubstitute[strlen(input->functionInput[id - 1].inputVariables[i].fieldName)] = '_';
        stringSubstitute[strlen(input->functionInput[id - 1].inputVariables[i].fieldName) + 1] = '_';
        strcpy(stringSubstitute + strlen(input->functionInput[id - 1].inputVariables[i].fieldName) + 2, input->functionInput[id - 1].inputVariables[i].name);
        
        
        // DEBUG: show the names to search for and substitute
        // printf("String search '%s'\n", stringSearch);
        
        // saving to file the part that reads inputs
        fprintf(function, "double %s;\n", stringSubstitute);
        fprintf(function, "sscanf(argv[%d], \"%%lf\", &%s);\n", i + 1, stringSubstitute);
        
        
        // checking where this variable is in the expression
        for (int j = 0; j < input->functionInput[id - 1].quantityOfLinesInExpression; j++){
            offset = 0;
            while((stringPointer = strstr(input->functionInput[id - 1].expression[j] + offset, stringSearch)) != NULL){
                stringPointer[strlen(input->functionInput[id - 1].inputVariables[i].fieldName)] = '_';
                stringPointer[strlen(input->functionInput[id - 1].inputVariables[i].fieldName) + 1] = '_';
                
                offset = strlen(input->functionInput[id - 1].expression[j]) - strlen(stringPointer) + strlen(stringSearch);
                
                // DEBUG: show the names to search for and substitute
                printf("Show the changed expression '%s'\n", input->functionInput[id - 1].expression[j]);
                
                if (strlen(input->functionInput[id - 1].expression[j]) - offset < strlen(stringSearch)){
                    break;
                }
            }
        }
    }
    
    // now that I have substituted all the input variables by their proper name, I can write
    // the expression to file
    if (input->functionInput[id - 1].quantityOfLinesInExpression == 1){
        fprintf(function, "return %s;\n", input->functionInput[id - 1].expression[0]);
    } else {
        if (input->functionInput[id - 1].outputVariableNameDefined != 0){
            fprintf(function, "double %s;\n", input->functionInput[id - 1].outputVariableName);
            
            for (int j = 0; j < input->functionInput[id - 1].quantityOfLinesInExpression; j++){
                fprintf(function, "%s\n", input->functionInput[id - 1].expression[j]);
            }
            fprintf(function, "return %s;\n", input->functionInput[id - 1].outputVariableName);
        } else {
            errorTLMnumber = 540;
            sendErrorCodeAndMessage(errorTLMnumber, &id, NULL, NULL, NULL);
        }
        
    }
    
    // closing the file
    fprintf(function, "%s", input->functionInput[id - 1].filePart_end1);
    fclose(function);
    
    
    // creating the command to compile the function
    char *command;
    command = (char*) malloc(sizeof(char)*( strlen(input->functionInput[id - 1].compileDirectives1)
            + strlen(filename) + strlen(" -o \0") + strlen(filename) + 1  + 
            strlen(input->functionInput[id - 1].compileDirectives2)) );
    
    strcpy(command, input->functionInput[id - 1].compileDirectives1);
    strcpy(command + strlen(input->functionInput[id - 1].compileDirectives1), filename);
    strcpy(command + strlen(input->functionInput[id - 1].compileDirectives1) + strlen(filename),
            " -o \0");
    strncpy(command + strlen(input->functionInput[id - 1].compileDirectives1) + strlen(filename) + 
            strlen(" -o \0"), filename, 10);
    strcpy(command + strlen(input->functionInput[id - 1].compileDirectives1) + strlen(filename) + 
            strlen(" -o \0") + 10, input->functionInput[id - 1].compileDirectives2);
            
    system(command);
    
    free(stringSearch);
    free(stringSubstitute);
    
    stringSearch = NULL;
    stringSubstitute = NULL;

    
    
    
    
    return errorTLMnumber;
}

/*
 * createVariableNamesTable: given a character name inputed on a FUNCTION, it finds
 * what variable it is. Assumes input is "FIELDNAME::VARIABLENAME"
 */
unsigned int createVariableNamesTable(char* input, struct dataForSimulation* simuData, struct variableID* output) {
    // steps:
    // 3) find what field FIELDNAME refers to
    // 4) save field in output->fieldLocation
    // 5) find the id of this FIELDNAME
    // 6) save id in output->fieldId
    // 8) given the field, find what variable VARIABLENAME refers to
    // 9) save the variable id on output->fieldVariableID
    // 10) done

    char *foundColon;
    int lengthFieldName, lengthVariableName;

    // searchers for "::" in input
    foundColon = strstr(input, "::");

    // if we didn't find ::, we should get out of here and issue an error
    if (foundColon == NULL) {
        sendErrorCodeAndMessage(500, input, NULL, NULL, NULL);
        return 500;
    }

    lengthVariableName = strlen(foundColon) - 2; // -2 for "::"
    lengthFieldName = strlen(input) - strlen(foundColon);

    output->fieldName = (char*) malloc(sizeof (char)*(lengthFieldName + 1));
    output->name = (char*) malloc(sizeof (char)*(lengthVariableName + 1));

    strncpy(output->fieldName, input, lengthFieldName);
    output->fieldName[lengthFieldName] = '\0';
    strcpy(output->name, foundColon + 2);

    // now its time to go over all possible configuration to identify what 
    // fieldName is referring to
    enum configName config = NOTHING;
    // switch is not needed here but it makes coding easier
    switch (config) {
        case NOTHING:
            // config = SIMULATION;
            /* FALLTHRU */
            // break;

        case SIMULATION:
            // checking if the input is "STD", which makes it the simulation itself
            if (compareCaseInsensitive(output->fieldName, "STD") == 0) {
                output->fieldLocation = SIMULATION;
                output->fieldId = 0;

                // we found the field now we will look for the variable
                if (compareCaseInsensitive(output->name, "absolute_zero") == 0) {
                    output->fieldVariableId = 0;
                } else if (compareCaseInsensitive(output->name, "Stefan_Boltzmann_constant") == 0) {
                    output->fieldVariableId = 1;
                } else if (compareCaseInsensitive(output->name, "x") == 0) {
                    output->fieldVariableId = 2;
                } else if (compareCaseInsensitive(output->name, "y") == 0) {
                    output->fieldVariableId = 3;
                } else if (compareCaseInsensitive(output->name, "z") == 0) {
                    output->fieldVariableId = 4;
                } else {
                    sendErrorCodeAndMessage(501, input, output->name, NULL, NULL);
                    return 501;
                }
                
                break;
            }

            // config = MESH;
            /* FALLTHRU */
            // break;

        case MESH:
            // nothing is expected to happen here
            //config = EQUATION;
            /* FALLTHRU */
            // break;

        case EQUATION:
            // looking if the name refers to an equation name
            for (int i = 0; i < simuData->quantityOfEquationsRead; i++) {
                if (compareCaseInsensitive(output->fieldName, simuData->equationInput[i].equationName) == 0) {

                    // the name refers to an equation. Looking for what variable
                    // it refers to
                    output->fieldLocation = EQUATION;
                    output->fieldId = i;

                    if (compareCaseInsensitive(output->name, "time") == 0) {
                        output->fieldVariableId = 0;
                    } else if (compareCaseInsensitive(output->name, "time_step") == 0) {
                        output->fieldVariableId = 1;
                    } else if (compareCaseInsensitive(output->name, "time_jump") == 0) {
                        output->fieldVariableId = 2;
                    } else if (compareCaseInsensitive(output->name, "final_time") == 0) {
                        output->fieldVariableId = 3;
                    } else if (compareCaseInsensitive(output->name, "scalar") == 0 || compareCaseInsensitive(output->name, simuData->equationInput[i].nameScalar) == 0) {
                        output->fieldVariableId = 4;
                    } else if (compareCaseInsensitive(output->name, "flux") == 0 || compareCaseInsensitive(output->name, simuData->equationInput[i].nameVector) == 0) {
                        output->fieldVariableId = 5;
                    } else {
                        sendErrorCodeAndMessage(511, input, output->fieldName, output->name, NULL);
                        return 511;
                    }
                    break;
                }
            }
            
            // config = MATERIAL;
            /* FALLTHRU */
            // break;
            
        case MATERIAL:
            // looking if the name refers to a material name
            for (int i = 0; i < simuData->quantityOfMaterialsRead; i++) {
                if (compareCaseInsensitive(output->fieldName, simuData->materialInput[i].materialName) == 0) {

                    // the name refers to a material. Looking for what variable
                    // it refers to
                    output->fieldLocation = MATERIAL;
                    output->fieldId = i;

                    if (compareCaseInsensitive(output->name, "diffusion_coefficient") == 0) {
                        output->fieldVariableId = 0;
                    } else if (compareCaseInsensitive(output->name, "coefficient_b") == 0) {
                        output->fieldVariableId = 1;
                    } else if (compareCaseInsensitive(output->name, "relaxation_time") == 0) {
                        output->fieldVariableId = 2;
                    } else if (compareCaseInsensitive(output->name, "sink_a") == 0) {
                        output->fieldVariableId = 3;
                    } else if (compareCaseInsensitive(output->name, "source") == 0) {
                        output->fieldVariableId = 4;
                    } else if (compareCaseInsensitive(output->name, "vectorial_source[0]") == 0) {
                        output->fieldVariableId = 5;
                    } else if (compareCaseInsensitive(output->name, "vectorial_source[1]") == 0) {
                        output->fieldVariableId = 6;
                    } else if (compareCaseInsensitive(output->name, "vectorial_source[2]") == 0) {
                        output->fieldVariableId = 7;
                    } else if (compareCaseInsensitive(output->name, "initial_scalar") == 0) {
                        output->fieldVariableId = 8;
                    } else if (compareCaseInsensitive(output->name, "density") == 0) {
                        output->fieldVariableId = 9;
                    } else if (compareCaseInsensitive(output->name, "specific_heat") == 0) {
                        output->fieldVariableId = 10;
                    } else if (compareCaseInsensitive(output->name, "thermal_conductivity") == 0) {
                        output->fieldVariableId = 11;
                    } else if (compareCaseInsensitive(output->name, "blood_perfusion") == 0) {
                        output->fieldVariableId = 12;
                    } else if (compareCaseInsensitive(output->name, "blood_density") == 0) {
                        output->fieldVariableId = 13;
                    } else if (compareCaseInsensitive(output->name, "blood_specific_heat") == 0) {
                        output->fieldVariableId = 14;
                    } else if (compareCaseInsensitive(output->name, "blood_temperature") == 0) {
                        output->fieldVariableId = 15;
                    } else if (compareCaseInsensitive(output->name, "internal_heat_generation") == 0) {
                        output->fieldVariableId = 16;
                    } else if (compareCaseInsensitive(output->name, "initial_temperature") == 0) {
                        output->fieldVariableId = 17;
                    } else {
                        sendErrorCodeAndMessage(521, input, output->fieldName, output->name, NULL);
                        return 521;
                    }
                    break;
                }
            }
            
            // config = BOUNDARY;
            /* FALLTHRU */
            // break;
            
        case BOUNDARY:
            // looking if the name refers to a boundary name
            for (int i = 0; i < simuData->quantityOfMaterialsRead; i++) {
                if (compareCaseInsensitive(output->fieldName, simuData->boundaryInput[i].boundaryName) == 0) {

                    // the name refers to a boundary. Looking for what variable
                    // it refers to
                    output->fieldLocation = BOUNDARY;
                    output->fieldId = i;

                    if (compareCaseInsensitive(output->name, "scalar") == 0) {
                        output->fieldVariableId = 0;
                    } else if (compareCaseInsensitive(output->name, "flux") == 0) {
                        output->fieldVariableId = 1;
                    } else if (compareCaseInsensitive(output->name, "convection_scalar") == 0) {
                        output->fieldVariableId = 2;
                    } else if (compareCaseInsensitive(output->name, "convection_coefficient") == 0) {
                        output->fieldVariableId = 3;
                    } else if (compareCaseInsensitive(output->name, "temperature") == 0) {
                        output->fieldVariableId = 4;
                    } else if (compareCaseInsensitive(output->name, "heat_flux") == 0) {
                        output->fieldVariableId = 5;
                    } else if (compareCaseInsensitive(output->name, "convection_temperature") == 0) {
                        output->fieldVariableId = 6;
                    } else if (compareCaseInsensitive(output->name, "radiation_temperature") == 0) {
                        output->fieldVariableId = 7;
                    } else if (compareCaseInsensitive(output->name, "radiation_emissivity") == 0) {
                        output->fieldVariableId = 8;
                    } else {
                        sendErrorCodeAndMessage(531, input, output->fieldName, output->name, NULL);
                        return 531;
                    }
                    break;
                }
            }
            
            // config = SOURCE;
            /* FALLTHRU */
            // break;
            
        case SOURCE:
            // config = FUNCTION;
            /* FALLTHRU */
            // break;
            
        case FUNCTION:
            break;
    }


    return 0;

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

