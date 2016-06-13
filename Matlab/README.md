Transmission-line Modeling Method applied to BioHeat Transfer Problems. Version 0.1. Copyright © 2015 to 2016 by Cornell University. Written by Hugo Fernando Maia Milan (hugofernando@gmail.com). All Rights Reserved.

Please, see the files for deep explanation.

This folder contains:

&bull; D2_BHE_f.m - Solve the 2D analytical problem showed in (Milan and Gebremedhin, 2016a).

&bull; D3_BHE_f.m - Solve the 3D analytical problem showed in (Milan and Gebremedhin, 2016b).

&bull; calling.m - The main function. In this function the inputs are selected.

&bull; openmsh.m - Selected the .msh file (from Gmsh; Geuzaine and Remacle, 2009) to read and converts to the TLMBHT to interprets.

&bull; parallelepiped_6BC.geo - contains the geometry code for creating the meshes using Gmsh. The 3D mesh created using Gmsh were refined and optimized.
  
&bull; parallelepiped_6BC_1368e.msh - mesh1 3D file from Gmsh.
  
&bull; plate_6BC_1368e.tms.msh - mesh1 3D file converted from Gmsh (using openmsh.m) and ready to be used.
  
&bull; parallelepiped_6BC_206e.msh - mesh1 3D file from Gmsh.
  
&bull; plate_6BC_206e.tms.msh - mesh1 3D file converted from Gmsh (using openmsh.m) and ready to be used.
  
&bull; parallelepiped_6BC_69941e.msh - mesh1 3D file from Gmsh.
  
&bull; plate_6BC_69941e.tms.msh - mesh1 3D file converted from Gmsh (using openmsh.m) and ready to be used.

&bull; plate_4BC.geo - contains the geometry code for creating the meshes using Gmsh. The 2D mesh created using Gmsh were refined and optimized.
  
&bull; plate_4BC_40e.msh - mesh1 2D file from Gmsh.
  
&bull; plate_4BC_40e.tms.msh - mesh1 2D file converted from Gmsh (using openmsh.m) and ready to be used.
  
&bull; plate_4BC_682e.msh - mesh2 2D file from Gmsh.
  
&bull; plate_4BC_682e.tms.msh - mesh2 2D file converted from Gmsh (using openmsh.m) and ready to be used.
  
&bull; plate_4BC_10766e.msh - mesh3 2D file from Gmsh.
  
&bull; plate_4BC_10766e.tms.msh - mesh3 2D file converted from Gmsh (using openmsh.m) and ready to be used.

&bull; proproc.m - Calculate the matrixes for solving the problem.

<b>References and Acknoledgements:</b> Please, refer to these files in the root.
