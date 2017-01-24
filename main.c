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
 * File:   main.c
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
 * 
 * Main file for the TLMBHT Version 0.2.0
 * We use Semantic Versioning 2.0.0 http://semver.org/
 * 
 * This function receives a script file as input and performs the operations requested
 * by the script file. See the README file to know my features.
 * 
 * In this software, the functions will return zero when they work correctly.
 * 
 */


/*
 * Acknowledgment: Brazilian National Counsel of Technological and Scientific
 * Development (Proc. 203312/2014-7) for scholarship to HFMM (2015-2016).
 */


/*
 * TODO (hugomilan#1#):
 * 1) radiation boundary condition. More specifically, non-linear BC and sources. 
 * 2) Create --help and --version.
 * 3) Find a best way to initiate the number of levels in 'allocateForEachLevel'
 * 4) Implement the source and function headers
 * 5) Create a debug mode
 * 6) make the equations interact, this is, solve for multiphysics.
 * 7) Create a automatic time-step algorithm.
 * 8) Parallelize (with openMP?) reading the mesh input file (libgmshreader and 
 * libtbnreader) and create matrices functions.
 * See instructions on my Netbeans folder. May be I should have
 * a flag that says until what file size I should parallelize (remember of 
 * parallelization overhead; it should get better as the file size increases though).
 * However, this is not priority. These functions consume so little computational time.
 * 9) Make the software international--i.e., outputs in languages other than English.
 * 10) Simulation header input is not required.
 * 
 * 
 *  List of possible improvements:
 * 1) Do we need a better algorithm to sort the number of ports in the
 * allocatePointsPort function? The sorting function is in libmiscellaneous.h
 * 2) I'm using qsort() in the function wrapTLMnumbers(). Should I change to a better
 * algorithm?
 * 3) test different libraries for solving sparse matrix equations.
 * 
 * List of tests that can be implemented:
 * 1) Test if inputs are within the expected range, are the required type, etc.
 * 2) Test if the inputs in Material and Boundary are for the selected equation.
 * 3) Test if the number of the material was set more than once for different 
 * materials of the same equation 
 * 4) What should be tested when starting the solvers?
 * 
 * Future works (or research):
 * 1) Use material line in 2D and 3D problems; use material triangle in 3D problems,
 * etc. How something like this should be implemented?
 * 
 */


/*
 * Including a new geometry for TLM? What functions should you see:
 * #) go to the function getTLMnumbers in the file libtlmsolver.c, see switch(l)
 * and define how ports and points will be allocated
 * 
 * #) file libtlmpenneseigen.cpp, function calculateMatricesPennesEigen. Create
 * a function material. Inside the newly created function, you have to  create
 * getGeometricalVariablesTLM inside the file tlmsolver.c
 * 
 * #) file tlmsolver.c, function getBetweenPointFromRealPortNumber. Create
 * a function getBetween.
 * 
 * #)  file tlmsolver.c, function getProjectionFromRealPortNumber. Create
 * a function getOutsideProjection.
 */


/* 
 * Standards used in this software:
 * 
 * #) Timing:
 * The function does not time itself. It times
 * sub-functions. The time is shown right after it was obtained. Sub-functions
 * known to be fast don't need to be timed. The timing is important for functions
 * that may take long times and are, hence, targets for optimization.
 * The total time of the function will be shown by the parent-function
 * that calls it. The main file is an exception.
 * 
 * 
 * #) Nomenclature:
 * Consider the tetrahedral geometry:
 * I call its whole geometry as element. I can also refer to it as node but,
 * in this case, node refers to the TLM mathematical model.
 * Sometimes, I rever to the vertex as point or node but, in these cases, they
 * refer to the geometrical element. If you want to remove this ambiguity referring
 * to 'node', you have to revise the whole code...
 * 
 *                      vertex 1
 *                          __
 *                         /\ \_
 *                        /  \  \_
 *                       /    \   \_
 *                      /      \    \_  edge 3
 *                     /        \     \_
 *                    /          \      \_
 *                   /          ar\ea 2   \_
 *                  /              \        \_
 *         edge 1  /                \  area 3 \_
 *                /      area 1      \ edge 2   \_  vertex 4
 *               /                    \    _____//
 *              /                  ____\__/     /
 *             /     edge 5 ______/     \      / edge 6
 *            /      ______/             \    /
 *           /______/           area 4    \  /
 * vertex 2 //_____________________________\/ vertex 3
 *                      edge 4
 * 
 */

#include <stdio.h>
#include <time.h>

#include "src/configs/libconfig.h"
#include "src/miscellaneous/liberrorcode.h"
#include "src/meshreader/libmeshreader.h"
#include "src/solver/libsolver.h"
#include "src/miscellaneous/benchmark/libbenchmark.h"

/*
 * Declaring functions used here in main
 */
unsigned int initializeAll(struct dataForSimulation *);
unsigned int terminateAll(struct dataForSimulation *);

/*
 * main: opens and reads the input file
 */
