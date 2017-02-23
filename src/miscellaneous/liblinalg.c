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
 * File:   liblinalg.c
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on February 22, 2017.
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
 * Solves vectorial, linear algebra, and geometric problems.
 * file 
 *
 */
#include "liblinalg.h"

#include <math.h>



/*
 * nodesNorm: Calculates the norm of a vector defined between two nodes
 */
void nodesNorm(const struct node *N1, const struct node *N2, double *out) {
    double deltaX, deltaY, deltaZ;
    deltaX = N1->x - N2->x;
    deltaY = N1->y - N2->y;
    deltaZ = N1->z - N2->z;
    *out = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

/*
 * nodesMixedProduct: Calculates the mixed product from vectors defined by the 
 * nodes (N2 - N1), (N3 - N1), (N4 - N1).
 */
void nodesMixedProduct(const struct node *N1, const struct node *N2, const struct node *N3,
            const struct node *N4, double *out){
    double v1[3], v2[3], v3[3];
    
    v1[0] = N2->x - N1->x;
    v1[1] = N2->y - N1->y;
    v1[2] = N2->z - N1->z;
    
    v2[0] = N3->x - N1->x;
    v2[1] = N3->y - N1->y;
    v2[2] = N3->z - N1->z;
    
    v3[0] = N4->x - N1->x;
    v3[1] = N4->y - N1->y;
    v3[2] = N4->z - N1->z;
    
    mixedProduct(v1, v2, v3, out);
    
}
/*
 * mixedProduct: Calculates the mixed product from 3 vectors
 */    
void mixedProduct(const double *v1, const double *v2, const double *v3, double *out){
    *out = v1[0]*(v2[1]*v3[2] - v2[2]*v3[1]) + v1[1]*(v2[2]*v3[0] - v2[0]*v3[2])
            + v1[2]*(v2[0]*v3[1] - v2[1]*v3[0]);
}

/*
 * triangleArea: Calculates the area of the triangle defined by N1, N2, N3.
 */
void triangleArea(const struct node *N1, const struct node *N2, const struct node *N3, double *out){
    double deltaXL[2], deltaYL[2], deltaZL[2];
    deltaXL[0] = N2->x - N1->x;
    deltaYL[0] = N2->y - N1->y;
    deltaZL[0] = N2->z - N1->z;

    deltaXL[1] = N3->x - N1->x;
    deltaYL[1] = N3->y - N1->y;
    deltaZL[1] = N3->z - N1->z;

    *out = sqrt((deltaYL[0] * deltaZL[1] - deltaYL[1] * deltaZL[0])*(deltaYL[0] * deltaZL[1] - deltaYL[1] * deltaZL[0])
            + (deltaXL[0] * deltaYL[1] - deltaXL[1] * deltaYL[0])*(deltaXL[0] * deltaYL[1] - deltaXL[1] * deltaYL[0])
            + (deltaXL[0] * deltaZL[1] - deltaXL[1] * deltaZL[0])*(deltaXL[0] * deltaZL[1] - deltaXL[1] * deltaZL[0])
            ) / 2;
}


/*
 * quadrangleArea: Calculates the area of the quadrangle defined by N1, N2, N3, N4.
 * Divide the quadrangle in two triangles and then sum up their areas
 */
void quadrangleArea(const struct node *N1, const struct node *N2, const struct node *N3,
         const struct node *N4, double *out){
    double temp;
    triangleArea(N1, N2, N3, &temp);
    triangleArea(N2, N3, N4, out);
    *out = *out + temp;
    
}

/*
 * tetrahedronVolume: Calculates the volume of the tetrahedron defined by N1, N2, N3, N4.
 * Calculates the volume by using mixed product (or the scalar triple product)
 */
void tetrahedronVolume(const struct node *N1, const struct node *N2, const struct node *N3,
         const struct node *N4, double *out){
    nodesMixedProduct(N1, N2, N3, N4, out);
    *out = fabs(*out)/6.;
}

/*
 * hexahedronVolume: Calculates the volume of the tetrahedron defined by N1, N2, N3, N4.
 * Calculates the volume by using mixed product (or the scalar triple product)
 */
void hexahedronVolume(const struct node *N1, const struct node *N2, const struct node *N3,
        const struct node *N4, const struct node *N5, const struct node *N6, const struct node *N7,
        const struct node *N8, double *out){
    // nomenclature in Grandy (1997) and mine:
    // 0 -> 1
    // 1 -> 2
    // 2 -> 4
    // 3 -> 3
    // 4 -> 5
    // 5 -> 6
    // 6 -> 8
    // 7 -> 7
    double temp, v1[3], v2[3], v3[3], v4[3], v5[3], v6[3], v7[3];
    
    v1[0] = N7->x - N1->x;
    v1[1] = N7->y - N1->y;
    v1[2] = N7->z - N1->z;
    
    v2[0] = N2->x - N1->x;
    v2[1] = N2->y - N1->y;
    v2[2] = N2->z - N1->z;
    
    v3[0] = N3->x - N6->x;
    v3[1] = N3->y - N6->y;
    v3[2] = N3->z - N6->z;
    
    v4[0] = N5->x - N1->x;
    v4[1] = N5->y - N1->y;
    v4[2] = N5->z - N1->z;
    
    v5[0] = N6->x - N8->x;
    v5[1] = N6->y - N8->y;
    v5[2] = N6->z - N8->z;
    
    v6[0] = N4->x - N1->x;
    v6[1] = N4->y - N1->y;
    v6[2] = N4->z - N1->z;
    
    v7[0] = N8->x - N3->x;
    v7[1] = N8->y - N3->y;
    v7[2] = N8->z - N3->z;
    
    mixedProduct(v1, v2, v3, &temp);
    *out = temp/6.0;
    mixedProduct(v1, v4, v5, &temp);
    *out = *out + temp/6.0;
    mixedProduct(v1, v6, v7, &temp);
    *out = *out + temp/6.0;    
}