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
 * File:   libequationconfig.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on January 12, 2017.
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
 * the governing equations. See libequationconfig.c for more details.
 *
 */

#ifndef LIBEQUATIONCONFIG_H_INCLUDED
#define LIBEQUATIONCONFIG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

    enum typeSim // defines the type of simulation
    {
        DIFFUSION, // solves the diffusion equation.
        HYPERBOLIC_DIFFUSION, // solves the hyperbolic diffusion equation
        HEAT, // solves the heat transfer equation.
        HYPERBOLIC_HEAT, // solves the hyperbolic heat transfer equation.
        PENNES, // solves the Pennes' equation
        HYPERBOLIC_PENNES, // solves the hyperbolic Pennes' equation
        EM, // FUTURE IMPLEMENTATION
        CFD //FUTURE IMPLEMENTATION

    };

    enum libraryForCalc // the library to be used for the calculations
    {
        EIGEN, // solve using Eigen library
        CUDA // solve using CUDA. Future implementation
    };

    enum dimSim // dimension to be solved
    {
        ONE, // solves for 1D problems. future implementation
        TWO, // solves for 2D problems.
        THREE // solves for 3D problems.
    };

    enum method // defines the type of numerical method to be used
    {
        TLM, // solves using the transmission-line modeling method
        FDTD, // solves using the finite-differences time-domain method
        FEM // solves using the finite element method
    };

    enum method2 // defines adjustments for specific methods
    {
        NONE,
        SPECIAL1,
        SPECIAL2,
        SPECIAL3
    };

    enum solve // defines if it should be solved for steady-state or dynamic simulation
    {
        STEADY,
        DYNAMIC
    };

    struct Equation // structure that contains the information for the governing equation
    {
        enum libraryForCalc libraryForCalculation;

        enum typeSim typeS;
        // type of equation to be solved
        int typeSimDefined;

        char *equationName;
        int equationNameDefined;

        int *materialNumbers;
        // this pointer contains the number of the materials that are related to this equation
        int numberOfMaterials;
        // this is the total number of materials related to this equation.

        int *boundaryNumbers;
        // this pointer contains the number of the boundaries that are related to this equation
        int numberOfBoundaries;
        // this is the total number of boundaries related to this equation.

        char *nameScalar;
        // name of scalar: to link with other calls (Material, Boundary, Source, etc.)
        // The default names are:
        //
        //         name scalar      |       type
        //__________________________|_____________________
        //          scalar n        |       diffusion
        //          scalar n        | hyperbolic diffusion
        //          temperature n   |       heat
        //          temperature n   |   hyperbolic heat
        //          temperature n   |       pennes
        //          temperature n   |   hyperbolic pennes
        //
        // where 'n' is an automatic and unique number given by the software

        char *nameVector;
        // name of scalar: to link with other calls (Material, Boundary, Source, etc.)
        // The default names are:
        //
        //         name vector      |       type
        //__________________________|_____________________
        //          flux n          |       diffusion
        //       heat flux n        |       heat
        //       heat flux n        |   hyperbolic heat
        //       heat flux n        |       pennes
        //       heat flux n        |   hyperbolic pennes
        //
        // where 'n' is an automatic and unique number given by the software

        enum dimSim dimen;
        int dimSimDefined;

        enum method Meth;
        enum method2 Meth2;

        enum solve Solv;
        int solveDefined;
        // 1: steady-state simulation
        // 2: dynamic simulation

        double timeStep;
        int timeStepDefined; // only required if dynamic simulation

        unsigned int timeJump;
        double finalTime;
        int finalTimeDefined; // only required if dynamic simulation

        // flags to what to save
        int saveScalar;
        int saveScalarBetween;
        int saveFlux;


        // internal flags that dictates if this equation can be solved directly,
        // or if it needs to be solved iteratively. Directly means that as soon
        // as all the parameters from this equation are calculated, it can be solved.
        // Iteratively means that this equation needs to be solved in conjunction
        // with other equations (that would be a multiphysics problem).
        // The directly procedure is more efficient than the iteratively procedure.
        //
        // 1: can be solved directly (default).
        // 2: needs to be solved interactively. Future implementation for Multiphysics problems
        int solveDirectly;

        // internal flags that says if this equation needs to be solved.
        // 
        // 0: do not need to be solved (was solved already or is not being solved).
        // 1: needs to be solved
        int needToSolve;

    };

    // function used to initialized the Simulation variable
    unsigned int initiateEquationVariable(struct Equation*);

    // function used to terminate the Equation variable
    unsigned int terminateEquationConfig(struct Equation*);


    // take the input text and extract the values of the variables. Returns 0 if no error.
    unsigned int setConfigurationEquation(char *, struct Equation *, int *);

    // tests if the inputs are correct
    unsigned int testInputEquation(struct Equation *, int);

    // function used to call the prints to print all the information from the Equation
    void printfAllEquaData(struct Equation*);

    void printfMaterialNumbers(int *, int *);
    void printfBoundaryNumbers(int *, int *);

    void printfLibrary(enum libraryForCalc*);

    void printWhatToSave(struct Equation*);
    void printfTypeS(enum typeSim *);
    void printfDimen(enum dimSim *);
    void printfMeth(enum method *);
    //void printfMeth(enum method2 *); // future implementation
    void printfSolv(enum solve *);
    void printHowToSolve(int);



#ifdef __cplusplus
}
#endif

#endif /* LIBEQUATIONCONFIG_H */


