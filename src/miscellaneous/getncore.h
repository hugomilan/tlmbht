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
 * File:   getncore.h
 * Author: hugomilan
 *
 * Created on August 9, 2016, 1:35 PM
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
 * 
 *
 */


#ifndef GETTHREAD_H
#define GETTHREAD_H

void getncores(unsigned int *);

#ifdef __cplusplus
extern "C" {
#endif
    
    void call_from_c_getncores(unsigned int *);

#ifdef __cplusplus
}
#endif


#endif /* GETTHREAD_H */

