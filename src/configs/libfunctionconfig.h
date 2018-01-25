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
 * File:   libfunctionconfig.h
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
 * contains declaration of functions, structures, and enumerations for working
 * with external defined functions . See libfunctionconfig.c for more details.
 */

#ifndef LIBFUNCTIONCONFIG_H
#define LIBFUNCTIONCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libvariablename.h"
    
struct FunctionConfig // structure that contains the information for the material input
    {
        char *functionName;
        int functionNameDefined;
        
        char *outputVariableName;
        int outputVariableNameDefined;
        
        char **library;
        int quantityOfLibrariesInput;
        int libraryDefined;
        
        struct variableID *inputVariables;
        char **inputVariablesName;
        int numberOfVariables;
        int inputVariablesDefined;
        
        char **expression;
        int quantityOfLinesInExpression;
        int expressionDefined;
        int bracketsUsed;
        int readingBrackets;
        
        
    };


    unsigned int initializeFunctionConfig(struct FunctionConfig *);

    unsigned int terminateFunctionConfig(struct FunctionConfig *);

    unsigned int setConfigurationFunction(char *, struct FunctionConfig *, int *);

    //unsigned int testInputFunction(struct FunctionConfig *, struct Equation *, int);


#ifdef __cplusplus
}
#endif

#endif /* LIBFUNCTIONCONFIG_H */


