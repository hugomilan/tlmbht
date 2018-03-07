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
 * File:   libboundconfig.c
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
 * Description of this function:
 * Contains the functions for reading from the input file and 
 * configuring the boundary inputs.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "libboundconfig.h"

#include "libsimuconfig.h"
#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * initializeBoundaryConfig: initialize the boundary variable used for configuration
 * with standard values
 */
unsigned int initializeBoundaryConfig(struct BoundaryConfig * bound) {
    bound->equationName = NULL;
    bound->equationNameDefined = 0;

    bound->equationNumber = -1;

    bound->boundaryName = NULL;
    bound->boundaryNameDefined = 0;

    bound->typeOfEquation = PENNES;

    bound->numberInput = NULL;
    bound->quantityOfNumberInput = 0; // initializing as zero

    // Inputs associated with diffusion and hyperbolic diffusion
    bound->scalarBoundary = 0;
    bound->scalarBoundaryDefined = 0;
    bound->scalarBoundaryFromFunc = NULL;
    bound->scalarBoundaryType = 1;

    bound->fluxBoundary = 0;
    bound->fluxBoundaryDefined = 0;
    bound->fluxBoundaryFromFunc = NULL;
    bound->fluxBoundaryType = 1;

    bound->convectionCoefficient = 0;
    bound->convectionCoefficientFromFunc = NULL;
    bound->convectionCoefficientType = 1;
    bound->convectionCoefficientDefined = 0;
    bound->convectionDefined = 0;

    bound->adiabaticDefined = 0;

    // Inputs associated with heat, hyperbolic heat, pennes, and hyperbolic pennes
    bound->temperatureBoundary = 0;
    bound->temperatureDefined = 0;
    bound->temperatureBoundaryFromFunc = NULL;
    bound->temperatureBoundaryType = 1;

    bound->heatFluxBoundary = 0;
    bound->heatFluxDefined = 0;
    bound->heatFluxBoundaryFromFunc = NULL;
    bound->heatFluxBoundaryType = 1;

    bound->radiationTemperature = 0;
    bound->radiationTemperatureDefined = 0;
    bound->radiationTemperatureFromFunc = NULL;
    bound->radiationTemperatureType = 1;

    bound->radiationEmissivity = 0;
    bound->radiationEmissivityDefined = 0;
    bound->radiationDefined = 0;
    bound->radiationEmissivityFromFunc = NULL;
    bound->radiationEmissivityType = 1;

    // these are the same defiend for diffusion and hyperbolic diffusion
    //    bound->convectionCoefficient = 0;
    //    bound->convectionCoefficientDefined = 0;
    //    bound->convectionDefined = 0;
    //    
    //    bound->adiabaticDefined = 0;

    return 0;

}

/*
 * terminateBoundaryConfig: terminate the boundary variable used for configuration
 */
unsigned int terminateBoundaryConfig(struct BoundaryConfig * bound) {
    free(bound->equationName);
    bound->equationName = NULL;

    free(bound->boundaryName);
    bound->boundaryName = NULL;

    free(bound->numberInput);
    bound->numberInput = NULL;

    free(bound->scalarBoundaryFromFunc);
    bound->scalarBoundaryFromFunc = NULL;

    free(bound->fluxBoundaryFromFunc);
    bound->fluxBoundaryFromFunc = NULL;

    free(bound->convectionCoefficientFromFunc);
    bound->convectionCoefficientFromFunc = NULL;

    free(bound->temperatureBoundaryFromFunc);
    bound->temperatureBoundaryFromFunc = NULL;

    free(bound->heatFluxBoundaryFromFunc);
    bound->heatFluxBoundaryFromFunc = NULL;

    free(bound->radiationTemperatureFromFunc);
    bound->radiationTemperatureFromFunc = NULL;

    free(bound->radiationEmissivityFromFunc);
    bound->radiationEmissivityFromFunc = NULL;

    return 0;
}

