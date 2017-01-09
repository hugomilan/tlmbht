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

#include "libsimuconfig.h"

    struct MaterialConfig // structure that contains the information for the material input
    {
        // data for the quantity of materials
        int *numberInput;
        int quantityOfNumberInput;
        // flags
        int numberInputDefined;

        // data for the material itself. Used in Pennes and Heat
        double matDensity;
        double matSpecificHeat;
        double matThermalConductivity;
        double initialTemperature;
        // flags
        int matDensityDefined;
        int matSpecificHeatDefined;
        int matThermalConductivityDefined;
        int initialTemperatureDefined;

        // data for the blood inside the material. Used in Pennes
        double bloodPerfusion;
        double bloodDensity;
        double bloodSpecificHeat;
        double bloodTemperature;

        // data for the internal heat generation. Used in Pennes and Heat
        double internalHeatGeneration;



    };


    unsigned int intializeMaterialConfig(struct MaterialConfig *);

    unsigned int terminateMaterialConfig(struct MaterialConfig *);

    unsigned int setConfigurationMaterial(char *, struct MaterialConfig *, int *);

    unsigned int testInputMaterial(struct MaterialConfig *, enum typeSim *, enum solve*, int);

    void printfMatConfig(struct MaterialConfig *, enum typeSim *, enum solve *);
    void printfNumberOfInputsMat(int **, int *);
    void printfMatDensity(double *);
    void printfMatSpecificHeat(double *);
    void printfMatThermalConductivity(double *);
    void printfInitialTemperature(double *);
    void printfBloodPerfusion(double *);
    void printfBloodDensity(double *);
    void printfBloodSpecificHeat(double *);
    void printfBloodTemperature(double *);
    void printfInternalHeatGeneration(double *);



#ifdef __cplusplus
}
#endif

#endif // LIBMATCONFIG_H_INCLUDED
