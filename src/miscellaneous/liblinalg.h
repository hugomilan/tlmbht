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
 * File:   liblinalg.h
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
 * contains declaration of functions, structures, and enumerations for
 * vectorial operations, linear algebra and geometric calculations. See liblinalg.c for more details. 
 *
 */

#ifndef LIBLINALG_H
#define LIBLINALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../meshreader/libmeshtlmbht.h"
    
    void nodesNorm(const struct node *, const struct node *, double*);
    
    void nodesMixedProduct(const struct node*, const struct node *, const struct node*,
            const struct node*, double*);
    
    void mixedProduct(const double *, const double *, const double*, double*);
    
    void triangleArea(const struct node *, const struct node *, const struct node *, double*);
    
    void quadrangleArea(const struct node *, const struct node *, const struct node *,
            const struct node *, double*);
    
    void tetrahedronVolume(const struct node *, const struct node *, const struct node *,
            const struct node *, double*);
    
    void hexahedronVolume(const struct node *, const struct node *, const struct node *,
            const struct node *, const struct node *, const struct node *, const struct node *,
            const struct node *, double*);
    
    

#ifdef __cplusplus
}
#endif

#endif /* LIBLINALG_H */


