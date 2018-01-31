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
 * File:   libfunctionconfig.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on January 1, 2018.
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
 * Contains the configuration code for setting up the function modes and its
 * solution procedure.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfunctionconfig.h"

#include "../miscellaneous/libstringtlmbht.h"
#include "../miscellaneous/liberrorcode.h"

/*
 * initializeFunctionConfig: initialize the function variable used for configuration
 * with standard values
 */
unsigned int initializeFunctionConfig(struct FunctionConfig* function) {
    
    function->functionName = NULL;
    function->functionNameDefined = 0;
    
    function->outputVariableName = NULL;
    function->outputVariableNameDefined = 0;
    
    function->library = NULL;
    function->quantityOfLibrariesInput = 0;
    function->libraryDefined = 0;
    
    function->inputVariables = NULL;
    function->inputVariablesName = NULL;
    function->numberOfVariables = 0;
    function->inputVariablesDefined = 0;
    
    function->expression = NULL;
    function->quantityOfLinesInExpression = 0;
    function->expressionDefined = 0;
    function->bracketsUsed = 0;
    function->readingBrackets = 0;
    
    return 0;
}

/*
 * terminateFunctionConfig: terminate the function variable used for configurations
 */
unsigned int terminateFunctionConfig(struct FunctionConfig *function) {
    free(function->functionName);
    function->functionName = NULL;

    free(function->outputVariableName);
    function->outputVariableName = NULL;

    free(function->library);
    function->library = NULL;

    free(function->inputVariablesName);
    function->inputVariablesName = NULL;
    
    free(function->inputVariables);
    function->inputVariables = NULL;

    free(function->expression);
    function->expression = NULL;

    return 0;
}


/*
 * setConfigurationFunction: reads the line and configures the required equation parameter
 */
