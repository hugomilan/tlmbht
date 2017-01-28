---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
# 
# Set autoNumber to "all" if you want every displayed equation to be numbered
# 
layout: page
title: Analytical solutions
permalink: theory/ana/index.html
---
<script src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML" type="text/javascript" ></script>

<script type="text/x-mathjax-config">
MathJax.Hub.Config({
  TeX: { equationNumbers: { autoNumber: "AMS" } } 
});
</script>

Analytical solutions are essential to the verification process of numerical solvers. The analytical solutions are used to validate the predictions made using numerical solver to problems that have analytical solutions. Here, we show how to obtain the analytical solutions that were used to validate TLMBHT.

### Diffusion equation and Heat equation

Analytical solutions of the heat equation can be used to solve the diffusion equation because these equations are similar.

We define the diffusion equation as:

\begin{equation}
    \frac{\partial C}{\partial t} = D\nabla^2 C + S
\end{equation}

with the following definition of flux

\\[ q^{''} = -D\nabla C\\]

where \\(C\\) is concentration, \\(t\\) is time, \\(D\\) is diffusivity, and \\(S\\) is source.

And we define the heat equation as:

\\[ \rho c_{p}\frac{\partial T}{\partial t} = k\nabla^2 T + S\\]

with the following definition of flux

\\[ q^{''} = -k\nabla T\\]

where \\(T\\) is temperature, \\(t\\) is time, \\(\rho\\) is density, \\(c_{p}\\) is specific heat, \\(k\\) is heat conductivity, and \\(S\\) is source.

Note that the diffusion equation and the heat equation have the same form when \\(\rho c_{p} = 1\\).

1. One-dimension.

Initial condition and boundary conditions:

* \\(C(y=0, t)\\) = \\(C_{c}\\) or \\(T(y=0, t)\\) = \\(T_{c}\\)
* \\(C(y, t = 0)\\) = \\(C_{c}\\) or \\(T(y, t = 0)\\) = \\(T_{c}\\)
* \\(C(y=H, t)\\) = \\(C_{s}\\) or \\(T(y=H, t)\\) = \\(T_{s}\\)

2. Two-dimensions

Initial condition and boundary conditions:

* \\(C(x, y=0, t)\\) = \\(C_{c}\\) or \\(T(x, y=0, t)\\) = \\(T_{c}\\)
* \\(C(x, y, t = 0)\\) = \\(C_{c}\\) or \\(T(x, y, t = 0)\\) = \\(T_{c}\\)
* \\(C(x, y=H, t)\\) = \\(C_{s}\\) or \\(T(x, y=H, t)\\) = \\(T_{s}\\)
* \\(\frac{\partial C}{\partial x}(x = 0, y, t)\\) = \\(0\\) or \\(T(x, y=H, t)\\) = \\(T_{s}\\)
* \\(\frac{\partial C}{\partial x}(x = 0, y, t)\\) = \\(q^{''}_{x}\\) or \\(T(x, y=H, t)\\) = \\(T_{s}\\)

3. Three-dimensions

### Pennes equation

1. One-dimension
2. Two-dimensions
3. Three-dimensions
