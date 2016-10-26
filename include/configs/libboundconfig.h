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
 * File:   libboundconfig.h
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
 * the boundary conditions. See libboundcnofig.c for more details.
 *
 */
#ifndef LIBBOUNDCONFIG_H_INCLUDED
#define LIBBOUNDCONFIG_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#include "libsimuconfig.h"

    // structure that contains the information for the material input

    struct BoundaryConfig {
        // data for the quantity of materials
        int *numberInput;
        int quantityOfNumberInput;
        // flags
        int numberInputDefined;

        // data for the material itself. Used in Pennes and Heat
        double temperatureBoundary;
        // flags
        int temperatureDefined;

        double heatFluxBoundary;
        // flags
        int heatFluxDefined;

        double ConvectionTemperature;
        double ConvectionCoefficient;
        //flags
        int convectionDefined;
        int convectionTemperatureDefined;
        int convectionCoefficientDefined;

        double RadiationTemperature;
        double RadiationEmissivity;
        // flags
        int radiationDefined;
        int radiationTemperatureDefined;
        int radiationEmissivityDefined;

        // flags for Pennes and Heat
        int adiabaticDefined;
    };

    unsigned int initializeBoundaryConfig(struct BoundaryConfig *);

    unsigned int terminateBoundaryConfig(struct BoundaryConfig *);

    unsigned int setConfigurationBoundary(char *, struct BoundaryConfig*, int*);

    unsigned int testInputBoundary(struct BoundaryConfig *, enum typeSim *, int);

    void printfBoundConfig(struct BoundaryConfig *, enum typeSim*);
    void printfNumberOfInputsBound(int **, int *);
    void printfAdiabatic(void);
    void printfTemperature(double);
    void printfHeatFlux(double);
    void printfConvection(double, double);
    void printfRadiation(double, double);


#ifdef __cplusplus
}
#endif

#endif // LIBBOUNDCONFIG_H_INCLUDED
