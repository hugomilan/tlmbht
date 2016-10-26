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
 * File:   libmeshtlmbht.h
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
 * contains declaration of functions, structures, and enumerations for tlm mesh file 
 * See libmeshtlmbht.c for more details.
 *
 */

#ifndef LIBMESHTLMBHT_H
#define LIBMESHTLMBHT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

    struct node {
        double x;
        double y;
        double z;
    };

    struct elementLine { // 1
        long long unsigned int N1;
        long long unsigned int N2;
        long unsigned int tag;
    };

    struct elementTriangle { // 2
        long long unsigned int N1;
        long long unsigned int N2;
        long long unsigned int N3;
        long unsigned int tag;
    };

    struct elementQuadrangle { // 3
        long long unsigned int N1;
        long long unsigned int N2;
        long long unsigned int N3;
        long long unsigned int N4;
        long unsigned int tag;
    };

    struct elementTetrahedron { // 4
        long long unsigned int N1;
        long long unsigned int N2;
        long long unsigned int N3;
        long long unsigned int N4;
        long unsigned int tag;
    };

    struct elementHexahedron { // 5
        long long unsigned int N1;
        long long unsigned int N2;
        long long unsigned int N3;
        long long unsigned int N4;
        long long unsigned int N5;
        long long unsigned int N6;
        long long unsigned int N7;
        long long unsigned int N8;
        long unsigned int tag;
    };

    struct elementPrism { // 6
        long long unsigned int N1;
        long long unsigned int N2;
        long long unsigned int N3;
        long long unsigned int N4;
        long long unsigned int N5;
        long long unsigned int N6;
        long unsigned int tag;
    };

    struct elementPyramid { // 7
        long long unsigned int N1;
        long long unsigned int N2;
        long long unsigned int N3;
        long long unsigned int N4;
        long long unsigned int N5;
        long unsigned int tag;
    };

    struct elementPoint { // 15
        long long unsigned int N1;
        long unsigned int tag;
    };

    struct elementAny {
        int elementCode;
        long unsigned int tag;
        long long unsigned int N1;
        long long unsigned int N2;
        long long unsigned int N3;
        long long unsigned int N4;
        long long unsigned int N5;
        long long unsigned int N6;
        long long unsigned int N7;
        long long unsigned int N8;
    };

    struct elementAll {
        struct elementLine *Line; // Element code: 1
        struct elementTriangle *Triangle; // Element code: 2
        struct elementQuadrangle *Quadrangle; // Element code: 3
        struct elementTetrahedron *Tetrahedron; // Element code: 4
        struct elementHexahedron *Hexahedron; // Element code: 5
        struct elementPrism *Prism; // Element code: 6
        struct elementPyramid *Pyramid; // Element code: 7
        struct elementPoint *Point; // Element code: 15
    };

    struct tlmInternalMesh {
        long long unsigned int numberOfNode;
        struct node *nodes;

        long long unsigned int *quantityOfSpecificElement;
        unsigned int quantityOfElementTypes;
        struct elementAll elements;
    };


    unsigned int initiateTlmInternalMesh(struct tlmInternalMesh *);

    unsigned int terminateTlmInternalMesh(struct tlmInternalMesh *);

    unsigned int allocateTLMInternalMeshNodesAndElements(struct tlmInternalMesh *,
            unsigned long long int, long long unsigned int*);

    unsigned int allocateTLMInternalMeshElementsOnly(struct tlmInternalMesh *, const int *);

    unsigned int writeElementToFileTlmFormat(struct tlmInternalMesh *, FILE *);

    unsigned int testTlmInternalMesh(struct tlmInternalMesh *);

    //print additional information about the mesh
    void printfAllMeshData(struct tlmInternalMesh *);

    //print number of nodes
    void printMeshTlmbhtNodesInformation(struct tlmInternalMesh *);
    void printMeshTlmbhtElementsInformation(struct tlmInternalMesh *);

#ifdef __cplusplus
}
#endif

#endif /* LIBMESHTLMBHT_H */

