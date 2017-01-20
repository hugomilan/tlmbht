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
 * File:   libequationconfig.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on January 12, 2017.
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
 * Contains the configuration code for setting up the governing equation and its
 * solution procedure.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libequationconfig.h"

#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * initiateSimulationVariable: initialize the simulation variable used for configuration
 * with standard values
 */
unsigned int initiateEquationVariable(struct Equation* equation) {

    equation->libraryForCalculation = EIGEN;

    equation->typeS = PENNES;
    equation->typeSimDefined = 0;

    equation->equationName = NULL;
    equation->equationNameDefined = 0;

    equation->materialNumbers = NULL;
    equation->numberOfMaterials = 0;

    equation->boundaryNumbers = NULL;
    equation->numberOfBoundaries = 0;

    equation->nameScalar = NULL;
    equation->nameVector = NULL;

    equation->dimen = TWO;
    equation->dimSimDefined = 0;

    equation->Meth = TLM;

    equation->Meth2 = NONE;

    equation->Solv = DYNAMIC;
    equation->solveDefined = 0; // 2 if dynamic simulation

    equation->timeStep = 1;
    equation->timeStepDefined = 0; // only required if dynamic simulation

    equation->timeJump = 1;

    equation->finalTime = 1;
    equation->finalTimeDefined = 0; // only required if dynamic simulation

    // flags to what to save
    equation->saveScalar = 0;
    equation->saveScalarBetween = 0;
    equation->saveFlux = 0;

    // internal flags
    equation->solveDirectly = 1;
    equation->needToSolve = 1;

    return 0;
}

/*
 * terminateEquationConfig: terminate the equation variable used for configurations
 */
unsigned int terminateEquationConfig(struct Equation *equation) {
    free(equation->equationName);
    equation->equationName = NULL;

    free(equation->materialNumbers);
    equation->materialNumbers = NULL;

    free(equation->boundaryNumbers);
    equation->boundaryNumbers = NULL;

    free(equation->nameScalar);
    equation->nameScalar = NULL;

    free(equation->nameVector);
    equation->nameVector = NULL;

    return 0;
}

/*
 * setConfigurationEquation: reads the line and configures the required equation parameter
 */
