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
 * File:   libtbnreader.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on July 1, 2016.
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
 * the tbn mesh file. See libtbnreader.c for more details.
 *
 */

#ifndef LIBTBNREADER_H
#define LIBTBNREADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libmeshtlmbht.h"
#include "../configs/libmeshconfig.h"
    
    enum tlmtbnConfig {
        NOTHING_TBN,
        NODES_TBN,
        ELEMENTS_TBN
    };
    
unsigned int tbnReader(struct MeshConfig *, struct tlmInternalMesh *);

unsigned int setMeshTbnConfigTo(char *, enum tlmtbnConfig *);


#ifdef __cplusplus
}
#endif

#endif /* LIBTBNREADER_H */

