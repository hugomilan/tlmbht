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
 * File:   libsolvereigen.cpp
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
 * This file is the one that will include all the Eigen includes. All C++ files
 * that I want to use Eigen will have to include it. Furthermore, this library
 * is purposed to have all function calls that would be common for the solvers
 * that use the Eigen library.
 *
 */

#include "libsolvereigen.h"
#include "tlmsolver/pennes/libtlmpennes2dsolvereigen.h"
#include "tlmsolver/pennes/libtlmpennes3dsolvereigen.h"

// functions to be called from a C environment
extern "C" {

    /*
     * call_from_c_solverEigenTLM: redirects the flow of the algorithm to the adequate function call
     */
    unsigned int call_from_c_solverEigenTLM(struct dataForSimulation* input, int id, void** generalPointer) {

        switch (input->equationInput[id].typeS) {
            case DIFFUSION: // the solver to these equations is the same
                // break;
                /* FALLTHRU */
            case HYPERBOLIC_DIFFUSION: // the solver to these equations is the same
                // break;
                /* FALLTHRU */
            case HEAT: // the solver to these equations is the same
                // break;
                /* FALLTHRU */
            case HYPERBOLIC_HEAT: // the solver to these equations is the same
                // break;
                /* FALLTHRU */
            case PENNES: // the solver to these equations is the same
                // break;
                /* FALLTHRU */
            case HYPERBOLIC_PENNES:
                switch (input->equationInput[id].dimen) {
                    case ONE: // future implementation
                        break;
                    case TWO:
                        break;
                    case THREE:
                        break;
                }
                break;

            case EM:
                break;
            case CFD:
                break;
        }

    }

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
            unsigned int* quantityToReserve) {

        return (initiate_matrices_calculationTLMEigen((struct calculationTLMEigen*) matrices,
                quantityToReserve));
    }

}