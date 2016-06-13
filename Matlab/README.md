Transmission-line Modeling Method applied to BioHeat Transfer Problems. Version 0.1. Copyright © 2015 to 2016 by Cornell University. Written by Hugo Fernando Maia Milan (hugofernando@gmail.com). All Rights Reserved.

Please, see the files for deep explanation.

This folder contains:

&bull; D2_BHE_f.m - Solve the 2D analytical problem showed in (Milan and Gebremedhin, 2016a).

&bull; D3_BHE_f.m - Solve the 3D analytical problem showed in (Milan and Gebremedhin, 2016b).

&bull; plate_4BC.geo - contains the geometry code for creating the meshes using Gmsh. The 2D mesh created using Gmsh were refined and optimized.
  
&bull; plate_4BC_40e.msh - mesh1 file from Gmsh.
  
&bull; plate_4BC_40e.tms.msh - mesh1 file converted from Gmsh (using openmsh.m) and ready to be used.
  
&bull; plate_4BC_682e.msh - mesh2 file from Gmsh.
  
&bull; plate_4BC_682e.tms.msh - mesh2 file converted from Gmsh (using openmsh.m) and ready to be used.
  
&bull; plate_4BC_10766e.msh - mesh3 file from Gmsh.
  
&bull; plate_4BC_10766e.tms.msh - mesh3 file converted from Gmsh (using openmsh.m) and ready to be used.


  
  
Files for solving the mesh using the triangular node:
    calling.m - inputs configuration
    openmsh.m - this function allows the user to select the .msh file for input
        and convert the data in format suitable to TLMBHT
    proproc.m - calculate the matrix for the TLM matrix equation

<b>References and Acknoledgements:</b> Please, refer to these files in the root.
