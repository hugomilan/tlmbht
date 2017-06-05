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
 * File:   libsimuconfig.c
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
 * This file contains the variables and functions used for configuring the simulation.
 * Also important is the header file.
 */



#include <stdlib.h>
#include <string.h>

// ignoring openmp when it is not available
#if defined(_OPENMP)
#include <omp.h>
#endif

#include "libsimuconfig.h"

#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * initiateSimulationVariable: initialize the simulation variable used for configuration
 * with standard values
 */
unsigned int initiateSimulationVariable(struct Simulation* simu) {
    simu->Benchmark = DONT;

#if defined(_OPENMP)
    simu->nOpenMPcores = omp_get_max_threads(); // maximum number of cores allowed to run
    simu->nOpenMPcoresMax = omp_get_num_procs(); // maximum number of possible cores
#else
    simu->nOpenMPcores = 1; // maximum number of cores allowed to run
    simu->nOpenMPcoresMax = 1; // maximum number of possible cores
#endif

    simu->AbsoluteZero = -273.15;
    simu->StefanBoltzmann = 5.6704e-8;

    simu->nameOfInputFile = NULL;
    simu->nameOfOutputFile = malloc( ( strlen("--case")  + 1 )*sizeof(char));
    strcpy(simu->nameOfOutputFile,"--case");
    // Initialize this variable with the default value "--case". When the input is read, I will allocate
    // the name on it without the extension. The extension will be used to define
    // how to write the output.
    simu->outputFileExtension = 1;

    simu->outputFileCreated = 0;
    simu->fullNameOfOutputFile = NULL;

    simu->outputFileHandler = NULL;

    simu->comentary_string = NULL;

    simu->printAdditionalMode = 0;
    simu->verboseMode = 0;
    simu->timingMode = 0;

    return 0;
}

/*
 * terminateSimulationVariable: terminate the simulation variable used for configuration
 */
unsigned int terminateSimulationVariable(struct Simulation* simu) {
    unsigned int errorTLMnumber = 0;

    free(simu->nameOfInputFile);
    simu->nameOfInputFile = NULL;

    free(simu->nameOfOutputFile);
    simu->nameOfOutputFile = NULL;

    // I will do fclose() if the handler is different than NULL
    if (simu->outputFileHandler != NULL &&
            fclose(simu->outputFileHandler) == EOF) {
        errorTLMnumber = 345;
        printf("Output file: %s", simu->fullNameOfOutputFile);
    }

    free(simu->fullNameOfOutputFile);
    simu->fullNameOfOutputFile = NULL;

    free(simu->comentary_string);
    simu->comentary_string = NULL;

    return errorTLMnumber;
}

/*
 * printfAllSimuData: print the status of all the simulation variables
 */
void printfAllSimuData(struct Simulation* simu) {

    printf("Input data for the Simulation configuration:\n");

    printfBench(&simu->Benchmark);
    printf("Using %d cores of maximum of %d.\n", simu->nOpenMPcores, simu->nOpenMPcoresMax);

    printf("Absolute zero = %9.4e oC.\n", simu->AbsoluteZero);
    printf("Stefan-Boltzmann constant = %9.4e (W/(K4-m2)).\n", simu->StefanBoltzmann);

    //    printfInputCaseFile()
    printf("Input case file: %s\n", simu->nameOfInputFile);

    printfOutputFile(simu);

    printfPrintAdditionMode(simu->verboseMode);
    printfVerboseMode(simu->verboseMode);
    printfTimingMode(simu->verboseMode);

}

/*
 * printfBench: prints if we will do benchmark
 */
void printfBench(enum benchmark* bench) {
    switch (*bench) {
        case DONT:
            break;
        case RUN:
            printf("Running benchmark and exiting\n");
            break;
        case RUN_AND_SOLVE:
            printf("Running benchmark and solving the problem.\n");
            break;
        case RUN_FAST:
            printf("Running simplified benchmark and exiting.\n");
            break;
        case RUN_FAST_AND_SOLVE:
            printf("Running simplified benchmark and solving the problem.\n");
            break;
    }
}

/*
 * printfOutputFile: prints the name the extension of the output file
 */
void printfOutputFile(struct Simulation *simu) {
    printf("Output file: %s.", simu->nameOfOutputFile);
    switch (simu->outputFileExtension) {

        case 1: // tmo file; standard tlmbht
            printf("tmo\n");
            break;

        case 2: // m file: to open in matlab/octave
            printf("m\n");
            break;

        default:
            printf("Unknown error\n");
    }
}

/*
 * printfPrintAdditionMode: prints the status of the printAdditionalMode
 */
void printfPrintAdditionMode(int input) {
    printf("Print additional mode: ");
    switch (input) {
        case 0:
            printf("OFF\n");
            break;

        case 1:
            printf("ON\n");
            break;

        default:
            printf("Unknown error\n");
    }
}

/*
 * printfVerboseMode: prints the status of the verboseMode
 */
void printfVerboseMode(int input) {
    printf("Verbose mode: ");
    switch (input) {
        case 0:
            printf("OFF\n");
            break;

        case 1:
            printf("ON\n");
            break;

        default:
            printf("Unknown error\n");
    }
}

/*
 * printfTimingMode: prints the status of the TimingMode
 */
void printfTimingMode(int input) {
    printf("Timing mode: ");
    switch (input) {
        case 0:
            printf("OFF\n");
            break;

        case 1:
            printf("ON\n");
            break;

        default:
            printf("Unknown error\n");
    }
}

