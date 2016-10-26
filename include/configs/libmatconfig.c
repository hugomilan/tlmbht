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
 * File:   libmatconfig.c
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
 * Contains the functions for reading from the input file and 
 * configuring the material inputs.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "libmatconfig.h"
#include "../miscellaneous/libstringtlmbht.h"
#include "libsimuconfig.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * intializeMaterialConfig: initialize the material variable used for configuration
 * with standard values
 */
unsigned int intializeMaterialConfig(struct MaterialConfig * mat) {
    // in the initialization, I will assume that this has only one material
    mat->numberInput = (int*) malloc(sizeof (int));
    mat->quantityOfNumberInput = 0; // initializing as zero

    // data for the material itself. Used in Pennes and Heat
    mat->matDensity = 0;
    mat->matSpecificHeat = 0;
    mat->matThermalConductivity = 0;
    mat->initialTemperature = 0;

    // data for the blood inside the material. Used in Pennes
    mat->bloodPerfusion = 0;
    mat->bloodDensity = 0;
    mat->bloodSpecificHeat = 0;
    mat->bloodTemperature = 0;

    // data for the internal heat generation. Used in Pennes and Heat
    mat->internalHeatGeneration = 0;

    // flags
    mat->numberInputDefined = 0;
    mat->matDensityDefined = 0;
    mat->matSpecificHeatDefined = 0;
    mat->matThermalConductivityDefined = 0;
    mat->initialTemperatureDefined = 0;

    return 0;
}

/*
 * terminateMaterialConfig: terminate the material variable used for configuration
 */
unsigned int terminateMaterialConfig(struct MaterialConfig * mat) {
    free(mat->numberInput);
    mat->numberInput = NULL;

    return 0;
}

/*
 * setConfigurationMaterial: reads the line and configures the required material parameters
 */