unsigned int setConfigurationEquation(char * input, struct Equation * configInput, int * startEndBrackets) {
    unsigned int errorTLMnumber, tempVarInt;

    // DEBUG: Shows that we got inside and the input
    // printf("inside condigurationSimu %s\n", input);

    // The Brackets for SIMULATION were not started yet
    if (*startEndBrackets == 0) {
        if ((errorTLMnumber = testStartBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 1; //define as the bracket started

    } else if (compareCaseInsensitive(input, "library") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);

        if (compareCaseInsensitive(input, "eigen") == 0) {
            configInput->libraryForCalculation = EIGEN;
        } else if (compareCaseInsensitive(input, "cuda") == 0) {
            configInput->libraryForCalculation = CUDA;
        } else {
            return 3883;
        }

    } else if (compareCaseInsensitive(input, "type") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information. This is only valid for inputs that are not separated
        // by blank spaces
        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "diffusion") == 0) {
            configInput->typeS = DIFFUSION;
        } else if (compareCaseInsensitive(input, "hyperbolic diffusion") == 0) {
            configInput->typeS = HYPERBOLIC_DIFFUSION;
        } else if (compareCaseInsensitive(input, "heat") == 0) {
            configInput->typeS = HEAT;
        } else if (compareCaseInsensitive(input, "hyperbolic heat") == 0) {
            configInput->typeS = HYPERBOLIC_HEAT;
        } else if (compareCaseInsensitive(input, "pennes") == 0) {
            configInput->typeS = PENNES;
        } else if (compareCaseInsensitive(input, "hyperbolic pennes") == 0) {
            configInput->typeS = HYPERBOLIC_PENNES;
        } else if (compareCaseInsensitive(input, "em") == 0) {
            configInput->typeS = EM;
        } else if (compareCaseInsensitive(input, "CFD") == 0) {
            configInput->typeS = CFD;
        } else {
            return 3874;
        }

        // flag that the simulation type was defined
        configInput->typeSimDefined = 1;

    } else if (compareCaseInsensitive(input, "equation name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // the input is the name
        configInput->equationName = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(configInput->equationName, input);

        configInput->equationNameDefined = 1;



    } else if (compareCaseInsensitive(input, "scalar name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // the input is the name
        configInput->nameScalar = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(configInput->nameScalar, input);



    } else if (compareCaseInsensitive(input, "vector name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // the input is the name
        configInput->nameVector = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(configInput->nameVector, input);



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
        } else {
            return 3876;
        }



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

    } else if (compareCaseInsensitive(input, "save") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);

        // DEBUG: the input after removing blank spaces
        // printf("%s\n", input);
        if (compareCaseInsensitive(input, "scalar") == 0 ||
                compareCaseInsensitive(input, "temperature") == 0) {
            configInput->saveScalar = 1;
        } else if (compareCaseInsensitive(input, "scalar between") == 0 ||
                compareCaseInsensitive(input, "temperature between") == 0) {
            configInput->saveScalarBetween = 1;
        } else if (compareCaseInsensitive(input, "vector") == 0 ||
                compareCaseInsensitive(input, "heat flux") == 0) {
            configInput->saveFlux = 1;
        } else {
            return 3881;
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
 * testInputEquation: tests if all the required inputs were read
 */
unsigned int testInputEquation(struct Equation *equation, int id) {
    int errorFound = 0;
    char idChar[5];
    // converting the integer equation number to the character.
    // This will be used, if necessary, to create automatic names
    // for the scalar and vector variables.
    sprintf(idChar, "%d", id);

    // type of simulation was not defined
    if (equation->typeSimDefined == 0) {
        sendErrorCodeAndMessage(1858, &id, NULL, NULL, NULL);
        errorFound = 1;
    }

    // The name of the equation
    if (equation->equationNameDefined == 0) {
        sendErrorCodeAndMessage(1864, &id, NULL, NULL, NULL);
        errorFound = 1;
    }

    // testing if the scalar and vector have names or if they need to be defined
    // scalar variable
    if (equation->nameScalar == NULL) {
        // "temperature " is the longest length name for the nameVector
        equation->nameScalar = (char *) realloc(equation->nameScalar, sizeof (char)*(strlen("temperature ") + 1 + 5));

        switch (equation->typeS) {
            case HYPERBOLIC_DIFFUSION:
                //break;
                /* FALLTHRU */
            case DIFFUSION:
                strcpy(equation->nameScalar, "scalar ");
                break;
            case HYPERBOLIC_HEAT:
                //break;
                /* FALLTHRU */
            case HYPERBOLIC_PENNES:
                //break;
                /* FALLTHRU */
            case HEAT:
                //break;
                /* FALLTHRU */
            case PENNES:
                strcpy(equation->nameScalar, "temperature ");
                break;
            case EM:
                //future implementation
                break;
            case CFD:
                //future implementation
                break;
            default: // not expected to happen
                break;
        }

        strcat(equation->nameScalar, idChar);
    }

    // vectorial variable
    if (equation->nameVector == NULL) {
        // "heat flux " is the longest length name for the nameVector
        equation->nameVector = (char *) realloc(equation->nameVector, sizeof (char)*(strlen("heat flux ") + 1 + 5));

        switch (equation->typeS) {
            case HYPERBOLIC_DIFFUSION:
                //break;
                /* FALLTHRU */
            case DIFFUSION:
                strcpy(equation->nameVector, "flux ");
                break;
            case HYPERBOLIC_HEAT:
                //break;
                /* FALLTHRU */
            case HYPERBOLIC_PENNES:
                //break;
                /* FALLTHRU */
            case HEAT:
                //break;
                /* FALLTHRU */
            case PENNES:
                strcpy(equation->nameVector, "heat flux ");
                break;
            case EM:
                //future implementation
                break;
            case CFD:
                //future implementation
                break;
            default: // not expected to happen
                break;
        }

        strcat(equation->nameVector, idChar);
    }

    // dimension of simulation was not
    if (equation->dimSimDefined == 0) {
        sendErrorCodeAndMessage(1859, &id, NULL, NULL, NULL);
        errorFound = 1;
    }

    // the solver (steady or dynamic) was not defined
    if (equation->solveDefined == 0) {
        sendErrorCodeAndMessage(1861, &id, NULL, NULL, NULL);
        errorFound = 1;
    }

    // if the time-step was not defined and we are solving for steady-state,
    // we simply defined the time-step to 1. We do this because in the TLM formalism
    // we need the time-step to calculate the impedance of the transmission line.
    if (equation->Solv == STEADY && equation->timeStep == 0) {
        equation->timeStep == 1;
    }

    if (equation->solveDefined == 2) {
        // the solver was defined as dynamic but time-step was not defined
        if (equation->timeStepDefined == 0) {
            sendErrorCodeAndMessage(1862, &id, NULL, NULL, NULL);
            errorFound = 1;
        }
        // the solver was defined as dynamic but the final time was not defined
        if (equation->finalTimeDefined == 0) {
            sendErrorCodeAndMessage(1863, &id, NULL, NULL, NULL);
            errorFound = 1;
        }
    }
    return errorFound;
}

/*
 * printfAllEquaData: print the status of all the equation variables
 */
void printfAllEquaData(struct Equation* equation) {

    printfLibrary(&equation->libraryForCalculation);

    printfMaterialNumbers(equation->materialNumbers, &equation->numberOfMaterials);
    printfBoundaryNumbers(equation->boundaryNumbers, &equation->numberOfBoundaries);

    printfTypeS(&equation->typeS);

    printf("Equation name: %s\n", equation->equationName);
    printf("Scalar name: %s\n", equation->nameScalar);
    printf("Vector name: %s\n", equation->nameVector);


    printfDimen(&equation->dimen);
    printfMeth(&equation->Meth);
    // printfMeth2(&equation->Meth2); // future implementation
    printfSolv(&equation->Solv);
    if (equation->Solv == DYNAMIC) {
        printf("Time-step = %9.4e s.\n", equation->timeStep);
        printf("Time-jump = %4u.\n", equation->timeJump);
        printf("Final time = %9.4e s.\n", equation->finalTime);
    }

    printWhatToSave(equation);

    printHowToSolve(equation->solveDirectly);

}

/*
 * printfLibrary: prints the library being used for the calculation
 */
void printfLibrary(enum libraryForCalc* lib) {
    switch (*lib) {

        case EIGEN:
            printf("Using Eigen library.\n");
            break;

        case CUDA:
            printf("Using CUDA library.\n");
            break;

        default:
            printf("Unknown error\n");
    }
}

/*
 * printfMaterialNumbers: prints the total number of materials associated with
 * an equation AND the number of those materials
 */
void printfMaterialNumbers(int *numbers, int *totalNumber) {
    int i;

    printf("Number of the materials of this equation: ");
    for (i = 0; i < *totalNumber - 1; i++) {
        printf("%d, ", numbers[i] + 1);
    }
    printf("%d.\n", numbers[i] + 1);
    printf("With a total number of %d materials.\n", *totalNumber);
}

/*
 * printfBoundaryNumbers: prints the total number of boundaries associated with
 * an equation AND the number of those boundaries
 */
void printfBoundaryNumbers(int *numbers, int *totalNumber) {
    int i;

    printf("Number of the boundaries configured of this equation: ");
    for (i = 0; i < *totalNumber - 1; i++) {
        printf("%d, ", numbers[i] + 1);
    }
    printf("%d.\n", numbers[i] + 1);
    printf("With a total number of %d boundaries configured.\n", *totalNumber);
}

/*
 * printfTypeS: prints the type of simulation
 */
void printfTypeS(enum typeSim *type) {

    printf("Solving the ");
    switch (*type) {
        case DIFFUSION: printf("diffusion equation");
            break;
        case HYPERBOLIC_DIFFUSION: printf("hyperbolic diffusion equation");
            break;
        case HEAT: printf("heat transfer equation");
            break;
        case HYPERBOLIC_HEAT: printf("hyperbolic heat transfer equation");
            break;
        case PENNES: printf("Pennes' equation");
            break;
        case HYPERBOLIC_PENNES: printf("hyperbolic Pennes' equation");
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
void printWhatToSave(struct Equation* equation) {
    int savingSomething = 0;

    switch (equation->Meth) {
        case TLM:
            switch (equation->typeS) {
                case HYPERBOLIC_DIFFUSION:
                    // break;
                    /* FALLTHRU */
                case DIFFUSION:
                    if (equation->saveScalar == 1) {
                        savingSomething = 1;
                        printf("Saving the scalar in the center of the node.\n");
                    }

                    if (equation->saveScalarBetween == 1) {
                        savingSomething = 1;
                        printf("Saving the scalar between the nodes.\n");
                    }

                    if (equation->saveFlux == 1) {
                        savingSomething = 1;
                        printf("Saving the flux between nodes.\n");
                    }

                    if (savingSomething == 0)
                        fprintf(stderr, "\nWARNING: Nothing was selected to be saved.\nTherefore, only the "
                            "TLM nodes' voltages will be calculated and saved.\nScalar values "
                            "and flux values can be obtained from TLM nodes' voltage as post-processing.\n\n");

                    break;
                case HYPERBOLIC_HEAT:
                    // break;
                    /* FALLTHRU */
                case HYPERBOLIC_PENNES:
                    // break;
                    /* FALLTHRU */
                case HEAT:
                    // break;
                    /* FALLTHRU */
                case PENNES:

                    if (equation->saveScalar == 1) {
                        savingSomething = 1;
                        printf("Saving the temperature variable in the center of the node.\n");
                    }

                    if (equation->saveScalarBetween == 1) {
                        savingSomething = 1;
                        printf("Saving the temperatures between the elements.\n");
                    }

                    if (equation->saveFlux == 1) {
                        savingSomething = 1;
                        printf("Saving the heat flux.\n");
                    }

                    if (savingSomething == 0)
                        fprintf(stderr, "\nWARNING: Nothing was selected to be saved.\nTherefore, only the "
                            "TLM nodes' voltages will be calculated and saved.\nTemperature "
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
    }
}

/*
 * printHowToSolve: prints how this equation is going to be solved
 */
void printHowToSolve(int solveDirectly) {
    switch (solveDirectly) {
        case 1:
            printf("This equation will be solved directly\n");
            break;
        case 2:
            printf("This equation will be solved iteratively with other equations\n");
            break;
    }

}