/*
 * setConfigurationSimu: reads the line and configures the required simulation parameter
 */
unsigned int setConfigurationSimu(char * input, struct Simulation * configInput, int * startEndBrackets) {
    unsigned int errorTLMnumber, tempVarInt;

    // DEBUG: Shows that we got inside and the input
    // printf("inside condigurationSimu %s\n", input);

    // The Brackets for SIMULATION were not started yet
    if (*startEndBrackets == 0) {
        if ((errorTLMnumber = testStartBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 1; //define as the bracket started

    } else if (compareCaseInsensitive(input, "benchmark") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);

        if (compareCaseInsensitive(input, "dont") == 0) {
            configInput->Benchmark = DONT;
        } else if (compareCaseInsensitive(input, "run") == 0) {
            configInput->Benchmark = RUN;
        } else if (compareCaseInsensitive(input, "run and solve") == 0) {
            configInput->Benchmark = RUN_AND_SOLVE;
        } else if (compareCaseInsensitive(input, "run fast") == 0) {
            configInput->Benchmark = RUN_FAST;
        } else if (compareCaseInsensitive(input, "run fast and solve") == 0) {
            configInput->Benchmark = RUN_FAST_AND_SOLVE;
        } else {
            return 3882;
        }



    } else if (compareCaseInsensitive(input, "OpenMP cores") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);

        if (compareCaseInsensitive(input, "max") == 0) {
            configInput->nOpenMPcores = configInput->nOpenMPcoresMax;
        } else {
            sscanf(input, "%d", &configInput->nOpenMPcoresMax);
            // return 3884;
        }

#if defined(_OPENMP)
        omp_set_num_threads(configInput->nOpenMPcores);
#endif

    } else if (compareCaseInsensitive(input, "absolute zero") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // as this can be a negative number, we need an additional step to remove
        // all the blank spaces.
        tempVarInt = 0;
        unsigned int tempVarInt2 = 0;
        while (input[tempVarInt2]) {
            if (input[tempVarInt2] != ' ')
                input[(tempVarInt)++] = input[tempVarInt2]; // here count is
            tempVarInt2++;
        }
        input[tempVarInt] = '\0';

        sscanf(input, "%lf", &configInput->AbsoluteZero);



    } else if (compareCaseInsensitive(input, "stefan-boltzmann constant") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &configInput->StefanBoltzmann);



    } else if (compareCaseInsensitive(input, "output name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);
        
        // add one for the terminating character '\0'
        configInput->nameOfOutputFile = realloc(configInput->nameOfOutputFile, (strlen(input) + 1)*sizeof (char) );
        strcpy(configInput->nameOfOutputFile, input);


    } else if (compareCaseInsensitive(input, "output extension") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // I will put the correct name here latter, when I do the input testing.
        if (compareCaseInsensitive(input, "tmo") == 0) {
            configInput->outputFileExtension = 1;

        } else if (compareCaseInsensitive(input, "m") == 0) {
            configInput->outputFileExtension = 2;

        } else {
            return 3890;

        }


    } else if (compareCaseInsensitive(input, "timing mode") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "true") == 0) {
            configInput->timingMode = 1;
            printf("Timing mode on\n");
        } else if (compareCaseInsensitive(input, "false") == 0) {
            configInput->timingMode = 0;
            printf("Timing mode off\n");
        } else {
            return 3891;
        }



    } else if (compareCaseInsensitive(input, "print additional") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "true") == 0) {
            configInput->printAdditionalMode = 1;
            printf("Print additional mode on\n");
        } else if (compareCaseInsensitive(input, "false") == 0) {
            configInput->printAdditionalMode = 0;
            printf("Print additional mode off\n");
        } else {
            return 3880;
        }



    } else if (compareCaseInsensitive(input, "verbose mode") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "true") == 0) {
            configInput->verboseMode = 1;
            printf("Verbose mode on\n");
        } else if (compareCaseInsensitive(input, "false") == 0) {
            configInput->verboseMode = 0;
            printf("Verbose mode off\n");
        } else {
            return 3892;
        }



    } else if (input[0] == '}' && *startEndBrackets == 1) {
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
 * testInputSimu: tests if all the required inputs were read
 */
unsigned int testInputSimu(struct Simulation *simu, char *meshName) {
    int errorFound = 0;

    // Extra things to test here:
    // 1) OpenMP cores?
    // 2) Absolute zero?
    // 3) Stefan-Boltzmann constant?

    // Now I will adjust the nameOfOutputFile
    if (compareCaseInsensitive(simu->nameOfOutputFile, "--case") == 0) {

        simu->nameOfOutputFile = realloc(simu->nameOfOutputFile,
                sizeof (char)*(strlen(simu->nameOfInputFile) + 1));

        strcpy(simu->nameOfOutputFile, simu->nameOfInputFile);

    } else if (compareCaseInsensitive(simu->nameOfOutputFile, "--mesh") == 0) {

        simu->nameOfOutputFile = (char *) realloc(simu->nameOfOutputFile,
                sizeof (char)*(strlen(meshName) + 1));

        strcpy(simu->nameOfOutputFile, meshName);

    }


    // if verbose mode is ON, also is print additional mode.
    if (simu->verboseMode == 1) {
        simu->printAdditionalMode == 1;
    }

    return errorFound;
}


