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
 * File:   liberrorcode.h
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
 * prints error message
 *
 */

#include <stdio.h>

#include "liberrorcode.h"

/*
 * sendErrorCodeAndMessage: prints the error message by its errorCode into the
 * standard error output
 */
void sendErrorCodeAndMessage(unsigned int errorCode, void *input1v, void *input2v,
        void *input3v, void *input4v) {

    fprintf(stderr, "ERROR: ");

    unsigned int *input1ui = (unsigned int *) input1v;
    unsigned long long *input2llui = (unsigned long long *) input2v;
    char *input1c = (char*) input1v;
    char *input2c = (char*) input2v;
    char *input3c = (char*) input3v;

    unsigned int *input3ui = (unsigned int *) input3v;
    char *input4c = (char*) input4v;


    switch (errorCode) {
        case 0:
            // this should not be an error
            fprintf(stderr, "Unknown error OR no error at all. Maybe a programming error?");
            break;

        case 8:
            fprintf(stderr, "Unknown error reading the file");
            break;

        case 12:
            // this should not be an error
            fprintf(stderr, "Error starting the variables");
            break;

        case 200:
            // error when choosing the configuration (SIMULATION, MESH, etc.)
            fprintf(stderr, "Unknown error. Last line read was  line %04u: %s", *input1ui, input2c);
            break;

        case 201:
            // error when choosing the configuration (SIMULATION, MESH, etc.)
            fprintf(stderr, "More than one 'simulation' configuration field was found. Last line read was  line %04u: %s", *input1ui, input2c);
            break;

        case 202:
            // error when choosing the configuration (SIMULATION, MESH, etc.)
            fprintf(stderr, "More than one 'mesh' configuration field was found. Last line read was  line %04u: %s", *input1ui, input2c);
            break;
            
        case 205:
            // error when finishing reading one configuration and going to read another
            fprintf(stderr, "Unknown error. Last line read was line %04u: %s", *input1ui, input2c);
            break;
            
        case 206:
            // error when finishing reading one configuration and going to read another
            fprintf(stderr, "Unknown error. Last line read was line %04u: %s", *input1ui, input2c);
            break;

        case 208:
            fprintf(stderr, "Simulation configuration field was not found");
            break;

        case 210:
            fprintf(stderr, "Mesh configuration field was not found");
            break;

        case 211:
            fprintf(stderr, "Material configuration field was not found");
            break;

        case 212:
            fprintf(stderr, "Equation configuration field was not found");
            break;

        case 315:
            // error when we do not find the input file
            fprintf(stderr, "Requires either an input file OR a case.tlm file in the same folder of the calling directory\n");
            break;

        case 316:
            // error for unknown command
            fprintf(stderr, "Unknown command '%s'\n", input1c);
            break;

        case 317:
            // error for multiple inputs
            fprintf(stderr, "Two input locations were found. Please, revise your input. We got '%s' at position %u and '%s' at position %u\n", input2c, *input1ui, input4c, *input3ui);
            break;

        case 345:
            //We couldn't find the input file
            fprintf(stderr, "Unknown error closing the output file");
            break;

        case 365:
            // error when choosing the configuration (SIMULATION, MESH, etc.)
            fprintf(stderr, "Unknown command in line %04u: %s", *input1ui, input2c);
            break;
            
        case 500:
            // error when creating the variable name table
            fprintf(stderr, "Could not find \"::\" in the variable \"%s\"", input1c);
            break;
            
        case 501:
            // error when reading variable name for simulation
            fprintf(stderr, "the input \"%s\" was identified as from STD (standard for simulation) but the variable name \"%s\" was not identified as an acceptable input. The acceptable input names are: \nabsolute_zero \nStefan_Boltzmann_constant \nx \ny \nz \n", input1c, input2c);
            break;
            
        case 511:
            // error when reading variable name for equation
            fprintf(stderr, "the input \"%s\" was identified as from the equation \"%s\" but the variable name \"%s\" was not identified as an acceptable input. The acceptable input names are: \ntime \ntime-step \ntime-jump \nfinal_time \nscalar or the scalar variable name \n flux or the flux variable name", input1c, input2c, input3c);
            break;
            
        case 521:
            // error when reading variable name for equation
            fprintf(stderr, "the input \"%s\" was identified as from the material \"%s\" but the variable name \"%s\" was not identified as an acceptable input. The acceptable input names are: \ndiffusion_coefficient \ncoefficient_b \nrelaxation_time \nsink_a \nsource \nvectorial_source[0] \nvectorial_source[1] \nvectorial_source[2] \ninitial_scalar \ndensity \nspecific_heat \nthermal_conductivity \nblood_perfusion \nblood_density \nblood_specific_heat \nblood_temperature \ninternal_heat_generation \ninitial_temperature", input1c, input2c, input3c);
            break;
            
        case 531:
            // error when reading variable name for equation
            fprintf(stderr, "the input \"%s\" was identified as from the boundary \"%s\" but the variable name \"%s\" was not identified as an acceptable input. The acceptable input names are: \nscalar \nflux \nconvection_scalar \nconvection_coefficient \ntemperature \nheat_flux \nconvection_temperature \nradiation_temperature \nradiation_emissivity", input1c, input2c, input3c);
            break;
            
        case 764:
            //We couldn't find the input file
            fprintf(stderr, "Could not open file %s", input1c);
            break;

        case 1147:
            // error when reading the SIMULATION configurations
            fprintf(stderr, "Unknown command in line %04u: %s", *input1ui, input2c);
            break;

        case 1358:
            // this error happen when the software finds something more than { on its line
            fprintf(stderr, "Should have only '{' in line %04u: %s", *input1ui, input2c);
            break;

        case 1359:
            // this error happen when the software finds something more than } on its line
            fprintf(stderr, "Should have only '}' in line %04u: %s", *input1ui, input2c);
            break;

        case 1858:
            // error when reading the EQUATION configurations
            fprintf(stderr, "Type of equations for simulation was not defined within Equation group %04d", *input1ui);
            break;

        case 1859:
            // error when reading the EQUATION configurations
            fprintf(stderr, "Type of dimension was not defined within Equation group %04d", *input1ui);
            break;

            //        case 1860:
            //            // error when reading the EQUATION configurations
            //            fprintf(stderr, "Method for solution was not defined within Equation");
            //            break;

        case 1861:
            // error when reading the EQUATION configurations
            fprintf(stderr, "Type of solution (steady-state or dynamic) was not defined within Equation group %04d", *input1ui);
            break;

        case 1862:
            // error when reading the EQUATION configurations
            fprintf(stderr, "Type of solution was defined as dynamic but time-step was not defined within Equation group %04d", *input1ui);
            break;

        case 1864:
            // error when reading the EQUATION configurations
            fprintf(stderr, "The name of the equation was not defined within Equation group %04d", *input1ui);
            break;

        case 1870:
            // error when reading the SIMULATION configurations
            fprintf(stderr, "Error reading the Simulation configuration");
            break;

        case 1871:
            // error when reading the MESH configurations
            fprintf(stderr, "Error reading the mesh configuration");
            break;

        case 1872:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "Error reading the materials configuration for group %04d", *input1ui);
            break;

        case 1873:
            // error when reading the SIMULATION configurations
            fprintf(stderr, "Error reading the boundaries configuration for group %04d", *input1ui);
            break;

        case 1874:
            // error when reading the SIMULATION configurations
            fprintf(stderr, "Error reading the sources configuration for group %04d", *input1ui);
            break;

        case 1875:
            // The name of the mesh input was not given
            fprintf(stderr, "The name of the input mesh was not given");
            break;

        case 1877:
            // error when reading the SIMULATION configurations
            fprintf(stderr, "Equation group %04d (name: %s) and equation group %04d (name: %s) have the same name", *input1ui, input2c, *input3ui, input4c);
            break;

        case 1876:
            // error when reading the SIMULATION configurations
            fprintf(stderr, "Error reading the equation configuration for group %04d", *input1ui);
            break;

        case 2314:
            // No node was read in the TLM internal mesh
            fprintf(stderr, "No node was read");
            break;

        case 2315:
            // No node was read in the TLM internal mesh
            fprintf(stderr, "No element was read");
            break;

        case 3668:
            // this error happen when '=' is not found
            fprintf(stderr, "Equal sign '=' was not found in line %04u: %s", *input1ui, input2c);
            break;

        case 3669:
            // this error happen when ';' is not found
            fprintf(stderr, "Semicolon ';' was not found in line %04u: %s", *input1ui, input2c);
            break;

        case 3874:
            // Unknown command for simulation
            fprintf(stderr, "Unknown type for simulation in line %04u: %s", *input1ui, input2c);
            break;

        case 3875:
            // Unknown dimension for simulation
            fprintf(stderr, "Unknown dimension for simulation in line %04u: %s", *input1ui, input2c);
            break;

        case 3876:
            // Unknown method for simulation
            fprintf(stderr, "Unknown method for simulation in line %04u: %s", *input1ui, input2c);
            break;

        case 3877:
            // Unknown special adjustment for method for simulation
            fprintf(stderr, "Unknown special adjustment for method for simulation in line %04u: %s", *input1ui, input2c);
            break;

        case 3878:
            // Unknown method for simulation
            fprintf(stderr, "Unknown special adjustment for method for simulation in line %04u: %s", *input1ui, input2c);
            break;

        case 3879:
            // Unknown input format for mesh
            fprintf(stderr, "Unknown input format for mesh in line %04u: %s", *input1ui, input2c);
            break;

        case 3880:
            // Unknown input for print additional mode
            fprintf(stderr, "Unknown input for print additional mode in line %04u: %s", *input1ui, input2c);
            break;

        case 3881:
            // Unknown input for print additional
            fprintf(stderr, "Unknown input for save in line %04u: %s", *input1ui, input2c);
            break;

        case 3882:
            // Unknown input for benchmark
            fprintf(stderr, "Unknown input for benchmark in line %04u: %s", *input1ui, input2c);
            break;

        case 3883:
            // Unknown input for benchmark
            fprintf(stderr, "Unknown input for library in line %04u: %s", *input1ui, input2c);
            break;

        case 3884:
            // Unknown input for benchmark
            fprintf(stderr, "Unknown input for number of OpenMP cores in line %04u: %s", *input1ui, input2c);
            break;

        case 3890:
            // Unknown input for output extension
            fprintf(stderr, "Unknown input for output extension in line %04u: %s", *input1ui, input2c);
            break;

        case 3891:
            // Unknown input for timing mode
            fprintf(stderr, "Unknown input for timing mode in line %04u: %s", *input1ui, input2c);

        case 3892:
            // Unknown input for verbose mode
            fprintf(stderr, "Unknown input for verbose mode in line %04u: %s", *input1ui, input2c);
            break;

        case 4441:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "The flux model for the materials group %04d was defined as hyperbolic but the relaxation time was not defined", *input1ui);
            break;

        case 4442:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "We didn't find a match for the equation name from materials group %04d: '%s'", *input1ui, input2c);
            break;

        case 4443:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "The equation name for materials group %04d was not defined", *input1ui);
            break;

        case 4444:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "The density for materials group %04d was not defined", *input1ui);
            break;

        case 4445:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "The specific heat for materials group %04d was not defined", *input1ui);
            break;

        case 4446:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "The thermal conductivity for materials group %04d was not defined", *input1ui);
            break;

        case 4447:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "The Equation was defined was dynamic but initial temperature for materials group %04d was not defined", *input1ui);
            break;

        case 4448:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "The diffusion coeeficient for materials group %04d was not defined", *input1ui);
            break;

        case 4449:
            // error when reading the MATERIAL configurations
            fprintf(stderr, "Simulation was defined was dynamic but initial value of the scalar field for materials group %04d was not defined", *input1ui);
            break;

        case 5165:
            // Unknown input format for mesh
            fprintf(stderr, "Unknown input for adiabatic for boundary in line %04u: %s", *input1ui, input2c);
            break;

        case 5499:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the nodes during reading");
            break;

        case 5501:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 2 nodes line elements during reading");
            break;

        case 5502:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 3 nodes triangle elements during reading");
            break;

        case 5503:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 4 nodes quadrangle elements during reading");
            break;

        case 5504:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 4 nodes tetrahedron elements during reading");
            break;

        case 5505:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 8 nodes hexahedron elements during reading");
            break;

        case 5506:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 6 nodes prism elements during reading");
            break;

        case 5507:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 5 nodes pyramid elements during reading");
            break;

        case 5515:
            // Not enough memory
            fprintf(stderr, "Not enough memory to allocate the 1 node point elements during reading");
            break;

        case 6514:
            // Unknown element number
            fprintf(stderr, "Unknown element number in line %04u: %s. The known elements "
                    "are: 1 - 2 nodes line; 2 - 3 nodes triangle; 3 - 4 nodes quadrangle; "
                    "5 - 8 nodes hexahedron; 6 - 6 nodes prism; 7 - 5 nodes pyramid; "
                    "15 - 1 node point", *input1ui, input2c);
            break;

        case 6515:
            // Unknown element number
            fprintf(stderr, "Less than 2 tags were given for the element in line %04u: %s", *input1ui, input2c);
            break;

        case 6516:
            // Unknown element number
            fprintf(stderr, "More nodes were found than the number of nodes described"
                    " by the .msh file. The last read line was line %04u: %s", *input1ui, input2c);
            break;

        case 6517:
            // Unknown element number
            fprintf(stderr, "More elements were found than the number of elements described"
                    " by the .msh file. The last read line was line %04u: %s", *input1ui, input2c);
            break;

        case 6599:
            fprintf(stderr, "Unknown error reading the mesh file type");
            break;

        case 6600:
            // no 2d element for tlm 2d
            fprintf(stderr, "No two-dimensional element was found and the simulation chosen was 2D");
            break;

        case 6601:
            // no 3d element for tlm 3d
            fprintf(stderr, "No three-dimensional element was found and the simulation chosen was 3D");
            break;

        case 6602:
            // no 1d element for tlm 1d
            fprintf(stderr, "No one-dimensional element was found and the simulation chosen was 1D");
            break;

        case 6610:
            // no material element
            fprintf(stderr, "No material element was found associated with the tag numbers supplied. "
                    "Consider to re-check the tag numbers");
            break;

        case 6743:
            // this error happen when the software finds something more than { on its line
            fprintf(stderr, "Should have only '{' in line %04u: %s", *input1ui, input2c);
            break;

        case 7152:
            // this error happen when the software finds something more than } on its line
            fprintf(stderr, "Only found ']' (should have found '[' and ']') in line %04u: %s", *input1ui, input2c);
            break;

        case 7153:
            // this error happen when the software finds something more than } on its line
            fprintf(stderr, "Only found '[' (should have found '[' and ']') in line %04u: %s", *input1ui, input2c);
            break;

        case 7154:
            /* FALLTHRU */
        case 7155:
            // this error happen when the software finds something more than } on its line
            fprintf(stderr, "We found more than one number but we did not found '[' and ']' in line %04u: %s", *input1ui, input2c);
            break;

        case 7156:
            // Unknown number of inputs for scale
            fprintf(stderr, "Wrong number of inputs to scale the mesh. It must be one or three. Line %04u: %s", *input1ui, input2c);
            break;

        case 7410:
            // Unknown method for simulation
            fprintf(stderr, "Unknown configuration inside $MeshFormat in line %04u: %s", *input1ui, input2c);
            break;

        case 7411:
            // Unknown method for simulation
            fprintf(stderr, "The header $MeshFormat was not found before the header $Nodes in line %04u: %s", *input1ui, input2c);
            break;

        case 7412:
            // Unknown method for simulation
            fprintf(stderr, "The header $MeshFormat was not found before the header $Elements in line %04u: %s", *input1ui, input2c);
            break;

        case 7510:
            fprintf(stderr, "Not enough space for allocating the nodes in the memory. The number of nodes read was %s", input2c);
            break;

        case 7511:
            fprintf(stderr, "Not enough space for allocating the elements in the memory. The number of elements read was %s", input2c);
            break;

        case 7652:
            // Unknown method for simulation
            fprintf(stderr, "$PhysicalNames was not implemented. All data here will not be considered. "
                    "Line %04u: %s", *input1ui, input2c);
            break;

        case 7653:
            // Unknown method for simulation
            fprintf(stderr, "$Periodic was not implemented. All data here will not be considered. "
                    "Line %04u: %s", *input1ui, input2c);
            break;

        case 7654:
            // Unknown method for simulation
            fprintf(stderr, "$NodeData was not implemented. All data here will not be considered. "
                    "Line %04u: %s", *input1ui, input2c);
            break;

        case 7655:
            // Unknown method for simulation
            fprintf(stderr, "$ElementData was not implemented. All data here will not be considered. L"
                    "Line %04u: %s", *input1ui, input2c);
            break;

        case 7656:
            // Unknown method for simulation
            fprintf(stderr, "$ElementNodeData was not implemented. All data here will not be considered. "
                    "Line %04u: %s", *input1ui, input2c);
            break;

        case 7657:
            // Unknown method for simulation
            fprintf(stderr, "$InterpolationScheme was not implemented. All data here will not be considered. "
                    "Line %04u: %s", *input1ui, input2c);
            break;

        case 7658:
            // Unknown method for simulation
            fprintf(stderr, "Unrecognized header. All data here will not be considered. "
                    "Line %04u: %s", *input1ui, input2c);
            break;

        case 7659:
            // Unknown method for simulation
            fprintf(stderr, "Unknown command in line %04u: %s", *input1ui, input2c);
            break;

        case 8520:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "We didn't find a match for the equation name from boundary group %04d: '%s'", *input1ui, input2c);
            break;

        case 8521:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The convection coefficient for boundary group %04d was defined but the convection temperature was not defined", *input1ui);
            break;

        case 8522:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The convection temperature for boundary group %04d was defined but the convection coefficient was not defined", *input1ui);
            break;

        case 8523:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The radiation emissivity for boundary group %04d was defined but the radiation temperature was not defined", *input1ui);
            break;

        case 8524:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The radiation temperature for boundary group %04d was defined but the radiation emissivity was not defined", *input1ui);
            break;

        case 8525:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The temperature for boundary group %04d was defined but the convection boundary condition was also defined. "
                    "It can only solve when one of the two is selected", *input1ui);
            break;

        case 8526:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The temperature for boundary group %04d was defined but the radiation boundary condition was also defined. "
                    "It can only solve when one of the two is selected", *input1ui);
            break;

        case 8527:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The temperature for boundary group %04d was defined but the heat flux boundary condition was also defined. "
                    "It can only solve when one of the two is selected", *input1ui);

        case 8528:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The convection coefficient for boundary group %04d was defined but the convection scalar value was not defined", *input1ui);
            break;

        case 8529:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The convection scalar for boundary group %04d was defined but the convection coefficient was not defined", *input1ui);
            break;

        case 8530:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The scalar value for boundary group %04d was defined but the convection boundary condition was also defined. "
                    "It can only solve when one of the two is selected", *input1ui);
            break;

        case 8031:
            // error when reading the BOUNDARY configurations
            fprintf(stderr, "The scalar value for boundary group %04d was defined but the flux boundary condition was also defined. "
                    "It can only solve when one of the two is selected", *input1ui);

            break;
        case 8693:
            /* FALLTHRU */
        case 8694:
            /* FALLTHRU */
        case 8695:
            /* FALLTHRU */
        case 8696:
            /* FALLTHRU */
        case 8697:
            fprintf(stderr, "Failed to wrap the memory for the connection variable at level %u and quantity of %llu", *input1ui, *input2llui);
            break;
        case 8698:
            /* FALLTHRU */
        case 8699:
            fprintf(stderr, "Failed to allocate memory for the connection variable at level %u and quantity of %llu", *input1ui, *input2llui);
            break;
        case 8700:
            /* FALLTHRU */
        case 8701:
            /* FALLTHRU */
        case 8702:
            /* FALLTHRU */
        case 8703:
            /* FALLTHRU */
        case 8704:
            /* FALLTHRU */
        case 8705:
            /* FALLTHRU */
        case 8706:
            /* FALLTHRU */
        case 8707:
            /* FALLTHRU */
        case 8708:
            /* FALLTHRU */
        case 8709:
            /* FALLTHRU */
        case 8710:
            /* FALLTHRU */
        case 8711:
            /* FALLTHRU */
        case 8712:
            /* FALLTHRU */
        case 8713:
            /* FALLTHRU */
        case 8714:
            /* FALLTHRU */
        case 8715:
            /* FALLTHRU */
        case 8716:
            /* FALLTHRU */
        case 8717:
            /* FALLTHRU */
        case 8718:
            /* FALLTHRU */
        case 8719:
            /* FALLTHRU */
        case 8720:
            /* FALLTHRU */
        case 8721:
            /* FALLTHRU */
        case 8722:
            /* FALLTHRU */
        case 8723:
            /* FALLTHRU */
        case 8724:
            /* FALLTHRU */
        case 8725:
            /* FALLTHRU */
        case 8726:
            /* FALLTHRU */
        case 8727:
            /* FALLTHRU */
        case 8728:
            /* FALLTHRU */
        case 8729:
            /* FALLTHRU */
        case 8730:
            /* FALLTHRU */
        case 8731:
            /* FALLTHRU */
        case 8732:
            /* FALLTHRU */
        case 8733:
            /* FALLTHRU */
        case 8734:
            /* FALLTHRU */
        case 8735:
            /* FALLTHRU */
        case 8736:
            /* FALLTHRU */
        case 8737:
            /* FALLTHRU */
        case 8738:
            /* FALLTHRU */
        case 8739:
            /* FALLTHRU */
        case 8740:
            /* FALLTHRU */
        case 8741:
            /* FALLTHRU */
        case 8742:
            /* FALLTHRU */
        case 8743:
            /* FALLTHRU */
        case 8744:
            /* FALLTHRU */
        case 8745:
            /* FALLTHRU */
        case 8750:
            fprintf(stderr, "Failed to allocate memory for the processing");
            break;

        case 9998:
            // this should not be an error. This is a flag number
            fprintf(stderr, "Unknown error");
            break;

        case 9999:
            // this should not be an error. This is a flag number
            fprintf(stderr, "Unknown error");
            break;

        default:
            fprintf(stderr, "Unknown error");
            break;
    }

    fprintf(stderr, ".\nERROR: Error code: %04u\n", errorCode);

}
