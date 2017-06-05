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
        const struct node *N4, double *out) {
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
void mixedProduct(const double *v1, const double *v2, const double *v3, double *out) {
    *out = v1[0]*(v2[1] * v3[2] - v2[2] * v3[1]) + v1[1]*(v2[2] * v3[0] - v2[0] * v3[2])
            + v1[2]*(v2[0] * v3[1] - v2[1] * v3[0]);
}

/*
 * triangleArea: Calculates the area of the triangle defined by N1, N2, N3.
 */
void triangleArea(const struct node *N1, const struct node *N2, const struct node *N3, double *out) {
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
 * quadranglePlaneArea: Calculates the area of the quadrangle defined by N1, N2, N3, N4.
 * The quadrangle is divided into two planes. This does not guarantee that center point
 * (N1 + N2 + N3 + N4)/4 is within any of the two plans. See the function
 * quadrangleArea which guarantees that the center point is within some plane defined
 * by the quadrangle.
 * Divide the quadrangle in two triangles and then sum up their areas. ATTENTION:
 * THE NUMBER OF THE QUADRANGLE SHOULD FOLLOW THE DEFINITION BELOW. THAT IS, THE
 * POINTS N1, N2, N3, N4 SHOULD BE CLOCKWISE (COUNTERCLOCKWISE WILL ALSO WORK).
 * triangular nomenclature.
 * 
 * Take the quadrangle
 * 
 *                   face 1   
 *       vertex 1  __________ vertex 2
 *                /         / 
 *               /         / 
 *      face 4  /         / face 2
 *             /         /
 *   vertex 4 /_________/ vertex 3
 *              face 3
 * 
 * Divide into two triangles
 * 
 *                   face 1   
 *       vertex 1  __________ vertex 2
 *                /\        / 
 *               /  \      / 
 *      face 4  /    \    / face 2
 *             /      \  /
 *   vertex 4 /________\/ vertex 3
 *              face 3
 */
void quadranglePlaneArea(const struct node *N1, const struct node *N2, const struct node *N3,
        const struct node *N4, double *out) {
    double temp;
    triangleArea(N1, N2, N3, &temp);
    triangleArea(N1, N3, N4, out);
    *out = *out + temp;
}

/*
 * quadrangleArea: Calculates the area of the quadrangle defined by N1, N2, N3, N4.
 * The quadrangle is divided into four triangles that have the center point 
 * (N1 + N2 + N3 + N4)/4.
 * Divide the quadrangle in four triangles and then sum up their areas. ATTENTION:
 * THE NUMBER OF THE QUADRANGLE SHOULD FOLLOW THE DEFINITION BELOW. THAT IS, THE
 * POINTS N1, N2, N3, N4 SHOULD BE CLOCKWISE (COUNTERCLOCKWISE WILL ALSO WORK).
 * triangular nomenclature.
 * 
 * Take the quadrangle
 * 
 *                   face 1   
 *       vertex 1  ___________ vertex 2
 *                |           |
 *                |           |
 *                /           | 
 *               |            |
 *               |            |
 *               /            |
 *              |             |
 *      face 4  |             |  face 2
 *              /             |
 *             |              |
 *             |              |
 *             /              |
 *            |               |
 *            |               |
 *            /               |
 *           |                |
 *  vertex 4 |_____           |
 *                 \____      |
 *            face 3    \_____|  vertex 3
 *                           
 *
 * 
 * Divide into four triangles
 * 
 *                   face 1   
 *       vertex 1  ___________ vertex 2
 *                |\         /|
 *                | \       / |
 *                /  \     /  | 
 *               |    \   /   |
 *               |     \ /    |
 *               /      X     |
 *              |center/ \    |
 *      face 4  |     /   |   |  face 2
 *              /    |    |   |
 *             |    /      \  |
 *             |   /       |  |
 *             /  |        |  |
 *            |  /          \ |
 *            | /           | |
 *            / |           | |
 *           | /            \ |
 *  vertex 4 |/____          ||
 *                 \____     ||
 *            face 3    \____\|  vertex 3
 *                           
 *  
 *              
 */
void quadrangleArea(const struct node *N1, const struct node *N2, const struct node *N3,
        const struct node *N4, double *out) {
    // central node
    struct node Nc;
    Nc.x = (N1->x + N2->x + N3->x + N4->x) / 4;
    Nc.y = (N1->y + N2->y + N3->y + N4->y) / 4;
    Nc.z = (N1->z + N2->z + N3->z + N4->z) / 4;

    double temp;
    // I calculate the area of four triangles
    triangleArea(N1, N2, &Nc, out);
    triangleArea(N1, N4, &Nc, &temp);
    *out = *out + temp;
    triangleArea(N3, N2, &Nc, &temp);
    *out = *out + temp;
    triangleArea(N3, N4, &Nc, &temp);
    *out = *out + temp;
}

/*
 * tetrahedronVolume: Calculates the volume of the tetrahedron defined by N1, N2, N3, N4.
 * Calculates the volume by using mixed product (or the scalar triple product)
 */
void tetrahedronVolume(const struct node *N1, const struct node *N2, const struct node *N3,
        const struct node *N4, double *out) {
    nodesMixedProduct(N1, N2, N3, N4, out);
    *out = fabs(*out) / 6.;
}

/*
 * hexahedronLDVolume: Calculates the volume of the hexahedron defined by N1, N2,
 * N3, N4, N5, N6, N7, and N8 (nomenclature below). LD means Long Diagonal Hexahedron. That is, the 
 * hexahedron's quadrangle face is split in two triangles and the whole hexahedron is divided
 * into 6 tetrahedra. See Grandy (1997) for more details. Calculates the volume by 
 * using mixed product (or the scalar triple product).
 * 
 * hexahedral nomenclature.
 * 
 * 
 *            vertex 4____________________________ vertex 3
 *                   /|                          /|
 *                  / .                         / |
 *                 /  .      5                 /  |
 *                /   |                       /   |
 *               /    .    a                 /    |
 *              /     .   e                 /     |
 *             /      |  r     area 1      /      |
 *            /       . a                 /       |
 *  vertex 8 /___________________________/vertex 7|
 *          |         | vertex 1         |        |
 *          |        , _.._.._.._.._.._..|_.._ .._| vertex 2
 *          |     3 ,                    |     4 /
 *          |      /   2                 |      /
 *          |   a ,                      |   a /
 *          |  e ,   a                   |  e /
 *          | r /   e       area 6       | r /
 *          |a ,   r                     |a /
 *          | ,   a                      | /
 *          |/___________________________|/
 *      vertex 5                        vertex 6
 * 
     
 */
void hexahedronLDVolume(const struct node *N1, const struct node *N2, const struct node *N3,
        const struct node *N4, const struct node *N5, const struct node *N6, const struct node *N7,
        const struct node *N8, double *out) {
    // volume of the hexahedron calculated following eq. 14 in Grandy (1997)
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

    // x7 - x0
    v1[0] = N7->x - N1->x;
    v1[1] = N7->y - N1->y;
    v1[2] = N7->z - N1->z;

    // x1 - x0
    v2[0] = N2->x - N1->x;
    v2[1] = N2->y - N1->y;
    v2[2] = N2->z - N1->z;

    // x3 - x5
    v3[0] = N3->x - N6->x;
    v3[1] = N3->y - N6->y;
    v3[2] = N3->z - N6->z;

    // x4 - x0
    v4[0] = N5->x - N1->x;
    v4[1] = N5->y - N1->y;
    v4[2] = N5->z - N1->z;

    // x5 - x6
    v5[0] = N6->x - N8->x;
    v5[1] = N6->y - N8->y;
    v5[2] = N6->z - N8->z;

    // x2 - x0
    v6[0] = N4->x - N1->x;
    v6[1] = N4->y - N1->y;
    v6[2] = N4->z - N1->z;

    // x6 - x3
    v7[0] = N8->x - N3->x;
    v7[1] = N8->y - N3->y;
    v7[2] = N8->z - N3->z;

    mixedProduct(v1, v2, v3, &temp);
    *out = temp / 6.0;
    mixedProduct(v1, v4, v5, &temp);
    *out = *out + temp / 6.0;
    mixedProduct(v1, v6, v7, &temp);
    *out = *out + temp / 6.0;
}

/*
 * hexahedronTHVolume: Calculates the volume of the hexahedron defined by N1, N2,
 * N3, N4, N5, N6, N7, and N8 (nomenclature below). TH means Tetrakis Hexahedron. That is, the 
 * hexahedron's quadrangle face is split in four triangles.
 * See Grandy (1997) for more details. Calculates the volume by 
 * using mixed product (or the scalar triple product).
 * 
 * hexahedral nomenclature.
 * 
 * 
 *            vertex 4____________________________ vertex 3
 *                   /|                          /|
 *                  / .                         / |
 *                 /  .      5                 /  |
 *                /   |                       /   |
 *               /    .    a                 /    |
 *              /     .   e                 /     |
 *             /      |  r     area 1      /      |
 *            /       . a                 /       |
 *  vertex 8 /___________________________/vertex 7|
 *          |         | vertex 1         |        |
 *          |        , _.._.._.._.._.._..|_.._ .._| vertex 2
 *          |     3 ,                    |     4 /
 *          |      /   2                 |      /
 *          |   a ,                      |   a /
 *          |  e ,   a                   |  e /
 *          | r /   e       area 6       | r /
 *          |a ,   r                     |a /
 *          | ,   a                      | /
 *          |/___________________________|/
 *      vertex 5                        vertex 6
 * 
     
 */
void hexahedronTHVolume(const struct node *N1, const struct node *N2, const struct node *N3,
        const struct node *N4, const struct node *N5, const struct node *N6, const struct node *N7,
        const struct node *N8, double *out) {
    // volume of the hexahedron calculated following eq. 12 in Grandy (1997)
    // nomenclature in Grandy (1997) and mine:
    // 0 -> 1
    // 1 -> 2
    // 2 -> 4
    // 3 -> 3
    // 4 -> 5
    // 5 -> 6
    // 6 -> 8
    // 7 -> 7
    double temp, v1[3], v2[3], v3[3], v4[3], v5[3], v6[3], v7[3], v8[3], v9[3];

    // x7 - x1
    v1[0] = N7->x - N2->x;
    v1[1] = N7->y - N2->y;
    v1[2] = N7->z - N2->z;

    // x6 - x0
    v2[0] = N8->x - N1->x;
    v2[1] = N8->y - N1->y;
    v2[2] = N8->z - N1->z;

    // x7 - x2
    v3[0] = N7->x - N4->x;
    v3[1] = N7->y - N4->y;
    v3[2] = N7->z - N4->z;

    // x3 - x0
    v4[0] = N3->x - N1->x;
    v4[1] = N3->y - N1->y;
    v4[2] = N3->z - N1->z;

    // x5 - x0
    v5[0] = N6->x - N1->x;
    v5[1] = N6->y - N1->y;
    v5[2] = N6->z - N1->z;

    // x7 - x4
    v6[0] = N7->x - N5->x;
    v6[1] = N7->y - N5->y;
    v6[2] = N7->z - N5->z;

    // (x7 - x1) + (x6 - x0)
    v7[0] = v1[0] + v2[0];
    v7[1] = v1[1] + v2[1];
    v7[2] = v1[2] + v2[2];

    // (x7 - x2) + (x5 - x0)
    v8[0] = v3[0] + v5[0];
    v8[1] = v3[1] + v5[1];
    v8[2] = v3[2] + v5[2];

    // (x7 - x4) + (x3 - x0)
    v9[0] = v6[0] + v4[0];
    v9[1] = v6[1] + v4[1];
    v9[2] = v6[2] + v4[2];

    mixedProduct(v7, v3, v4, &temp);
    *out = temp / 12.0;
    mixedProduct(v2, v8, v6, &temp);
    *out = *out + temp / 12.0;
    mixedProduct(v1, v5, v9, &temp);
    *out = *out + temp / 12.0;
}