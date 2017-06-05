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
 * File:   libsolver.c
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
#include <stdlib.h>
#include <time.h>

#include "libsolver.h"

#include "tlmsolver/libtlmsolver.h"

/*
 * solve: redirects the flow of the algorithm so that the required method is used
 * for solving the problem
 */
unsigned int solve(struct dataForSimulation *input) {

    unsigned int errorTLMnumber = 0;

    void **generalMatrices;
    generalMatrices = malloc(input->quantityOfEquationsRead * sizeof (void*));

    int i;
    clock_t begin = clock();
    // I will do a loop for each equation to configure them. The equations will
    // be solved after the configuration.
    for (i = 0; i < input->quantityOfEquationsRead; i++) {
        
        printf("\nConfiguring the solver for equation group %04d\n", i + 1);
        
        // Each method above has its one way of solving the problem. Here, I just
        // redirect the flow of the software.
        switch (input->equationInput[i].Meth) {
            case TLM: // transmission-line modeling method
                errorTLMnumber = solverTLM(input, i, &generalMatrices[i]);
                /*
                 * Brief explanation of how the solverTLM solves the equations:
                 * TLM is based in incident (k_V^i) and reflected (k_V^r) voltage pulses.
                 * They are used to move in time and space. For the same time-step k,
                 * we have:
                 * [k_V^r] = [S] * [k_V^i] + [Zhat * I_S]                   (1)
                 * where [S] is the scattering matrix and [Zhat * I_S] are sources
                 * 
                 * moving to the next time-step:
                 * [k+1_V^i] = [C] * [k_V^r] + [B]                          (2)
                 * where [C] is the connection matrix and [B] contains effects
                 * of boundaries and vectorial sources
                 * 
                 * We can manipulate these equations to get
                 * [k+1_V^i] = [M] * [k_V^i] + [E]                          (3)
                 * where [M] = [C][S] and [E] = [C] * [Zhat * I_S] + [B].
                 * 
                 * The relationship  between k_V^i and the temperature at
                 * the center of the node (k_T_c) is:
                 * [k_T_c] = [tau]*[k_V^i] + [Zhat * I_S]                   (4)
                 * where [tau] is the matrix used to convert incident voltages
                 * in temperature.
                 * 
                 * We can also calculate the temperature between two nodes. There
                 * are two ways to calculate it. One uses reflected voltages and
                 * the other uses the temperature at the center of the node. I've
                 * obtained good results using the equation with k_V^r, which is:
                 * [k_T_b] = [beta]*[k_V^r] + [T_B]                         (5)
                 * where [beta] is the matrix used to convert reflected voltages
                 * in temperature and [T_B] represents the effect of boundaries
                 * and vectorial sources on the temperature.
                 * 
                 * Finaly, we can append the equation of [k_T_b] and [k_T_c] to get
                 * one equation for the temperatures:
                 * [k_T] = [T_M]*[k_V^i] + [T_E]                            (6)
                 * where [T_M] = [tau] + [beta]*[S] and [T_E] = [Zhat * I_S] + 
                 * [beta] * [Zhat * I_S] + [B]
                 * 
                 * Lastly, the heat flux [q''] is calculated using
                 * [q''] = [(k_T_b - k_T_c)/( L_2 * R_n)]                   (7)
                 * I didn't expand the above equation but it can be written as
                 * a matrix equation, like
                 * [q''] = [a]*[k_V^i] + [b]                                (8)
                 * 
                 * What I do in this algorithm are: a) find [M] and [E], then
                 * I append all the outputs requested by the user (T_b, T_c, q'')
                 * is one equation and get a [tau] and [E_output] such that
                 * [k_output] = [tau]*[k_V^i] + [E_output]                  (9)
                 * 
                 * At the end, I solve equations (3) and (9), which is what consumes
                 * the most the processing. Getting the matrix doesn't take long.
                 * 
                 * I hope you are ready to get into this function. =)
                 * Go nicely and, if you have any questions, don't hesitate
                 * to contact me.
                 * 
                 * Hugo Milan
                 */
                break;
            case FEM: // finite element method; future implementation
                fprintf(stderr, "WARNING: FEM (Finite Element Method) was not implemented yet.\n");
                break;
            case FDTD: // finite difference time-domain method; future implementation
                fprintf(stderr, "WARNING: FDTD (Finite Difference time-domain method) was not implemented yet.\n");
                break;
        }
    }

    clock_t end = clock();

    if (input->simulationInput.timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("\n\nTime to create the equations (and solve equations directly, if any "
                "equation was solved in this category) %g ms (or %g s, or %g min, or %g hours).\n\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }

    // At this point I have all the matrices calculated and ready to solve.
    // This would be the point that I would do the multiphysics part. How?
    // May be with a function solveTimeStep(); then updateMatrices();
    // solveTimeStep() would solve for only ONE time-step (or the steady-state
    // problem if that's the case). updateMatrices() would update the matrices
    // accordingly to what it calculated.
    //
    // solveNextStep is 1 if we need to solve a next time-step.
    // It is 0 if we don't need.
    //
    // This is only needed if I'm interacting multiphysically. I should put a flag
    // to indicates if I can solve it directly on the codes above and clean after myself
    // or if I indeed need this probably slower part of the code.
    //    int solveNextStep = 1;
    //    while (solveNextStep == 1) {
    //        // First we set it to 0. It will be reset to 1 by one of the solvers
    //        solveNextStep = 0;
    //        for (i = 0; i < input->quantityOfEquationsRead; i++) {
    //            // Each method above has its one way of solving the problem. Here, I just
    //            // redirect the flow of the software.
    //            if (input->equationInput[i].needToSolve == 1) {
    //                switch (input->equationInput[i].Meth) {
    //                    case TLM: // transmission-line modeling method
    //                        //                errorTLMnumber = solveTimeStepTLM(input, i, &generalMatrices[i]);
    //                        break;
    //                    case FEM: // finite element method; future implementation
    //                        fprintf(stderr, "WARNING: FEM (Finite Element Method) was not implemented yet.\n");
    //                        break;
    //                    case FDTD: // finite difference time-domain method; future implementation
    //                        fprintf(stderr, "WARNING: FDTD (Finite Difference time-domain method) was not implemented yet.\n");
    //                        break;
    //                }
    //            }
    //
    //            // errorTLMnumer = 9999 is my code to indicate that we need to solve 
    //            // one more time (next time-step).
    //            if (errorTLMnumber == 9999) {
    //                solveNextStep = 1;
    //                errorTLMnumber = 0;
    //            }
    //        }
    //    }

    return errorTLMnumber;
}