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
 * File:   libvariablename.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on December 30, 2017.
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
 * the variables names. See libvariablename.c for more details.
 *
 */

#ifndef LIBVARIABLENAME_H
#define LIBVARIABLENAME_H

#ifdef __cplusplus
extern "C" {
#endif

    enum configName // Types of configurations
    {
        NOTHING, // it is not inside any specific configuration group
        SIMULATION, // it is inside the SIMULATION configuration group
        MESH, // it is inside the MESH configuration group
        EQUATION, // it is inside the EQUATION configuration group
        MATERIAL, // it is inside the MATERIAL configuration group
        BOUNDARY, // it is inside the BOUNDARY configuration group
        SOURCE, // it is inside the SOURCES configuration group
        FUNCTION // it is inside the FUNCTION configuration group
    };

    struct variableID {
        char *name; // has the name of the variables as given by the user

        enum configName fieldLocation; // what field is that variable in
        char *fieldName; // given name for the field. E.g., the name of the equation "BHE"
        int fieldId; // id of the field. E.g., the number of the equation "BHE" is 0
        int fieldVariableId; // id that says which variable it is on that field.
        /* They are:
         * SIMULATION:
         * 0: AbsoluteZero
         * 1: StefanBoltzmann
         * 2: x
         * 3: y
         * 4: z
         * 
         * MESH:
         * None
         * 
         * EQUATION:
         * 0: time
         * 1: timeStep
         * 2: timeJump
         * 3: finalTime
         * 4: scalar // maps to the scalar name. Scalar at the center of the node
         * 5: flux // maps to the flux name. Absolute flux at the center of the node
         * 6: flux[0] // maps to the flux name. Flux in direction 0 at the center of the node
         * 7: flux[1] // maps to the flux name. Flux in direction 1 at the center of the node
         * 8: flux[2] // maps to the flux name. Flux in direction 2 at the center of the node
         * 
         * MATERIAL:
         * 0: diffusionCoeff
         * 1: coefficient_b
         * 2: relaxationTime
         * 3: sink_a
         * 4: source
         * 5: vectorialSource // absolute value of vectorialSource
         * 6: vectorialSource[0]
         * 7: vectorialSource[1]
         * 8: vectorialSource[2]
         * 9: initialScalar
         * 10: matDensity
         * 11: matSpecificHeat
         * 12: matThermalConductivity
         * 13: bloodPerfusion
         * 14: bloodDensity
         * 15: bloodSpecificHeat
         * 16: bloodTemperature
         * 17: internalHeatGeneration
         * 18: initialTemperature
         * 
         * BOUNDARY:
         * 0: scalarBoundary
         * 1: fluxBoundary
         * 2: convectionScalar
         * 3: convectionCoefficient
         * 4: convectionTemperature
         * 5: temperatureBoundary
         * 6: heatFluxBoundary
         * 7: radiationTemperature
         * 8: radiationEmissivity
         * 
         * SOURCES:
         * TODO
         * 
         * FUNCTION:
         * 0: outputVariable
         */


    };
    
    unsigned int initializeVariableID(struct variableID);
    
    unsigned int terminateVariableID(struct variableID);


#ifdef __cplusplus
}
#endif

#endif /* LIBVARIABLENAME_H */


