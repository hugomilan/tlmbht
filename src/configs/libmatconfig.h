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
 * File:   libmatconfig.h
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
 * contains declaration of functions, structures, and enumerations for configuring
 * the materials. See libmatconfig.c for more details.
 *
 */
#ifndef LIBMATCONFIG_H_INCLUDED
#define LIBMATCONFIG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "libequationconfig.h"

    struct MaterialConfig // structure that contains the information for the material input
    {
        char *equationName;
        // name of the equation associated with this material
        int equationNameDefined;

        int equationNumber;
        // number of the equation. Used to call the right equation from the
        // libequationconfig->Equation.
        // This is an integer because -1 (initial value) indicates that we did
        // not find a match of the Equations names and the name given herein.
        
        char *materialName;
        int materialNameDefined;

        enum typeSim typeOfEquation;
        // type of simulation

        // data for the quantity of materials
        int *numberInput;
        int quantityOfNumberInput;
        // flags
        int numberInputDefined;

        
        
        
        // Inputs associated with diffusion and hyperbolic diffusion
        double diffusionCoeff;
        int diffusionCoeffDefined;
        char *diffusionCoeffFromFunc; // name of the function that will be used to calculate this value
        int diffusionCoeffType; // 1: double; 2: from function

        // the default value of this coefficient is 1.
        double coefficient_b;
        char *coefficient_bFromFunc; // name of the function that will be used to calculate this value
        int coefficient_bType; // 1: double; 2: from function

        double relaxationTime;
        int relaxationTimeDefined;
        char *relaxationTimeFromFunc; // name of the function that will be used to calculate this value
        int relaxationTimeType; // 1: double; 2: from function

        double sink_a;
        char *sink_aFromFunc; // name of the function that will be used to calculate this value
        int sink_aType; // 1: double; 2: from function
        
        double source;
        char *sourceFromFunc; // name of the function that will be used to calculate this value
        int sourceType; // 1: double; 2: from function
        
        double *vectorialSource;
        // one for each position of the vectorial source position
        char **vectorialSourceFromFunc; // name of the function that will be used to calculate this value
        int *vectorialSourceType; // 1: double; 2: from function

        double initialScalar; // initial condition
        int initialScalarDefined; // required if solve = dynamic
        char *initialScalarFromFunc; // name of the function that will be used to calculate this value
        int initialScalarType; // 1: double; 2: from function


        // Inputs associated with heat, hyperbolic heat, pennes, and hyperbolic pennes
        double matDensity;
        int matDensityDefined;
        char *matDensityFromFunc; // name of the function that will be used to calculate this value
        int matDensityType; // 1: double; 2: from function

        double matSpecificHeat;
        int matSpecificHeatDefined;
        char *matSpecificHeatFromFunc; // name of the function that will be used to calculate this value
        int matSpecificHeatType; // 1: double; 2: from function

        double matThermalConductivity;
        int matThermalConductivityDefined;
        char *matThermalConductivityFromFunc; // name of the function that will be used to calculate this value
        int matThermalConductivityType; // 1: double; 2: from function

        // these are the same defined for diffusion and hyperbolic diffusion
        //        double relaxationTime;
        //        int relaxationTimeDefined;
        //        
        //        double sink_a;
        //        double source;
        //        double vectorialSource[3];

        // data for the blood inside the material. Used in Pennes
        double bloodPerfusion;
        char *bloodPerfusionFromFunc; // name of the function that will be used to calculate this value
        int bloodPerfusionType; // 1: double; 2: from function
        
        double bloodDensity;
        char *bloodDensityFromFunc; // name of the function that will be used to calculate this value
        int bloodDensityType; // 1: double; 2: from function
        
        double bloodSpecificHeat;
        char *bloodSpecificHeatFromFunc; // name of the function that will be used to calculate this value
        int bloodSpecificHeatType; // 1: double; 2: from function
        
        double bloodTemperature;
        char *bloodTemperatureFromFunc; // name of the function that will be used to calculate this value
        int bloodTemperatureType; // 1: double; 2: from function
        
        // data for the internal heat generation.
        double internalHeatGeneration;
        char *internalHeatGenerationFunc; // name of the function that will be used to calculate this value
        int internalHeatGenerationType; // 1: double; 2: from function

        double initialTemperature; // initial condition
        int initialTemperatureDefined; // required if solve = dynamic
        char *initialTemperatureFromFunc; // name of the function that will be used to calculate this value
        int initialTemperatureType; // 1: double; 2: from function

        // general values used to generalize the following equations: diffusion,
        // hyperbolic diffusion, heat, hyperbolic heat, pennes, hyperbolic pennes.
        // The generalization is done during the test inputs.
        // b*d( m )/dt = - div( n ) - a*m + R
        // 
        // With the following definition of flux:
        // n + tau*d( n )/dt = f - D*grad( m )
        //
        // The parameters for diffusion and hyperbolic diffusion are changed to:
        // generalized_coefficient_b = coefficient_b;
        // generalized_diffusionCoeff = diffusionCoeff;
        // generalized_relaxationTime = relaxationTime;
        // generalized_vectorialSource = vectorialSource;
        // generalized_initialScalar = initialScalar;
        // generalized_sink_a = sink_a;
        // generalized_source = source;
        //
        //
        // The parameters for heat and hyperbolic heat are changed to:
        // generalized_coefficient_b = matDensity*matSpecificHeat;
        // generalized_diffusionCoeff = matThermalConductivity;
        // generalized_relaxationTime = relaxationTime;
        // generalized_vectorialSource = vectorialSource;
        // generalized_initialScalar = initialTemperature;
        // generalized_sink_a = sink_a;
        // generalized_source = source;
        // 
        // 
        // The parameters for Pennes and hyperbolic Pennes are changed to:
        // generalized_coefficient_b = matDensity*matSpecificHeat;
        // generalized_diffusionCoeff = matThermalConductivity;
        // generalized_relaxationTime = relaxationTime;
        // generalized_vectorialSource = vectorialSource;
        // generalized_initialScalar = initialTemperature;
        // generalized_sink_a = bloodPerfusion*bloodDensity*bloodSpecificHeat;
        // generalized_source = source + internalHeatGeneration + bloodPerfusion*bloodDensity*bloodSpecificHeat*bloodTemperature;
        //


        // generalized inputs
        double generalized_coefficient_b;
        char *generalized_coefficient_bFromFunc; // name of the function that will be used to calculate this value
        int generalized_coefficient_bType; // 1: double; 2: from function
        
        double generalized_diffusionCoeff;
        char *generalized_diffusionCoeffFromFunc; // name of the function that will be used to calculate this value
        int generalized_diffusionCoeffType; // 1: double; 2: from function
        
        double generalized_relaxationTime;
        char *generalized_relaxationTimeFromFunc; // name of the function that will be used to calculate this value
        int generalized_relaxationTimeType; // 1: double; 2: from function
        
        double *generalized_vectorialSource;
        char *generalized_vectorialSourceFromFunc; // name of the function that will be used to calculate this value
        int generalized_vectorialSourceType; // 1: double; 2: from function
        
        double generalized_initialScalar;
        char *generalized_initialScalarFromFunc; // name of the function that will be used to calculate this value
        int generalized_initialScalarType; // 1: double; 2: from function
        
        double generalized_sink_a;
        char *generalized_sink_aFromFunc; // name of the function that will be used to calculate this value
        int generalized_sink_aType; // 1: double; 2: from function
        
        double generalized_source;
        char *generalized_sourceFromFunc; // name of the function that will be used to calculate this value
        int generalized_sourceType; // 1: double; 2: from function
        
        // These variables are flags used in the TLM model. They are set by
        // the code when testing the material's input.
        
        int numberOfStubs;
        // number of stubs of this material. Stub is an extra transmission-line.
        // It is used to model the hyperbolic equations and variables that
        // change with time. For a simple material, the number of stubs is 0.


    };


    unsigned int intializeMaterialConfig(struct MaterialConfig *);

    unsigned int terminateMaterialConfig(struct MaterialConfig *);

    unsigned int setConfigurationMaterial(char *, struct MaterialConfig *, int *);

    unsigned int testInputMaterial(struct MaterialConfig *, struct Equation *, int);

    void generalizeDiffusionInput(struct MaterialConfig *, enum solve);
    void generalizeHeatInput(struct MaterialConfig *, enum solve);
    void generalizePennesInput(struct MaterialConfig *, enum solve);

    void printfMatConfig(struct MaterialConfig *, struct Equation*);
    void printfNumberOfInputsMat(int **, int *);

    void printfRelaxationTime(double *);
    void printfCoefficient_b(double *);
    void printfDiffusionCoeff(double *);
    void printfSink_a(double *);
    void printfSource(double*);
    void printfVectorialSource(double*);
    void printfInitialScalar(double*);

    void printfRelaxationTimeHeat(double *);
    void printfMatDensity(double *);
    void printfMatSpecificHeat(double *);
    void printfMatThermalConductivity(double *);
    void printfInitialTemperature(double *);
    void printfSink_aHeat(double *);
    void printfSourceHeat(double*);
    void printfVectorialSourceHeat(double*);

    void printfBloodPerfusion(double *);
    void printfBloodDensity(double *);
    void printfBloodSpecificHeat(double *);
    void printfBloodTemperature(double *);
    void printfInternalHeatGeneration(double *);



#ifdef __cplusplus
}
#endif

#endif // LIBMATCONFIG_H_INCLUDED
