# tlmbht 0.2.0 (work in progress)
 TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
  
 Copyright (C) 2015 to 2017 by Cornell University. All Rights Reserved.
  
 Written by Hugo Fernando Maia Milan.
 Support from Brazilian National Counsel of Technological and Scientific Development (Proc. 203312/2014-7) for scholarship to HFMM.
  
 Free for educational, research and non-profit purposes.
 Refer to the license file for details.
 
# Compilation
 
    *) How to make this code?
    *) Download Eigen (http://eigen.tuxfamily.org/index.php?title=Main_Page).
    *)
 
# Description

This is an Open-Source scientific code used to solve partial differential equations (pde) using the Transmission-line modeling (TLM; see Wiki and references for more information) numerical method. In this version, the following pde are supported:
    1) Heat transfer in 1D, 2D, and 3D.
    2) Bio-heat transfer in 1D, 2D, and 3D (Pennes, 1948; Minkowycz and Sparrow, 2009; Xu et al., 2011).
    3) Diffusion in 1D, 2D, and 3D.

The numerical solution is based on a mesh of geometrical elements (or control volumes) that can be created using the Gmsh software (www.gmsh.info). The geometrical elements currently supported are based on my research on TLM (see the reference file for citation to the original papers) and are:
    1) One-dimension.
        1.1) Line in Cartesian coordinates.
    2) Two-dimensions.
        2.1) Triangle in Cartesian coordinates.
        2.2) Quadrangle in Cartesian coordinates.
    3) Three-dimensions.
        3.1) Tetrahedron in Cartesian coordinates.
        3.2) Hexahedron in Cartesian coordinates.

The language is C/C++ and Eigen Library 3.3.1 (www.eigen.tuxfamily.org) is used for matrix calculations. Some analytical solutions and older versions of the code are available in Octave/Matlab in the folder src/octave/.

Binaries/executable files (tested in Ubuntu 16.10) are available at https://github.com/hugomilan/tlmbht/releases. I have plans to release binaries for Windows and Mac too (when?), but you can compile the source code to your system if you prefer.

I hope you will find it useful for learning/using the TLM. I have plans to provide more information on the theory in the Wiki. Until then, you can see the reference and/or the Wikipedia page (https://en.wikipedia.org/wiki/Transmission-line_matrix_method).

Please, fell free to use the software for educational, research and non-profit purposes. You can modify it and your modifications can be (if you want to) included into this project. For profitable purpose, please contact the Center for Technology Licensing at Cornell University (see the LICENSE file for more information).

<b>References and Acknoledgements:</b> Please, refer to these files in the root.

We use Semantic Versioning 2.0.0 http://semver.org/
