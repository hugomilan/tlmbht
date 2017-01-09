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
 * File:   libsolvercalls.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 8, 2016.
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
 * redirects the flow of the algorithm to the appropriated solver
 *
 */

#include "libsolvercalls.h"


#include "../configs/libconfig.h"

/*
 * solve: redirects the flow of the algorithm so that the required method is used
 * for solving the problem
 */
unsigned int solve(struct dataForSimulation *input) {
    clock_t begin = clock();
    unsigned int errorTLMnumber = 0;

    // Each method above has its one way of solving the problem. Here, I just
    // redirect the flow of the software.
    switch (input->simulationInput.Meth) {
        case TLM: // transmission-line modeling method
            errorTLMnumber = solverTLM(input);
            break;
        case FEM: // finite element method; future implementation
            fprintf(stderr, "WARNING: FEM (Finite Element Method) was not implemented yet.\n");
            break;
        case FDTD: // finite difference time-domain method; future implementation
            fprintf(stderr, "WARNING: FDTD (Finite Difference time-domain method) was not implemented yet.\n");
            break;
        case ANALYTICAL: // analytical solver for specific problems; future implementation
            fprintf(stderr, "WARNING: Analytical solver was not implemented yet.\n");
            break;
    }

    clock_t end = clock();

    if (input->timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("\n\nTotal solver time %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }

    return errorTLMnumber;
}

/*
 * solverTLM: redirects the flow of the algorithm to the adequate function call
 */
unsigned int solverTLM(struct dataForSimulation* input) {
    unsigned int errorTLMnumber = 0;

    switch (input->simulationInput.dimen) {
        case ONE: // future implementation
            switch (input->simulationInput.typeS) {
                case PENNES:
                    /* FALLTHRU */
                case HEAT: // the solver for heat and pennes is the same
                    break;
                case EM:
                    break;
                case CFD:
                    break;
            }
            break;
        case TWO:
            switch (input->simulationInput.typeS) {
                case PENNES:
                    /* FALLTHRU */
                case HEAT: // the solver for heat and pennes is the same
                    switch (input->simulationInput.libraryForCalculation) {
                        case FOOL:
                            printf("Non-implemented. Please, use 'library = eigen;'\n");
                            break;
                        case EIGEN:
                            errorTLMnumber = call_from_c_solverTLMPennes2DEigen(input);
                            break;
                        case CUDA:
                            break;
                    }


                    break;
                case EM:
                    break; // future implementation
                case CFD:
                    break; // future implementation
            }
            break;
        case THREE:// future implementation
            switch (input->simulationInput.typeS) {
                case PENNES:
                    /* FALLTHRU */
                case HEAT: // the solver for heat and pennes is the same
                    switch (input->simulationInput.libraryForCalculation) {
                        case FOOL:
                            printf("Non-implemented. Please, use 'library = eigen;'\n");
                            break;
                        case EIGEN:
                            errorTLMnumber = call_from_c_solverTLMPennes3DEigen(input);
                            break;
                        case CUDA:
                            break;
                    }
                    break;
                case EM:
                    break;
                case CFD:
                    break;
            }
            break;
    }

    return errorTLMnumber;
}