/*
 * setConfigurationBoundary: reads the line and configures the required boundary parameters
 */
unsigned int setConfigurationBoundary(char *input, struct BoundaryConfig *boundInput, int* startEndBrackets) {
    unsigned int errorTLMnumber;

    if (*startEndBrackets == 0) {

        if ((errorTLMnumber = testStartBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 1; //define as the bracket started

    } else if (compareCaseInsensitive(input, "equation") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // the input is the name
        boundInput->equationName = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(boundInput->equationName, input);

        boundInput->equationNameDefined = 1;

    } else if (compareCaseInsensitive(input, "name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // the input is the name
        boundInput->boundaryName = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(boundInput->boundaryName, input);

        boundInput->boundaryNameDefined = 1;

    } else if (compareCaseInsensitive(input, "number") == 0) {
        if ((errorTLMnumber = readVectorIntInputs(input,
                &boundInput->quantityOfNumberInput, &boundInput->numberInput)) != 0)
            return errorTLMnumber;

        boundInput->numberInputDefined = 1;

        // DEBUG: Checking the numbers we got
        // printf("The number (%d) we got were",matInput->quantityOfNumberInput);
        // for (int i = 0; i < boundInput->quantityOfNumberInput; i++){
        //      printf(" %d", boundInput->numberInput[i]);
        // }

    } else if (compareCaseInsensitive(input, "scalar") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->scalarBoundary) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->scalarBoundaryFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->scalarBoundaryFromFunc, input);
            printf("%s\n", boundInput->scalarBoundaryFromFunc);
            boundInput->scalarBoundaryType = 2;
        } else {
            boundInput->scalarBoundaryType = 1;
        }
        boundInput->scalarBoundaryDefined = 1;



    } else if (compareCaseInsensitive(input, "flux") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->fluxBoundary) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->fluxBoundaryFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->fluxBoundaryFromFunc, input);
            printf("%s\n", boundInput->fluxBoundaryFromFunc);
            boundInput->fluxBoundaryType = 2;
        } else {
            boundInput->fluxBoundaryType = 1;
        }
        boundInput->fluxBoundaryDefined = 1;



    } else if (compareCaseInsensitive(input, "Convection scalar") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->convectionScalar) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->convectionScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->convectionScalarFromFunc, input);
            printf("%s\n", boundInput->convectionScalarFromFunc);
            boundInput->convectionScalarType = 2;
        } else {
            boundInput->convectionScalarType = 1;
        }
        boundInput->convectionDefined = 1;
        boundInput->convectionScalarDefined = 1;



    } else if (compareCaseInsensitive(input, "adiabatic") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%s", input);

        if (compareCaseInsensitive(input, "true") == 0) {
            boundInput->adiabaticDefined = 1;
        } else if (compareCaseInsensitive(input, "false") == 0) {
            boundInput->adiabaticDefined = 0;
        } else {
            return 5165;
        }



    } else if (compareCaseInsensitive(input, "temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->temperatureBoundary) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->temperatureBoundaryFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->temperatureBoundaryFromFunc, input);
            printf("%s\n", boundInput->temperatureBoundaryFromFunc);
            boundInput->temperatureBoundaryType = 2;
        } else {
            boundInput->temperatureBoundaryType = 1;
        }
        boundInput->temperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "heat flux") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->heatFluxBoundary) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->heatFluxBoundaryFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->heatFluxBoundaryFromFunc, input);
            printf("%s\n", boundInput->heatFluxBoundaryFromFunc);
            boundInput->heatFluxBoundaryType = 2;
        } else {
            boundInput->heatFluxBoundaryType = 1;
        }
        boundInput->heatFluxDefined = 1;



    } else if (compareCaseInsensitive(input, "Convection Temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->convectionTemperature) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->convectionTemperatureFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->convectionTemperatureFromFunc, input);
            printf("%s\n", boundInput->convectionTemperatureFromFunc);
            boundInput->convectionTemperatureType = 2;
        } else {
            boundInput->convectionTemperatureType = 1;
        }
        boundInput->convectionDefined = 1;
        boundInput->convectionTemperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "Convection coefficient") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->convectionCoefficient) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->convectionCoefficientFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->convectionCoefficientFromFunc, input);
            printf("%s\n", boundInput->convectionCoefficientFromFunc);
            boundInput->convectionCoefficientType = 2;
        } else {
            boundInput->convectionCoefficientType = 1;
        }
        boundInput->convectionDefined = 1;
        boundInput->convectionCoefficientDefined = 1;



    } else if (compareCaseInsensitive(input, "Radiation Temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->radiationTemperature) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->radiationTemperatureFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->radiationTemperatureFromFunc, input);
            printf("%s\n", boundInput->radiationTemperatureFromFunc);
            boundInput->radiationTemperatureType = 2;
        } else {
            boundInput->radiationTemperatureType = 1;
        }
        boundInput->radiationDefined = 1;
        boundInput->radiationTemperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "Radiation Emissivity") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &boundInput->radiationEmissivity) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            boundInput->radiationEmissivityFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(boundInput->radiationEmissivityFromFunc, input);
            printf("%s\n", boundInput->radiationEmissivityFromFunc);
            boundInput->radiationEmissivityType = 2;
        } else {
            boundInput->radiationEmissivityType = 1;
        }
        boundInput->radiationDefined = 1;
        boundInput->radiationEmissivityDefined = 1;



    } else if (input[0] == '}' && *startEndBrackets == 1) {
        if ((errorTLMnumber = testEndBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 0; //define as the bracket as finalized
        return 9999; // that is my code to indicate that it was done.
    } else {
        return 1147;
    }
    return 0;
}

/*
 * testInputBoundary: tests if all the required inputs were read
 */
unsigned int testInputBoundary(struct BoundaryConfig * input,
        struct Equation * equation, int id, struct Simulation *inputSimu,
        struct FunctionConfig * Functions, int numberOfFunctions) {
    int errorFound = 0;

    switch (equation->typeS) {
        case HYPERBOLIC_DIFFUSION:
            //break;
            /* FALLTHRU */
        case DIFFUSION:
            if (input->scalarBoundaryDefined == 0 && input->fluxBoundaryDefined == 0 &&
                    input->convectionDefined == 0 && input->adiabaticDefined == 0) {
                printf("\nWARNING: Nothing was defined for boundary group %04d and, "
                        "therefore, it was assumed adiabatic boundary condition.\n\n", id);
                // defining the boundary as  adiabatic
                input->adiabaticDefined = 1;

            } else if (input->adiabaticDefined == 1 && (input->scalarBoundaryDefined == 1
                    || input->fluxBoundaryDefined == 1 || input->convectionDefined == 1)) {
                printf("\nWARNING: Adiabatic boundary condition was explicitely defined "
                        "for boundary group %04d but others parameters were also defined. "
                        "They will be ignored and this boundary will be considered as adiabatic.\n\n", id);

                input->scalarBoundaryDefined = 0;
                input->fluxBoundaryDefined = 0;
                input->convectionDefined = 0;

            } else {

                if (input->convectionCoefficientDefined == 1 && input->convectionScalarDefined == 0) {
                    sendErrorCodeAndMessage(8528, &id, NULL, NULL, NULL);
                    errorFound = 1;

                } else if (input->convectionCoefficientDefined == 0 && input->convectionScalarDefined == 1) {
                    sendErrorCodeAndMessage(8529, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }

                if (input->scalarBoundaryDefined == 1 && input->convectionDefined == 1) {
                    sendErrorCodeAndMessage(8530, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }

                if (input->scalarBoundaryDefined == 1 && input->heatFluxBoundary == 1) {
                    sendErrorCodeAndMessage(8031, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }
            }

            if (generalizeDiffusionBoundary(input,
                    Functions, numberOfFunctions, id) != 0) {
                errorFound = 1;
            }

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
            if (input->temperatureDefined == 0 && input->heatFluxDefined == 0 &&
                    input->convectionDefined == 0 && input->radiationDefined == 0 &&
                    input->adiabaticDefined == 0) {
                printf("\nWARNING: Nothing was defined for boundary group %04d and, "
                        "therefore, it was assumed adiabatic boundary condition.\n\n", id);
                // defining the boundary as  adiabatic
                input->adiabaticDefined = 1;

            } else if (input->adiabaticDefined == 1 && (input->temperatureDefined == 1
                    || input->heatFluxDefined == 1 || input->convectionDefined == 1 ||
                    input->radiationDefined == 1)) {
                printf("\nWARNING: Adiabatic boundary condition was explicitely defined "
                        "for boundary group %04d but others parameters were also defined. "
                        "They will be ignored and this boundary will be considered as adiabatic.\n\n", id);

                input->temperatureDefined = 0;
                input->heatFluxDefined = 0;
                input->convectionDefined = 0;
                input->radiationDefined = 0;
            } else {

                if (input->convectionCoefficientDefined == 1 && input->convectionTemperatureDefined == 0) {
                    sendErrorCodeAndMessage(8521, &id, NULL, NULL, NULL);
                    errorFound = 1;

                } else if (input->convectionCoefficientDefined == 0 && input->convectionTemperatureDefined == 1) {
                    sendErrorCodeAndMessage(8522, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }

                if (input->radiationEmissivityDefined == 1 && input->radiationTemperatureDefined == 0) {
                    sendErrorCodeAndMessage(8523, &id, NULL, NULL, NULL);
                    errorFound = 1;
                } else if (input->radiationEmissivityDefined == 0 && input->radiationTemperatureDefined == 1) {
                    sendErrorCodeAndMessage(8524, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }

                if (input->temperatureDefined == 1 && input->convectionDefined == 1) {
                    sendErrorCodeAndMessage(8525, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }

                if (input->temperatureDefined == 1 && input->radiationDefined == 1) {
                    sendErrorCodeAndMessage(8526, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }

                if (input->temperatureDefined == 1 && input->heatFluxBoundary == 1) {
                    sendErrorCodeAndMessage(8527, &id, NULL, NULL, NULL);
                    errorFound = 1;
                }
            }

            if (generalizeHeatBoundary(input,
                    Functions, numberOfFunctions, id) != 0) {
                errorFound = 1;
            }

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

    return errorFound;
}

/*
 * generalizeDiffusionBoundary: copy the values of the diffusion boundary input to the
 * generalized variables. I do this so that I can have only one solver for 
 * diffusion, heat, and pennes.
 */
unsigned int generalizeDiffusionBoundary(struct BoundaryConfig *input,
        struct FunctionConfig * Functions, int numberOfFunctions, int id) {
    int errorFound = 0;

    // generalized inputs.
    input->generalized_scalar = input->scalarBoundary;
    input->generalized_scalarDefined = input->scalarBoundaryDefined;
    input->generalized_scalarType = input->scalarBoundaryType;
    if (input->generalized_scalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_scalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->scalarBoundaryFromFunc) + 1));
        strcpy(input->generalized_scalarFromFunc, input->scalarBoundaryFromFunc);
        if (findFunctionName(input->generalized_scalarFromFunc, &input->generalized_scalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4701, input->generalized_scalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_flux = input->fluxBoundary;
    input->generalized_fluxDefined = input->fluxBoundaryDefined;
    input->generalized_fluxType = input->fluxBoundaryType;
    if (input->generalized_fluxType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_fluxFromFunc = (char*) malloc(sizeof (char)*(strlen(input->fluxBoundaryFromFunc) + 1));
        strcpy(input->generalized_fluxFromFunc, input->fluxBoundaryFromFunc);
        if (findFunctionName(input->generalized_fluxFromFunc, &input->generalized_fluxGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4702, input->generalized_fluxFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_convectionScalar = input->convectionScalar;
    input->generalized_convectionScalarType = input->convectionScalarType;
    if (input->generalized_convectionScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_convectionScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->convectionScalarFromFunc) + 1));
        strcpy(input->generalized_convectionScalarFromFunc, input->convectionScalarFromFunc);
        if (findFunctionName(input->generalized_convectionScalarFromFunc, &input->generalized_convectionScalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4703, input->generalized_convectionScalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_convectionCoefficient = input->convectionCoefficient;
    input->generalized_convectionCoefficientType = input->convectionCoefficientType;
    if (input->generalized_convectionCoefficientType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_convectionCoefficientFromFunc = (char*) malloc(sizeof (char)*(strlen(input->convectionCoefficientFromFunc) + 1));
        strcpy(input->generalized_convectionCoefficientFromFunc, input->convectionCoefficientFromFunc);
        if (findFunctionName(input->generalized_convectionCoefficientFromFunc, &input->generalized_convectionCoefficientGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4704, input->generalized_convectionCoefficientFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }
    input->generalized_convectionDefined = input->convectionDefined;

    // these options are not defined for diffusion
    //            input->generalized_radiationScalar = input->radiationTemperature;
    //            input->generalized_radiationCoefficient = input->radiationEmissivity;
    //            input->generalized_radiationDefined = input->radiationDefined;

    return errorFound;
}

/*
 * generalizeHeatBoundary: copy the values of the heat and Pennes boundary input to the
 * generalized variables. I do this so that I can have only one solver for 
 * diffusion, heat, and pennes.
 */
unsigned int generalizeHeatBoundary(struct BoundaryConfig *input,
        struct FunctionConfig * Functions, int numberOfFunctions, int id) {
    int errorFound = 0;

    // generalized inputs.
    input->generalized_scalar = input->temperatureBoundary;
    input->generalized_scalarDefined = input->temperatureDefined;
    input->generalized_scalarType = input->temperatureBoundaryType;
    if (input->generalized_scalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_scalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->temperatureBoundaryFromFunc) + 1));
        strcpy(input->generalized_scalarFromFunc, input->temperatureBoundaryFromFunc);
        if (findFunctionName(input->generalized_scalarFromFunc, &input->generalized_scalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4731, input->generalized_scalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_flux = input->heatFluxBoundary;
    input->generalized_fluxDefined = input->heatFluxDefined;
    input->generalized_fluxType = input->heatFluxBoundaryType;
    if (input->generalized_fluxType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_fluxFromFunc = (char*) malloc(sizeof (char)*(strlen(input->heatFluxBoundaryFromFunc) + 1));
        strcpy(input->generalized_fluxFromFunc, input->heatFluxBoundaryFromFunc);
        if (findFunctionName(input->generalized_fluxFromFunc, &input->generalized_fluxGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4732, input->generalized_fluxFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }


    input->generalized_convectionScalar = input->convectionTemperature;
    input->generalized_convectionScalarType = input->convectionTemperatureType;
    if (input->generalized_convectionScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_convectionScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->convectionTemperatureFromFunc) + 1));
        strcpy(input->generalized_convectionScalarFromFunc, input->convectionTemperatureFromFunc);
        if (findFunctionName(input->generalized_convectionScalarFromFunc, &input->generalized_convectionScalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4733, input->generalized_convectionScalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_convectionCoefficient = input->convectionCoefficient;
    input->generalized_convectionCoefficientType = input->convectionCoefficientType;
    if (input->generalized_convectionCoefficientType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_convectionCoefficientFromFunc = (char*) malloc(sizeof (char)*(strlen(input->convectionCoefficientFromFunc) + 1));
        strcpy(input->generalized_convectionCoefficientFromFunc, input->convectionCoefficientFromFunc);
        if (findFunctionName(input->generalized_convectionCoefficientFromFunc, &input->generalized_convectionCoefficientGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4704, input->generalized_convectionCoefficientFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }
    input->generalized_convectionDefined = input->convectionDefined;


    input->generalized_radiationScalar = input->radiationTemperature;
    input->generalized_radiationScalarType = input->radiationTemperatureType;
    if (input->generalized_radiationScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_radiationScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->radiationTemperatureFromFunc) + 1));
        strcpy(input->generalized_radiationScalarFromFunc, input->radiationTemperatureFromFunc);
        if (findFunctionName(input->generalized_radiationScalarFromFunc, &input->generalized_radiationScalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4734, input->generalized_radiationScalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_radiationCoefficient = input->radiationEmissivity;
    input->generalized_radiationCoefficientType = input->radiationEmissivityType;
    if (input->generalized_radiationCoefficientType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_radiationCoefficientFromFunc = (char*) malloc(sizeof (char)*(strlen(input->radiationEmissivityFromFunc) + 1));
        strcpy(input->generalized_radiationCoefficientFromFunc, input->radiationEmissivityFromFunc);
        if (findFunctionName(input->generalized_radiationCoefficientFromFunc, &input->generalized_radiationCoefficientGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4735, input->generalized_radiationCoefficientFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }
    input->generalized_radiationDefined = input->radiationDefined;

    return errorFound;
}

/*
 * printfBoundConfig: prints the boundary configurations that were read
 */
void printfBoundConfig(struct BoundaryConfig *input, struct Equation *equation) {
    printfNumberOfInputsBound(&input->numberInput, &input->quantityOfNumberInput);
    switch (equation->typeS) {
        case HYPERBOLIC_DIFFUSION:
            // break;
            /* FALLTHRU */
        case DIFFUSION:

            if (input->adiabaticDefined == 1 ||
                    (input->scalarBoundaryDefined == 0 && input->fluxBoundaryDefined == 0 &&
                    input->convectionDefined == 0 &&
                    input->adiabaticDefined == 0)) {
                printfAdiabatic();
            } else {
                if (input->scalarBoundaryDefined == 1)
                    printfScalarBoundary(input->scalarBoundary, input->scalarBoundaryFromFunc, input->scalarBoundaryType);
                if (input->fluxBoundaryDefined == 1)
                    printfFluxBoundary(input->fluxBoundary, input->fluxBoundaryFromFunc, input->fluxBoundaryType);
                if (input->convectionDefined == 1)
                    printfConvection(input->convectionCoefficient, input->convectionScalar,
                        input->convectionCoefficientFromFunc, input->convectionScalarFromFunc,
                        input->convectionCoefficientType, input->convectionScalarType);
            }

            break;
        case HYPERBOLIC_HEAT:
            // break;
            /* FALLTHRU */
        case HEAT:
            // break;
            /* FALLTHRU */
        case HYPERBOLIC_PENNES:
            // break;
            /* FALLTHRU */

        case PENNES:

            if (input->adiabaticDefined == 1 ||
                    (input->temperatureDefined == 0 && input->heatFluxDefined == 0 &&
                    input->convectionDefined == 0 && input->radiationDefined == 0 &&
                    input->adiabaticDefined == 0)) {
                printfAdiabatic();
            } else {
                if (input->temperatureDefined == 1)
                    printfTemperature(input->temperatureBoundary, input->temperatureBoundaryFromFunc, input->temperatureBoundaryType);
                if (input->heatFluxDefined == 1)
                    printfHeatFlux(input->heatFluxBoundary, input->heatFluxBoundaryFromFunc, input->heatFluxBoundaryType);
                if (input->convectionDefined == 1)
                    printfConvectionHeat(input->convectionCoefficient, input->convectionScalar,
                        input->convectionCoefficientFromFunc, input->convectionScalarFromFunc,
                        input->convectionCoefficientType, input->convectionScalarType);
                if (input->radiationDefined)
                    printfRadiation(input->radiationEmissivity, input->radiationTemperature,
                        input->radiationEmissivityFromFunc, input->radiationTemperatureFromFunc,
                        input->radiationEmissivityType, input->radiationTemperatureType);
            }
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

}

/*
 * printfNumberOfInputsBound: prints the numbers of the boundaries that will have
 * the characteristics as defined here
 */
void printfNumberOfInputsBound(int ** input, int * number) {
    printf("Boundary numbers: %d", *input[0]);

    for (int i = 1; i < *number; i++) {
        printf(" %d", *(input[0] + i));
    }

    printf(".\n");
}

/*
 * printfAdiabatic: prints for adiabatic boundary condition
 */
void printfAdiabatic() {
    printf("Adiabatic boundary.\n");
}

/*
 * printfScalarBoundary: prints for scalar boundary condition value
 */
void printfScalarBoundary(double inputDouble, char *inputChar, int inputType) {
    if (inputType == 1) {
        printf("Scalar value at the boundary with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Scalar value at the boundary calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfFluxBoundary: prints for flux boundary condition
 */
void printfFluxBoundary(double inputDouble, char *inputChar, int inputType) {
    if (inputType == 1) {
        printf("Flux boundary with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Flux boundary with value calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

}

/*
 * printfConvection: prints for convection boundary condition
 */
void printfConvection(double coeffDouble, double scalarDouble, char* coeffChar, char* scalarChar,
        int coeffType, int scalarType) {

    if (coeffType == 1) {
        printf("Convection boundary condition with convection "
                "coefficient of %9.4e ", coeffDouble);
    } else if (coeffType == 2) {
        printf("Convection boundary condition with convection "
                "coefficient calculated from function %s ", coeffChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

    if (scalarType == 1) {
        printf("and convective scalar value of %9.4e.\n", scalarDouble);
    } else if (scalarType == 2) {
        printf("and convective scalar calculated from function %s.\n", scalarChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfTemperature: prints for temperature boundary condition
 */
void printfTemperature(double inputDouble, char *inputChar, int inputType) {
    if (inputType == 1) {
        printf("Temperature at the boundary with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Temperature at the boundary calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfHeatFlux: prints for heat flux boundary condition
 */
void printfHeatFlux(double inputDouble, char *inputChar, int inputType) {
    if (inputType == 1) {
        printf("Heat flux boundary with value of %9.4e W/m2.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Heat flux boundary with value calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfConvectionHeat: prints for convection heat transfer boundary condition
 */
void printfConvectionHeat(double coeffDouble, double scalarDouble, char* coeffChar, char* scalarChar,
        int coeffType, int scalarType) {
    if (coeffType == 1) {
        printf("Convection heat transfer (as Newton's cooling law) boundary condition with heat transfer "
                "coefficient of %9.4e W/(K-m2)", coeffDouble);
    } else if (coeffType == 2) {
        printf("Convection heat transfer (as Newton's cooling law) boundary condition with heat transfer "
                "coefficient calculated from function %s", coeffChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

    if (scalarType == 1) {
        printf("and convective temperature of %9.4e oC.\n", scalarDouble);
    } else if (scalarType == 2) {
        printf("and convective temperature calculated from function %s.\n", scalarChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfRadiation: prints for radiation boundary condition
 */
void printfRadiation(double emissDouble, double tempDouble, char *emissChar, char* tempChar,
        int emissType, int tempType) {

    if (emissType == 1) {
        printf("Radiation heat transfer with emissivity coefficient of %9.4e ", emissDouble);
    } else if (emissType == 2) {
        printf("Radiation heat transfer with emissivity coefficient calculated from function %s", emissChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

    if (tempType == 1) {
        printf("and radiative temperature of %9.4e oC.\n", tempDouble);
    } else if (tempType == 2) {
        printf("and radiative temperature calculated from function %s.\n", tempChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}