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
 * File:   libmiscellaneous.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 10, 2016.
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
 * error messages. See libmiscellaneous.c for more details.
 *
 */

#ifndef LIBMISCELLANEOUS_H
#define LIBMISCELLANEOUS_H

#ifdef __cplusplus
extern "C" {
#endif

    void sortOptmizedBubbleSort(unsigned long long *);

    int compareLLU(const void *, const void *);


#ifdef __cplusplus
}
#endif

#endif /* LIBMISCELLANEOUS_H */

