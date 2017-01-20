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
 * File:   libmiscellaneous.c
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
 * Contain diversified functions that do not compose a whole library
 *
 */

#include "libmiscellaneous.h"

/*
 * sortOptmizedBubbleSort: Sorts unsigned long long using the optimized 
 * bubble sort algorithm. It assumes that the position zero contains the length
 * of the pointer. Sorting from the smallest to the biggest
 */
void sortOptmizedBubbleSort(unsigned long long *input) {
    unsigned int endSort = input[0], swap = 1;
    unsigned long long temp;

    while (swap != 0) {
        swap = 0;
        for (unsigned int i = 1; i < endSort; i++) {
            if (input[i] > input[i + 1]) {
                temp = input[i];
                input[i] = input[i + 1];
                input[i + 1] = temp;
                swap = i;
            }
        }
        endSort = swap;
    }
}

/*
 * compareLLU: function used in the qsort() function to sort
 * unsigned long long.
 */
int compareLLU(const void *a, const void *b) {

    return (*(unsigned long long*) a - *(unsigned long long*) b);
}