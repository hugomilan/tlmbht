# tlmbht 0.2.0-alpha (work in progress)
 TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
  
 Copyright (C) 2015 to 2017 by Cornell University. All Rights Reserved.
  
 Written by Hugo Fernando Maia Milan. Contact: hugofernando@gmail.com

 Support from Brazilian National Counsel of Technological and Scientific Development (Proc. 203312/2014-7) for scholarship to HFMM.
  
 Free for educational, research and non-profit purposes.
 Refer to the license file for details.
 
## Installation
### Option 1
You can use one of the binaries compiled for your system. However, compiling the code (Option 2) may yields better performance. If you have any questions, do not hesitate to contact me.

Binaries for Linux.

Binaries for Windows.

    
### Option 2
In simple terms:

    make
    
Then, go to the folder build/ and use the binary output (named tlmbht).

#### A more complete set of instructions to compile the code
        
1) Download this repository. Type in the terminal: git clone https://github.com/hugomilan/tlmbht.git). Alternatively, you can use the green button on the right side of this page (https://github.com/hugomilan/tlmbht) that says "Clone or download". Remember to extract the zip if you download it as zip.

2) Getting Eigen library. Download it at (http://eigen.tuxfamily.org/index.php?title=Main_Page). Extract the files, open the extracted folder, and copy the folder Eigen (this is the folder inside the extracted folder; it contains the Eigen library's codes) and past it in the include/ folder of this project. Alternatively, you may tell the compiler (using the -I flag) where the Eigen folder is.

3) Compiling the code. It should be as simple as typing `make` on the terminal. Use `make -jn` to compile faster (i.e., for 4 threads, type `make -j4`).

I prepared different makefile configurations. You may try to use the default (only do `make`). This default configuration was prepared for the gcc/g++ compiler. If you have a different compiler, you have to edit the file /nbproject/Makefile-release.mk and include the name (or the full path, if necessary) of your C compiler in the variable CC, your C++ compiler in the variables CCC and CXX, and you can delete the variables FC and AS.

If you get any problem during the compilation, you may clean (`make clean`) and try other options to compile the code. Then, you can try `make CONF=no_flags` to disable all the optimization flags. If you have any questions, do not hesitate to contact me.
 
## Description

This is an Open-Source scientific code used to solve partial differential equations (pde) using the Transmission-line modeling numerical method (TLM; see [Wikipedia](https://en.wikipedia.org/wiki/Transmission-line_matrix_method) and [references](https://github.com/hugomilan/tlmbht/blob/master/references.md) for more information). In this version, the following partial differential equations (pde) are supported:

1) Heat transfer in 1D, 2D, and 3D.

2) Bio-heat transfer in 1D, 2D, and 3D (Pennes, 1948; Minkowycz and Sparrow, 2009; Xu et al., 2011).

3) Diffusion in 1D, 2D, and 3D.

The numerical solution is based on a mesh of geometrical elements (or control volumes) that can be created using the [Gmsh software](www.gmsh.info). The geometrical elements currently supported are based on my research on TLM (see the reference file for citation to the original papers) and are:

1) One-dimension.

 1.1) Line in Cartesian coordinates.
 
2) Two-dimensions.

 2.1) Triangle in Cartesian coordinates.
 
3) Three-dimensions.

 3.1) Tetrahedron in Cartesian coordinates.

The language is C/C++ and the [Eigen Library](www.eigen.tuxfamily.org) is used for matrix calculations. Older versions of the code written in Octave/Matlab and some analytical solutions used to validate the code are available in the folder src/octave/.

Binaries/executable files were tested in Ubuntu 16.10 and Windows 10. See Installation:Option 1 to know how to get the binaries.

Not everything is documented yet. I'm currently working on new approaches in how to use the TLM and will provide more information on the theory in the Wiki when my papers get published. Until then, you can see the reference and/or the [Wikipedia page](https://en.wikipedia.org/wiki/Transmission-line_matrix_method).

I hope you will find this software useful for learning/using the TLM.

Please, fell free to use the software for educational, research and non-profit purposes. You can modify it and your modifications can be (if you want to) included into this project. For profitable purpose, please contact the Center for Technology Licensing at Cornell University (see the LICENSE file for more information).

## Usage

This software does not provide a graphical user interface (yet). Everything is controlled via script and the solver is called via terminal.

Refer to the case.tlm file to see instructions on how to configure the solver.

### Linux 
Run in the terminal `tlmbht`.

### Windows
You will need to open to use the command prompt or PowerShell (which depends on your windows version). Alternatively, you can use [MinGW](http://www.mingw.org/) or [CynGw](http://www.cygwin.com/).

Then, you will run `tlmbht` in the terminal.

To get start, you may want to try the validations first. You will need [Octave](https://www.gnu.org/software/octave/) (free) or [Matlab](https://www.mathworks.com/products/matlab.html) (paid) to plot the results and see the comparison with the analytical results.

## [References](https://github.com/hugomilan/tlmbht/blob/master/references.md) and [Acknowledgements](https://github.com/hugomilan/tlmbht/blob/master/acknowledgements.md):

Please, refer to these files in the root.

We use [Semantic Versioning 2.0.0](http://semver.org/)
