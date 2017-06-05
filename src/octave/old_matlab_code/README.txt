TLMBHT stands for (T)ransmission-(L)ine (M)odeling numerical method applied to
(B)io(H)eat (T)ransfer problems.

This algorithm was developed by Hugo Fernando Maia Milan. If you need any help,
please, send an e-mail to hugofernando@gmail.com.

Copyright (C) 2016 Hugo Fernando Maia Milan

Acknowledgment: Brazilian National Counsel of Technological and Scientific
Development (Proc. 203312/2014-7) for scholarship to HFMM

Version 0.1

This versions contains:

Files applicable for our paper Milan HFM, Gebremedhin KG. Transmission-line 
modeling (TLM) triangular node for bioheat transfer. Journal ??, 2016.
    plate_4BC.geo - contains the geometry code for creating the meshes using Gmsh.
        The 2D mesh created using Gmsh were refined and optimized.
                    
    plate_4BC_40e.msh - mesh1 file from Gmsh.
    plate_4BC_40e.tms.msh - mesh1 file converted from Gmsh and ready to be used.
    plate_4BC_682e.msh - mesh2 file from Gmsh.
    plate_4BC_682e.tms.msh - mesh2 file converted from Gmsh and ready to be used.
    plate_4BC_10766e.msh - mesh3 file from Gmsh.
    plate_4BC_10766e.tms.msh - mesh3 file converted from Gmsh and ready to be used.
    
    D2_BHE_f.m - Solve the analytical problem showed in our paper.
    
    
Files for solving the mesh using the triangular node:
    calling.m - inputs configuration
    openmsh.m - this function allows the user to select the .msh file for input
        and convert the data in format suitable to TLMBHT
    proproc.m - calculate the matrix for the TLM matrix equation