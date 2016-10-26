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

#include "libsimuconfig.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <omp.h>

#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * TODO (hugomilan#1#): List of possible improvements
 * 1) Implement the second option for method
 * 2) allow for input of expressions, like cos(30)*20
 * 3) test if the inputs are within a certain expected range, what is supposed 
 * to be greater than zero is greater than zero, send warnings otherwise
 */

/*
 * initiateSimulationVariable: initialize the simulation variable used for configuration
 * with standard values
 */
unsigned int initiateSimulationVariable(struct Simulation* simu) {
    simu->typeS = PENNES;
    simu->dimen = TWO;
    simu->Meth = TLM;
    simu->Meth2 = NONE;
    simu->Solv = DYNAMIC;
    simu->Benchmark = DONT;
    simu->timeStep = 1;
    simu->timeJump = 1;
    simu->finalTime = 1;
    simu->AbsoluteZero = -273.15;
    simu->StefanBoltzmann = 5.6704e-8;
    simu->printAdditional = 0;

    simu->libraryForCalculation = EIGEN;
    
    simu->nOpenMPcores = omp_get_max_threads(); // maximum number of cores allowed to run
    simu->nOpenMPcoresMax = omp_get_num_procs(); // maximum possible number of cores
    

    //flags to identify if the required parameters were defined.
    // 0 = not defined
    simu->typeSimDefined = 0;
    simu->dimSimDefined = 0;
    simu->methodDefined = 0;
    simu->solveDefined = 0; // 2 if dynamic simulation
    simu->timeStepDefined = 0; // only required if dynamic simulation
    simu->finalTimeDefined = 0; // only required if dynamic simulation

    // flags to what to save
    simu->saveTemperature = 0;
    simu->saveTemperatureBetween = 0;
    simu->saveHeatFlux = 0;

    return 0;
}

/*
 * terminateSimulationVariable: terminate the simulation variable used for configuration
 */
unsigned int terminateSimulationVariable(struct Simulation* simu) {
    return 0;
}

/*
 * printfAllSimuData: print the status of all the simulation variables
 */
void printfAllSimuData(struct Simulation* simu) {

    printf("Input data for the Simulation configuration:\n");

    printfTypeS(&simu->typeS);
    printfDimen(&simu->dimen);
    printfMeth(&simu->Meth);
    // printfMeth2(&simu->Meth2); // future implementation
    printfSolv(&simu->Solv);
    if (simu->Solv == DYNAMIC) {
        printf("Time-step = %9.4e s.\n", simu->timeStep);
        printf("Time-jump = %4d.\n", simu->timeJump);
        printf("Final time = %9.4e s.\n", simu->finalTime);
    }

    printWhatToSave(simu);

    printf("Absolute zero = %9.4e oC.\n", simu->AbsoluteZero);
    printf("Stefan-Boltzmann constant = %9.4e (W/(K4-m2)).\n", simu->StefanBoltzmann);
    printfBench(&simu->Benchmark);
    printfLibrary(&simu->libraryForCalculation);
    printf("Using %d cores of maximum of %d.\n",simu->nOpenMPcores, simu->nOpenMPcoresMax);
}

/*
 * printfTypeS: prints the type of simulation
 */
void printfTypeS(enum typeSim *type) {

    printf("Solving ");
    switch (*type) {
        case PENNES: printf("Pennes' equation");
            break;
        case HEAT: printf("heat transfer equation"); // future implementation
            break;
        case EM: printf("electromagnetism scattering"); // future implementation
            break;
        case CFD: printf("computational fluid mechanics"); // future implementation
            break;
        default: printf("Unknown");
    }

    printf(".\n");

}

/*
 * printfDimen: prints the dimension of simulation
 */
void printfDimen(enum dimSim *dim) {
    printf("Solving in ");
    switch (*dim) {
        case ONE: printf("one-dimension");
            break;
        case TWO: printf("two-dimensions"); // future implementation
            break;
        case THREE: printf("three-dimensions"); // future implementation
            break;
        default: printf("Unknown");
    }

    printf(".\n");
}

