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
 * C++ functions of Eigen. It is essential because the header cannot contain C++ code.
 *
 */

#include "libinterfaceceigen.h"
#include "tlmsolver/pennes/libtlmpenneseigen.h"

// functions to be called from a C environment
extern "C" {

    /*
     * call_from_c_solverEigenTLM: redirects the flow of the algorithm to the adequate function call
     */
    unsigned int call_from_c_solverEigenTLM(struct dataForSimulation* input, int id, void** generalMatrix) {

        if (input->simulationInput.printAdditionalMode == 1) {
            printf("Eigen library\n");
        }


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
                return solverTLMPennesEigenGeneral(input, id, generalMatrix);
                break;

            case EM:
                break;
            case CFD:
                break;
        }
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