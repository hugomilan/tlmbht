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
 * File:   libvariablename.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on December 31, 2017.
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
 * Contains the functions for dealing with variables.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "libvariablename.h"

/*
 * initializeVariableID: initialize the variableID with standard values
 */
unsigned int initializeVariableID(struct variableID input){
    input.name = NULL;
    input.fieldLocation = NOTHING;
    input.fieldName = NULL;
    input.fieldId = -1;
    input.fieldVariableId = -1;
    
    return 0;
}

/*
 * terminateVariableID: terminate the variableID with standard values
 */
unsigned int terminateVariableID(struct variableID input){
    free(input.name);
    input.name = NULL;
    
    free(input.fieldName);
    input.fieldName = NULL;
    
    return 0;
}