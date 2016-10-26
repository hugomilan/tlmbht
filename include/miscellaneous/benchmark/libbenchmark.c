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
 * File:   libbenchmark.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 8, 2016.
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

#include "libbenchmark.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

unsigned int runBenchmark(){
    unsigned int errorTLMnumber = 0;
    
    printf("Running benchmark for matrix multiplication\n");
    long long unsigned int L1 = 750, L2 = 750, maxElement;
    int repetitions = 7, numberOfSparsityTests = 5;
    double lowerTime = 0.5, higherTime = 1;
    FILE *file;
    
    maxElement = 1e9;
    
    printf("Opening the file benchmark.txt\n");
    
    if ((file = fopen("benchmark.txt", "w")) == NULL) {//We couldn't open the input file
        sendErrorCodeAndMessage(764, "benchmark.txt", NULL, NULL, NULL);
        free(file);
        file = NULL;
        return 764;
    }
    
    if ((errorTLMnumber = benchmarkSAXPY(L1, L2, repetitions, lowerTime,
            higherTime, numberOfSparsityTests, file, maxElement)) != 0){
        sendErrorCodeAndMessage(errorTLMnumber, NULL, NULL, NULL, NULL);
    }
    
    fclose(file);
    
    
    return errorTLMnumber;
}