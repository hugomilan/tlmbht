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
 * File:   libsimuconfig.h
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
 * the simulation. See libsimuconfig.c for more details.
 *
 */

#ifndef LIBSIMUCONFIG_H_INCLUDED
#define LIBSIMUCONFIG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

    enum typeSim // defines the type of simulation
    {
        PENNES, // solves the Pennes equation
        HEAT, // solves the heat transfer equation. future implementation
        EM, // future implementation
        CFD // future implementation
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
        FDTD, // future implementation
        FEM, // future implementation
        ANALYTICAL // future implementation
    };

    enum method2 // defines adjustments for specific methods
    {
        NONE,
        SPECIAL2,
        SPECIAL3
    };

    enum solve // defines if it should be solved for steady-state or dynamic simulation
    {
        STEADY,
        DYNAMIC
    };

    enum benchmark // defines if it should do benchmark
    {
        DONT,
        RUN,
        RUN_AND_SOLVE,
        RUN_FAST,
        RUN_FAST_AND_SOLVE

    };

    enum libraryForCalc // the library to be used for the calculations
    {
        FOOL, // my fool version of matrix calculation
        EIGEN, // solve using Eigen library
        CUDA // solve using CUDA. Future implementation
    };

    struct Simulation // structure that contains the information for the simulation
    {
        enum typeSim typeS;
        enum dimSim dimen;
        enum method Meth;
        enum method2 Meth2;
        enum solve Solv;
        enum benchmark Benchmark;
        double timeStep;
        unsigned int timeJump;
        double finalTime;
        double AbsoluteZero;
        double StefanBoltzmann;
        
        
        int nOpenMPcores;
        int nOpenMPcoresMax;

        int printAdditional; // if 1 will print additional information. Otherwise
        // only essential information

        enum libraryForCalc libraryForCalculation;

        //flags to identify if the required parameters were defined.
        // 0 if not defined 1 otherwise
        int typeSimDefined;
        int dimSimDefined;
        int methodDefined;
        int solveDefined; // 2 if dynamic simulation
        int timeStepDefined; // only required if dynamic simulation
        int finalTimeDefined; // only required if dynamic simulation


        // flags to what to save
        int saveTemperature;
        int saveTemperatureBetween;
        int saveHeatFlux;

    };

    enum configSimu {
        TYPE,
        DIMENSIONS,
        METHOD,
        METHOD2,
        SOLVE,
        TIME_STEP,
        TIME_jUMP,
        FINAL_TIME,
        ABSOLUTE_ZERO,
        STEFAN_BOLTZMANN_CONSTANT
    };


    // function used to initialized the Simulation variable
    unsigned int initiateSimulationVariable(struct Simulation*);

    // function used to terminate the Simulation variable
    unsigned int terminateSimulationVariable(struct Simulation*);

    // function used to call the prints to print all the information from the simulation
    void printfAllSimuData(struct Simulation*);

    // specific printers
    void printfTypeS(enum typeSim *);
    void printfDimen(enum dimSim *);
    void printfMeth(enum method *);
    //void printfMeth(enum method2 *); // future implementation
    void printfSolv(enum solve *);
    void printWhatToSave(struct Simulation*);
    void printfBench(enum benchmark *);
    void printfLibrary(enum libraryForCalc*);


    // take the input text and extract the values of the variables. Returns 0 if no error.
    unsigned int setConfigurationSimu(char *, struct Simulation *, int *);

    // this function will test if all necessary variables were correctly initialized. Return 0 if no error.
    unsigned int testInputSimu(struct Simulation *);





#ifdef __cplusplus
}
#endif

#endif // LIBSIMUCONFIG_H_INCLUDED

