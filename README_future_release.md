# tlmbht 0.2.0 (work in progress)
 TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
  
 Copyright (C) 2015 to 2017 by Cornell University. All Rights Reserved.
  
 Written by Hugo Fernando Maia Milan. Contact: hugofernando@gmail.com

 Support from Brazilian National Counsel of Technological and Scientific Development (Proc. 203312/2014-7) for scholarship to HFMM.
  
 Free for educational, research and non-profit purposes.
 Refer to the license file for details.
 
# Installation
    ## Option 1
        You can use one of the binaries compiled for your system. However, compile the code (Option 2) for better performance. If you have any questions, do not hesitate to contact me.

        Binaries for Linux.

        Binaries for Windows.

    
    ## Option 2
        Here are the instructions to compile the code. You may get better performance if you compile the code in your machine.
        
        1) Download this repository. Type in the terminal: git clone https://github.com/hugomilan/tlmbht.git). Alternatively, you can use the green button on the right side of this page (https://github.com/hugomilan/tlmbht) that says "Clone or download". Remember to extract the zip if you download it as zip.

        2) Getting Eigen library. Download it at (http://eigen.tuxfamily.org/index.php?title=Main_Page). Extract the files, Open the folder extracted, and copy the folder Eigen (this is the folder inside the extracted folder; it contains the Eigen library's codes) and past it in the include/ folder of this project. Alternatively, you may tell the compiler (using the -I flag) where the Eigen folder is.

        3) Compiling the code. It should be as simple as typing 'make' on the terminal. Use 'make -jn' to compile faster using 'n' threads (i.e., for 4 threads, do 'make -j4').

        I prepared different makefile configurations. You may try to use the default (only do 'make'). However, this default configuration was prepared for the gcc/g++ compiler. If you have a different compiler, you have to edit the file /nbproject/Makefile-release.mk and include the name (or the full path, if necessary) of your C compiler in the variable CC, your C++ compiler in the variables CCC and CXX, and you can delete the variables FC and AS.

        If you get any problem during the compilation, you may clean ('make clean') and try other options to compile the code. Then, you can try 'make CONF=no_flags' to disable all the optimization flags. If you have any questions, do not hesitate to contact me.
 
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
