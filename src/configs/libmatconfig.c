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
#include <string.h>

#include "libmatconfig.h"

#include "libsimuconfig.h"
#include "libfunctionconfig.h"
#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * intializeMaterialConfig: initialize the material variable used for configuration
 * with standard values
 */
unsigned int intializeMaterialConfig(struct MaterialConfig * mat) {

    mat->equationName = NULL;
    mat->equationNameDefined = 0;

    mat->equationNumber = -1;

    mat->materialName = NULL;
    mat->materialNameDefined = 0;

    mat->typeOfEquation = PENNES;

    mat->numberInput = NULL;
    mat->quantityOfNumberInput = 0; // initializing as zero
    mat->numberInputDefined = 0;

    // inputs associated with diffusion and hyperbolic diffusion
    mat->diffusionCoeff = 0;
    mat->diffusionCoeffDefined = 0;
    mat->diffusionCoeffFromFunc = NULL;
    mat->diffusionCoeffType = 1;

    mat->coefficient_b = 1;
    mat->coefficient_bFromFunc = NULL;
    mat->coefficient_bType = 1;

    mat->relaxationTime = 0;
    mat->relaxationTimeDefined = 0;
    mat->relaxationTimeFromFunc = NULL;
    mat->relaxationTimeType = 1;

    mat->sink_a = 0;
    mat->sink_aFromFunc = NULL;
    mat->sink_aType = 1;

    mat->source = 0;
    mat->sourceFromFunc = NULL;
    mat->sourceType = 1;

    mat->vectorialSource = (double*) malloc(sizeof (double)*3);
    mat->vectorialSource[0] = 0;
    mat->vectorialSource[1] = 0;
    mat->vectorialSource[2] = 0;
    mat->vectorialSourceFromFunc = (char**) malloc(sizeof (char*)*3);
    mat->vectorialSourceFromFunc[0] = NULL;
    mat->vectorialSourceFromFunc[1] = NULL;
    mat->vectorialSourceFromFunc[2] = NULL;
    mat->vectorialSourceType = (int*) malloc(sizeof (int)*3);
    mat->vectorialSourceType[0] = 1;
    mat->vectorialSourceType[1] = 1;
    mat->vectorialSourceType[2] = 1;
    mat->generalized_vectorialSourceGetFromFuncNumb = (int*) malloc(sizeof (int)*3);
    mat->generalized_vectorialSourceGetFromFuncNumb[0] = -1;
    mat->generalized_vectorialSourceGetFromFuncNumb[1] = -1;
    mat->generalized_vectorialSourceGetFromFuncNumb[2] = -1;


    mat->initialTemperature = 0;
    mat->initialTemperatureDefined = 0;
    mat->initialTemperatureFromFunc = NULL;
    mat->initialTemperatureType = 1;



    // Inputs associated with heat, hyperbolic heat, pennes, and hyperbolic pennes
    mat->matDensity = 0;
    mat->matDensityDefined = 0;
    mat->matDensityFromFunc = NULL;
    mat->matDensityType = 1;

    mat->matSpecificHeat = 0;
    mat->matSpecificHeatDefined = 0;
    mat->matSpecificHeatFromFunc = NULL;
    mat->matSpecificHeatType = 1;

    mat->matThermalConductivity = 0;
    mat->matThermalConductivityDefined = 0;
    mat->matThermalConductivityFromFunc = NULL;
    mat->matThermalConductivityType = 1;

    // these are the same defined for diffusion and hyperbolic diffusion
    //    mat->relaxationTime = 0;
    //    mat->relaxationTimeDefined = 0;
    //    
    //    mat->sink_a = 0;
    //    mat->source = 0;
    //    mat->vectorialSource[0] = 0;
    //    mat->vectorialSource[1] = 0;
    //    mat->vectorialSource[2] = 0;



    // data for the blood inside the material. Used in Pennes
    mat->bloodPerfusion = 0;
    mat->bloodPerfusionFromFunc = NULL;
    mat->bloodPerfusionType = 1;

    mat->bloodDensity = 0;
    mat->bloodDensityFromFunc = NULL;
    mat->bloodDensityType = 1;

    mat->bloodSpecificHeat = 0;
    mat->bloodSpecificHeatFromFunc = NULL;
    mat->bloodSpecificHeatType = 1;

    mat->bloodTemperature = 0;
    mat->bloodTemperatureFromFunc = NULL;
    mat->bloodTemperatureType = 1;

    // data for the internal heat generation. Used in Pennes and Heat
    mat->internalHeatGeneration = 0;
    mat->internalHeatGenerationFromFunc = NULL;
    mat->internalHeatGenerationType = 1;

    mat->initialTemperature = 0;
    mat->initialTemperatureDefined = 0;
    mat->initialTemperatureFromFunc = NULL;
    mat->initialTemperatureType = 1;

    mat->generalized_vectorialSource = (double*) malloc(sizeof (double)*3);
    mat->generalized_vectorialSource[0] = 0;
    mat->generalized_vectorialSource[1] = 0;
    mat->generalized_vectorialSource[2] = 0;
    mat->generalized_vectorialSourceFromFunc = (char**) malloc(sizeof (char*)*3);
    mat->generalized_vectorialSourceFromFunc[0] = NULL;
    mat->generalized_vectorialSourceFromFunc[1] = NULL;
    mat->generalized_vectorialSourceFromFunc[2] = NULL;
    mat->generalized_vectorialSourceType = (int*) malloc(sizeof (int)*3);
    mat->generalized_vectorialSourceType[0] = 1;
    mat->generalized_vectorialSourceType[1] = 1;
    mat->generalized_vectorialSourceType[2] = 1;

    mat->generalized_coefficient_bFromFunc = NULL;
    mat->generalized_coefficient_bType = 1;

    mat->generalized_diffusionCoeffFromFunc = NULL;
    mat->generalized_diffusionCoeffType = 1;

    mat->generalized_relaxationTimeFromFunc = NULL;
    mat->generalized_relaxationTimeType = 1;

    mat->generalized_initialScalarFromFunc = NULL;
    mat->generalized_initialScalarType = 1;

    mat->generalized_sink_aFromFunc = NULL;
    mat->generalized_sink_aType = 1;

    mat->generalized_sourceFromFunc = NULL;
    mat->generalized_sourceType = 1;

    // TLM flags
    mat->numberOfStubs = 0;

    return 0;
}

