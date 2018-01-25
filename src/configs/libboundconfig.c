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

        sscanf(input, "%lf", &boundInput->scalarBoundary);
        boundInput->scalarBoundaryDefined = 1;



    } else if (compareCaseInsensitive(input, "flux") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->fluxBoundary);
        boundInput->fluxBoundaryDefined = 1;



    } else if (compareCaseInsensitive(input, "Convection scalar") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->convectionScalar);
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

        sscanf(input, "%lf", &boundInput->temperatureBoundary);
        boundInput->temperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "heat flux") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->heatFluxBoundary);
        boundInput->heatFluxDefined = 1;



    } else if (compareCaseInsensitive(input, "Convection Temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->convectionTemperature);
        boundInput->convectionDefined = 1;
        boundInput->convectionTemperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "Convection coefficient") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->convectionCoefficient);
        boundInput->convectionDefined = 1;
        boundInput->convectionCoefficientDefined = 1;



    } else if (compareCaseInsensitive(input, "Radiation Temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->radiationTemperature);
        boundInput->radiationDefined = 1;
        boundInput->radiationTemperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "Radiation Emissivity") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->radiationEmissivity);
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
        struct Equation * equation, int id, struct Simulation *inputSimu) {
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
            
            // generalized inputs.
            input->generalized_scalar = input->scalarBoundary;
            input->generalized_scalarDefined = input->scalarBoundaryDefined;
            
            input->generalized_flux = input->fluxBoundary;
            input->generalized_fluxDefined = input->fluxBoundaryDefined;
            
            input->generalized_convectionScalar = input->convectionScalar;
            input->generalized_convectionCoefficient = input->convectionCoefficient;
            input->generalized_convectionDefined = input->convectionDefined;
            
            // these options are not defined for diffusion
//            input->generalized_radiationScalar = input->radiationTemperature;
//            input->generalized_radiationCoefficient = input->radiationEmissivity;
//            input->generalized_radiationDefined = input->radiationDefined;

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
            
            // generalized inputs.
            input->generalized_scalar = input->temperatureBoundary;
            input->generalized_scalarDefined = input->temperatureDefined;
            
            input->generalized_flux = input->heatFluxBoundary;
            input->generalized_fluxDefined = input->heatFluxDefined;
            
            input->generalized_convectionScalar = input->convectionTemperature;
            input->generalized_convectionCoefficient = input->convectionCoefficient;
            input->generalized_convectionDefined = input->convectionDefined;
            
            input->generalized_radiationScalar = input->radiationTemperature -
                    inputSimu->AbsoluteZero;
            input->generalized_radiationCoefficient = input->radiationEmissivity;
            input->generalized_radiationDefined = input->radiationDefined;
            
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
                    printfScalarBoundary(input->scalarBoundary);
                if (input->fluxBoundaryDefined == 1)
                    printfFluxBoundary(input->fluxBoundary);
                if (input->convectionDefined == 1)
                    printfConvection(input->convectionCoefficient, input->convectionScalar);
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
                    printfTemperature(input->temperatureBoundary);
                if (input->heatFluxDefined == 1)
                    printfHeatFlux(input->heatFluxBoundary);
                if (input->convectionDefined == 1)
                    printfConvectionHeat(input->convectionCoefficient, input->convectionTemperature);
                if (input->radiationDefined)
                    printfRadiation(input->radiationEmissivity, input->radiationTemperature);
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
void printfScalarBoundary(double input) {
    printf("Scalar value at the boundary with value of %9.4e .\n", input);
}

/*
 * printfFluxBoundary: prints for flux boundary condition
 */
void printfFluxBoundary(double input) {
    printf("Flux boundary with value of %9.4e \n", input);
}

/*
 * printfConvection: prints for convection boundary condition
 */
void printfConvection(double coeff, double scalar) {
    printf("Convection boundary condition with convection "
            "coefficient of %9.4e  and convective scalar value of %9.4e .\n", coeff, scalar);
}

/*
 * printfTemperature: prints for temperature boundary condition
 */
void printfTemperature(double input) {
    printf("Temperature at the boundary with value of %9.4e oC.\n", input);
}

/*
 * printfHeatFlux: prints for heat flux boundary condition
 */
void printfHeatFlux(double input) {
    printf("Heat flux at the boundary with value of %9.4e W/m2.\n", input);
}

/*
 * printfConvectionHeat: prints for convection heat transfer boundary condition
 */
void printfConvectionHeat(double coeff, double temp) {
    printf("Convection heat transfer (as Newton's cooling law) boundary condition with heat transfer "
            "coefficient of %9.4e W/(K-m2) and convective temperature of %9.4e oC.\n", coeff, temp);
}

/*
 * printfRadiation: prints for radiation boundary condition
 */
void printfRadiation(double emiss, double temp) {
    printf("Radiation heat transfer with emissivity coefficient of %9.4e and radiative temperature of %9.4e oC.\n", emiss, temp);
}