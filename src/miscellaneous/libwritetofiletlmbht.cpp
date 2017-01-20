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
 * File:   libwritetofiletlmbht.cpp
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on January 18, 2017.
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
 * Designed to have functions that will write to output files. The control of the
 * writing process is on the simulationInput variable.
 *
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "libwritetofiletlmbht.h"
#include "../solver/tlmsolver/libtlmsolvereigen.h"

/*
 * creatOutputFile: Create the output file where the data will be saved
 * 
 */
unsigned int creatOutputFile(struct dataForSimulation *input) {

    if (input->simulationInput.outputFileCreated == 1) {
        // the output file was already created.
        return 0;
    }

    input->simulationInput.outputFileCreated = 1;
    // I'm going to create the output file now

    // Now it is writing mode. If there is a file with the same name, it will be replaced
    input->simulationInput.fullNameOfOutputFile = (char*) malloc(strlen(input->simulationInput.nameOfOutputFile) + 5);
    strcpy(input->simulationInput.fullNameOfOutputFile, input->simulationInput.nameOfOutputFile);
    switch (input->simulationInput.outputFileExtension) {
            // tmo (default): 1
            // m: 2
        case 1:
            strcat(input->simulationInput.fullNameOfOutputFile, ".tmo");
            input->simulationInput.comentary_string = (char *) malloc(3 * sizeof (char));
            strcpy(input->simulationInput.comentary_string, "//");
            break;
        case 2:
            strcat(input->simulationInput.fullNameOfOutputFile, ".m");
            input->simulationInput.comentary_string = (char *) malloc(2 * sizeof (char));
            strcpy(input->simulationInput.comentary_string, "%");
            break;
    }

    if ((input->simulationInput.outputFileHandler = fopen(input->simulationInput.fullNameOfOutputFile, "w")) == NULL) {
        // error opening the file
        // sendErrorCodeAndMessage(764, nameOfFile, NULL, NULL, NULL);
        free(input->simulationInput.outputFileHandler);
        return 764;
    }

    time_t t = time(NULL);
    struct tm time_now = *localtime(&t);

    // my name and my version
    fprintf(input->simulationInput.outputFileHandler, "%s %s %s\n",
            input->simulationInput.comentary_string, input->myName, input->myVersion);
    // when this file was created
    fprintf(input->simulationInput.outputFileHandler, "%s Output file created in "
            "%d-%d-%d %d:%d:%d\n", input->simulationInput.comentary_string,
            time_now.tm_year + 1900, time_now.tm_mon + 1,
            time_now.tm_mday, time_now.tm_hour, time_now.tm_min, time_now.tm_sec);

    fprintf(input->simulationInput.outputFileHandler, "%s Solved for %d equation(s)\n",
            input->simulationInput.comentary_string, input->quantityOfEquationsRead);
    fprintf(input->simulationInput.outputFileHandler, "%s The variable of each equation is followed"
            "by '_' and the number of the equation\n",
            input->simulationInput.comentary_string);
    
    return 0;
}

/*
 * writeFirstDataToFile: Intended to be the first function to write something
 * to the file. This is likely going to include configuration and stuffs. Right now
 * it only prints the output points
 */
unsigned int writeFirstDataToFile(struct dataForSimulation *input,
        void* generalMatrix, int id) {

    unsigned long long i;

    // this switch is used to known to what format should the void pointer be 
    // casted to.
    //
    // I should have another switch to change the format of the writing accordingly
    // to the output format. FUTURE IMPLEMENTATION
    switch (input->equationInput[id].libraryForCalculation) {
        case EIGEN:
        { // including brackets to not let the variable 'matrices' visible to the next case
            fprintf(input->simulationInput.outputFileHandler, "save_%d = [%d, %d, %d];\n", id + 1,
                    input->equationInput[id].saveScalar,
                    input->equationInput[id].saveScalarBetween,
                    input->equationInput[id].saveFlux);

            struct calculationTLMEigen *matrices = (struct calculationTLMEigen*) generalMatrix;

            fprintf(input->simulationInput.outputFileHandler, "numbers_%d = [%llu, %llu, %llu, %llu, %llu];\n", id + 1,
                    matrices->numbers.Nodes,
                    matrices->numbers.Ports, matrices->numbers.Intersections, matrices->numbers.Output,
                    matrices->numbers.Points_Output);

            switch (input->equationInput[id].Solv) {
                case DYNAMIC:
                    fprintf(input->simulationInput.outputFileHandler,
                            "%s Dynamic Solution for equation group %d\n",
                            input->simulationInput.comentary_string, id + 1);

                    fprintf(input->simulationInput.outputFileHandler, "times_%d = [%g %g %g %u %llu];\n", id + 1,
                            input->equationInput[id].timeStep * input->equationInput[id].timeJump,
                            input->equationInput[id].finalTime, input->equationInput[id].timeStep,
                            input->equationInput[id].timeJump,
                            (unsigned long long) (input->equationInput[id].finalTime / (
                            input->equationInput[id].timeStep * input->equationInput[id].timeJump)));

                    break;
                case STEADY:
                    fprintf(input->simulationInput.outputFileHandler,
                            "%s Steady-State Solution for equation group %d",
                            input->simulationInput.comentary_string, id + 1);

                    break;
            }



            fprintf(input->simulationInput.outputFileHandler, "Points_Output_%d = [", id + 1);

            for (i = 0; i < matrices->numbers.Output; i++)
                fprintf(input->simulationInput.outputFileHandler, "\n%.17g, %.17g, %.17g", matrices->Points_output[i].x,
                    matrices->Points_output[i].y, matrices->Points_output[i].z);

            fprintf(input->simulationInput.outputFileHandler, "];\n");
        }

            break;
        case CUDA:
            break;
    }

    return 0;
}

/*
 * writePartialOutput: writes the output to the file
 */
unsigned int writePartialOutput(struct dataForSimulation *input,
        void * generalMatrix, int id, unsigned long long position) {

    unsigned long long i;
    switch (input->equationInput[id].libraryForCalculation) {
        case EIGEN:
        { // including brackets to not let the variable 'matrices' visible to the next case
            fprintf(input->simulationInput.outputFileHandler, "\n");
            fprintf(input->simulationInput.outputFileHandler, "output_%d(:,%llu) = [", id + 1, position);

            struct calculationTLMEigen *matrices = (struct calculationTLMEigen*) generalMatrix;

            for (i = 0; i < matrices->numbers.Output; i++) {
                fprintf(input->simulationInput.outputFileHandler, "\n%.17g;", matrices->output(i));
            }


            fprintf(input->simulationInput.outputFileHandler, "];");
            fprintf(input->simulationInput.outputFileHandler, "\n");
        }
            break;

        case CUDA:
            break;
    }

    return 0;
}