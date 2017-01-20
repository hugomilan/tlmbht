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

    mat->typeOfEquation = PENNES;

    mat->numberInput = NULL;
    mat->quantityOfNumberInput = 0; // initializing as zero
    mat->numberInputDefined = 0;

    // inputs associated with diffusion and hyperbolic diffusion
    mat->diffusionCoeff = 0;
    mat->diffusionCoeffDefined = 0;

    mat->coefficient_b = 1;

    mat->relaxationTime = 0;
    mat->relaxationTimeDefined = 0;

    mat->sink_a = 0;
    mat->source = 0;
    mat->vectorialSource = (double*) malloc(sizeof (double)*3);
    mat->vectorialSource[0] = 0;
    mat->vectorialSource[1] = 0;
    mat->vectorialSource[2] = 0;

    mat->initialTemperature = 0;
    mat->initialTemperatureDefined = 0;



    // Inputs associated with heat, hyperbolic heat, pennes, and hyperbolic pennes
    mat->matDensity = 0;
    mat->matDensityDefined = 0;

    mat->matSpecificHeat = 0;
    mat->matSpecificHeatDefined = 0;

    mat->matThermalConductivity = 0;
    mat->matThermalConductivityDefined = 0;

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
    mat->bloodDensity = 0;
    mat->bloodSpecificHeat = 0;
    mat->bloodTemperature = 0;

    // data for the internal heat generation. Used in Pennes and Heat
    mat->internalHeatGeneration = 0;

    mat->initialTemperature = 0;
    mat->initialTemperatureDefined = 0;

    mat->generalized_vectorialSource = (double*) malloc(sizeof (double)*3);
    mat->generalized_vectorialSource[0] = 0;
    mat->generalized_vectorialSource[1] = 0;
    mat->generalized_vectorialSource[2] = 0;
    
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

    free(mat->numberInput);
    mat->numberInput = NULL;

    free(mat->vectorialSource);
    mat->vectorialSource = NULL;

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

        sscanf(input, "%lf", &matInput->diffusionCoeff);
        matInput->diffusionCoeffDefined = 1;

    } else if (compareCaseInsensitive(input, "coefficient b") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->coefficient_b);

    } else if (compareCaseInsensitive(input, "relaxation time") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->relaxationTime);
        matInput->relaxationTimeDefined = 1;

    } else if (compareCaseInsensitive(input, "sink a") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->sink_a);

    } else if (compareCaseInsensitive(input, "source") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->source);

    } else if (compareCaseInsensitive(input, "vectorial source") == 0) {
        if ((errorTLMnumber = readVectorDoubleLengthThreeInputs(input, matInput->vectorialSource)) != 0)
            return errorTLMnumber;

    } else if (compareCaseInsensitive(input, "initial scalar") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        sscanf(input, "%lf", &matInput->initialScalar);
        matInput->initialScalarDefined = 1;

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
void printfMatConfig(struct MaterialConfig *input, struct Equation *equation) {

    printfTypeS(&equation->typeS);
    printf("Equation name: %s\n", equation->equationName);
    printf("Equation number: %d\n", input->equationNumber + 1);

    printfNumberOfInputsMat(&input->numberInput, &input->quantityOfNumberInput);

    switch (equation->typeS) {
        case HYPERBOLIC_DIFFUSION:
            printfRelaxationTime(&input->relaxationTime);

            // break;
            /* FALLTHRU */
        case DIFFUSION:

            printfCoefficient_b(&input->coefficient_b);
            printfDiffusionCoeff(&input->diffusionCoeff);
            printfSink_a(&input->sink_a);
            printfSource(&input->source);
            printfVectorialSource(input->vectorialSource);

            if (equation->Solv == DYNAMIC) printfInitialScalar(&input->initialTemperature);

            break;
        case HYPERBOLIC_HEAT:
            printfRelaxationTime(&input->relaxationTime);

            // break;
            /* FALLTHRU */
        case HEAT:
            printfMatDensity(&input->matDensity);
            printfMatSpecificHeat(&input->matSpecificHeat);
            printfMatThermalConductivity(&input->matThermalConductivity);

            printfSink_aHeat(&input->sink_a);
            printfSourceHeat(&input->source);
            printfVectorialSourceHeat(input->vectorialSource);


            if (equation->Solv == DYNAMIC) printfInitialTemperature(&input->initialTemperature);

            break;

        case HYPERBOLIC_PENNES:
            printfRelaxationTime(&input->relaxationTime);

            // break;
            /* FALLTHRU */
        case PENNES:
            printfMatDensity(&input->matDensity);
            printfMatSpecificHeat(&input->matSpecificHeat);
            printfMatThermalConductivity(&input->matThermalConductivity);

            printfSourceHeat(&input->source);
            printfInternalHeatGeneration(&input->internalHeatGeneration);

            printfVectorialSourceHeat(input->vectorialSource);

            printfBloodPerfusion(&input->bloodPerfusion);
            printfBloodDensity(&input->bloodDensity);
            printfBloodSpecificHeat(&input->bloodSpecificHeat);
            printfBloodTemperature(&input->bloodTemperature);


            if (equation->Solv == DYNAMIC) printfInitialTemperature(&input->initialTemperature);

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
void printfRelaxationTime(double *input) {
    printf("Relaxation time of the material: %9.4e\n", *input);
}

/*
 * printfCoefficient_b: prints the coefficient b value
 */
void printfCoefficient_b(double *input) {
    printf("Coefficient b of the material: %9.4e\n", *input);
}

/*
 * printfDiffusionCoeff: prints the diffusion coefficient value
 */
void printfDiffusionCoeff(double *input) {
    printf("Diffusion coefficient of the material: %9.4e\n", *input);
}

/*
 * printfSink_a: prints the sink 'a' value
 */
void printfSink_a(double *input) {
    printf("Sink 'a' value of the material: %9.4e\n", *input);
}

/*
 * printfSource: prints the source value
 */
void printfSource(double *input) {
    printf("Source of the material: %9.4e\n", *input);
}

/*
 * printfVectorialSource: prints the vectorial source value
 */
void printfVectorialSource(double *input) {
    printf("Vectorial source of the material: [%9.4e, %9.4e, %9.4e] \n", input[0], input[1], input[2]);
}

/*
 * printfInitialScalar: prints the initial scalar value
 */
void printfInitialScalar(double * input) {
    printf("Initial scalar value of the material: %9.4e \n", *input);
}

/*
 * printfRelaxationTimeHeat: prints the relaxation time value
 */
void printfRelaxationTimeHeat(double *input) {
    printf("Relaxation time of the material: %9.4e s.\n", *input);
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
 * printfSink_aHeat: prints the sink 'a' value
 */
void printfSink_aHeat(double *input) {
    printf("Sink 'a' value of the material: %9.4e J/(m3-s-K)\n", *input);
}

/*
 * printfSourceHeat: prints the heat source value
 */
void printfSourceHeat(double *input) {
    printf("Heat source of the material: %9.4e W/m3\n", *input);
}

/*
 * printfVectorialSourceHeat: prints the vectorial source value
 */
void printfVectorialSourceHeat(double *input) {
    printf("Vectorial source of the material: [%9.4e, %9.4e, %9.4e] W/m2\n", input[0], input[1], input[2]);
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
        struct Equation * equation, int id) {

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

            generalizeDiffusionInput(input, equation[input->equationNumber].Solv);

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
                generalizePennesInput(input, equation[input->equationNumber].Solv);
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
                generalizeHeatInput(input, equation[input->equationNumber].Solv);
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
void generalizeDiffusionInput(struct MaterialConfig *input, enum solve Sol) {
    input->generalized_coefficient_b = input->coefficient_b;
    input->generalized_diffusionCoeff = input->diffusionCoeff;
    input->generalized_relaxationTime = input->relaxationTime;
    input->generalized_vectorialSource[0] = input->vectorialSource[0];
    input->generalized_vectorialSource[1] = input->vectorialSource[1];
    input->generalized_vectorialSource[2] = input->vectorialSource[2];
    input->generalized_initialScalar = input->initialScalar;
    input->generalized_sink_a = input->sink_a;
    input->generalized_source = input->source;

    // if we are solving for steady-state, we need a value for the coefficient b
    // We do this because in the TLM formalism we need the coefficient b to calculate
    // the impedance of the transmission line.
    if (Sol == STEADY && input->generalized_coefficient_b == 0) {
        input->generalized_coefficient_b = 1;
    }
}

/*
 * generalizeHeatInput: copy the values of the heat input to the
 * generalized variables. I do this so that I can have only one solver for 
 * diffusion, heat, and pennes.
 */
void generalizeHeatInput(struct MaterialConfig *input, enum solve Sol) {
    input->generalized_coefficient_b = input->matDensity * input->matSpecificHeat;
    input->generalized_diffusionCoeff = input->matThermalConductivity;
    input->generalized_relaxationTime = input->relaxationTime;
    input->generalized_vectorialSource[0] = input->vectorialSource[0];
    input->generalized_vectorialSource[1] = input->vectorialSource[1];
    input->generalized_vectorialSource[2] = input->vectorialSource[2];
    input->generalized_initialScalar = input->initialTemperature;
    input->generalized_sink_a = input->sink_a;
    input->generalized_source = input->source;

    // if we are solving for steady-state, we need a value for the coefficient b
    // We do this because in the TLM formalism we need the coefficient b to calculate
    // the impedance of the transmission line.
    if (Sol == STEADY && input->generalized_coefficient_b == 0) {
        input->generalized_coefficient_b = 1;
    }
}

/*
 * generalizePennesInput: copy the values of the pennes input to the
 * generalized variables. I do this so that I can have only one solver for 
 * diffusion, heat, and pennes.
 */
void generalizePennesInput(struct MaterialConfig *input, enum solve Sol) {
    input->generalized_coefficient_b = input->matDensity * input->matSpecificHeat;
    input->generalized_diffusionCoeff = input->matThermalConductivity;
    input->generalized_relaxationTime = input->relaxationTime;
    input->generalized_vectorialSource[0] = input->vectorialSource[0];
    input->generalized_vectorialSource[1] = input->vectorialSource[1];
    input->generalized_vectorialSource[2] = input->vectorialSource[2];
    input->generalized_initialScalar = input->initialTemperature;
    input->generalized_sink_a = input->bloodPerfusion * input->bloodDensity * input->bloodSpecificHeat;
    input->generalized_source = input->source + input->internalHeatGeneration +
            input->bloodPerfusion * input->bloodDensity * input->bloodSpecificHeat * input->bloodTemperature;

    // if we are solving for steady-state, we need a value for the coefficient b
    // We do this because in the TLM formalism we need the coefficient b to calculate
    // the impedance of the transmission line.
    if (Sol == STEADY && input->generalized_coefficient_b == 0) {
        input->generalized_coefficient_b = 1;
    }
}