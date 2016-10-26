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
 * File:   libsourceconfig.c
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
 * Contains the functions for reading from the input file and 
 * configuring the source inputs.
 *
 */

#include <stdio.h>

#include "libsourceconfig.h"

#include "libsimuconfig.h"
#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * initializeSourceConfig: initialize the source variable used for configuration
 * with standard values
 */
unsigned int initializeSourceConfig(struct SourceConfig * source) {
    //future implementation
    return 0;
}

/*
 * terminateSourceConfig: terminate the source variable used for configuration
 */
unsigned int terminateSourceConfig(struct SourceConfig * source) {
    //future implementation
    return 0;
}

/*
 * setConfigurationSource: reads the line and configures the required source parameters
 */
unsigned int setConfigurationSource(char *input, struct SourceConfig* sourceInput, int* startEndBrackets) {
    //future implementation
    unsigned int errorTLMnumber;
    if (*startEndBrackets == 0) {

        if ((errorTLMnumber = testStartBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 1; //define as the bracket started

    } else if (input[0] == '}' && *startEndBrackets == 1) {
        if ((errorTLMnumber = testEndBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 0; //define as the bracket as finalized
        return 9999; // that is my code to indicate that it was done.
    } else {
        return 1147;
    }
    return 0;
}

/*
 * testInputSource: tests if all the required inputs were read
 */
unsigned int testInputSource(struct SourceConfig *input, enum typeSim *typeS, int id) {
    //future implementation
    return 0;
}

/*
 * printfSourceConfig: prints the source configurations that were read
 */
void printfSourceConfig(struct SourceConfig *input, enum typeSim *typeS) {
    //future implementation
}