/*
 * terminateMaterialConfig: terminate the material variable used for configuration
 */
unsigned int terminateMaterialConfig(struct MaterialConfig * mat) {
    free(mat->equationName);
    mat->equationName = NULL;

    free(mat->materialName);
    mat->materialName = NULL;

    free(mat->numberInput);
    mat->numberInput = NULL;

    free(mat->vectorialSource);
    mat->vectorialSource = NULL;

    free(mat->diffusionCoeffFromFunc);
    mat->diffusionCoeffFromFunc = NULL;

    free(mat->coefficient_bFromFunc);
    mat->coefficient_bFromFunc = NULL;

    free(mat->relaxationTimeFromFunc);
    mat->relaxationTimeFromFunc = NULL;

    free(mat->sink_aFromFunc);
    mat->sink_aFromFunc = NULL;

    free(mat->sourceFromFunc);
    mat->sourceFromFunc = NULL;

    free(mat->vectorialSourceFromFunc[0]);
    free(mat->vectorialSourceFromFunc[1]);
    free(mat->vectorialSourceFromFunc[2]);

    mat->vectorialSourceFromFunc[0] = NULL;
    mat->vectorialSourceFromFunc[1] = NULL;
    mat->vectorialSourceFromFunc[2] = NULL;

    free(mat->vectorialSourceFromFunc);
    mat->vectorialSourceFromFunc = NULL;

    free(mat->vectorialSourceType);
    mat->vectorialSourceType = NULL;

    free(mat->generalized_vectorialSourceGetFromFuncNumb);
    mat->generalized_vectorialSourceGetFromFuncNumb = NULL;

    free(mat->initialTemperatureFromFunc);
    mat->initialTemperatureFromFunc = NULL;

    free(mat->matDensityFromFunc);
    mat->matDensityFromFunc = NULL;

    free(mat->matSpecificHeatFromFunc);
    mat->matSpecificHeatFromFunc = NULL;

    free(mat->matThermalConductivityFromFunc);
    mat->matThermalConductivityFromFunc = NULL;

    free(mat->bloodPerfusionFromFunc);
    mat->bloodPerfusionFromFunc = NULL;

    free(mat->bloodDensityFromFunc);
    mat->bloodDensityFromFunc = NULL;

    free(mat->bloodSpecificHeatFromFunc);
    mat->bloodSpecificHeatFromFunc = NULL;

    free(mat->bloodTemperatureFromFunc);
    mat->bloodTemperatureFromFunc = NULL;

    free(mat->internalHeatGenerationFromFunc);
    mat->internalHeatGenerationFromFunc = NULL;

    free(mat->initialTemperatureFromFunc);
    mat->initialTemperatureFromFunc = NULL;

    free(mat->generalized_vectorialSourceFromFunc[0]);
    free(mat->generalized_vectorialSourceFromFunc[1]);
    free(mat->generalized_vectorialSourceFromFunc[2]);

    mat->generalized_vectorialSourceFromFunc[0] = NULL;
    mat->generalized_vectorialSourceFromFunc[1] = NULL;
    mat->generalized_vectorialSourceFromFunc[2] = NULL;

    free(mat->generalized_vectorialSourceFromFunc);
    mat->generalized_vectorialSourceFromFunc = NULL;

    free(mat->generalized_vectorialSourceType);
    mat->generalized_vectorialSourceType = NULL;

    free(mat->generalized_coefficient_bFromFunc);
    mat->generalized_coefficient_bFromFunc = NULL;

    free(mat->generalized_diffusionCoeffFromFunc);
    mat->generalized_diffusionCoeffFromFunc = NULL;

    free(mat->generalized_relaxationTimeFromFunc);
    mat->generalized_relaxationTimeFromFunc = NULL;

    free(mat->generalized_initialScalarFromFunc);
    mat->generalized_initialScalarFromFunc = NULL;

    free(mat->generalized_sink_aFromFunc);
    mat->generalized_sink_aFromFunc = NULL;

    free(mat->generalized_sourceFromFunc);
    mat->generalized_sourceFromFunc = NULL;

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

    } else if (compareCaseInsensitive(input, "equation") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // the input is the name
        matInput->equationName = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(matInput->equationName, input);

        matInput->equationNameDefined = 1;

    } else if (compareCaseInsensitive(input, "name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        removeBlankSpacesBeforeAndAfter(input);

        // the input is the name
        matInput->materialName = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(matInput->materialName, input);

        matInput->materialNameDefined = 1;

    } else if (compareCaseInsensitive(input, "number") == 0) {
        if ((errorTLMnumber = readVectorIntInputs(input,
                &matInput->quantityOfNumberInput, &matInput->numberInput)) != 0)
            return errorTLMnumber;

        matInput->numberInputDefined = 1;

        // DEBUG: Checking the number we got
        // printf("The number (%d) we got were",matInput->quantityOfNumberInput);
        // for (int i = 0; i < matInput->quantityOfNumberInput; i++){
        //      printf(" %d", matInput->numberInput[i]);
        // }

    } else if (compareCaseInsensitive(input, "diffusion coefficient") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->diffusionCoeff) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->diffusionCoeffFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->diffusionCoeffFromFunc, input);
            matInput->diffusionCoeffType = 2;
        } else {
            matInput->diffusionCoeffType = 1;
        }
        matInput->diffusionCoeffDefined = 1;

    } else if (compareCaseInsensitive(input, "coefficient b") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->coefficient_b) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->coefficient_bFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->coefficient_bFromFunc, input);
            matInput->coefficient_bType = 2;
        } else {
            matInput->coefficient_bType = 1;
        }

    } else if (compareCaseInsensitive(input, "relaxation time") == 0 ||
            compareCaseInsensitive(input, "thermal relaxation time") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->relaxationTime) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->relaxationTimeFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->relaxationTimeFromFunc, input);
            matInput->relaxationTimeType = 2;
        } else {
            matInput->relaxationTimeType = 1;
        }
        matInput->relaxationTimeDefined = 1;

    } else if (compareCaseInsensitive(input, "sink a") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;


        if (sscanf(input, "%lf", &matInput->sink_a) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->sink_aFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->sink_aFromFunc, input);
            matInput->sink_aType = 2;
        } else {
            matInput->sink_aType = 1;
        }

    } else if (compareCaseInsensitive(input, "source") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->source) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->sourceFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->sourceFromFunc, input);
            matInput->sourceType = 2;
        } else {
            matInput->sourceType = 1;
        }

    } else if (compareCaseInsensitive(input, "vectorial source") == 0) {
        if ((errorTLMnumber = readVectorDoubleLengthThreeInputs(input, matInput->vectorialSource,
                matInput->vectorialSourceFromFunc, matInput->vectorialSourceType)) != 0)
            return errorTLMnumber;

    } else if (compareCaseInsensitive(input, "initial scalar") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->initialScalar) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->initialScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->initialScalarFromFunc, input);
            matInput->initialScalarType = 2;
        } else {
            matInput->initialScalarType = 1;
        }
        matInput->initialScalarDefined = 1;

    } else if (compareCaseInsensitive(input, "density") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->matDensity) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->matDensityFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->matDensityFromFunc, input);
            matInput->matDensityType = 2;
        } else {
            matInput->matDensityType = 1;
        }
        matInput->matDensityDefined = 1;

    } else if (compareCaseInsensitive(input, "specific heat") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->matSpecificHeat) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->matSpecificHeatFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->matSpecificHeatFromFunc, input);
            matInput->matSpecificHeatType = 2;
        } else {
            matInput->matSpecificHeatType = 1;
        }
        matInput->matSpecificHeatDefined = 1;

    } else if (compareCaseInsensitive(input, "thermal conductivity") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;


        if (sscanf(input, "%lf", &matInput->matThermalConductivity) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->matThermalConductivityFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->matThermalConductivityFromFunc, input);
            matInput->matThermalConductivityType = 2;
        } else {
            matInput->matThermalConductivityType = 1;
        }
        matInput->matThermalConductivityDefined = 1;

    } else if (compareCaseInsensitive(input, "initial temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->initialTemperature) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->initialTemperatureFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->initialTemperatureFromFunc, input);
            matInput->initialTemperatureType = 2;
        } else {
            matInput->initialTemperatureType = 1;
        }
        matInput->initialTemperatureDefined = 1;

    } else if (compareCaseInsensitive(input, "blood perfusion") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->bloodPerfusion) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->bloodPerfusionFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->bloodPerfusionFromFunc, input);
            matInput->bloodPerfusionType = 2;
        } else {
            matInput->bloodPerfusionType = 1;
        }

    } else if (compareCaseInsensitive(input, "blood density") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->bloodDensity) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->bloodDensityFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->bloodDensityFromFunc, input);
            matInput->bloodDensityType = 2;
        } else {
            matInput->bloodDensityType = 1;
        }

    } else if (compareCaseInsensitive(input, "blood specific heat") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->bloodSpecificHeat) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->bloodSpecificHeatFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->bloodSpecificHeatFromFunc, input);
            matInput->bloodSpecificHeatType = 2;
        } else {
            matInput->bloodSpecificHeatType = 1;
        }

    } else if (compareCaseInsensitive(input, "blood temperature") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->bloodTemperature) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->bloodTemperatureFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->bloodTemperatureFromFunc, input);
            matInput->bloodTemperatureType = 2;
        } else {
            matInput->bloodTemperatureType = 1;
        }

    } else if (compareCaseInsensitive(input, "internal heat generation") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        if (sscanf(input, "%lf", &matInput->internalHeatGeneration) == 0) {
            // if sscanf returns zero, then it was not able to obtain the 
            // output. So, we assume that is a function
            removeBlankSpacesBeforeAndAfter(input);
            matInput->internalHeatGenerationFromFunc = (char*) malloc(sizeof (char)*(strlen(input) + 1));
            strcpy(matInput->internalHeatGenerationFromFunc, input);
            matInput->internalHeatGenerationType = 2;
        } else {
            matInput->internalHeatGenerationType = 1;
        }

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
void printfMatConfig(struct MaterialConfig *input, struct Equation *equation) {

    printfTypeS(&equation->typeS);
    printf("Equation name: %s\n", equation->equationName);
    printf("Equation number: %d\n", input->equationNumber + 1);

    printfNumberOfInputsMat(&input->numberInput, &input->quantityOfNumberInput);

    switch (equation->typeS) {
        case HYPERBOLIC_DIFFUSION:
            printfRelaxationTime(input->relaxationTime, input->relaxationTimeFromFunc, input->relaxationTimeType);

            // break;
            /* FALLTHRU */
        case DIFFUSION:

            printfCoefficient_b(input->coefficient_b, input->coefficient_bFromFunc, input->coefficient_bType);
            printfDiffusionCoeff(input->diffusionCoeff, input->diffusionCoeffFromFunc, input->diffusionCoeffType);
            printfSink_a(input->sink_a, input->sink_aFromFunc, input->sink_aType);
            printfSource(input->source, input->sourceFromFunc, input->sourceType);
            printfVectorialSource(input->vectorialSource, input->vectorialSourceFromFunc, input->vectorialSourceType);

            if (equation->Solv == DYNAMIC) printfInitialScalar(input->initialScalar, input->initialScalarFromFunc, input->initialScalarType);

            break;
        case HYPERBOLIC_HEAT:
            printfRelaxationTimeHeat(input->relaxationTime, input->relaxationTimeFromFunc, input->relaxationTimeType);

            // break;
            /* FALLTHRU */
        case HEAT:
            printfMatDensity(input->matDensity, input->matDensityFromFunc, input->matDensityType);
            printfMatSpecificHeat(input->matSpecificHeat, input->matSpecificHeatFromFunc, input->matSpecificHeatType);
            printfMatThermalConductivity(input->matThermalConductivity, input->matThermalConductivityFromFunc, input->matThermalConductivityType);

            printfSink_aHeat(input->sink_a, input->sink_aFromFunc, input->sink_aType);
            printfSourceHeat(input->source, input->sourceFromFunc, input->sourceType);
            printfVectorialSourceHeat(input->vectorialSource, input->vectorialSourceFromFunc, input->vectorialSourceType);


            if (equation->Solv == DYNAMIC) printfInitialTemperature(input->initialTemperature, input->initialTemperatureFromFunc, input->initialTemperatureType);

            break;

        case HYPERBOLIC_PENNES:
            printfRelaxationTimeHeat(input->relaxationTime, input->relaxationTimeFromFunc, input->relaxationTimeType);

            // break;
            /* FALLTHRU */
        case PENNES:
            printfMatDensity(input->matDensity, input->matDensityFromFunc, input->matDensityType);
            printfMatSpecificHeat(input->matSpecificHeat, input->matSpecificHeatFromFunc, input->matSpecificHeatType);
            printfMatThermalConductivity(input->matThermalConductivity, input->matThermalConductivityFromFunc, input->matThermalConductivityType);

            printfSourceHeat(input->source, input->sourceFromFunc, input->sourceType);
            printfInternalHeatGeneration(input->internalHeatGeneration, input->internalHeatGenerationFromFunc, input->internalHeatGenerationType);

            printfVectorialSourceHeat(input->vectorialSource, input->vectorialSourceFromFunc, input->vectorialSourceType);

            printfBloodPerfusion(input->bloodPerfusion, input->bloodPerfusionFromFunc, input->bloodPerfusionType);
            printfBloodDensity(input->bloodDensity, input->bloodDensityFromFunc, input->bloodDensityType);
            printfBloodSpecificHeat(input->bloodSpecificHeat, input->bloodSpecificHeatFromFunc, input->bloodSpecificHeatType);
            printfBloodTemperature(input->bloodTemperature, input->bloodTemperatureFromFunc, input->bloodTemperatureType);


            if (equation->Solv == DYNAMIC) printfInitialTemperature(input->initialTemperature, input->initialTemperatureFromFunc, input->initialTemperatureType);

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
 * printfRelaxationTime: prints the relaxation time value
 */
void printfRelaxationTime(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Relaxation time with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Relaxation time calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfCoefficient_b: prints the coefficient b value
 */
void printfCoefficient_b(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Coefficient b with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Coefficient b calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfDiffusionCoeff: prints the diffusion coefficient value
 */
void printfDiffusionCoeff(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Diffusion coefficient with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Diffusion coefficient calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfSink_a: prints the sink 'a' value
 */
void printfSink_a(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Sink 'a' with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Sink 'a' calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfSource: prints the source value
 */
void printfSource(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Source with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Source calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfVectorialSource: prints the vectorial source value
 */
void printfVectorialSource(double *inputDouble, char **inputChar, int *inputType) {
    printf("Vectorial source: [");
    if (inputType[0] == 1) {
        printf("%9.4e, ", inputDouble[0]);
    } else if (inputType[0] == 2) {
        printf("from function %s, ", inputChar[0]);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

    if (inputType[1] == 1) {
        printf("%9.4e, ", inputDouble[1]);
    } else if (inputType[1] == 2) {
        printf("from function %s, ", inputChar[1]);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

    if (inputType[2] == 1) {
        printf("%9.4e\n", inputDouble[2]);
    } else if (inputType[2] == 2) {
        printf("from function %s]\n", inputChar[2]);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfInitialScalar: prints the initial scalar value
 */
void printfInitialScalar(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Initial scalar with value of %9.4e.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Initial scalar calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfRelaxationTimeHeat: prints the relaxation time value
 */
void printfRelaxationTimeHeat(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Relaxation time with value of %9.4e s.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Relaxation time calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfMatDensity: prints the density value
 */
void printfMatDensity(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Density with value of %9.4e kg/m3.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Density calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfMatSpecificHeat: prints the specific heat value
 */
void printfMatSpecificHeat(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Specific heat with value of %9.4e J/(K-kg).\n", inputDouble);
    } else if (inputType == 2) {
        printf("Specific heat calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

}

/*
 * printfMatThermalConductivity: prints the thermal conductivity value
 */
void printfMatThermalConductivity(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Thermal conductivity with value of %9.4e W/(K-m).\n", inputDouble);
    } else if (inputType == 2) {
        printf("Thermal conductivity calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfInitialTemperature: prints the initial temperature value
 */
void printfInitialTemperature(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Initial temperature with value of %9.4e oC.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Initial temperature calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfSink_aHeat: prints the sink 'a' value
 */
void printfSink_aHeat(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Sink 'a' with value of %9.4e J/(m3-s-K).\n", inputDouble);
    } else if (inputType == 2) {
        printf("Sink 'a' calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfSourceHeat: prints the heat source value
 */
void printfSourceHeat(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Heat source with value of %9.4e W/m3.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Heat source calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfVectorialSourceHeat: prints the vectorial source value
 */
void printfVectorialSourceHeat(double *inputDouble, char **inputChar, int *inputType) {
    printf("Vectorial source (W/m2): [");
    if (inputType[0] == 1) {
        printf("%9.4e, ", inputDouble[0]);
    } else if (inputType[0] == 2) {
        printf("from function %s, ", inputChar[0]);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

    if (inputType[1] == 1) {
        printf("%9.4e, ", inputDouble[1]);
    } else if (inputType[1] == 2) {
        printf("from function %s, ", inputChar[1]);
    } else {
        printf("BUG: Unexpected behavior\n");
    }

    if (inputType[2] == 1) {
        printf("%9.4e\n", inputDouble[2]);
    } else if (inputType[2] == 2) {
        printf("from function %s]\n", inputChar[2]);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfBloodPerfusion: prints the blood perfusion value
 */
void printfBloodPerfusion(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Blood perfusion with value of %9.4e m3/(m3-s).\n", inputDouble);
    } else if (inputType == 2) {
        printf("Blood perfusion calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfBloodPerfusion: prints the blood perfusion value
 */
void printfBloodDensity(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Blood density with value of %9.4e kg/m3.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Blood density calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfBloodPerfusion: prints the blood perfusion value
 */
void printfBloodSpecificHeat(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Blood specific heat with value of %9.4e J/(K-kg).\n", inputDouble);
    } else if (inputType == 2) {
        printf("Blood specific heat calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfBloodTemperature: prints the blood temperature value
 */
void printfBloodTemperature(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Blood temperature with value of %9.4e oC.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Blood temperature calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * printfInternalHeatGeneration: prints the internal heat generation value
 */
void printfInternalHeatGeneration(double inputDouble, char *inputChar, int inputType) {

    if (inputType == 1) {
        printf("Internal heat generation with value of %9.4e W/m3.\n", inputDouble);
    } else if (inputType == 2) {
        printf("Internal heat generation calculated from function %s.\n", inputChar);
    } else {
        printf("BUG: Unexpected behavior\n");
    }
}

/*
 * testInputMaterial: tests if all the required inputs were read
 */
unsigned int testInputMaterial(struct MaterialConfig * input,
        struct Equation * equation, int id, struct FunctionConfig *Functions, int numberOfFunctions) {

    int errorFound = 0;

    switch (equation->typeS) {
        case HYPERBOLIC_DIFFUSION:
            if (input->relaxationTimeDefined == 0) {
                sendErrorCodeAndMessage(4441, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            //break;
            /* FALLTHRU */
        case DIFFUSION:

            if (input->diffusionCoeffDefined == 0) {
                sendErrorCodeAndMessage(4448, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            if (equation->Solv == DYNAMIC && input->initialScalarDefined == 0) {
                sendErrorCodeAndMessage(4449, &id, NULL, NULL, NULL);
                errorFound = 1;
            }

            if (generalizeDiffusionInput(input, equation[input->equationNumber].Solv,
                    Functions, numberOfFunctions, id) != 0) {
                errorFound = 1;
            }

            break;
        case HYPERBOLIC_PENNES:
            //break;
            /* FALLTHRU */
        case HYPERBOLIC_HEAT:

            if (input->relaxationTimeDefined == 0) {
                sendErrorCodeAndMessage(4441, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            // setting the TLM flag for including a stub for this material
            input->numberOfStubs = 1;
            //break;
            /* FALLTHRU */
        case PENNES:
            // since I use fall through to make the code more efficient, I will
            // verify here if the type of equation is not hyperbolic_heat because
            // I will apply the generalization of the input parameters to the pennes
            // type of equation
            if (equation->typeS != HYPERBOLIC_HEAT) {
                if (generalizePennesInput(input, equation[input->equationNumber].Solv,
                        Functions, numberOfFunctions, id) != 0) {
                    errorFound = 1;
                }
            }
            //break;
            /* FALLTHRU */
        case HEAT:
            if (equation->Solv == DYNAMIC && input->matDensityDefined == 0) {
                sendErrorCodeAndMessage(4444, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            if (equation->Solv == DYNAMIC && input->matSpecificHeatDefined == 0) {
                sendErrorCodeAndMessage(4445, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            if (input->matThermalConductivityDefined == 0) {
                sendErrorCodeAndMessage(4446, &id, NULL, NULL, NULL);
                errorFound = 1;
            }
            if (equation->Solv == DYNAMIC && input->initialTemperatureDefined == 0) {
                sendErrorCodeAndMessage(4447, &id, NULL, NULL, NULL);
                errorFound = 1;
            }

            // since I use fall through to make the code more efficient, I will
            // verify here if the type of equation is hyperbolic_heat or heat because
            // I will apply the generalization of the input parameters to the heat
            // type of equation
            if (equation->typeS == HYPERBOLIC_HEAT || equation->typeS == HEAT) {
                if (generalizeHeatInput(input, equation[input->equationNumber].Solv,
                        Functions, numberOfFunctions, id) != 0) {
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
 * generalizeDiffusionInput: copy the values of the diffusion input to the
 * generalized variables. I do this so that I can have only one solver for 
 * diffusion, heat, and pennes.
 */
unsigned int generalizeDiffusionInput(struct MaterialConfig *input, enum solve Sol,
        struct FunctionConfig * Functions, int numberOfFunctions, int id) {
    int errorFound = 0;
    input->generalized_coefficient_b = input->coefficient_b;
    input->generalized_coefficient_bType = input->coefficient_bType;
    if (input->generalized_coefficient_bType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_coefficient_bFromFunc = (char*) malloc(sizeof (char)*(strlen(input->coefficient_bFromFunc) + 1));
        strcpy(input->generalized_coefficient_bFromFunc, input->coefficient_bFromFunc);
        if (findFunctionName(input->generalized_coefficient_bFromFunc, &input->generalized_coefficient_bGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4601, input->generalized_coefficient_bFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }


    input->generalized_diffusionCoeff = input->diffusionCoeff;
    input->generalized_diffusionCoeffType = input->diffusionCoeffType;
    if (input->generalized_diffusionCoeffType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_diffusionCoeffFromFunc = (char*) malloc(sizeof (char)*(strlen(input->diffusionCoeffFromFunc) + 1));
        strcpy(input->generalized_diffusionCoeffFromFunc, input->diffusionCoeffFromFunc);
        if (findFunctionName(input->generalized_diffusionCoeffFromFunc, &input->generalized_diffusionCoeffGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4602, input->generalized_diffusionCoeffFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_relaxationTime = input->relaxationTime;
    input->generalized_relaxationTimeType = input->relaxationTimeType;
    if (input->generalized_relaxationTimeType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_relaxationTimeFromFunc = (char*) malloc(sizeof (char)*(strlen(input->relaxationTimeFromFunc) + 1));
        strcpy(input->generalized_relaxationTimeFromFunc, input->relaxationTimeFromFunc);
        if (findFunctionName(input->generalized_relaxationTimeFromFunc, &input->generalized_relaxationTimeGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4603, input->generalized_relaxationTimeFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_vectorialSource[0] = input->vectorialSource[0];
    input->generalized_vectorialSourceType[0] = input->vectorialSourceType[0];
    if (input->generalized_vectorialSourceType[0] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[0] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[0]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[0], input->vectorialSourceFromFunc[0]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[0], &input->generalized_vectorialSourceGetFromFuncNumb[0], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4604, input->generalized_vectorialSourceFromFunc[0], &id, NULL, NULL);
            errorFound = 1;
        }
    }
    input->generalized_vectorialSource[1] = input->vectorialSource[1];
    input->generalized_vectorialSourceType[1] = input->vectorialSourceType[1];
    if (input->generalized_vectorialSourceType[1] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[1] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[1]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[1], input->vectorialSourceFromFunc[1]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[1], &input->generalized_vectorialSourceGetFromFuncNumb[1], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4605, input->generalized_vectorialSourceFromFunc[1], &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_vectorialSource[2] = input->vectorialSource[2];
    input->generalized_vectorialSourceType[2] = input->vectorialSourceType[2];
    if (input->generalized_vectorialSourceType[2] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[2] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[2]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[2], input->vectorialSourceFromFunc[1]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[2], &input->generalized_vectorialSourceGetFromFuncNumb[2], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4606, input->generalized_vectorialSourceFromFunc[2], &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_initialScalar = input->initialScalar;
    input->generalized_initialScalarType = input->initialScalarType;
    if (input->generalized_initialScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_initialScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->initialScalarFromFunc) + 1));
        strcpy(input->generalized_initialScalarFromFunc, input->initialScalarFromFunc);
        if (findFunctionName(input->generalized_initialScalarFromFunc, &input->generalized_initialScalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4607, input->generalized_initialScalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_sink_a = input->sink_a;
    input->generalized_sink_aType = input->sink_aType;
    if (input->generalized_initialScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_sink_aFromFunc = (char*) malloc(sizeof (char)*(strlen(input->sink_aFromFunc) + 1));
        strcpy(input->generalized_sink_aFromFunc, input->sink_aFromFunc);
        if (findFunctionName(input->generalized_sink_aFromFunc, &input->generalized_sink_aGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4608, input->generalized_sink_aFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_source = input->source;
    input->generalized_sourceType = input->sourceType;
    if (input->generalized_sourceType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_sourceFromFunc = (char*) malloc(sizeof (char)*(strlen(input->sourceFromFunc) + 1));
        strcpy(input->generalized_sourceFromFunc, input->sourceFromFunc);
        if (findFunctionName(input->generalized_sourceFromFunc, &input->generalized_sourceGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4609, input->generalized_sourceFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }



    // if we are solving for steady-state, we need a value for the coefficient b
    // We do this because in the TLM formalism we need the coefficient b to calculate
    // the impedance of the transmission line.
    if (Sol == STEADY && input->generalized_coefficient_b == 0) {
        input->generalized_coefficient_b = 1;
    }

    return errorFound;
}

/*
 * generalizeHeatInput: copy the values of the heat input to the
 * generalized variables. I do this so that I can have only one solver for 
 * diffusion, heat, and pennes.
 */
unsigned int generalizeHeatInput(struct MaterialConfig *input, enum solve Sol,
        struct FunctionConfig * Functions, int numberOfFunctions, int id) {
    int errorFound = 0;
    input->generalized_coefficient_b = input->matDensity * input->matSpecificHeat;
    if (input->matDensityType == 2 || input->matSpecificHeatType == 2) {
        input->generalized_coefficient_bType = 2;
        // This input is uses a function but we don't know from which variable this function is coming from
        input->generalized_coefficient_bGetFromFuncNumb = -2; // -2: flag that tells we should look for inner functions
        if (input->matDensityType == 2) {
            if (findFunctionName(input->matDensityFromFunc, &input->matDensityGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4630, input->matDensityFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->matDensityGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

        if (input->matSpecificHeatType == 2) {
            if (findFunctionName(input->matSpecificHeatFromFunc, &input->matSpecificHeatGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4631, input->matSpecificHeatFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->matSpecificHeatGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

    } else {
        input->generalized_coefficient_bType = 1;
    }

    input->generalized_diffusionCoeff = input->matThermalConductivity;
    input->generalized_diffusionCoeffType = input->matThermalConductivityType;
    if (input->generalized_diffusionCoeffType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_diffusionCoeffFromFunc = (char*) malloc(sizeof (char)*(strlen(input->matThermalConductivityFromFunc) + 1));
        strcpy(input->generalized_diffusionCoeffFromFunc, input->matThermalConductivityFromFunc);
        if (findFunctionName(input->generalized_diffusionCoeffFromFunc, &input->generalized_diffusionCoeffGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4632, input->generalized_diffusionCoeffFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_relaxationTime = input->relaxationTime;
    input->generalized_relaxationTimeType = input->relaxationTimeType;
    if (input->generalized_relaxationTimeType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_relaxationTimeFromFunc = (char*) malloc(sizeof (char)*(strlen(input->relaxationTimeFromFunc) + 1));
        strcpy(input->generalized_relaxationTimeFromFunc, input->relaxationTimeFromFunc);
        if (findFunctionName(input->generalized_relaxationTimeFromFunc, &input->generalized_relaxationTimeGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4603, input->generalized_relaxationTimeFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_vectorialSource[0] = input->vectorialSource[0];
    input->generalized_vectorialSourceType[0] = input->vectorialSourceType[0];
    if (input->generalized_vectorialSourceType[0] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[0] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[0]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[0], input->vectorialSourceFromFunc[0]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[0], &input->generalized_vectorialSourceGetFromFuncNumb[0], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4604, input->generalized_vectorialSourceFromFunc[0], &id, NULL, NULL);
            errorFound = 1;
        }
    }
    input->generalized_vectorialSource[1] = input->vectorialSource[1];
    input->generalized_vectorialSourceType[1] = input->vectorialSourceType[1];
    if (input->generalized_vectorialSourceType[1] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[1] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[1]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[1], input->vectorialSourceFromFunc[1]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[1], &input->generalized_vectorialSourceGetFromFuncNumb[1], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4605, input->generalized_vectorialSourceFromFunc[1], &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_vectorialSource[2] = input->vectorialSource[2];
    input->generalized_vectorialSourceType[2] = input->vectorialSourceType[2];
    if (input->generalized_vectorialSourceType[2] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[2] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[2]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[2], input->vectorialSourceFromFunc[1]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[2], &input->generalized_vectorialSourceGetFromFuncNumb[2], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4606, input->generalized_vectorialSourceFromFunc[2], &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_initialScalar = input->initialTemperature;
    input->generalized_initialScalarType = input->initialTemperatureType;
    if (input->generalized_initialScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_initialScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->initialTemperatureFromFunc) + 1));
        strcpy(input->generalized_initialScalarFromFunc, input->initialTemperatureFromFunc);
        if (findFunctionName(input->generalized_initialScalarFromFunc, &input->generalized_initialScalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4633, input->generalized_initialScalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }


    input->generalized_sink_a = input->sink_a;
    input->generalized_sink_aType = input->sink_aType;
    if (input->generalized_initialScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_sink_aFromFunc = (char*) malloc(sizeof (char)*(strlen(input->sink_aFromFunc) + 1));
        strcpy(input->generalized_sink_aFromFunc, input->sink_aFromFunc);
        if (findFunctionName(input->generalized_sink_aFromFunc, &input->generalized_sink_aGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4608, input->generalized_sink_aFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_source = input->source;
    input->generalized_sourceType = input->sourceType;
    if (input->generalized_sourceType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_sourceFromFunc = (char*) malloc(sizeof (char)*(strlen(input->sourceFromFunc) + 1));
        strcpy(input->generalized_sourceFromFunc, input->sourceFromFunc);
        if (findFunctionName(input->generalized_sourceFromFunc, &input->generalized_sourceGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4609, input->generalized_sourceFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    // if we are solving for steady-state, we need a value for the coefficient b
    // We do this because in the TLM formalism we need the coefficient b to calculate
    // the impedance of the transmission line.
    if (Sol == STEADY && input->generalized_coefficient_b == 0) {
        input->generalized_coefficient_b = 1;
    }

    return errorFound;
}

/*
 * generalizePennesInput: copy the values of the pennes input to the
 * generalized variables. I do this so that I can have only one solver for 
 * diffusion, heat, and pennes.
 */
unsigned int generalizePennesInput(struct MaterialConfig *input, enum solve Sol,
        struct FunctionConfig * Functions, int numberOfFunctions, int id) {
    int errorFound = 0;

    input->generalized_coefficient_b = input->matDensity * input->matSpecificHeat;
    if (input->matDensityType == 2 || input->matSpecificHeatType == 2) {
        input->generalized_coefficient_bType = 2;
        // This input is uses a function but we don't know from which variable this function is coming from
        input->generalized_coefficient_bGetFromFuncNumb = -2; // -2: flag that tells we should look for inner functions
        if (input->matDensityType == 2) {
            if (findFunctionName(input->matDensityFromFunc, &input->matDensityGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4630, input->matDensityFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->matDensityGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

        if (input->matSpecificHeatType == 2) {
            if (findFunctionName(input->matSpecificHeatFromFunc, &input->matSpecificHeatGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4631, input->matSpecificHeatFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->matSpecificHeatGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

    } else {
        input->generalized_coefficient_bType = 1;
    }

    input->generalized_diffusionCoeff = input->matThermalConductivity;
    input->generalized_diffusionCoeffType = input->matThermalConductivityType;
    if (input->generalized_diffusionCoeffType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_diffusionCoeffFromFunc = (char*) malloc(sizeof (char)*(strlen(input->matThermalConductivityFromFunc) + 1));
        strcpy(input->generalized_diffusionCoeffFromFunc, input->matThermalConductivityFromFunc);
        if (findFunctionName(input->generalized_diffusionCoeffFromFunc, &input->generalized_diffusionCoeffGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4632, input->generalized_diffusionCoeffFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_relaxationTime = input->relaxationTime;
    input->generalized_relaxationTimeType = input->relaxationTimeType;
    if (input->generalized_relaxationTimeType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_relaxationTimeFromFunc = (char*) malloc(sizeof (char)*(strlen(input->relaxationTimeFromFunc) + 1));
        strcpy(input->generalized_relaxationTimeFromFunc, input->relaxationTimeFromFunc);
        if (findFunctionName(input->generalized_relaxationTimeFromFunc, &input->generalized_relaxationTimeGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4603, input->generalized_relaxationTimeFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_vectorialSource[0] = input->vectorialSource[0];
    input->generalized_vectorialSourceType[0] = input->vectorialSourceType[0];
    if (input->generalized_vectorialSourceType[0] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[0] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[0]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[0], input->vectorialSourceFromFunc[0]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[0], &input->generalized_vectorialSourceGetFromFuncNumb[0], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4604, input->generalized_vectorialSourceFromFunc[0], &id, NULL, NULL);
            errorFound = 1;
        }
    }
    input->generalized_vectorialSource[1] = input->vectorialSource[1];
    input->generalized_vectorialSourceType[1] = input->vectorialSourceType[1];
    if (input->generalized_vectorialSourceType[1] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[1] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[1]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[1], input->vectorialSourceFromFunc[1]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[1], &input->generalized_vectorialSourceGetFromFuncNumb[1], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4605, input->generalized_vectorialSourceFromFunc[1], &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_vectorialSource[2] = input->vectorialSource[2];
    input->generalized_vectorialSourceType[2] = input->vectorialSourceType[2];
    if (input->generalized_vectorialSourceType[2] == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_vectorialSourceFromFunc[2] = (char*) malloc(sizeof (char)*(strlen(input->vectorialSourceFromFunc[2]) + 1));
        strcpy(input->generalized_vectorialSourceFromFunc[2], input->vectorialSourceFromFunc[1]);
        if (findFunctionName(input->generalized_vectorialSourceFromFunc[2], &input->generalized_vectorialSourceGetFromFuncNumb[2], Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4606, input->generalized_vectorialSourceFromFunc[2], &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_initialScalar = input->initialTemperature;
    input->generalized_initialScalarType = input->initialTemperatureType;
    if (input->generalized_initialScalarType == 2) {
        // if this input is a function, we copy the rest of the variables into it
        input->generalized_initialScalarFromFunc = (char*) malloc(sizeof (char)*(strlen(input->initialTemperatureFromFunc) + 1));
        strcpy(input->generalized_initialScalarFromFunc, input->initialTemperatureFromFunc);
        if (findFunctionName(input->generalized_initialScalarFromFunc, &input->generalized_initialScalarGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
            sendErrorCodeAndMessage(4633, input->generalized_initialScalarFromFunc, &id, NULL, NULL);
            errorFound = 1;
        }
    }

    input->generalized_sink_a = input->bloodPerfusion * input->bloodDensity * input->bloodSpecificHeat;
    if (input->bloodPerfusionType == 2 || input->bloodDensityType == 2 || input->bloodSpecificHeatType == 2) {
        input->generalized_sink_aType = 2;
        // This input is uses a function but we don't know from which variable this function is coming from
        input->generalized_coefficient_bGetFromFuncNumb = -2; // -2: flag that tells we should look for inner functions
        if (input->bloodPerfusionType == 2) {
            if (findFunctionName(input->bloodPerfusionFromFunc, &input->bloodPerfusionGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4634, input->bloodPerfusionFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->bloodPerfusionGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

        if (input->bloodDensityType == 2) {
            if (findFunctionName(input->bloodDensityFromFunc, &input->bloodDensityGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4635, input->bloodDensityFromFunc, &id, NULL, NULL);
                errorFound = 1;

            }
        } else {
            input->bloodDensityGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

        if (input->bloodSpecificHeatType == 2) {
            if (findFunctionName(input->bloodSpecificHeatFromFunc, &input->bloodSpecificHeatGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4636, input->bloodSpecificHeatFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->bloodSpecificHeatGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

    } else {
        input->generalized_sink_aType = 1;
    }


    input->generalized_source = input->source + input->internalHeatGeneration +
            input->bloodPerfusion * input->bloodDensity * input->bloodSpecificHeat * input->bloodTemperature;

    if (input->sourceType == 2 || input->internalHeatGenerationType == 2 ||
            input->bloodPerfusionType == 2 || input->bloodDensityType == 2 || input->bloodSpecificHeatType == 2 || input->bloodTemperatureType == 2) {
        input->generalized_sourceType = 2;
        // This input is uses a function but we don't know from which variable this function is coming from
        input->generalized_coefficient_bGetFromFuncNumb = -2; // -2: flag that tells we should look for inner functions
        // don't need to test for bloodPerfusion, bloodDensity, and bloodSpecificHeat because they were tested above for generalized_sink_a

        if (input->sourceType == 2) {
            if (findFunctionName(input->sourceFromFunc, &input->sourceGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4609, input->sourceFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->sourceGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

        if (input->internalHeatGenerationType == 2) {
            if (findFunctionName(input->internalHeatGenerationFromFunc, &input->internalHeatGenerationGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4609, input->internalHeatGenerationFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->internalHeatGenerationGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

        if (input->bloodTemperatureType == 2) {
            if (findFunctionName(input->bloodTemperatureFromFunc, &input->bloodTemperatureGetFromFuncNumb, Functions, numberOfFunctions) != 0) {
                sendErrorCodeAndMessage(4637, input->bloodTemperatureFromFunc, &id, NULL, NULL);
                errorFound = 1;
            }
        } else {
            input->bloodTemperatureGetFromFuncNumb = -1; // flag that tells it's not a function--use the numeric value input
        }

    } else {
        input->generalized_sourceType = 1;
    }

    // if we are solving for steady-state, we need a value for the coefficient b
    // We do this because in the TLM formalism we need the coefficient b to calculate
    // the impedance of the transmission line.
    if (Sol == STEADY && input->generalized_coefficient_b == 0) {
        input->generalized_coefficient_b = 1;
    }

    return errorFound;
}