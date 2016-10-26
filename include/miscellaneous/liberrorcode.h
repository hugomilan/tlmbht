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
 * File:   liberrorcode.h
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
 * contains declaration of functions, structures, and enumerations for printing 
 * error messages. See liberrorcode.c for more details.
 *
 */

#ifndef LIBERRORCODE_H
#define LIBERRORCODE_H

#ifdef __cplusplus
extern "C" {
#endif


    void sendErrorCodeAndMessage(unsigned int, void *, void *, void *, void *);


#ifdef __cplusplus
}
#endif

#endif /* LIBERRORCODE_H */

