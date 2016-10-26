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
 * File:   libinterfaceceigen.cpp
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 9, 2016.
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
 * Contains the functions that make the interface between the C functions and the
 * C++ functions of Eigen.
 *
 */

#include "libinterfaceceigen.h"
#include "tlmsolver/pennes/libtlmpennes2dsolvereigen.h"
#include "tlmsolver/pennes/libtlmpennes3dsolvereigen.h"

// functions to be called from a C environment
extern "C" {

    /*
     * call_from_c_solverTLMPennes2DDynamicEigen: C interface for calling the
     * C++ function solverTLMPennes2DEigen.
     */
    unsigned int call_from_c_solverTLMPennes2DEigen(struct dataForSimulation* input) {
        return solverTLMPennes2DEigen(input);
    }
    
    
    /*
     * call_from_c_solverTLMPennes3DEigen: C interface for calling the
     * C++ function solverTLMPennes3DEigen.
     */
    unsigned int call_from_c_solverTLMPennes3DEigen(struct dataForSimulation* input) {
        return solverTLMPennes3DEigen(input);
    }
    
    
    /*
     * call_from_c_initiate_matrices_calculationTLMEigen: C interface for calling
     * initiate_matrices_calculationTLMEigen()
     */
    unsigned int call_from_c_initiate_matrices_calculationTLMEigen(void * matrices,
            unsigned int* quantityToReserve){
        
        return (initiate_matrices_calculationTLMEigen((struct calculationTLMEigen*) matrices,
                quantityToReserve));
    }

}