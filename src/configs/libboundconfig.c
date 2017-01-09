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
    // in the initialization, I will assume that this has only one boundary
    bound->numberInput = (int*) malloc(sizeof (int));
    bound->quantityOfNumberInput = 0; // initializing as zero

    // initiating the flags for pennes and heat
    bound->adiabaticDefined = 0;
    bound->temperatureDefined = 0;
    bound->heatFluxDefined = 0;
    bound->convectionDefined = 0;
    bound->radiationDefined = 0;

    return 0;

}

/*
 * terminateBoundaryConfig: terminate the boundary variable used for configuration
 */
unsigned int terminateBoundaryConfig(struct BoundaryConfig * bound) {
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

    } else if (compareCaseInsensitive(input, "number") == 0) {
        if ((errorTLMnumber = readVectorInputs(input,
                &boundInput->quantityOfNumberInput, &boundInput->numberInput)) != 0)
            return errorTLMnumber;

        boundInput->numberInputDefined = 1;

        // DEBUG: Checking the numbers we got
        // printf("The number (%d) we got were",matInput->quantityOfNumberInput);
        // for (int i = 0; i < boundInput->quantityOfNumberInput; i++){
        //      printf(" %d", boundInput->numberInput[i]);
        // }

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

        sscanf(input, "%lf", &boundInput->ConvectionTemperature);
        boundInput->convectionDefined = 1;
        boundInput->convectionTemperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "Convection coefficient") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->ConvectionCoefficient);
        boundInput->convectionDefined = 1;
        boundInput->convectionCoefficientDefined = 1;



    } else if (compareCaseInsensitive(input, "Radiation Temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->RadiationTemperature);
        boundInput->radiationDefined = 1;
        boundInput->radiationTemperatureDefined = 1;



    } else if (compareCaseInsensitive(input, "Radiation Emissivity") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &boundInput->RadiationEmissivity);
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
        enum typeSim * typeS, int id) {
    int errorFound = 0;

    switch (*typeS) {
        case PENNES:
            /* FALLTHRU */
        case HEAT:
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
                        "They will be ignored.\n\n", id);

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
void printfBoundConfig(struct BoundaryConfig *input, enum typeSim *typeS) {
    printfNumberOfInputsBound(&input->numberInput, &input->quantityOfNumberInput);
    switch (*typeS) {
        case PENNES:
            /* FALLTHRU */
        case HEAT:

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
                    printfConvection(input->ConvectionCoefficient, input->ConvectionTemperature);
                if (input->radiationDefined)
                    printfRadiation(input->RadiationEmissivity, input->RadiationTemperature);

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
 * printfTemperature: prints for temperature boundary condition
 */
void printfTemperature(double input) {
    printf("Temperature constant at the boundary with value of %9.4e oC.\n", input);
}

/*
 * printfHeatFlux: prints for heat flux boundary condition
 */
void printfHeatFlux(double input) {
    printf("Heat flux constant at the boundary with value of %9.4e W/m2.\n", input);
}

/*
 * printfConvection: prints for convection boundary condition
 */
void printfConvection(double coeff, double temp) {
    printf("Convection heat transfer (as Newton's cooling law) with heat transfer "
            "coefficient of %9.4e W/(K-m2) and convective temperature of %9.4e oC.\n", coeff, temp);
}

/*
 * printfRadiation: prints for radiation boundary condition
 */
void printfRadiation(double emiss, double temp) {
    printf("Radiation heat transfer with emissivity coefficient of %9.4e and radiative temperature of %9.4e oC.\n", emiss, temp);
}