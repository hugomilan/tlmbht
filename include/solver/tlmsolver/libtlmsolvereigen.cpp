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
 * File:   libtlmsolvereigen.cpp
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
 * redirects the flow of the algorithm for choosing the adequate TLM solver.
 * These functions are based on the Eigen library.
 *
 */

#include "libtlmsolvereigen.h"
#include <iostream>
#include <stdexcept>
#include <omp.h>

/*
 * initiate_matrices_calculationTLMEigen: allocate the memory for the matrices
 * used in the calculation process
 */
unsigned int initiate_matrices_calculationTLMEigen(struct calculationTLMEigen *matrices,
        unsigned int *quantityToReserve) {

    matrices->M.resize(matrices->numbers.Ports, matrices->numbers.Ports);
    // this means that each column has reserved 'n' spaces
    matrices->M.reserve(VectorXi::Constant(matrices->numbers.Ports, quantityToReserve[0]));


    matrices->E.resize(matrices->numbers.Ports);

    // allocating for all the resistances
    if (((*matrices).R = (double*) malloc(sizeof (double)*matrices->numbers.Ports)) == NULL) {
        return 8718;
    }

    // allocating for all the impedances
    if (((*matrices).Z = (double*) malloc(sizeof (double)*matrices->numbers.Ports)) == NULL) {
        return 8717;
    }

    matrices->Vi.resize(matrices->numbers.Ports);

    // allocating the characteristic length vector
    if (((*matrices).L = (double*) malloc(sizeof (double)*matrices->numbers.Ports)) == NULL) {
        return 8725;
    }

    // allocating the port's lengths
    if (((*matrices).deltal = (double*) malloc(sizeof (double)*matrices->numbers.Ports)) == NULL) {
        return 8726;
    }

    
    matrices->tau.resize(matrices->numbers.Output, matrices->numbers.Ports);
    // this means that each line has reserved 'm' spaces
    matrices->tau.reserve(VectorXi::Constant(matrices->numbers.Ports, quantityToReserve[1]));
    matrices->output.resize(matrices->numbers.Output);

    matrices->E_output.resize(matrices->numbers.Output);
    
    // allocating the port's lengths
    if (((*matrices).Points_output = (struct Point*)
            malloc(sizeof (struct Point)*matrices->numbers.Points_Output)) == NULL) {
        return 8739;
    }

    return 0;
}



/*
 * terminate_matrices_calculationTLMEigen: deallocate the matrices used in the
 * calculation process
 */
unsigned int terminate_matrices_calculationTLMEigen(struct calculationTLMEigen *matrices) {
    // deallocating matrix M and tau columns
    matrices->M.resize(0, 0);
    matrices->tau.resize(0, 0);
    matrices->identity.resize(0, 0);
    matrices->Msteady.resize(0, 0); 

    matrices->E.resize(0);

    free((*matrices).R);
    (*matrices).R = NULL;

    free((*matrices).Z);
    (*matrices).Z = NULL;

    matrices->Vi.resize(0);

    free((*matrices).L);
    (*matrices).L = NULL;

    free((*matrices).deltal);
    (*matrices).deltal = NULL;

    matrices->output.resize(0);

    matrices->E_output.resize(0);
    
    free((*matrices).Points_output);
    (*matrices).Points_output = NULL;

    terminateTLMnumbers(&(matrices->numbers));


    return 0;
}


/*
 * tlmInitializeSteadyMatrices: Initiate the matrices used to solve steady-state
 * problems in the TLM method
 */
unsigned int tlmInitializeSteadyMatrices(struct calculationTLMEigen *matrices) {

    matrices->identity.resize(matrices->numbers.Ports, matrices->numbers.Ports);
    matrices->M.reserve(VectorXi::Constant(matrices->numbers.Ports, 1));
    for (long long unsigned int i = 0; i < matrices->numbers.Ports; i++)
        matrices->identity.insert(i,i) = 1;
    matrices->identity.makeCompressed();
        
    matrices->Msteady.resize(matrices->numbers.Ports, matrices->numbers.Ports);
    matrices->Msteady = matrices->identity - matrices->M;
    matrices->Msteady.makeCompressed();
    

    return 0;
}
/*
 * terminateSome_calculationTLMEigen: deallocate some matrices used in the
 * calculation process
 */