unsigned int setConfigurationMaterial(char * input, struct MaterialConfig * matInput, int * startEndBrackets) {
    unsigned int errorTLMnumber;


    if (*startEndBrackets == 0) {

        if ((errorTLMnumber = testStartBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 1; //define as the bracket started

    } else if (compareCaseInsensitive(input, "number") == 0) {
        if ((errorTLMnumber = readVectorInputs(input,
                &matInput->quantityOfNumberInput, &matInput->numberInput)) != 0)
            return errorTLMnumber;

        matInput->numberInputDefined = 1;

        // DEBUG: Checking the number we got
        // printf("The number (%d) we got were",matInput->quantityOfNumberInput);
        // for (int i = 0; i < matInput->quantityOfNumberInput; i++){
        //      printf(" %d", matInput->numberInput[i]);
        // }

    } else if (compareCaseInsensitive(input, "density") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->matDensity);
        matInput->matDensityDefined = 1;

    } else if (compareCaseInsensitive(input, "specific heat") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->matSpecificHeat);
        matInput->matSpecificHeatDefined = 1;

    } else if (compareCaseInsensitive(input, "thermal conductivity") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->matThermalConductivity);
        matInput->matThermalConductivityDefined = 1;

    } else if (compareCaseInsensitive(input, "initial temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->initialTemperature);
        matInput->initialTemperatureDefined = 1;

    } else if (compareCaseInsensitive(input, "blood perfusion") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->bloodPerfusion);

    } else if (compareCaseInsensitive(input, "blood density") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->bloodDensity);

    } else if (compareCaseInsensitive(input, "blood specific heat") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->bloodSpecificHeat);

    } else if (compareCaseInsensitive(input, "blood temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->bloodTemperature);

    } else if (compareCaseInsensitive(input, "internal heat generation") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->internalHeatGeneration);

    } else if (input[0] == '}' && *startEndBrackets == 1) {
        // DEBUG: finished reading the material configuration
        // printf("inside the closure\n");
        if ((errorTLMnumber = testEndBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 0; //define as the bracket was finalized
        return 9999; // that is my code to indicate that it was done.
    } else {
        return 1147;
    }



    return 0;
}

/*
 * printfMatConfig: prints the material configurations that were read
 */
void printfMatConfig(struct MaterialConfig *input, enum typeSim * typeS, enum solve *Solve) {
    printfNumberOfInputsMat(&input->numberInput, &input->quantityOfNumberInput);

    switch (*typeS) {
        case PENNES:
            printfMatDensity(&input->matDensity);
            printfMatSpecificHeat(&input->matSpecificHeat);
            printfMatThermalConductivity(&input->matThermalConductivity);
            if (*Solve == DYNAMIC) printfInitialTemperature(&input->initialTemperature);

            printfBloodPerfusion(&input->bloodPerfusion);
            printfBloodDensity(&input->bloodDensity);
            printfBloodSpecificHeat(&input->bloodSpecificHeat);
            printfBloodTemperature(&input->bloodTemperature);

            printfInternalHeatGeneration(&input->internalHeatGeneration);
            break;

        case HEAT:
            printfMatDensity(&input->matDensity);
            printfMatSpecificHeat(&input->matSpecificHeat);
            printfMatThermalConductivity(&input->matThermalConductivity);
            if (*Solve == DYNAMIC) printfInitialTemperature(&input->initialTemperature);

            printfInternalHeatGeneration(&input->internalHeatGeneration);
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
 * printfNumberOfInputsMat: prints the numbers of the materials that will have
 * the characteristics as defined here
 */
void printfNumberOfInputsMat(int ** input, int * number) {
    printf("Materials numbers: %d", *input[0]);

    for (int i = 1; i < *number; i++) {
        printf(" %d", *(input[0] + i));
    }

    printf(".\n");
}

/*
 * printfMatDensity: prints the density value
 */
void printfMatDensity(double *input) {
    printf("Density of the material: %9.4e kg/m3.\n", *input);
}

/*
 * printfMatSpecificHeat: prints the specific heat value
 */
void printfMatSpecificHeat(double * input) {
    printf("Specific heat of the material: %9.4e J/(K-kg).\n", *input);

}

/*
 * printfMatThermalConductivity: prints the thermal conductivity value
 */
void printfMatThermalConductivity(double *input) {
    printf("Thermal conductivity of the material: %9.4e W/(K-m).\n", *input);
}

/*
 * printfInitialTemperature: prints the initial temperature value
 */
void printfInitialTemperature(double * input) {
    printf("Initial temperature of the material: %9.4e oC.\n", *input);
}

/*
 * printfBloodPerfusion: prints the blood perfusion value
 */
void printfBloodPerfusion(double * input) {
    printf("Blood perfusion: %9.4e m3/(m3-s).\n", *input);
}

/*
 * printfBloodPerfusion: prints the blood perfusion value
 */
void printfBloodDensity(double * input) {
    printf("Blood density: %9.4e kg/m3.\n", *input);
}

/*
 * printfBloodPerfusion: prints the blood perfusion value
 */
void printfBloodSpecificHeat(double * input) {
    printf("Blood specific heat: %9.4e J/(K-kg).\n", *input);
}

/*
 * printfBloodTemperature: prints the blood temperature value
 */
void printfBloodTemperature(double * input) {
    printf("Blood temperature: %9.4e oC.\n", *input);
}

/*
 * printfInternalHeatGeneration: prints the internal heat generation value
 */
void printfInternalHeatGeneration(double * input) {
    printf("Internal heat generation: %9.4e W/m3.\n", *input);
}

/*
 * testInputMaterial: tests if all the required inputs were read
 */
unsigned int testInputMaterial(struct MaterialConfig * input,
        enum typeSim * typeS, enum solve *Solve, int id) {

    int errorFound = 0;

    switch (*typeS) {
        case PENNES:
            //break;
            /* FALLTHRU */
        case HEAT:
            if (input->matDensityDefined == 0) {
                sendErrorCodeAndMessage(4444, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            if (input->matSpecificHeatDefined == 0) {
                sendErrorCodeAndMessage(4445, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            if (input->matThermalConductivityDefined == 0) {
                sendErrorCodeAndMessage(4446, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            if (*Solve == DYNAMIC && input->initialTemperatureDefined == 0) {
                sendErrorCodeAndMessage(4447, &id, NULL, NULL, NULL);
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