/*
 * printfMeth: prints the method choose for solving the simulation
 */
void printfMeth(enum method * meth) {
    printf("Using ");
    switch (*meth) {
        case TLM: printf("transmission-line modeling method");
            break;
        case FDTD: printf("finite-difference time-domain method"); // future implementation
            break;
        case FEM: printf("finite element method"); // future implementation
            break;
        case ANALYTICAL: printf("analytical method"); // future implementation
            break;
        default: printf("Unknown");
    }

    printf(".\n");
}

/*
 * printfSolv: prints if the solution is for steady state or dynamic problems
 */
void printfSolv(enum solve *sol) {
    printf("Solving ");
    switch (*sol) {
        case STEADY: printf("steady state problem");
            break;
        case DYNAMIC: printf("dynamic problem"); // future implementation
            break;
        default: printf("Unknown");
    }

    printf(".\n");
}

/*
 * printWhatToSave: prints what was choosen to be saved
 */
void printWhatToSave(struct Simulation* simu) {
    int savingSomething = 0;

    switch (simu->Meth) {
        case TLM:
            switch (simu->typeS) {
                case PENNES:
                    /* FALLTHRU */
                case HEAT:
                    if (simu->saveTemperature == 1) {
                        savingSomething = 1;
                        printf("Saving the temperatures inside the elements.\n");
                    }

                    if (simu->saveTemperatureBetween == 1) {
                        savingSomething = 1;
                        printf("Saving the temperatures between the elements.\n");
                    }

                    if (simu->saveHeatFlux == 1) {
                        savingSomething = 1;
                        printf("Saving the heat flux.\n");
                    }

                    if (savingSomething == 0)
                        fprintf(stderr, "\nWARNING: Nothing was selected to be saved.\nTherefore, only the "
                            "elements TLM nodes' voltages will be calculated and saved.\nTemperature "
                            "and heat flux can be obtained from TLM nodes' voltage as post-processing.\n\n");

                    break;
                case EM: // future implementation
                    break;
                case CFD: // future implementation
                    break;
            }
            break;



        case FEM:
            break;



        case FDTD:
            break;



        case ANALYTICAL:
            break;
    }
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
 * printfBench: prints the library being used for the calculation
 */
void printfLibrary(enum libraryForCalc* lib) {
    switch (*lib) {
        case FOOL:
            printf("Using a fool (a much slower) implementation.\n");
            break;
        case EIGEN:
            printf("Using Eigen library.\n");
            break;
        case CUDA:
            printf("Using CUDA library.\n");
            break;
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

    } else if (compareCaseInsensitive(input, "type") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "pennes") == 0) {
            configInput->typeS = PENNES;
        } else if (compareCaseInsensitive(input, "heat") == 0) {
            configInput->typeS = HEAT;
        } else if (compareCaseInsensitive(input, "em") == 0) {
            configInput->typeS = EM;
        } else if (compareCaseInsensitive(input, "CFD") == 0) {
            configInput->typeS = CFD;
        } else {
            return 3874;
        }

        // flag that the simulation type was defined
        configInput->typeSimDefined = 1;

    } else if (compareCaseInsensitive(input, "dimensions") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;



        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%u", &tempVarInt);

        switch (tempVarInt) {
            case 1: configInput->dimen = ONE;
                break;
            case 2: configInput->dimen = TWO;
                break;
            case 3: configInput->dimen = THREE;
                break;
            default:
                return 3875;
        }

        // flag that the dimension was defined
        configInput->dimSimDefined = 1;

    } else if (compareCaseInsensitive(input, "method") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);


        if (compareCaseInsensitive(input, "tlm") == 0) {
            configInput->Meth = TLM;
        } else if (compareCaseInsensitive(input, "fdtd") == 0) {
            configInput->Meth = FDTD;
        } else if (compareCaseInsensitive(input, "fem") == 0) {
            configInput->Meth = FEM;
        } else if (compareCaseInsensitive(input, "ana") == 0) {
            configInput->Meth = ANALYTICAL;
        } else {
            return 3876;
        }

        // flag that the simulation type was defined
        configInput->methodDefined = 1;



    } else if (compareCaseInsensitive(input, "adjust method") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);


        // return 3877;// this will be the error

        // Future implementation

    } else if (compareCaseInsensitive(input, "solve") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "steady") == 0) {
            configInput->Solv = STEADY;
            // flag that the simulation type was defined
            configInput->solveDefined = 1;
        } else if (compareCaseInsensitive(input, "dynamic") == 0) {
            configInput->Solv = DYNAMIC;
            // flag that the simulation type was defined
            configInput->solveDefined = 2;
        } else {
            return 3878;
        }



    } else if (compareCaseInsensitive(input, "time-step") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &configInput->timeStep);
        configInput->timeStepDefined = 1;

    } else if (compareCaseInsensitive(input, "time-jump") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%u", &tempVarInt);

        configInput->timeJump = tempVarInt;

    } else if (compareCaseInsensitive(input, "final time") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &configInput->finalTime);
        configInput->finalTimeDefined = 1;

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

    } else if (compareCaseInsensitive(input, "print additional") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "true") == 0) {
            configInput->printAdditional = 1;
        } else if (compareCaseInsensitive(input, "false") == 0) {
            configInput->printAdditional = 0;
        } else {
            return 3880;
        }



    } else if (compareCaseInsensitive(input, "save") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);

        // DEBUG: the input after removing blank spaces
        // printf("%s\n", input);
        if (compareCaseInsensitive(input, "temperature") == 0) {
            configInput->saveTemperature = 1;
        } else if (compareCaseInsensitive(input, "temperature between") == 0) {
            configInput->saveTemperatureBetween = 1;
        } else if (compareCaseInsensitive(input, "heat flux") == 0) {
            configInput->saveHeatFlux = 1;
        } else {
            return 3881;
        }



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

    } else if (compareCaseInsensitive(input, "library") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);

        if (compareCaseInsensitive(input, "fool") == 0) {
            configInput->libraryForCalculation = FOOL;
        } else if (compareCaseInsensitive(input, "eigen") == 0) {
            configInput->libraryForCalculation = EIGEN;
        } else if (compareCaseInsensitive(input, "cuda") == 0) {
            configInput->libraryForCalculation = CUDA;
        } else {
            return 3883;
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
        
        omp_set_num_threads(configInput->nOpenMPcores);

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
unsigned int testInputSimu(struct Simulation *simu) {
    int errorFound = 0;

    // type of simulation was not defined
    if (simu->typeSimDefined == 0) {
        sendErrorCodeAndMessage(1858, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }
    // dimension of simulation was not
    if (simu->dimSimDefined == 0) {
        sendErrorCodeAndMessage(1859, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }
    // method used to solve the simulation was not defined
    if (simu->methodDefined == 0) {
        sendErrorCodeAndMessage(1860, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }
    // the solver (steady or dynamic) was not defined
    if (simu->solveDefined == 0) {
        sendErrorCodeAndMessage(1861, NULL, NULL, NULL, NULL);
        errorFound = 1;
    }
    if (simu->solveDefined == 2) {
        // the solver was defined as dynamic but time-step was not defined
        if (simu->timeStepDefined == 0) {
            sendErrorCodeAndMessage(1862, NULL, NULL, NULL, NULL);
            errorFound = 1;
        }
        // the solver was defined as dynamic but the final time was not defined
        if (simu->finalTimeDefined == 0) {
            sendErrorCodeAndMessage(1863, NULL, NULL, NULL, NULL);
            errorFound = 1;
        }
    }
    return errorFound;
}


