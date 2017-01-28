---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page
permalink: conf/why.html
---
<script src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML" type="text/javascript"></script>

In this page, we will cover:

1) Why should I use tlmbht?

2) What can I solve with it?

3) What does TLM really mean?

# 1) Why should I use tlmbht?

The TLM method is a powerful numerical method used to solve partial differential equations. It is based on using transmission-lines and circuit elements to represent the geometry of a problem. Then, the solution of the electrical circuit is equivalent to the solution of the partial differential equation. The transformation from the real problem to its electrical circuit equivalent provides advantages for the TLM, which are, in general:
    
1. Stable calculation when solving problems in the time-domain.

2. Fast calculations because the solutions are obtained using matrix multiplications (instead of inverting matrices as some numerical methods do).

3. Accurate results.

Give it a try! [Download the latest release for free here](https://github.com/hugomilan/tlmbht/releases) and go to the [tutorials, examples, and validations]({{ site.baseurl }}{% link vte/index.md %}) to see how fast and accurate the tlmbht can be.

# 2) What can I solve with it?

You can solve:

1. Heat transfer in 1D, 2D, and 3D.

2. Bio-heat transfer in 1D, 2D, and 3D.

3. Diffusion in 1D, 2D, and 3D.

The numerical solution is based on a mesh of geometrical elements (or control volumes) that can be created using the [Gmsh software](http://www.gmsh.info). The geometrical elements currently supported are based on my research on TLM (see the reference file for citation to the original papers) and are:

1. One-dimension.

  1.1. Line in Cartesian coordinates.
 
2. Two-dimensions.

  2.1. Triangle in Cartesian coordinates.
 
3. Three-dimensions.

  3.1. Tetrahedron in Cartesian coordinates.



# What does TLM really mean?

TLM is an acronym that stands for *Transmission-Line Modeling* and/or *Transmission-Line Matrix*. The term Transmisison-Line Matrix was conceived by Johns, the creator of the TLM method, in Johns and Beurly (1971). In this paper, they showed how the TLM method could be used to solve 2D electromagnetic problems. The term was well suited for the time, when the TLM method had been applied only to electromagnetic problems and the meshes were all structured. At that time, the use of structured meshes resemble a matrix arrangement.

However, the TLM theory has been progressively evolving. Now, the TLM method is not exclusively used to solve electromagnetic problems and the mesh types have evolved to allow more complex elements (hence, accelerating the simulations and obtaining more accurate predictions). For example, we find the use of the TLM method in the literature to solve diffusion problems (de Cogan, 1998), Schrödinger equation (Pierantoni et al, 2008), bio-heat transfer (Milan and Gebremedhin, 2016c), and computational mechanics (de Cogan et al, 2006), to cite some (see more references here). Moreover, the term *Matrix* may lead someone unfamiliar with the TLM method to think that matrices are essential to the TLM method. In fact, the TLM method can be programmed without any matrices.

The term *Modeling* represents the essence of the TLM method more truthfully. In essence, the TLM method consists in substituting the geometry of a problem with transmission-lines. That is, **modeling** something using transmission-lines (in reality, you should be able to use real transmission-lines instead of the computational approach we do although using real transmission-lines would be more complex).

These are the reasons why I prefer the terminology Transmission-Line Modeling.

What to know more about the TLM theory? [Go here.]({{ site.baseurl }}{% link theory/tlm/index.md %})