unsigned int terminateSome_calculationTLMEigen(struct calculationTLMEigen * matrices){
    
    // deallocating matrix M and tau columns
    free((*matrices).R);
    (*matrices).R = NULL;

    free((*matrices).Z);
    (*matrices).Z = NULL;

    free((*matrices).L);
    (*matrices).L = NULL;

    free((*matrices).deltal);
    (*matrices).deltal = NULL;
    
    free((*matrices).Points_output);
    (*matrices).Points_output = NULL;

    terminateTLMnumbers(&(matrices->numbers));
    
    return 0;
}


/*
 * terminateSomeVariablesEigen: deallocate some variables used in the
 * calculation process
 */
unsigned int terminateSomeVariablesEigen(struct dataForSimulation *input,
        struct calculationTLMEigen *matrices,
        struct boundaryData** boundaries, struct connectionLeveln *connection) {
    
    terminateSome_calculationTLMEigen(matrices);
    
    terminateBoundaryTypeAndData(boundaries, input);

    terminate_connectionLeveln(connection);
    
    
    return 0;
}


/*
 * tlmSolveMatricesTimeDomainEigen: solve the TLM matrix equation in time-domain and write
 * data to file
 */
unsigned int tlmSolveMatricesTimeDomainEigen(struct calculationTLMEigen * matrices,
        const struct dataForSimulation * input, FILE *file) {
    
    if (input->timingMode == 1) {

        printf("\n\nSolving matrices... \n");
    }
    
    long long unsigned int quantityOfIterations, position = 1;
    double time;
    
    clock_t begin, end, begin_Vi, end_Vi, begin_T, end_T, begin_write, end_write;
    double time_Vi, time_T, time_write;
    time_Vi = 0; time_T = 0; time_write = 0;

    quantityOfIterations = input->simulationInput.finalTime / (
            input->simulationInput.timeStep *
            input->simulationInput.timeJump);
    
//    clock_t begin_JM, end_JM;
//    begin_JM = clock();
    //    if (input->simulationInput.timeJump != 1)
    //        adjustTimeJumpEigen(matrices, input->simulationInput.timeJump);
    //    end_JM = clock();
    //        double time_JM = (double) (end_JM - begin_JM) / CLOCKS_PER_SEC;

    begin = clock();
    for (long long unsigned int i = 0; i < quantityOfIterations; i++) {
        time = (i) *
                    input->simulationInput.timeStep *
                    input->simulationInput.timeJump +
                    (1) * 
                    input->simulationInput.timeStep;

            // DEBUG: show what time we are
            printf("Calculating for time %g s to ", time);
            time = (i) *
                    input->simulationInput.timeStep *
                    input->simulationInput.timeJump +
                    (input->simulationInput.timeJump) * 
                    input->simulationInput.timeStep;
            printf("%g s... ", time);
            
        for (long long unsigned int j = 0; j < input->simulationInput.timeJump; j++) {
            
            // Calculate Vi_(k+1)
            begin_Vi = clock();
            matrices->Vi = matrices->M * matrices->Vi + matrices->E;
            end_Vi = clock();
            time_Vi += (double) (end_Vi - begin_Vi) / CLOCKS_PER_SEC;
        }
        
        
        // calculate the output at k+1
        printf("Calculating the output... ");
        begin_T = clock();
        matrices->output = matrices->tau * matrices->Vi + matrices->E_output;
        end_T = clock();
        time_T += (double) (end_T - begin_T) / CLOCKS_PER_SEC;
        
        
        printf("Writing data to file... ");
        begin_write = clock();
        writePartialOutputEigen(matrices, position, file);
        end_write = clock();
        time_write += (double) (end_write - begin_write) / CLOCKS_PER_SEC;
        printf("Done\n\n");
        position++;
    }
    end = clock();
    
    if (input->timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

        printf("\n\nSolving the matrices:\n");
        
//        if (input->simulationInput.timeJump != 1)
//            printf("Time to calculate the matrix adjusted %d time jumps %g ms (or %g s, or %g min, or %g hours).\n",
//                input->simulationInput.timeJump, time_JM * 1e3, time_JM, time_JM / 60.0, time_JM / (60 * 60));
        
        printf("Time to calculate the incident voltages %g ms (or %g s, or %g min, or %g hours).\n",
                time_Vi * 1e3, time_Vi, time_Vi / 60.0, time_Vi / (60 * 60));
        
        printf("Time to calculate the outputs %g ms (or %g s, or %g min, or %g hours).\n",
                time_T * 1e3, time_T, time_T / 60.0, time_T / (60 * 60));
        
        printf("Time to write the outputs %g ms (or %g s, or %g min, or %g hours).\n",
                time_write * 1e3, time_write, time_write / 60.0, time_write / (60 * 60));
        
        printf("Total time to solve the matrices %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }

    return 0;

}




/*
 * tlmSolveMatricesSteadyStateEigen: solve the TLM matrix equation for steady-state
 * and write data to file
 */
unsigned int tlmSolveMatricesSteadyStateEigen(struct calculationTLMEigen * matrices,
        const struct dataForSimulation * input, FILE *file) {
    
    long long unsigned int position = 1;
    SparseLU< SparseMatrix<double, ColMajor, long long>, COLAMDOrdering<long long> > solver;
    
    clock_t begin, end, begin_Vi, end_Vi, begin_T, end_T, begin_write, end_write;
    double time_Vi, time_T, time_write;
    time_Vi = 0; time_T = 0; time_write = 0;
    
//    clock_t begin_JM, end_JM;
//    begin_JM = clock();
    //    if (input->simulationInput.timeJump != 1)
    //        adjustTimeJumpEigen(matrices, input->simulationInput.timeJump);
    //    end_JM = clock();
    //        double time_JM = (double) (end_JM - begin_JM) / CLOCKS_PER_SEC;
    
    if (input->timingMode == 1) {

        printf("\n\nSolving matrices...\n");
    }

    begin = clock();
    printf("Solving Vi using LU...\n");
    // Calculate Vi_(steady)
    begin_Vi = clock();
    solver.analyzePattern(matrices->Msteady);
    solver.factorize(matrices->Msteady);
    // if(solver.info() != Success) // decomposition failed
    matrices->Vi = solver.solve(matrices->E);
    // if(solver.info() != Success) // solver failed
    end_Vi = clock();
    time_Vi += (double) (end_Vi - begin_Vi) / CLOCKS_PER_SEC;
        
        
        // calculate the output at k+1
        printf("Calculating the output...\n");
        begin_T = clock();
        matrices->output = matrices->tau * matrices->Vi + matrices->E_output;
        end_T = clock();
        time_T += (double) (end_T - begin_T) / CLOCKS_PER_SEC;
        
        
        printf("Writing data to file...\n");
        begin_write = clock();
        writePartialOutputEigen(matrices, position, file);
        end_write = clock();
        time_write += (double) (end_write - begin_write) / CLOCKS_PER_SEC;
        printf("Done\n");
        position++;
    
    end = clock();
    
    if (input->timingMode == 1) {
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        
//        if (input->simulationInput.timeJump != 1)
//            printf("Time to calculate the matrix adjusted %d time jumps %g ms (or %g s, or %g min, or %g hours).\n",
//                input->simulationInput.timeJump, time_JM * 1e3, time_JM, time_JM / 60.0, time_JM / (60 * 60));
        
        printf("Time to calculate the incident voltages %g ms (or %g s, or %g min, or %g hours).\n",
                time_Vi * 1e3, time_Vi, time_Vi / 60.0, time_Vi / (60 * 60));
        
        printf("Time to calculate the outputs %g ms (or %g s, or %g min, or %g hours).\n",
                time_T * 1e3, time_T, time_T / 60.0, time_T / (60 * 60));
        
        printf("Time to write the outputs %g ms (or %g s, or %g min, or %g hours).\n",
                time_write * 1e3, time_write, time_write / 60.0, time_write / (60 * 60));
        
        printf("Total time to solve the matrices %g ms (or %g s, or %g min, or %g hours).\n",
                time_spent * 1e3, time_spent, time_spent / 60.0, time_spent / (60 * 60));
    }

    return 0;

}



/*
 * writePartialOutputEigen: writes the output to the file
 */
unsigned int writePartialOutputEigen(struct calculationTLMEigen * matrices,
        long long unsigned int position, FILE* file){
    
    long long unsigned int i;
    
    // I'm saving in a format to be read using matlab/octave
    fprintf(file, "\n");
    fprintf(file, "output(:,%llu) = [", position);
    
    
    for (i = 0; i < matrices->numbers.Output; i++){
        fprintf(file, "\n%.17g;", matrices->output(i));
    }
        
        
    fprintf(file, "];");
    fprintf(file, "\n");
            
    return 0;
}

/*
 * writeFirstDataToFileEigen: Intended to be the first function to write something
 * to the file. This is likely going to include configuration and stuffs. Right now
 * it only prints the output points
 */
unsigned int writeFirstDataToFileEigen(struct calculationTLMEigen * matrices,
        struct dataForSimulation *input, FILE* file){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    //printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    // think about saving the configuration files and loading
    
    long long unsigned int i;
    
    fprintf(file, "save = [%d, %d, %d];\n", input->simulationInput.saveTemperature,
            input->simulationInput.saveTemperatureBetween,
            input->simulationInput.saveHeatFlux);
    
    fprintf(file, "numbers = [%llu, %llu, %llu, %llu, %llu];\n", matrices->numbers.Nodes,
            matrices->numbers.Ports, matrices->numbers.Intersections, matrices->numbers.Output,
            matrices->numbers.Points_Output);
    fprintf(file, "times = [%g %g %g %u %llu];\n",
            input->simulationInput.timeStep * input->simulationInput.timeJump,
            input->simulationInput.finalTime, input->simulationInput.timeStep,
            input->simulationInput.timeJump,
            (long long unsigned int) (input->simulationInput.finalTime / (
            input->simulationInput.timeStep * input->simulationInput.timeJump))  );
    
        
    // I'm saving in a format to be read using matlab/octave
    fprintf(file, "Points_Output = [");
    
    for (i = 0; i < matrices->numbers.Output; i++)
        fprintf(file, "\n%.17g, %.17g, %.17g", matrices->Points_output[i].x,
                matrices->Points_output[i].y, matrices->Points_output[i].z);
        
    fprintf(file, "];\n");
    
            
    return 0;
}



/*
 * adjustTimeJumpEigen: used to make a previous multiplication of the matrices M and E.
 * Tests showed that it is faster to run the algorithm without this previous multiplication
 * for the sparse matrix. Therefore, this function is deprecated. It is not used.
 */
unsigned int adjustTimeJumpEigen(struct calculationTLMEigen *matrices, unsigned int jump){
    /*unsigned int i;
    matrices->M.uncompress();
    
    // creating the matrix that will multiply E
    SparseMatrix<double, RowMajor, long long> sum_for_E(matrices->numbers.Ports,
            matrices->numbers.Ports);
    SparseMatrix<double, RowMajor, long long> mult_for_M(matrices->numbers.Ports,
            matrices->numbers.Ports);    
    printf("Initiating the matrices for adjust the time jump \n");
    sum_for_E = matrices->M;
    mult_for_M = matrices->M;
    
    // jump 1 to jump - 1
    printf("Sparsity %g (%llu)\n", (double) matrices->M.nonZeros()/
                (matrices->numbers.Ports*matrices->numbers.Ports),
            matrices->M.nonZeros());
    for (i = 1; i < jump - 1; i++){
        printf("Calculating jump %u/%u\n", (i+1), jump);
        sum_for_E = sum_for_E + matrices->M;
        
        matrices->M = matrices->M*mult_for_M;
        printf("Sparsity %g (%llu)\n", (double) matrices->M.nonZeros()/
                (matrices->numbers.Ports*matrices->numbers.Ports),
                matrices->M.nonZeros());
    }
    printf("Calculating jump %u/%u\n", (i+1), jump);
    
    // last jump
    matrices->E = sum_for_E*matrices->E + matrices->E;
    matrices->M = matrices->M*mult_for_M;
    
    printf("Sparsity %g (%llu)\n", (double) matrices->M.nonZeros()/
                (matrices->numbers.Ports*matrices->numbers.Ports),
            matrices->M.nonZeros());
    
    matrices->M.makeCompressed();
    
    sum_for_E.resize(0,0);
    */
    return 0;
}


/*
 * terminateVariablesTLMEigen: deallocate in memory the variables used
 * for the TLM simulation
 */
unsigned int terminateVariablesTLMEigen(struct dataForSimulation *input, struct calculationTLMEigen *matrices,
        struct boundaryData** boundaries, struct connectionLeveln *intersections,
        FILE ** file) {

    terminate_matrices_calculationTLMEigen(matrices);

    terminateBoundaryTypeAndData(boundaries, input);

    terminate_connectionLeveln(intersections);

    if (fclose(*file) == EOF) {
        return 345;
    }

    return 0;
}