int main(int argc, char *argv[]) {
    clock_t begin = clock();
    struct dataForSimulation newDataConfig;
    unsigned int tlmErrorCode = 0;

    // initializing all the variables
    printf("Initiating the software...\n");
    if ((tlmErrorCode = initializeAll(&newDataConfig)) != 0) {
        sendErrorCodeAndMessage(12, NULL, NULL, NULL, NULL);
        return tlmErrorCode;
    }
    printf("\n");
    printfMyNameAndVersion(&newDataConfig);
    // If the name or the version are changed, see:
    // 1) myName and myVersion in dataForSimulation
    // 2) help file
    // 3) version file
    // 4) readme file
    // 5) CMakeLists.txt file

#if defined(_OPENMP)
    printf("With openMP.\n\n");
#else
    printf("\nWithout openMP. If you want openMP, re-compile the code and include the openMP library.\n\n\n");
#endif
    
    
    printf("Done initiating the software.\n");

    // Opening the input file
    printf("Reading the inputs...\n");
    clock_t begin_read = clock();
    if ((tlmErrorCode = testAndReadInputFile(argc, argv, &newDataConfig)) != 0) {
        terminateAll(&newDataConfig);
        return tlmErrorCode;
    }

    clock_t end_read = clock();
    printf("Done reading the inputs.\n");

    if (newDataConfig.simulationInput.timingMode == 1) {
        double time_spent_read = (double) (end_read - begin_read) / CLOCKS_PER_SEC;
        printf("\n\nTime to read the inputs %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent_read * 1e3, time_spent_read, time_spent_read / 60.0, time_spent_read / (60 * 60));
    }



    // Benchmark is future implementation
    if (newDataConfig.runningBenchmark == 1) {
        if ((tlmErrorCode = runBenchmark()) != 0) {
            terminateAll(&newDataConfig);
            return tlmErrorCode;
        }
    }

    // Will the simulation be run?
    if (newDataConfig.runningSimulation == 1) {

        if (newDataConfig.simulationInput.printAdditionalMode == 1)
            printfAllInputData(&newDataConfig);

        // Reading and Converting the input mesh if required
        printf("Reading the mesh file...\n");
        clock_t begin_read_mesh = clock();
        if ((tlmErrorCode = meshReaderAndConverter(&newDataConfig.meshInput,
                &newDataConfig.mesh)) != 0) {
            terminateAll(&newDataConfig);
            return tlmErrorCode;
        }
        clock_t end_read_mesh = clock();
        printf("Done reading the mesh file.\n");

        if (newDataConfig.simulationInput.timingMode == 1) {
            double time_spent_read_mesh = (double) (end_read_mesh - begin_read_mesh) / CLOCKS_PER_SEC;
            printf("\n\nTime to read the mesh file %g ms (or %g s, or %g min, or %g hours).\n\n",
                    time_spent_read_mesh * 1e3, time_spent_read_mesh, time_spent_read_mesh / 60.0, time_spent_read_mesh / (60 * 60));
        }

        if (newDataConfig.simulationInput.printAdditionalMode == 1)
            printfAllMeshData(&newDataConfig.mesh);


        // Solving
        printf("Starting the solver.\n");
        clock_t begin_solver = clock();
        if ((tlmErrorCode = solve(&newDataConfig)) != 0) {
            terminateAll(&newDataConfig);
            return tlmErrorCode;
        }
        clock_t end_solver = clock();
        printf("All the calculations are done.\n");

        if (newDataConfig.simulationInput.timingMode == 1) {
            double time_spent_solver = (double) (end_solver - begin_solver) / CLOCKS_PER_SEC;
            printf("\n\nTime to solve the problems %g ms (or %g s, or %g min, or %g hours).\n\n",
                    time_spent_solver * 1e3, time_spent_solver, time_spent_solver / 60.0, time_spent_solver / (60 * 60));
        }
    }


    //closing the input data for configuration
    if (newDataConfig.simulationInput.verboseMode == 1) {
        printf("Cleaning the used variables...\n");
    }

    if ((tlmErrorCode = terminateAll(&newDataConfig)) != 0)
        return tlmErrorCode;

    if (newDataConfig.simulationInput.verboseMode == 1) {
        printf("Done cleaning the used variables.\n");
    }


    clock_t end = clock();

    if (newDataConfig.simulationInput.timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("\n\nTotal execution time %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }

    return 0;
}

/*
 * initializeAll: initialize all the required variables.
 */
unsigned int initializeAll(struct dataForSimulation *input) {
    unsigned int errorTLMnumber;

    // initializing the configuration variables
    if ((errorTLMnumber = initiateAllConfigurationVarialbes(input)) != 0)
        return errorTLMnumber;

    // initializing the mesh variable.
    if ((errorTLMnumber = initiateTlmInternalMesh(&input->mesh)) != 0)
        return errorTLMnumber;

    return 0;
}

/*
 * terminateAll: Terminate the variables. This is, free the ones that were allocated.
 * As standard, I free the variables and then assigned them the value NULL. I do this
 * so that, in case the variable is accessed afterwards, the software won't crash.
 */
unsigned int terminateAll(struct dataForSimulation *input) {
    unsigned int errorTLMnumber;

    // terminating the configuration variables
    if ((errorTLMnumber = terminateAllConfigurationVarialbes(input)) != 0)
        return errorTLMnumber;

    // terminating the internal mesh variables
    if ((errorTLMnumber = terminateTlmInternalMesh(&input->mesh)) != 0)
        return errorTLMnumber;

    return 0;
}