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
 * File:   libsolvereigen.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on August 9, 2016.
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
 * contains declaration of functions, structures, and enumerations for using the
 * Eigen library with my C code. See libsolvereigen.cpp for more details.
 *
 */

#ifndef LIBSOLVEREIGEN_H
#define LIBSOLVEREIGEN_H

#include <Eigen/Sparse>

using namespace Eigen;

#ifdef __cplusplus
extern "C" {
#endif

#include "libsolver.h"


#ifdef __cplusplus
}
#endif

#endif /* LIBSOLVEREIGEN_H */

