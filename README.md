# tlmbht 0.2.0-alpha (work in progress)
 TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
  
 Copyright (C) 2015 to 2017 by Cornell University. All Rights Reserved.
  
 Written by Hugo Fernando Maia Milan. Contact: hugofernando@gmail.com

 Support from Brazilian National Counsel of Technological and Scientific Development (Proc. 203312/2014-7) for scholarship to HFMM.
  
 Free for educational, research and non-profit purposes.  Refer to the license file for details.
***

This README file provides instructions in how to obtain the software tlmbht and provides a brief description about it. If you want to know more about tlmbht and the theory behind it see [Why should I use the tlmbht](https://github.com/hugomilan/tlmbht/wiki/Why-should-I-use-the-tlmbht?) and the [Wiki](https://github.com/hugomilan/tlmbht/wiki).

## Installation
### Option 1

[Download here](https://github.com/hugomilan/tlmbht/releases). You will find binaries for Linux and Windows in 32 bits and 64 bits. However, compiling the code (Option 2) may yields better performance.
    
### Option 2
In simple terms:
    
    git clone https://github.com/hugomilan/tlmbht.git
    cd tlmbht
    make
    
Then, go to the folder build/ and use the binary output (named tlmbht).

#### A more complete set of instructions to compile the code
        
1. Download this repository. Type in the terminal: `git clone https://github.com/hugomilan/tlmbht.git`. Alternatively, you can use the green button on the right side of this page that says "Clone or download". Remember to extract the zip if you download it as zip.

2. Getting Eigen library. Download it [in this page](http://eigen.tuxfamily.org/index.php?title=Main_Page). Extract the files, open the extracted folder, and copy the folder Eigen (this is the folder inside the extracted folder; it contains the Eigen library's codes) and past it in the include/ folder of this project. Alternatively, you may tell the compiler (using the -I flag) where the Eigen folder is.

3. Compiling the code. It should be as simple as typing `make` on the terminal. Use `make -jn` to compile faster (i.e., for 4 threads, type `make -j4`).

I prepared different makefile configurations. You may try to use the default (only do `make`). This default configuration was prepared for the gcc/g++ compiler. If you have a different compiler, you have to edit the file /nbproject/Makefile-release.mk and include the name (or the full path, if necessary) of your C compiler in the variable CC, your C++ compiler in the variables CCC and CXX, and you can delete the variables FC and AS.

If you get any problem during the compilation, you may clean (`make clean`) and try other options to compile the code. Then, you can try `make CONF=no_flags` to disable all the optimization flags. If you have any questions, do not hesitate to contact me.
 
## Description

This is powerful Open-Source scientific code used to solve partial differential equations (pde) using the transmission-line modeling numerical method (TLM; see [Wikipedia](https://en.wikipedia.org/wiki/Transmission-line_matrix_method), [Wiki](https://github.com/hugomilan/tlmbht/wiki), and [references](https://github.com/hugomilan/tlmbht/blob/master/references.md) for more information).

The language is C/C++ and the [Eigen Library](http://www.eigen.tuxfamily.org) is used for matrix calculations. Older versions of the code and some analytical solutions, written in Octave/Matlab, are available in the folder src/octave/.

Binaries/executable files were tested in Ubuntu 16.10 and Windows 10. See Installation, Option 1 for information in how to get the binaries.

Not everything is documented yet. I'm currently working on new approaches in how to use the TLM and will provide more information on the theory in the Wiki when my papers get published. Until then, you can see the [Wiki](https://github.com/hugomilan/tlmbht/wiki), the [references](https://github.com/hugomilan/tlmbht/blob/master/references.md), the [Wikipedia page](https://en.wikipedia.org/wiki/Transmission-line_matrix_method), and/or write me an e-mail if you need help..

I hope you will find this software useful for learning/using the TLM.

You can modify the code and your modifications can be (if you want to) included into this project.

## Usage

This software does not provide a graphical user interface (yet). Everything is controlled via script and the solver is called via terminal.

Refer to the case.tlm file or to the link [How to configure a case file](https://github.com/hugomilan/tlmbht/wiki/How-to-configure-a-case-file) to see instructions on how to configure the solver.

### Linux 
Run in the terminal `tlmbht`.

### Windows
You will need to open to use the command prompt or PowerShell (which depends on your windows version). Alternatively, you can use [MinGW](http://www.mingw.org/) or [CynGw](http://www.cygwin.com/).

Then, you will run `tlmbht` in the terminal.

To get start, you may want to go to [Validations, Tutorials, and Examples](https://github.com/hugomilan/tlmbht/wiki/Validations,-Tutorials,-and-Examples) first. You will need [Octave](https://www.gnu.org/software/octave/) (free) or [Matlab](https://www.mathworks.com/products/matlab.html) (paid) to plot the results and see the comparison with the analytical results.

## Other TLM codes

Do you want to know what are the other TLM codes and software available on the internet? I compiled a list of what I found in the file [tlmcode.](https://github.com/hugomilan/tlmbht/blob/master/tlmcode.md)

## [References](https://github.com/hugomilan/tlmbht/blob/master/references.md) and [Acknowledgment](https://github.com/hugomilan/tlmbht/blob/master/acknowledgment.md):

Please, refer to these files in the root.

We use [Semantic Versioning 2.0.0](http://semver.org/)
