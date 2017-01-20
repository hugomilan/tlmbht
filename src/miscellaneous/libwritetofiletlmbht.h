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
 * File:   libwritetofiletlmbht.h
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
 * contains declaration of functions, structures, and enumerations for writing
 * output files. See libwritetofiletlmbht.c for more details.
 *
 */

#ifndef LIBWRITETOFILETLMBHT_H
#define LIBWRITETOFILETLMBHT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../configs/libconfig.h"

    unsigned int creatOutputFile(struct dataForSimulation *);
    
    unsigned int writeFirstDataToFile(struct dataForSimulation *, 
        void *, int);
    
    unsigned int writePartialOutput(struct dataForSimulation *,
        void *, int, unsigned long long);



#ifdef __cplusplus
}
#endif

#endif /* LIBWRITETOFILETLMBHT_H */


