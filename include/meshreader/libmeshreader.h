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
 * File:   libmeshreader.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 26, 2016.
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
 * contains declaration of functions, structures, and enumerations for reading 
 * and converting mesh files. See libmeshreader.c for more details.
 *
 */

#ifndef LIBMESHREADER_H
#define LIBMESHREADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../configs/libmeshconfig.h"
#include "libmeshtlmbht.h"

    unsigned int meshReaderAndConverter(struct MeshConfig *,
            struct tlmInternalMesh *, int);




#ifdef __cplusplus
}
#endif

#endif /* LIBMESHREADER_H */