unsigned int setConfigurationFunction(char * input, struct FunctionConfig * configInput, int * startEndBrackets) {
    unsigned int errorTLMnumber, tempVarInt;

    // DEBUG: Shows that we got inside and the input
    // printf("inside condigurationFunction %s\n", input);

    // The Brackets for SIMULATION were not started yet
    if (*startEndBrackets == 0) {
        if ((errorTLMnumber = testStartBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 1; //define as the bracket started

    } else if (compareCaseInsensitive(input, "name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);
        
        // the input is the name
        configInput->functionName = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(configInput->functionName, input);
        configInput->functionNameDefined = 1;

    } else if (compareCaseInsensitive(input, "library") == 0) {
        if ((errorTLMnumber = readVectorStrInputs(input,
                &configInput->quantityOfLibrariesInput, &configInput->library)) != 0)
            return errorTLMnumber;

        configInput->libraryDefined = 1;

        // DEBUG: Checking the number we got
        // printf("The number (%d) we got were", configInput->quantityOfLibrariesInput);
        // for (int i = 0; i < configInput->quantityOfLibrariesInput; i++){
        //      printf(" %d", matInput->library[i]);
        // }

    } else if (compareCaseInsensitive(input, "input variables") == 0) {
        if ((errorTLMnumber = readVectorStrInputs(input,
                &configInput->numberOfVariables, &configInput->inputVariablesName)) != 0)
            return errorTLMnumber;

        configInput->inputVariablesDefined = 1;

        // DEBUG: Checking the number we got
        // printf("The number (%d) we got were", configInput->quantityOfLibrariesInput);
        // for (int i = 0; i < configInput->quantityOfLibrariesInput; i++){
        //      printf(" %d", matInput->library[i]);
        // }

    } else if (compareCaseInsensitive(input, "output name") == 0) {
        if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
            return errorTLMnumber;

        // removes all the blank spaces before and after and only gets the string
        // with the information
        removeBlankSpacesBeforeAndAfter(input);
        
        // the input is the name
        configInput->outputVariableName = (char*) malloc(sizeof (char)*(strlen(input) + 1));
        strcpy(configInput->outputVariableName, input);
        configInput->outputVariableNameDefined = 1;

    } else if (compareCaseInsensitive(input, "expression") == 0 || configInput->readingBrackets != 0) {
        
        for (int i = 0; i < strlen(input); i++){
            if (strncmp(input + i, "{", 1) == 0){
                // read the first brackets
                configInput->readingBrackets = 1;
            }
            else if (strncmp(input + i, "}", 1) == 0){
                // read the last bracket
                if (configInput->readingBrackets == 1)
                    // this is the case that brackets were put for one line only
                    configInput->readingBrackets = 4;
                else
                    // this is the multi-line brackets case
                    configInput->readingBrackets = 3;
                // 2: reading between brackets
            }
        }
        char *firstBracketPosition, *lastBracketPosition;

        switch (configInput->readingBrackets) {
            case 0:
                // this is the case that expression is only one line and does not have a brackets
                if ((errorTLMnumber = getBetweenEqualAndSemicolon(input)) != 0)
                    return errorTLMnumber;

                // removes all the blank spaces before and after and only gets the string
                // with the information
                removeBlankSpacesBeforeAndAfter(input);

                // the input is the expression
                configInput->expression = (char**) malloc(sizeof (char*));
                configInput->expression[0] = (char*) malloc(sizeof (char)*(strlen(input) + 1));
                strcpy(configInput->expression[0], input);
                configInput->quantityOfLinesInExpression = 1;
                configInput->expressionDefined = 1;
                configInput->bracketsUsed = 0;

                break;
                
            case 1:
                // this is a multi-line expression and this is the first line
                // that starts with the brackets {
                firstBracketPosition = strchr(input, '{');
                
                // the input contains part of the expression
                configInput->expression = (char**) malloc(sizeof (char*));
                configInput->expression[0] = (char*) malloc(sizeof (char)*(strlen(firstBracketPosition))); // don't need to add + 1 for the null character because firstBracketPosition starts at '{' and I won't use it
                strncpy(configInput->expression[0], firstBracketPosition + 1, strlen(firstBracketPosition) - 1);
                configInput->expression[0][strlen(firstBracketPosition) - 1] = '\0';
                configInput->quantityOfLinesInExpression = 1;
                configInput->expressionDefined = 1;
                configInput->bracketsUsed = 1;
                configInput->readingBrackets = 2;
                
                break;
            case 2:
                // this is a multi-line expression and this is a line within it
                
                // the input contains part of the expression
                configInput->expression = (char **) realloc(configInput->expression,
                        sizeof (char *)*
                        (configInput->quantityOfLinesInExpression + 1));
                
                configInput->expression[configInput->quantityOfLinesInExpression] = (char*) malloc(sizeof (char)*(strlen(input) + 1)); // don't need to add + 1 for the null character because firstBracketPosition starts at '{' and I won't use it
                strcpy(configInput->expression[configInput->quantityOfLinesInExpression], input);
                configInput->quantityOfLinesInExpression++;
                
                break;
            case 3:
                // this is a multi-line expression and this is the last line
                // that ends with the brackets }
                lastBracketPosition = strchr(input, '}');
                
                // the input contains part of the expression
                configInput->expression = (char **) realloc(configInput->expression,
                        sizeof (char *)*
                        (configInput->quantityOfLinesInExpression + 1));
                
                configInput->expression[configInput->quantityOfLinesInExpression] = (char*) malloc(sizeof (char)*(strlen(input) - strlen(lastBracketPosition) + 1));
                strncpy(configInput->expression[configInput->quantityOfLinesInExpression], input,  strlen(input) - strlen(lastBracketPosition));
                configInput->expression[configInput->quantityOfLinesInExpression][strlen(input) - strlen(lastBracketPosition)] = '\0';
                configInput->quantityOfLinesInExpression++;
                
                // we are done reading what is inside brackets
                configInput->readingBrackets = 0;
                
                break;
            case 4:
                // this is an one line expression that starts and ends with brackets                
                firstBracketPosition = strchr(input, '{');
                lastBracketPosition = strchr(input, '}');

                // the input contains the expression
                configInput->expression = (char**) malloc(sizeof (char*));
                configInput->expression[0] = (char*) malloc(sizeof (char)*(strlen(firstBracketPosition) - strlen(lastBracketPosition))); // don't need to add + 1 for the null character because firstBracketPosition starts at '{' and I won't use it
                strncpy(configInput->expression[0], firstBracketPosition + 1, strlen(firstBracketPosition) - strlen(lastBracketPosition) - 2);
                configInput->expression[0][strlen(firstBracketPosition) - strlen(lastBracketPosition) - 1] = '\0';
                configInput->quantityOfLinesInExpression = 1;
                configInput->expressionDefined = 1;
                configInput->bracketsUsed = 0;
                break;
        }
        
        // DEBUG: show what was copied
        // printf("%d:%s\n", configInput->quantityOfLinesInExpression, configInput->expression[configInput->quantityOfLinesInExpression - 1]);
                
            
    } else if (input[0] == '}' && *startEndBrackets == 1) {
        if ((errorTLMnumber = testEndBrackets(input)) != 0)
            return errorTLMnumber;

        *startEndBrackets = 0; //defined as the bracket was finalized
        return 9999; // that is my code to indicate that it was done.
        
    } else {
        return 1147;
    }
    return 0;
}
