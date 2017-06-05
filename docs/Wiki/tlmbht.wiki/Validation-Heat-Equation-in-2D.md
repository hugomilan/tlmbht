Here, I will walk you through in how to use the TLM method to solve the Heat equation in 2D with the element Triangle and we will also validated the tlmbht predictions using an analytical solution.

Everything you need is available in the folder /vte/generalDiffusion/2D/TriangleNode. This folder was downloaded when you downloaded the source files. If you did not download the source files, you can download it from [here](https://github.com/hugomilan/tlmbht/tree/master/vte/generalDiffusion/2D/TriangleNode). If you do not want to, you do not need to download or install anything--all the relevant outputs will be shown here. If you want to download tlmbht, go to [releases](https://github.com/hugomilan/tlmbht/releases) and get the most updated version of tlmbht.

We will follow the 5 steps [showed here](https://github.com/hugomilan/tlmbht/wiki).

### 1) Create the geometry of the problem.

We will consider a simple two-dimensional problem that has analytical solution. In this problem, we will include heat source, two constant temperatures boundary conditions, one constant heat flux boundary condition, and one adiabatic boundary condition. The problem geometry is shown below.

![Geometry of the problem for the 2D problem](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Heat_Problem.png "Geometry of the problem for the 2D problem")

Note that we are building up on the problem description of the [validation and tutorial for Heat 1D.](https://github.com/hugomilan/tlmbht/wiki/Validating-Heat-Equation-in-1D-with-line-elements) We assume a fixed temperature T<sub>S</sub>, a constant heat flux q<sup>''</sup>), and a constant core temperature (T<sub>C</sub>).

Now that we defined the problem, we need to draw the problem geometry using a format that tlmbht knows how to read. This was already done for you. The geometry of the problem was created using [gmsh](http://www.gmsh.info). The script file is plate_4BC.geo. Note in the script file that we put 'Physical tags' in the Surface geometry and in the Lines. We did this so that we can tell tlmbht which tag is for material (number 7) and which tag is for boundary (number 8, 9, 10, and 11).

![Geometry of the problem drawn in gmsh](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Geometry.png "Geometry of the problem draw in gmsh")

Now we are ready to create the mesh.

### 2) Create the mesh of the problem.

The mesh was easily created using gmsh/Mesh/2D and Refined by Splitting. The mesh file (already converted to tlmbht native mesh format) is plate_4BC_682e.tbn. It contains 374 points (vertices), 64 line segments, and 682 triangles (elements).

![Triangular mesh of the problem geometry](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Mesh.png "Triangular mesh of the problem geometry")

Now we are ready to create the description of the problem

### 3) Create the description of the problem.

The description of the problem is in the file cheat2Tr.tlm. The script file is a text file. In particular, this script is long because you will see explanations in how to use it along with all the available options. I would like to emphasis only two headers, which you are more likely to change if you want to test different inputs with this script latter: Material and Boundary.

In the Material header, you set the values of the material variables, such as density, specific heat, etc. After you finish this tutorial, try to change the value of the 'source' and observe what happens with the temperature profile of the solution.

In the Boundary header, you set what boundary that tag is and its values. After finishing this tutorial, you may try different values for the boundary conditions or even different boundaries (such as convection and heat flux). See the Equation header for how to use other boundary types.

If you want more information in how to configure the case file, go to [How to configure a case file.](https://github.com/hugomilan/tlmbht/wiki/How-to-configure-a-case-file)

### 4) Solve the problem.

If you have the tlmbht binary in your path environment, simply type `tlmbht cheat2Tr.tlm`. If you don't have it in your path environment, you may copy the binary to the folder /vte/generalDiffusion/2D/TriangleNode/ and then type `./tlmbht cheat2Tr.tlm`. In some seconds, the calculation will be done.

If you want to see what is going on internally, you may run tlmbht with --verbose. If you want to see how long does it take to run tlmbht, you may run it with --timing. The command to run with both is simply `tlmbht cheat2Tr.tlm --verbose --timing`.

Now we are ready to visualize the output and compare what the TLM predictions with an analytical prediction.

### 5) Visualize the output.

After you have run tlmbht, it created the output file cheat2Tr.m. In this tutorial, you do not need to worry about this file. We will run a script that automatically loads the data into Octave/Matlab. The script is in the file vheat2Tr.m, which calls the analytical solver function D2_BHE_f.m.

This part should be as simple as opening vheat2Tr.m in Octave/Matlab and running it (press key F5). It will show you two plots and textual information. The figure below shows the two plots and part of the textual information. Temperature is shown in the left figure and heat flux is shown on the right figure. The analytical predictions are shown in blue, the tlmbht predictions are shown in red, and the green shows the difference of the calculated heat fluxes. In the temperature plots, asterisks represent temperatures calculated at the center of the TLM nodes and the circles represent the temperatures calculated between nodes. You can see that the predictions are almost identical, which you can confirm by looking at the textual information that tells you that the mean temperature error was 0.95 % and the mean heat flux difference was 8 MW/m<sup>2</sup> (which correspond to a fraction of 0.08 of the input heat flux). In this problem, the mean heat flux error is not a viable validation variable because the heat flux is close to zero in some parts of the mesh and, hence, very small differences yields very large percentage errors.

![Predictions using tlmbht with triangles vs. predictions using analytical solution for the 2D problem](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Heat_Result.png "Predictions using tlmbht with triangles vs. predictions using analytical solution for the 2D problem")

You are not satisfied with this accuracy? Well... this is a numerical method; we will never get the same exact numbers as the analytical solution gives. Besides, this analytical solution is based on a infinity sum and, by default, the script vheat2Tr.m only considers the first 50 terms. If you insist, you can try to increase the number of terms in the analytical solution. Then, as it is true for numerical methods, you can try to increase the number of elements and/or decrease the time-step.

### Done!

I hope you have enjoyed this validation/tutorial section! You may now try to change the case file (cheat2Tr.tlm) and see what happens. Remember that if you change any parameter in the cheat2Tr.tlm file you will need to do the same in the vheat2Tr.m file. Except for time and space positions, they are not linked.

Remember: you are using a powerful numerical solver. You do not need to be constrained by solutions that can be solved analytically. Explore! Try different boundary conditions, include more materials, etc. Make this problem more realistic!

If you want to, you can move to the [validation and tutorial for Heat 3D.](https://github.com/hugomilan/tlmbht/wiki/Validation-Heat-Equation-in-3D)