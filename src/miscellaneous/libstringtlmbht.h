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
 * File:   libstringtlmbht.h
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
 * Description of this function:
 * contains declaration of functions, structures, and enumerations for string
 * manipulation. See libstringtlmbht.c for more details.
 *
 */

#ifndef LIBSTRINGTLMBHT_H
#define LIBSTRINGTLMBHT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
    
    // this function makes comparison without considering for the case (upper or lower)
    unsigned int compareCaseInsensitive(char *, char *);

    unsigned int getUsefulContent(char *);

    void removeBlankSpacesBefore(char *);

    void removeBlankSpacesAfter(char *);

    void removeBlankSpacesBeforeAndAfter(char *);

    unsigned int getBetweenEqualAndSemicolon(char *);

    unsigned int getBetweenBrackets(char *);

    unsigned int getBeforePoint(char *);
    
    unsigned int getlineTlmbht(char**, long*, FILE*);

    unsigned int readVectorIntInputs(char *, int *, int **);
    
    unsigned int readVectorStrInputs(char *, int *, char ***);

    unsigned int readVectorDoubleLengthThreeInputs(char *, double *, char **, int *);

    unsigned int getQuantityOfBlankSpaces(char *, int *);
    
    unsigned int getQuantityOfCommas(char *, int *);

    unsigned int testStartBrackets(char *);

    unsigned int testEndBrackets(char *);
    


#ifdef __cplusplus
}
#endif

#endif /* LIBSTRINGTLMBHT_H */

