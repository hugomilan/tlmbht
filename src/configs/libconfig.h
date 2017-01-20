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
 * File:   libconfig.h
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
 * contains declaration of functions, structures, and enumerations for general
 * configurations. See libconfig.c for more details.
 *
 */


#ifndef LIBCONFIG_H_INCLUDED
#define LIBCONFIG_H_INCLUDED



#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#include "libsimuconfig.h"
#include "libequationconfig.h"
#include "libmeshconfig.h"
#include "libmatconfig.h"
#include "libboundconfig.h"
#include "libsourceconfig.h"
#include "../meshreader/libmeshtlmbht.h"

    enum configuringInside // Types of configurations
    {
        NOTHING,        // it is not inside any specific configuration group
        SIMULATION,     // it is inside the SIMULATION configuration group
        MESH,           // it is inside the MESH configuration group
        EQUATION,       // it is inside the EQUATION configuration group
        MATERIAL,       // it is inside the MATERIAL configuration group
        BOUNDARY,       // it is inside the BOUNDARY configuration group
        SOURCES         // it is inside the SOURCES configuration group
    };


    // this variable contains all the informations for the data input

    struct dataForSimulation {
        struct Simulation simulationInput;

        struct MeshConfig meshInput;

        struct Equation *equationInput;
        int quantityOfEquationsRead;

        struct tlmInternalMesh mesh;

        struct MaterialConfig *materialInput;
        int quantityOfMaterialsRead;

        struct BoundaryConfig *boundaryInput;
        int quantityOfBoundariesRead;

        struct SourceConfig *sourceInput;
        int quantityOfSourcesRead;

        //flags
        int simulationRead;
        int meshRead;

        // these variables help in managing the algorithm flow.
        int runningSimulation; // 0: not running simulation; 1: running simulation;
        int runningBenchmark; // 0: not running benchmark; 1: running benchmark;

        // who am I
        char *myName;
        char *myVersion;

    };

    // do basic testings on the opening file and call the responsible functions
    // for doing the opening
    unsigned int testAndReadInputFile(int, char*[], struct dataForSimulation *);


    // this function reads the input file
    unsigned int readFileTLM(FILE *, struct dataForSimulation *);

    // this function set the configuration type
    unsigned int setConfigurationTo(char *, enum configuringInside *);

    // this function initiate all the configuration variables
    unsigned int initiateAllConfigurationVarialbes(struct dataForSimulation *);

    // this function terminates all the configuration variables
    unsigned int terminateAllConfigurationVarialbes(struct dataForSimulation *);

    // this function tests all the configuration variables
    unsigned int testAllConfigurationVarialbes(struct dataForSimulation *);

    // this function tests if we are going to solve the problem or just do some testings
    unsigned int testRunSimulation(struct dataForSimulation *);

    // this function prints all the information from the input file
    void printfAllInputData(struct dataForSimulation *);

#ifdef __cplusplus
}
#endif

#endif // LIBCONFIG_H_INCLUDED
