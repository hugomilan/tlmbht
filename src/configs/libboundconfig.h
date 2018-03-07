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
#include "libsimuconfig.h"
#include "libfunctionconfig.h"

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
        
        char *boundaryName;
        int boundaryNameDefined;

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
        char *scalarBoundaryFromFunc; // name of the function that will be used to calculate this value
        int scalarBoundaryType; // 1: double; 2: from function

        double fluxBoundary;
        int fluxBoundaryDefined;
        char *fluxBoundaryFromFunc; // name of the function that will be used to calculate this value
        int fluxBoundaryType; // 1: double; 2: from function

        double convectionScalar;
        int convectionScalarDefined;
        char *convectionScalarFromFunc; // name of the function that will be used to calculate this value
        int convectionScalarType; // 1: double; 2: from function

        double convectionCoefficient;
        int convectionCoefficientDefined;
        int convectionDefined;
        char *convectionCoefficientFromFunc; // name of the function that will be used to calculate this value
        int convectionCoefficientType; // 1: double; 2: from function

        int adiabaticDefined;




        // Inputs associated with heat, hyperbolic heat, pennes, and hyperbolic pennes
        double temperatureBoundary;
        int temperatureDefined;
        char *temperatureBoundaryFromFunc; // name of the function that will be used to calculate this value
        int temperatureBoundaryType; // 1: double; 2: from function

        double heatFluxBoundary;
        int heatFluxDefined;
        char *heatFluxBoundaryFromFunc; // name of the function that will be used to calculate this value
        int heatFluxBoundaryType; // 1: double; 2: from function

        double radiationTemperature;
        int radiationTemperatureDefined;
        char *radiationTemperatureFromFunc; // name of the function that will be used to calculate this value
        int radiationTemperatureType; // 1: double; 2: from function

        double radiationEmissivity;
        int radiationEmissivityDefined;
        int radiationDefined;
        char *radiationEmissivityFromFunc; // name of the function that will be used to calculate this value
        int radiationEmissivityType; // 1: double; 2: from function

        double convectionTemperature;
        int convectionTemperatureDefined;
        char *convectionTemperatureFromFunc; // name of the function that will be used to calculate this value
        int convectionTemperatureType; // 1: double; 2: from function

        // these are the same defined for diffusion and hyperbolic diffusion
        //        double convectionCoefficient;
        //        int convectionCoefficientDefined;
        //        int convectionDefined;
        //        
        //        int adiabaticDefined;
        
        
        
        // general values used to generalize the boundary for the following equations: diffusion,
        // hyperbolic diffusion, heat, hyperbolic heat, pennes, hyperbolic pennes.
        // The generalization is done during the test inputs.


        // generalized inputs
        // int adiabaticDefined; // This is generalized by default
        
        double generalized_scalar;
        int generalized_scalarDefined;
        char *generalized_scalarFromFunc; // name of the function that will be used to calculate this value
        int generalized_scalarType; // 1: double; 2: from function
        int generalized_scalarGetFromFuncNumb; // Number of the function to get the variable from;
        
        double generalized_flux;
        int generalized_fluxDefined;
        char *generalized_fluxFromFunc; // name of the function that will be used to calculate this value
        int generalized_fluxType; // 1: double; 2: from function
        int generalized_fluxGetFromFuncNumb; // Number of the function to get the variable from
        
        double generalized_convectionScalar;
        char *generalized_convectionScalarFromFunc; // name of the function that will be used to calculate this value
        int generalized_convectionScalarType; // 1: double; 2: from function
        int generalized_convectionScalarGetFromFuncNumb; // Number of the function to get the variable from
        double generalized_convectionCoefficient;
        char *generalized_convectionCoefficientFromFunc; // name of the function that will be used to calculate this value
        int generalized_convectionCoefficientType; // 1: double; 2: from function
        int generalized_convectionCoefficientGetFromFuncNumb; // Number of the function to get the variable from
        int generalized_convectionDefined;
        
        double generalized_radiationScalar;
        char *generalized_radiationScalarFromFunc; // name of the function that will be used to calculate this value
        int generalized_radiationScalarType; // 1: double; 2: from function
        int generalized_radiationScalarGetFromFuncNumb; // Number of the function to get the variable from
        double generalized_radiationCoefficient;
        char *generalized_radiationCoefficientFromFunc; // name of the function that will be used to calculate this value
        int generalized_radiationCoefficientType; // 1: double; 2: from function
        int generalized_radiationCoefficientGetFromFuncNumb; // Number of the function to get the variable from
        int generalized_radiationDefined;
    };

    unsigned int initializeBoundaryConfig(struct BoundaryConfig *);

    unsigned int terminateBoundaryConfig(struct BoundaryConfig *);

    unsigned int setConfigurationBoundary(char *, struct BoundaryConfig*, int*);

    unsigned int testInputBoundary(struct BoundaryConfig *, struct Equation *, int,
            struct Simulation*, struct FunctionConfig *, int);
    
    unsigned int generalizeDiffusionBoundary(struct BoundaryConfig *, struct FunctionConfig *, int, int);
    unsigned int generalizeHeatBoundary(struct BoundaryConfig *, struct FunctionConfig *, int, int);
    // unsigned int generalizePennesBoundary(struct BoundaryConfig *, enum solve, struct FunctionConfig *, int, int);
    // types of boundaries for heat transfer and Pennes bioheat transfer are the same. Hence, we need only
    // to test for heat transfer

    void printfBoundConfig(struct BoundaryConfig *, struct Equation *equation);
    void printfNumberOfInputsBound(int **, int *);
    void printfAdiabatic(void);
    void printfScalarBoundary(double, char*, int);
    void printfFluxBoundary(double, char*, int);
    void printfConvection(double, double, char*, char*, int, int);
    void printfTemperature(double, char*, int);
    void printfHeatFlux(double, char*, int);
    void printfConvectionHeat(double, double, char*, char*, int, int);
    void printfRadiation(double, double, char*, char*, int, int);


#ifdef __cplusplus
}
#endif

#endif // LIBBOUNDCONFIG_H_INCLUDED
