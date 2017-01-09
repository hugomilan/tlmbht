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
 * File:   libsourceconfig.h
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
 * the sources. See libsourceconfig.c for more details.
 *
 */


#ifndef LIBSOURCECONFIG_H_INCLUDED
#define LIBSOURCECONFIG_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#include "libsimuconfig.h"

    struct SourceConfig // structure that contains the information for the material input
    {
        // future implementation
        int none; // not useful, just to start the struct

    };

    unsigned int initializeSourceConfig(struct SourceConfig *);

    unsigned int terminateSourceConfig(struct SourceConfig *);

    unsigned int setConfigurationSource(char *, struct SourceConfig*, int*);

    unsigned int testInputSource(struct SourceConfig *, enum typeSim *, int);

    void printfSourceConfig(struct SourceConfig *, enum typeSim*);


#ifdef __cplusplus
}
#endif


#endif // LIBSOURCECONFIG_H_INCLUDED
