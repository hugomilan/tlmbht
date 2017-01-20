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
 * File:   libsimuconfig.h
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
 * contains declaration of functions, structures, and enumerations for configuring
 * the simulation. See libsimuconfig.c for more details.
 *
 */

#ifndef LIBSIMUCONFIG_H_INCLUDED
#define LIBSIMUCONFIG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

    enum benchmark // defines if it should do benchmark
    {
        DONT,
        RUN,
        RUN_AND_SOLVE,
        RUN_FAST,
        RUN_FAST_AND_SOLVE

    };

    struct Simulation // structure that contains the information for the simulation
    {
        enum benchmark Benchmark;

        int nOpenMPcores;
        int nOpenMPcoresMax;

        double AbsoluteZero;
        double StefanBoltzmann;

        char *nameOfInputFile;

        char *nameOfOutputFile;
        // --case: the same name as the name of the case input file. (default)
        // --mesh: the same name as the name of the mesh input.
        // any name you want.

        int outputFileExtension;
        // tmo (default): 1
        // m: 2

        // internal flags to write file
        int outputFileCreated;
        // 0: the output file was not created.
        // 1: the output file was already created.

        char *fullNameOfOutputFile;
        // contains the full name of the output file, i.e., nameOfOutputFile + outputFileExtension

        FILE *outputFileHandler;
        // contains the handler of the output file.
        
        char *comentary_string;
        // variable that will hold the commentary string for the output chosen

        int printAdditionalMode;
        // true: 1; will print additional information.
        // false: 0; print only essential information

        int verboseMode;
        // true: 1; set printAdditional to 1 and print debug information
        // false: 0; print only essential information

        int timingMode;
        // true: 1; shows the time it takes in each function
        // false: 0; don't show.

    };


    // function used to initialized the Simulation variable
    unsigned int initiateSimulationVariable(struct Simulation*);

    // function used to terminate the Simulation variable
    unsigned int terminateSimulationVariable(struct Simulation*);

    // function used to call the prints to print all the information from the simulation
    void printfAllSimuData(struct Simulation*);

    // specific printers
    void printfBench(enum benchmark *);

    void printfOutputFile(struct Simulation *);

    void printfPrintAdditionMode(int);
    void printfVerboseMode(int);
    void printfTimingMode(int);


    // take the input text and extract the values of the variables. Returns 0 if no error.
    unsigned int setConfigurationSimu(char *, struct Simulation *, int *);

    // this function will test if all necessary variables were correctly initialized. Return 0 if no error.
    unsigned int testInputSimu(struct Simulation *, char *);





#ifdef __cplusplus
}
#endif

#endif // LIBSIMUCONFIG_H_INCLUDED

