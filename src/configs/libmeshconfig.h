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
 * File:   libmeshconfig.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on June 24, 2016.
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
 * contains declaration of functions, structures, and enumerations for configuring
 * the mesh. See libmeshconfig.c for more details.
 *
 */
#ifndef LIBMESHCONFIG_H_INCLUDED
#define LIBMESHCONFIG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
    // variable for the inputFormat

    enum inputFormat {
        TLMTBN, // .tbn (mine format)
        GMSH // .msh (MeshFormat 2.2)
    };

    struct MeshConfig {
        enum inputFormat inputF;
        int inputFormatDefined;

        char *nameOfInputFile;
        int inputNameDefined;

        char *nameOfOutputFile;

        double *scale;

        // flags




    };

    enum configMesh {
        FILE_NAME,
        INPUT_FORMAT
    };

    unsigned int initiateMeshVariable(struct MeshConfig*);
    unsigned int terminateMeshVariable(struct MeshConfig*);

    // functions for printing
    void printfMeshConfig(struct MeshConfig*);
    void printfInputFormatOutput(enum inputFormat*, char *);
    void printfNameOfInputFile(char *);
    void printfScale(double *);


    // take the input text and extract the values of the variables. Returns 0 if no error.
    unsigned int setConfigurationMesh(char *, struct MeshConfig *, int *);

    // this function test if all the inputs for mesh configuration were correct
    unsigned int testInputMesh(struct MeshConfig *, char*);




#ifdef __cplusplus
}
#endif

#endif // LIBMESHCONFIG_H_INCLUDED
