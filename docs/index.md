## Welcome to the tlmbht wiki!

### [Click here if you want to know why you should use tlmbht.](https://github.com/hugomilan/tlmbht/wiki/Why-should-I-use-the-tlmbht%3F)

### [Click here if you want to go through the validations, tutorials, and examples.](https://github.com/hugomilan/tlmbht/wiki/Validations,-Tutorials,-and-Examples)

### [Click here if you want to know how to configure a case file.](https://github.com/hugomilan/tlmbht/wiki/How-to-configure-a-case-file)

### [Click here if you want to know how more about the TLM theory.](https://github.com/hugomilan/tlmbht/wiki/TLM-theory)

### [Click here if you need help to run tlmbht in Windows.](https://github.com/hugomilan/tlmbht/wiki/Running-tlmbht-in-Windows)

## How to use the tlmbht

It is simple to use tlmbht! We need 5 steps:

### 1) Create the geometry of the problem.

You can do this using CAD software, such as [FreeCAD](http://www.freecadweb.org/) and [Gmsh software](http://www.gmsh.info). 

### 2) Create the mesh of the problem.

You can do this using the [Gmsh software](http://www.gmsh.info). Mesh from other software are not currently supported (but you can ask to include it by [opening an issue](https://github.com/hugomilan/tlmbht/issues)).

### 3) Create the description of the problem.

That is, create the case.tlm file. [Click here if you want to know how to configure a case file.](https://github.com/hugomilan/tlmbht/wiki/How-to-configure-a-case-file)

### 4) Solve the problem.

Simply type `tlmbht` in the folder that has the case.tlm file.

### 5) Visualize the output.

You will need other software to visualize the output. Currently supported is the Octave/Matlab data output (but you can ask to include support to your favorite software by [opening an issue](https://github.com/hugomilan/tlmbht/issues)).

### Done!

The [Validations, Tutorials, and Examples](https://github.com/hugomilan/tlmbht/wiki/Validations,-Tutorials,-and-Examples) pages go through these 5 steps. I recommend you to go through them to learn to tlmbht.
