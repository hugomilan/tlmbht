---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page_eqAMS_Disqus
permalink: vte/use-tlmbht.html
---

## TLMBHT Workflow
<span style="color:#697473">Jan 30, 2017</span> by [**Hugo Milan**](https://hugomilan.github.io/)

It is simple to use tlmbht! We need 5 steps:

### 1) Create the geometry of the problem.

You can do this using CAD software, such as [FreeCAD](http://www.freecadweb.org/) and [Gmsh software](http://www.gmsh.info). 

### 2) Create the mesh of the problem.

You can do this using the [Gmsh software](http://www.gmsh.info). Mesh from other software are not currently supported (but you can ask to include it by [opening an issue](https://github.com/hugomilan/tlmbht/issues)).

### 3) Create the description of the problem.

That is, create the case.tlm file. [Click here if you want to know how to configure a case file.]({{ site.baseurl }}{% link vte/configure case file.md %})

### 4) Solve the problem.

Simply type `tlmbht` in the folder that has the case.tlm file.

### 5) Visualize the output.

You will need other software to visualize the output. Currently supported is the Octave/Matlab data output (but you can ask to include support to your favorite software by [opening an issue](https://github.com/hugomilan/tlmbht/issues)).

### Done!

The [Tutorials, examples, and validationss]({{ site.baseurl }}{% link vte/index.md %}) pages go through these 5 steps. I recommend you to go through them to learn to tlmbht.
