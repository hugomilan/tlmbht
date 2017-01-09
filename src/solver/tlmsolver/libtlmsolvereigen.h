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
 * File:   libtlmsolvereigen.h
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
 * contains declaration of functions, structures, and enumerations for using the
 * transmission-line modeling method as the solver and by using the functions
 * from the Eigen library. See libtlmsolvereigen.cpp for more details.
 * 
 * Basic functions that do not need to be altered are in libtlmsolver.h
 *
 */

#ifndef LIBTLMSOLVEREIGEN_H
#define LIBTLMSOLVEREIGEN_H


#include "../libsolvereigen.h"
#include "libtlmsolvereigen.h"
#include <vector>
#include <Eigen/Sparse>

#ifdef __cplusplus
extern "C" {
#endif

#include "../../configs/libconfig.h"
#include "../../meshreader/libmeshtlmbht.h"
#include "libtlmsolver.h"

#ifdef __cplusplus
}
#endif

struct calculationTLMEigen {
    // matrix M and tau seem to be better allocated as a sparse matrix
    //SparseMatrix<double, RowMajor, long long> M; // connection * scattering.
    // SparseMatrix<double, RowMajor, long long> tau; // Converts incident voltage to the output variable
    // Change to ColMajor to take advantage of linear solvers. Furthermore, they seem to be slightly faster (~10%)
    SparseMatrix<double, ColMajor, long long> M; // connection * scattering
    SparseMatrix<double, ColMajor, long long> tau; // Converts incident voltage to the output variable
    SparseMatrix<double, ColMajor, long long> identity; // not initialized by default, only if required
    SparseMatrix<double, ColMajor, long long> Msteady; // not initialized by default, only if required
    VectorXd E; // boundaries and external sources: connection * Z_hat * I_source (internal heat generation) + B (boundaries)
    double *Z; // All the impedances
    double *R; // All the resistances
    VectorXd Vi; // incident voltages.
    double *L; // All the characteristic lengths. For triangle: length of the face. For tetrahedron: Area of the face
    double *deltal; // All the ports's lengths
    VectorXd output; // this contains the vector with the output data, the one
    // to be saved in files
    VectorXd E_output; // this contains the vector with the constant to add to the
    struct Point *Points_output;
    // output vector
    struct TLMnumbers numbers;

    // R and Z can be deallocated when no parameters is going to change during the simulation;
    // actually, depending of the case, they can be deallocated and only
    // some of them would be used--this will be resolved in future implementations.
    // Vi1 and Vi2 are used during the time-domain solution
};


unsigned int initiate_matrices_calculationTLMEigen(struct calculationTLMEigen *,
        unsigned int *);
unsigned int terminate_matrices_calculationTLMEigen(struct calculationTLMEigen *);

unsigned int tlmInitializeSteadyMatrices(struct calculationTLMEigen *);

unsigned int terminateVariablesTLMEigen(struct dataForSimulation*,
        struct calculationTLMEigen*, struct boundaryData**, struct connectionLeveln *,
        FILE **);

unsigned int terminateSomeVariablesEigen(struct dataForSimulation *,
        struct calculationTLMEigen *,
        struct boundaryData**, struct connectionLeveln *);

unsigned int tlmSolveMatricesTimeDomainEigen(struct calculationTLMEigen *,
        const struct dataForSimulation *, FILE *);

unsigned int tlmSolveMatricesSteadyStateEigen(struct calculationTLMEigen *,
        const struct dataForSimulation *, FILE *);

unsigned int adjustTimeJumpEigen(struct calculationTLMEigen *, unsigned int);

unsigned int writePartialOutputEigen(struct calculationTLMEigen *,
        long long unsigned int, FILE*);

unsigned int writeFirstDataToFileEigen(struct calculationTLMEigen *,
        struct dataForSimulation *, FILE*);



#endif /* LIBTLMSOLVEREIGEN_H */

