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

#include "libequationconfig.h"

    // structure that contains the information for the material input

    struct BoundaryConfig {
        char *equationName;
        // name of the equation associated with this material
        int equationNameDefined;

        int equationNumber;
        // number of the equation. Used to call the right equation from the
        // libequationconfig->Equation.
        // This is an integer because -1 (initial value) indicates that we did
        // not find a match of the Equations names and the name given herein.

        enum typeSim typeOfEquation;
        // type of simulation

        // data for the quantity of materials
        int *numberInput;
        int quantityOfNumberInput;
        // flags
        int numberInputDefined;

        // Inputs associated with diffusion and hyperbolic diffusion
        double scalarBoundary;
        int scalarBoundaryDefined;

        double fluxBoundary;
        int fluxBoundaryDefined;

        double convectionScalar;
        int convectionScalarDefined;

        double convectionCoefficient;
        int convectionCoefficientDefined;
        int convectionDefined;

        int adiabaticDefined;




        // Inputs associated with heat, hyperbolic heat, pennes, and hyperbolic pennes
        double temperatureBoundary;
        int temperatureDefined;

        double heatFluxBoundary;
        int heatFluxDefined;

        double radiationTemperature;
        int radiationTemperatureDefined;

        double radiationEmissivity;
        int radiationEmissivityDefined;
        int radiationDefined;

        double convectionTemperature;
        int convectionTemperatureDefined;

        // these are the same defined for diffusion and hyperbolic diffusion
        //        double convectionCoefficient;
        //        int convectionCoefficientDefined;
        //        int convectionDefined;
        //        
        //        int adiabaticDefined;
    };

    unsigned int initializeBoundaryConfig(struct BoundaryConfig *);

    unsigned int terminateBoundaryConfig(struct BoundaryConfig *);

    unsigned int setConfigurationBoundary(char *, struct BoundaryConfig*, int*);

    unsigned int testInputBoundary(struct BoundaryConfig *, struct Equation *, int);

    void printfBoundConfig(struct BoundaryConfig *, struct Equation *equation);
    void printfNumberOfInputsBound(int **, int *);
    void printfAdiabatic(void);
    void printfScalarBoundary(double);
    void printfFluxBoundary(double);
    void printfConvection(double, double);
    void printfTemperature(double);
    void printfHeatFlux(double);
    void printfConvectionHeat(double, double);
    void printfRadiation(double, double);


#ifdef __cplusplus
}
#endif

#endif // LIBBOUNDCONFIG_H_INCLUDED
