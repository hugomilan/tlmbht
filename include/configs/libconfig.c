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

#include "libconfig.h"
#include "libsimuconfig.h"
#include "libmeshconfig.h"
#include "libmatconfig.h"
#include "libboundconfig.h"
#include "libsourceconfig.h"
#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * testAndReadInputFile: Tests if the input file exists and call the function
 * responsible for reading it.
 */
unsigned int testAndReadInputFile(int nInput, char* pArgs[], struct dataForSimulation *newDataForSimu) {

    unsigned int tlmErrorCode, inputPosition = 0;
    FILE *pfile;

    // checking the inputs. I don't check input 0 because this is just
    // the function being called
    for (unsigned int i = 1; i < nInput; i++) {
        if (compareCaseInsensitive(pArgs[i], "--debug") == 0) {
            newDataForSimu->debugMode = 1;
            //            printf("Debug mode\n");
            printf("\n\n\nDebug mode was not implemented yet\n\n\n");

        } else if (compareCaseInsensitive(pArgs[i], "--timing") == 0) {
            newDataForSimu->timingMode = 1;
            printf("Timing mode\n");

        } else if (compareCaseInsensitive(pArgs[i], "--input") == 0) {
            inputPosition = i;

        } else {
            printf("Unknown command %s\n", pArgs[i]);
        }

    }

    // checking if the number of inputs is greater than 1. If not, them the case.tlm
    // file should exist in the same folder where the call is being made
    if (inputPosition == 0) {
        pfile = fopen("case.tlm", "r");
        if (pfile == NULL)
            sendErrorCodeAndMessage(315, NULL, NULL, NULL, NULL);

        printf("Reading case.tlm\n");

        // We received an input
    } else {
        printf("Input file: %s\n", pArgs[inputPosition]);

        pfile = fopen(pArgs[inputPosition], "r");

        //We couldn't find the input file
        if (pfile == NULL) {
            sendErrorCodeAndMessage(764, pArgs[1], NULL, NULL, NULL);
            return 764;
        }
    }

    clock_t begin = clock();

    // we could open the input. Now let's read it
    if ((tlmErrorCode = readFileTLM(pfile, newDataForSimu)) != 0) {
        return tlmErrorCode;
    }

    clock_t end = clock();

    if (newDataForSimu->timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("\n\nTime to read the input file %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }



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
    size_t lenLine = 0;


    int startEndBrackets = 0; // 0 - not started or finalized; 1 - started;
    enum configuringInside ConfigPoint = NOTHING;

    // readings line-by-line until we get to the end-of-file character
    while (getline(&pline, &lenLine, f) != EOF) {
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



        // if (newDataForSimu->debugMode == 1){
        // DEBUG: Shown the line number, quantity of characters in the line and content
        // printf("Line %04u (%zu): %s", lineNumber, strlen(pline), pline); 
        // }



        // I will copy the content of pline to show it for troubleshooting purposes.
        lineOriginal = (char *) realloc(lineOriginal, strlen(pline) + 1);
        strcpy(lineOriginal, pline);



        // It returns 0 if it got some useful content on that line.
        // it returns 9998 if it got nothing useful
        // if there's nothing useful or an error, we should go to the next loop iteration.
        if ((errorTLMnumber = getUsefulContent(pline)) != 0)
            continue; // go to next iteration


        // if (newDataForSimu->debugMode == 1){
        // DEBUG: what comes out the useful test. Shown the line number, quantity of characters in the line and content
        // printf("This line has something useful %04u (%zu): %s\n", lineNumber, strlen(pline), pline);
        // }


        switch (ConfigPoint) {
                // is ConfigPoint NOTHING? If so, we should check the line to find out
                // what sould we configure next
            case NOTHING:
                // DEBUG: got inside nothing
                // printf("Inside NOTHING %s", pline);

                // checking if we need to change what we are configuring now
                if ((errorTLMnumber = setConfigurationTo(pline, &ConfigPoint)) != 0)
                    continue; // go to the next iteration if we found an error

                break;

            case SIMULATION:
                // DEBUG: got inside simulation
                // printf("Inside SIMULATION %s", pline);

                // call the function responsible for configuring the simulation parameters
                if ((errorTLMnumber = setConfigurationSimu(pline, &newDataForSimu->simulationInput, &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error

                break;

            case MESH:
                // DEBUG: got inside mesh
                // printf("Inside MESH %s", pline);

                // call the function responsible for configuring the mesh parameters
                if ((errorTLMnumber = setConfigurationMesh(pline, &newDataForSimu->meshInput, &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 
                break;

            case MATERIAL:
                // DEBUG: got inside material
                // printf("Inside MATERIAL %s\n", pline);

                // call the function responsible for configuring the material parameters
                if ((errorTLMnumber = setConfigurationMaterial(pline,
                        &newDataForSimu->materialInput[newDataForSimu->quantityOfMaterialsRead],
                        &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 
                break;

            case BOUNDARY:
                // DEBUG: got inside boundary
                // printf("Inside BOUNDARY %s\n", pline);

                // call the function responsible for configuring the boundary parameters
                if ((errorTLMnumber = setConfigurationBoundary(pline,
                        &newDataForSimu->boundaryInput[newDataForSimu->quantityOfBoundariesRead],
                        &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 

                break;
            case SOURCES:
                // DEBUG: got inside source
                // printf("Inside SOURCE %s\n", pline);

                // call the function responsible for configuring the source parameters
                if ((errorTLMnumber = setConfigurationSource(pline,
                        &newDataForSimu->sourceInput[newDataForSimu->quantityOfSourcesRead],
                        &startEndBrackets)) != 0)
                    continue; // go to the next loop if we found an error 

                break;

                // this is not expected to happen
            default:
                fprintf(stderr, "ERROR: Unknown error reading the file. Code 8.");
                errorTLMnumber = 8;
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
    input->debugMode = 0;
    input->timingMode = 0;
    input->meshRead = 0;
    input->quantityOfMaterialsRead = 0;
    input->quantityOfBoundariesRead = 0;
    input->quantityOfSourcesRead = 0;

    // initializing simulation variables used for configuration
    if ((errorTLMnumber = initiateSimulationVariable(&input->simulationInput)) != 0)
        return errorTLMnumber;

    // initializing mesh variables used for configuration
    if ((errorTLMnumber = initiateMeshVariable(&input->meshInput)) != 0)
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

    // terminating the simulation variables used for configuration
    if ((errorTLMnumber = terminateSimulationVariable(&input->simulationInput)) != 0)
        return errorTLMnumber;

    // terminating the mesh variables used for configuration
    if ((errorTLMnumber = terminateMeshVariable(&input->meshInput)) != 0)
        return errorTLMnumber;

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
    printf("Information read from the input file:\n");
    // information from the simulation header
    printfAllSimuData(&input->simulationInput);
    printf("\n");

    // information from the mesh header
    printfMeshConfig(&input->meshInput);
    printf("\n");

    // information from the material headers
    printf("Input data for the Materials configuration:\n");
    for (int i = 0; i < input->quantityOfMaterialsRead; i++) {
        printf("Materials group %04d:\n", i + 1);
        printfMatConfig(&input->materialInput[i],
                &input->simulationInput.typeS, &input->simulationInput.Solv);
        printf("\n");
    }

    // information from the boundary headers
    printf("Input data for the Boundaries configuration:\n");
    for (int i = 0; i < input->quantityOfBoundariesRead; i++) {
        printf("Boundaries group %04d:\n", i + 1);
        printfBoundConfig(&input->boundaryInput[i],
                &input->simulationInput.typeS);
        printf("\n");
    }

    // future implementation
    // information from the source headers
    printf("Input data for the Sources configuration:\n");
    for (int i = 0; i < input->quantityOfSourcesRead; i++) {
        printf("Sources group %04d:\n", i + 1);
        printfSourceConfig(&input->sourceInput[i],
                &input->simulationInput.typeS);
        printf("\n");
    }

}

/*
 * testAllConfigurationVarialbes: tests if all the configuration variables
 * were adequately read
 */
unsigned int testAllConfigurationVarialbes(struct dataForSimulation *input) {
    unsigned int errorFound = 0;
    int i;

    // tests some variables that dictates if we are going to run the simulation
    // or not
    if ((errorFound = testRunSimulation(input)) != 0) {
        // if errorFound = 9999, then, we are not going to run simulation and,
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
    if (testInputSimu(&input->simulationInput) != 0) {
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
    if (testInputMesh(&input->meshInput) != 0) {
        sendErrorCodeAndMessage(1871, NULL, NULL, NULL, NULL);
        errorFound = 1;
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
        if (testInputMaterial(&input->materialInput[i - 1],
                &input->simulationInput.typeS, &input->simulationInput.Solv, i) != 0) {
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
        if (testInputBoundary(&input->boundaryInput[i - 1],
                &input->simulationInput.typeS, i) != 0) {
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
        if (testInputSource(&input->sourceInput[i - 1],
                &input->simulationInput.typeS, i) != 0) {
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
