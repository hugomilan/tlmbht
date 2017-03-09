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
 * File:   libgmshreader.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 25, 2016.
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
 * the gmsh mesh file. See libgmshreader.c for more details.
 *
 */

#ifndef LIBGMSHREADER_H
#define LIBGMSHREADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../configs/libmeshconfig.h"
#include "libmeshtlmbht.h"

    enum gmsh2_2Config {
        NOTHING_GMSH,
        MESH_FORMAT,
        NODES,
        ELEMENTS,
        PHYSICAL_NAMES,
        PERIODIC,
        NODE_DATA,
        ELEMENT_DATA,
        ELEMENT_NODE_DATA,
        INTERPOLATION_SCHEME,
        UNRECOGNIZED_HEADER
    };

    struct meshFormat {
        float version; // only 2.2
        int fileType; // 0 - ASCII; 1 - binary. Implemented only for ASCII
        int dataSize; // size of the float point numbers
        //flag
        int MeshFormatDefined; // 1 - the mesh was defined. 0 - the mesh was not  defined yet
    };

    struct dataGmsh2_2 {
        struct meshFormat MeshFormat;
        unsigned long long numberOfNode;
        unsigned long long numberOfNodeReads;

        unsigned long long numberOfElement;
        unsigned long long numberOfElementReads;

        unsigned long long *quantityOfSpecificElement;
        struct elementAny *saveElement;

        struct node *nodes;
    };

    unsigned int gmshReader(struct MeshConfig *, struct tlmInternalMesh *);

    // this function set the configuration type
    unsigned int setMeshGmshConfigTo(char *, enum gmsh2_2Config *);

    // this function initiate the dataGmsh2_2 variable
    unsigned int initiateDataGmsh2_2(struct dataGmsh2_2*);

    // this function terminate the dataGmsh2_2 variable
    unsigned int terminateDataGmsh2_2(struct dataGmsh2_2*);
    unsigned int deallocateOnlyTheNodeGmsh(struct dataGmsh2_2*);
    unsigned int deallocateOnlyTheElementGmsh(struct dataGmsh2_2*);

    // this function will copy the GMSH inputs into the output file and also
    // save the text file
    unsigned int readAndWriteGmsh(struct tlmInternalMesh *, struct dataGmsh2_2 *, FILE *);
    unsigned int onlyWriteGmsh2_2(struct dataGmsh2_2 *, FILE *);
    unsigned int writeThenReadGmsh(struct tlmInternalMesh *, struct dataGmsh2_2 *, FILE **,
            struct MeshConfig *);


    //testing the input I got
    unsigned int testInputGmsh(struct dataGmsh2_2*);






#ifdef __cplusplus
}
#endif

#endif /* LIBGMSHREADER_H */

