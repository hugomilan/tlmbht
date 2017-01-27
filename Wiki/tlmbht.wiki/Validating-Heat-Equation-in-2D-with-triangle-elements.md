Here, I will walk you through in how to use the TLM method to solve the Heat equation in 2D with the element Triangle and we will also validated the tlmbht predictions using an analytical solution.

Everything you need is available in the folder /vte/generalDiffusion/2D/TriangleNode. This folder was downloaded when you downloaded the source files. If you did not download the source files, you can download it from [here](https://github.com/hugomilan/tlmbht/tree/master/vte/generalDiffusion/2D/TriangleNode). If you do not want to, you do not need to download or install anything--all the relevant outputs will be shown here. If you want to download tlmbht, go to [releases](https://github.com/hugomilan/tlmbht/releases) and get the most updated version of tlmbht.

In this validation, we will follow the 5 steps [showed here](https://github.com/hugomilan/tlmbht/wiki).

### 1) Create the geometry of the problem.

We are building up from the problem description of the [validation for Heat 1D equation with line elements](https://github.com/hugomilan/tlmbht/wiki/Validating-Heat-Equation-in-1D-with-line-elements). Here, we will consider a simple two-dimensional problem that has analytical solution. In this problem, we will include heat source, two constant temperatures boundary conditions (constant core temperature T<sub>C</sub>, and constant surface temperature T<sub>S</sub>), one constant heat flux boundary condition (q<sup>''</sup>), and one adiabatic boundary condition. The problem geometry is shown below.

![Geometry of the problem used to validate the triangle element for the Heat equation in 2D](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Heat_Problem.png "Geometry of the problem used to validate the triangle element for the Heat equation in 2D")

Now that we defined the problem, we need to draw the problem geometry using a format that tlmbht knows how to read. This was already done for you. The geometry of the problem was created using [gmsh](http://www.gmsh.info). The script file is plate_4BC.geo. Note in the script file that we put 'Physical tags' in the Surface geometry and in the Lines. We did this so that we can tell tlmbht which tag is for material (number 7) and which tag is for boundary (number 8, 9, 10, and 11).

![Geometry of the problem drawn in gmsh](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Geometry.png "Geometry of the problem draw in gmsh")

Now we are ready to create the mesh.

### 2) Create the mesh of the problem.

The mesh was easily created using gmsh/Mesh/2D and Refined by Splitting. The mesh file (already converted to tlmbht native mesh format) is plate_4BC_628e.tbn. It contains 347 points (vertices), 64 line segments, and 628 triangles (elements).

![Triangular mesh of the problem geometry](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Mesh.png "Triangular mesh of the problem geometry")

Now we are ready to create the description of the problem

### 3) Create the description of the problem.

The description of the problem is in the file cheat2Tr.tlm. Here, I will explain how this case file was configured. However, if you are in a hurry, you can go skip this part and move on to the next section.

#### Explanation of this case file

The case file is structured in headers and I will go over every header in the file cheat2Tr.tlm. This script file is similar to the script file for [validation for Heat 1D equation with line elements](https://github.com/hugomilan/tlmbht/wiki/Validating-Heat-Equation-in-1D-with-line-elements). The differences are the inclusion of one additional boundary condition and changing to two-dimensions.

The first header you see (Simulation), is used to configure parameters common to the equations of the problem. In this case, we are only configuring the output extension to the 'm' (Octave/Matlab m format) so that it is easier to run the validation algorithm.

    Simulation
    {
        output extension = m;
    }

The Mesh header contains information about the mesh. Here we tell the software which mesh to use ("file name") and in what format it is ("input format").

    Mesh
    {
        file name = plate_4BC_628e;
        input format = tlmtbn;
    }

The Equation header tells the software what equation it should solve and how. You can have different Equations headers to defined different equations being solved simultaneously in a multi-physical problem. In the Equation header, we define the type of equation and give it a name. The name is essential to link what is in the Equation header with what is in the Material and Boundary headers. Then, we define the dimension of the problem and tell tlmbht to solve this problem in the time-domain ("Solve = dynamic"). Since we are solving a time-domain problem, we need to define the time-step and the final simulation time. The "time-jump" is a configuration that tells tlmbht after how many time-steps it should save the output data (which is defined in the "save" options). Here, we are saving at every 1000 time-steps, which is equivalent to saving at every 0.1 ms. Note that these 1000 time-steps are all solved.

    Equation
    {
        type = heat;
        equation name = heat_name;
        dimensions = 2;
        Solve = dynamic;
        time-step = 1e-7;
        time-jump = 1000;
        final time = 1e-3;
        save = scalar;
        save = scalar between;
        save = vector;
    }

The Material header defines the properties of the equation. You can have different Material headers for different properties in the same problem. We tell tlmbht to what equation this Material header is referred to by giving it the equation name ("heat_name"). In number, we give the Material header the Physical tag number that we gave to the line during the mesh generation. Therefore, the options we input here are going to be applied to the line with tag number 7. Finally, we defined density, specific heat, thermal conductivity, volumetric heat source, and initial temperature (required for time-domain simulations).

    Material
    {
        equation = heat_name;
        number =  7;
        density = 2700;
        specific heat = 900;
        thermal conductivity = 205;
        source = 5000;
        initial temperature = 100;
    }

The Boundary header defines the boundary conditions. You can have different Boundary headers for different boundary conditions and for each equation being solved. We proceed similar to what we did in the Material header. That is, we define the equation name and the Physical tag number that this boundary will be applied to. For the boundary conditions to defined temperatures, we define the "temperature" options that tells tlmbht that these boundaries are for defined temperature values. For the boundary condition with defined heat flux, we define the "heat flux" option. We do not need to include the adiabatic boundary condition because it is automatically applied to boundaries not defined by Boundary headers.

    Boundary
    {
        equation = heat_name;
        number = 10;
        Temperature = 100;
    }

    Boundary
    {
        equation = heat_name;
        number = 11;
        Temperature = 1000;
    }

    Boundary
    {
        equation = heat_name;
        number = 8;
        Heat flux = 1e8;
    }

The file cheat2Tr_full.tlm contains additional explanation about the input. If you want more information in how to configure the case file, go to [How to configure a case file.](https://github.com/hugomilan/tlmbht/wiki/How-to-configure-a-case-file)

### 4) Solve the problem.

If you have the tlmbht binary in your path environment, simply type `tlmbht cheat2Tr.tlm`. If you don't have it in your path environment, you may copy the binary to the folder /vte/generalDiffusion/2D/TriangleNode/ and then type `./tlmbht cheat2Tr.tlm`. In some seconds, the calculation will be done.

If you want to see what is going on internally, you may run tlmbht with --verbose. If you want to see how long does it take to run tlmbht, you may run it with --timing. The command to run with both is simply `tlmbht cheat2Tr.tlm --verbose --timing`.

Now we are ready to visualize the output and compare the TLM predictions with an analytical prediction.

### 5) Visualize the output.

After you have run tlmbht, it created the output file cheat2Tr.m. In this tutorial, you do not need to worry about this file. We will run a script that automatically loads the data into Octave/Matlab. The script is in the file vheat2Tr.m, which calls the analytical solver function D2_BHE_f.m.

This part should be as simple as opening vheat2Tr.m in Octave/Matlab and running it (press key F5). It will show you two plots and textual information. The figure below shows the two plots and part of the textual information. Temperature is shown in the left figure and heat flux is shown on the right figure. The analytical predictions are shown in blue, the tlmbht predictions are shown in red, and the green shows the difference of the calculated heat fluxes. In the temperature plots, asterisks represent temperatures calculated at the center of the TLM nodes and the circles represent the temperatures calculated between nodes. You can see that the predictions are almost identical, which you can confirm by looking at the textual information that tells you that the mean temperature error was 0.98 % and the mean heat flux difference was 8.1 MW/m<sup>2</sup> (which correspond to a fraction of 0.081 of the input heat flux). In this problem, the mean heat flux error is not a viable validation variable because the heat flux is close to zero in some parts of the mesh and, hence, very small differences yields very large percentage errors.

![Predictions using tlmbht with triangles vs. predictions using analytical solution for the 2D problem](https://github.com/hugomilan/tlmbht/blob/master/docs/images/2D_Triangle_Heat_Result.png "Predictions using tlmbht with triangles vs. predictions using analytical solution for the 2D problem")

You are not satisfied with this accuracy? Well... this is a numerical method; we will never get the same exact numbers as the analytical solution gives. Besides, this analytical solution is based on a infinity sum and, by default, the script vheat1Li.m only considers the first 50 terms. If you insist, you can try to increase the number of terms in the analytical solution. Then, as it is true for numerical methods, you can try to increase the number of elements and/or decrease the time-step. These changes will increase the computational time but also increase the accuracy.

### Done!

I hope you have enjoyed this validation section! You may now try to change the case file (cheat2Tr.tlm) and see what happens. Remember that if you change any parameter in the cheat2Tr.tlm file you will need to do the same in the vheat2Tr.m file. Except for time and space positions, they are not linked.

Remember: you are using a powerful numerical solver. You do not need to be constrained by solutions that can be solved analytically. Explore! Try different boundary conditions, include more materials, etc. Make this problem more realistic!

If you want to, you can move to the [validation for Heat 3D.](https://github.com/hugomilan/tlmbht/wiki/Validating-Heat-Equation-in-3D-with-tetrahedron-elements)