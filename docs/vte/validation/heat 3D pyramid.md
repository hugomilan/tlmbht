---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page_eqAMS_Disqus
title: Validation of TLMBHT to solve the Heat equation in time-domain for three-dimensions using Pyramid elements
permalink: vte/validation/heat-3D-Py.html
---

<span style="color:#697473">Jan 31, 2017</span> by [**Hugo Milan**](https://hugomilan.github.io/)

Here, I will walk you through in how to use the TLM method to solve the Heat equation in 3D with the element Tetrahedron by comparing the tlmbht predictions with [analytical solution]({{ site.baseurl }}{% link theory/ana/heat/heat 3D TTqqqq.md %}) predictions. 

Everything you need is available in the folder /vte/generalDiffusion/3D/TetrahedronNode. This folder was downloaded when you downloaded the source files. If you did not download the source files, you can download it from [here](https://github.com/hugomilan/tlmbht/tree/master/vte/generalDiffusion/3D/TetrahedronNode). If you do not want to, you do not need to download or install anything--all the relevant outputs will be shown here. If you want to download tlmbht, go to [releases](https://github.com/hugomilan/tlmbht/releases) and get the most updated version of tlmbht.

In this validation, we will follow the 5 steps [showed here]({{ site.baseurl }}{% link vte/how tlmbht.md %}).

### 1) Create the geometry of the problem.

We are building up from the problem description of the [validation for heat equation in 2D with Triangular elements]({{ site.baseurl }}{% link vte/validation/heat 2D triangle.md %}). Here, we will consider a simple three-dimensional problem that has [analytical solution]({{ site.baseurl }}{% link theory/ana/heat/heat 3D TTqqqq.md %}). In this problem, we will include effect of volumetric heat source, two constant temperature boundary conditions (constant core temperature T<sub>C</sub>, and constant surface temperature T<sub>S</sub>), two constant heat flux boundary condition (q<sub>x</sub> and q<sub>z</sub>), and two adiabatic boundary condition. The problem geometry is shown below.

<img src="{{ site.baseurl }}/assets/images/vte/3D_Tetrahedron_Heat_Problem.png" alt="Geometry of the problem used to validate the tetrahedron element for the Heat equation in 3D" width="500">

This geometry is a parallelepiped in a three-dimensional space, as you can see in the three-dimensional model below.
<div class="sketchfab-embed-wrapper"><iframe width="640" height="480" src="https://sketchfab.com/models/0ec8af1e3cc6475192fd38a75d4be8e7/embed" frameborder="0" allowvr allowfullscreen mozallowfullscreen="true" webkitallowfullscreen="true" onmousewheel=""></iframe>

<p style="font-size: 13px; font-weight: normal; margin: 5px; color: #4A4A4A;">
    <a href="https://sketchfab.com/models/0ec8af1e3cc6475192fd38a75d4be8e7?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">Parallelepiped 0.75 x 1 x 0.375</a>
    by <a href="https://sketchfab.com/hugomilan?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">hugomilan</a>
    on <a href="https://sketchfab.com?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">Sketchfab</a>
</p>
</div>

Now that we defined the problem, we need to draw the problem geometry using a format that tlmbht knows how to read. This was already done for you. The geometry of the problem was created using [gmsh](http://www.gmsh.info). The script file is parallelepiped_6BC.geo. Note in the script file that we put 'Physical tags' in the volume geometry and in the surfaces. We did this so that we can tell tlmbht which tag is for material (number 33) and which tag is for boundary (number 15, 18, 21, 24, 27, and 30).

![Geometry of the problem drawn in gmsh]({{ site.baseurl }}/assets/images/vte/3D_Tetrahedron_Geometry.png "Geometry of the problem draw in gmsh")

Now we are ready to create the mesh.

### 2) Create the mesh of the problem.

The mesh was easily created using gmsh/Mesh/3D. The mesh file (already converted to tlmbht native mesh format) is parallelepiped_6BC_1572e.tbn. It contains 464 points (vertices), 762 triangles, and 1572 tetrahedrons.

![Tetrahedron mesh of the problem geometry]({{ site.baseurl }}/assets/images/vte/3D_Tetrahedron_Mesh.png "Tetrahedron mesh of the problem geometry")

Now we are ready to create the description of the problem

### 3) Create the description of the problem.

The description of the problem is in the file cheat3Te.tlm. Here, I will explain how this case file was configured. However, if you are in a hurry, you can go skip this part and move on to the next section.

#### Explanation of this case file

The case file is structured in headers and I will go over every header in the file cheat3Te.tlm. This script file is similar to the script file for [validation for Heat equation in 2D with Triangular elements]({{ site.baseurl }}{% link vte/validation/heat 2D triangle.md %}). The differences are the inclusion of one additional boundary condition and changing to three-dimensions.

The first header you see (Simulation), is used to configure parameters common to the equations of the problem. In this case, we are only configuring the output extension to the 'm' (Octave/Matlab m format) so that it is easier to run the validation algorithm.

    Simulation
    {
        output extension = m;
    }

The Mesh header contains information about the mesh. Here we tell the software which mesh to use ("file name") and in what format it is ("input format").

    Mesh
    {
        file name = parallelepiped_6BC_1572e;
        input format = tlmtbn;
    }

The Equation header tells the software what equation it should solve and how. You can have different Equations headers to defined different equations being solved simultaneously in a multi-physical problem. In the Equation header, we define the type of equation and give it a name. The name is essential to link what is in the Equation header with what is in the Material and Boundary headers. Then, we define the dimension of the problem and tell tlmbht to solve this problem in the time-domain ("Solve = dynamic"). Since we are solving a time-domain problem, we need to define the time-step and the final simulation time. The "time-jump" is a configuration that tells tlmbht after how many time-steps it should save the output data (which is defined in the "save" options). Here, we are saving at every 100 time-steps, which is equivalent to saving at every 100 ms. Note that these 100 time-steps are all solved.

    Equation
    {
        type = heat;
        equation name = heat_name;
        dimensions = 2;
        Solve = dynamic;
        time-step = 1e-3;
        time-jump = 100;
        final time = 1;
        save = scalar;
        save = scalar between;
        save = vector;
    }

The Material header defines the properties of the equation. You can have different Material headers for different properties in the same problem. We tell tlmbht to what equation this Material header is referred to by giving it the equation name ("heat_name"). In number, we give the Material header the Physical tag number that we gave to the elements during the mesh generation. Therefore, the options we input here are going to be applied to the elements with tag number 33. Finally, we defined density, specific heat, thermal conductivity, blood perfusion, blood density, blood specific heat, blood temperature, internal heat generation, and initial temperature (required for time-domain simulations).

    Material
    {
        equation = heat_name;
        number =  33;
        density = 1200;
        specific heat = 3200;
        thermal conductivity = 0.3;
        source = 5000;
        initial temperature = 37;
    }

The Boundary header defines the boundary conditions. You can have different Boundary headers for different boundary conditions and for each equation being solved. We proceed similar to what we did in the Material header. That is, we define the equation name and the Physical tag number that this boundary will be applied to. For the boundary conditions to defined temperatures, we define the "temperature" options that tells tlmbht that these boundaries are for defined temperature values. For the boundary condition with defined heat flux, we define the "heat flux" option. We do not need to include the adiabatic boundary condition because it is automatically applied to boundaries not defined by Boundary headers.

    Boundary
    {
        equation = heat_name;
        number = 27;
        Temperature = 100;
    }

    Boundary
    {
        equation = heat_name;
        number = 30;
        Temperature = 200;
    }

    Boundary
    {
        equation = heat_name;
        number = 18;
        Heat flux = 2e4;
    }

    Boundary
    {
        equation = heat_name;
        number = 24;
        Heat flux = 1e4;
    }

The file cheat3Te_full.tlm contains additional explanation about the input. If you want more information in how to configure the case file, go to [How to configure a case file.]({{ site.baseurl }}{% link vte/configure case file.md %})

### 4) Solve the problem.

#### [**Click here if you have Windows and need help to run tlmbht in your machine.**]({{ site.baseurl }}{% link vte/run on windows.md %})

If you have the tlmbht binary in your path environment, simply type `tlmbht cheat3Te.tlm`. If you don't have it in your path environment, you may copy the binary to the folder /vte/generalDiffusion/3D/TetrahedronNode/ and then type `./tlmbht cheat3Te.tlm`. In some seconds, the calculation will be done.

If you want to see what is going on internally, you may run tlmbht with --verbose. If you want to see how long does it take to run tlmbht, you may run it with --timing. The command to run with both is simply `tlmbht cheat3Te.tlm --verbose --timing`.

Now we are ready to visualize the output and compare the TLM predictions with an analytical prediction.

### 5) Visualize the output.

After you have run tlmbht, it created the output file cheat3Te.m. In this tutorial, you do not need to worry about this file. We will run a script that automatically loads the data into Octave/Matlab. The script is in the file vheat3Te.m, which calls the analytical solver function D3_HEAT_f.m ([click here to read more about the analytical solution and how to use this function]({{ site.baseurl }}{% link theory/ana/heat/heat 3D TTqqqq.md %})).

This part should be as simple as opening vheat3Te.m in Octave/Matlab and running it (press key F5). It will show you two plots along the y-axis and textual information. The figure below shows the two plots and part of the textual information. Temperature is shown in the left figure and heat flux is shown on the right figure. The analytical predictions are shown in blue, the tlmbht predictions are shown in red, and the green shows the difference of the calculated heat fluxes. In the temperature plots, asterisks represent temperatures calculated at the center of the TLM nodes and the circles represent the temperatures calculated between nodes. You can see that the predictions are similar, which you can confirm by looking at the textual information that tells you that the mean temperature error was 1.37 % and the mean heat flux difference was 3938 W/m<sup>2</sup> (which correspond to a fraction of 0.197 of the highest input heat flux). In this problem, the mean heat flux error is not a viable validation variable because the heat flux is close to zero in some parts of the mesh and, hence, very small differences yields very large percentage errors.

![Predictions using tlmbht with tetrahedrons vs. predictions using analytical solution for the 3D problem]({{ site.baseurl }}/assets/images/vte/3D_Tetrahedron_Heat_Result.png "Predictions using tlmbht with tetrahedrons vs. predictions using analytical solution for the 3D problem")

You are not satisfied with this accuracy? Well... this is a numerical method; we will never get the same exact numbers as the analytical solution gives. Besides, this analytical solution is based on a infinity sum and, by default, the script vheat3Te.m considers up to the first 100 terms. If you insist, you can try to increase the number of terms in the analytical solution. Then, as it is true for numerical methods, you can try to increase the number of elements and/or decrease the time-step. These changes will increase the computational time but also increase the accuracy.

### Done!

I hope you have enjoyed this validation section! You may now try to change the case file (cheat3Te.tlm) and see what happens. Remember that if you change any parameter in the cheat3Te.tlm file you will need to do the same in the vheat3Te.m file. Except for time and space positions, they are not linked.

Remember: you are using a powerful numerical solver. You do not need to be constrained by solutions that can be solved analytically. Explore! Try different boundary conditions, include more materials, etc. Make this problem more realistic!

You might be wondering **how does the tlmbht models for Tetrahedron and Hexahedron elements change?** They don't change. These two validations solve the same problem. The only difference between them is that the mesh for the validation using Tetrahedron elements has only tetrahedrons and the mesh for the validation using Hexahedron elements has only hexahedrons. TLMBHT is capable of solving meshes that have any of these two elements.


Now, you can go to:

* [Tutorials, examples, and validations]({{ site.baseurl }}{% link vte/index.md %})
* [Validation in three-dimensions of Heat equation using the element hexahedron]({{ site.baseurl }}{% link vte/validation/heat 3D hexahedron.md %})
