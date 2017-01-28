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

Analytical solutions are essential to the verification process of numerical solvers. The analytical solutions are used to validate the predictions made using numerical solvers to problems that have analytical solutions. Here, we show how to obtain the analytical solutions that were used to validate TLMBHT.

### Diffusion equation and Heat equation

Analytical solutions of the heat equation can be used to solve the diffusion equation because these equations are similar.

We define the diffusion equation as:

\begin{equation}
    \frac{\partial C}{\partial t} = D\nabla^2 C + S
\end{equation}

with the following definition of flux

\begin{equation}
    \bar{q} = -D\nabla C
\end{equation}

where \\(C\\) is concentration, \\(\bar{q}\\) is concentration flux, \\(t\\) is time, \\(D\\) is diffusivity, and \\(S\\) is source.

And we define the heat equation as:

\begin{equation}
    \rho c_{p}\frac{\partial T}{\partial t} = k\nabla^2 T + S
\end{equation}

with the following definition of flux

\begin{equation}
    \bar{q} = -k\nabla T
\end{equation}

where \\(T\\) is temperature, \\(\bar{q}\\) is heat flux, \\(t\\) is time, \\(\rho\\) is density, \\(c_{p}\\) is specific heat, \\(k\\) is heat conductivity, and \\(S\\) is source.

Note that the diffusion equation and the heat equation have the same form when \\(\rho c_{p} = 1\\).

#### **1) One-dimension**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/heat 1D TT.md %})


$$
\begin{matrix}
    C(y, t = 0) = C_{c} & \text{ or } & T(y, t = 0) = T_{c}\\
    C(y = 0, t) = C_{c} & \text{ or } & T(y = 0, t) = T_{c}\\
    C(y = H, t) = C_{s} & \text{ or } & T(y = H, t) = T_{s}
\end{matrix}
$$

#### **2) Two-dimensions**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/heat 2D TTqq.md %})

$$
\begin{matrix}
    C(x,y, t = 0) = C_{c} & \text{ or } & T(x,y, t = 0) = T_{c}\\
    C(x,y = 0, t) = C_{c} & \text{ or } & T(x,y = 0, t) = T_{c}\\
    C(x,y = H, t) = C_{s} & \text{ or } & T(x,y = H, t) = T_{s}\\
    \dfrac{\partial C}{\partial x} (x = 0, y, t) = 0 & \text{ or } & \dfrac{\partial T}{\partial x}(x = 0, y, t) = 0\\
    \dfrac{\partial C}{\partial x}(x = L, y, t) = \dfrac{q_x}{D} & \text{ or } & \dfrac{\partial T}{\partial x}(x = L, y, t) = \dfrac{q_x}{k}
\end{matrix}
$$


#### **3) Three-dimensions**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/heat 3D TTqqqq.md %})

$$
\begin{matrix}
    C(x,y, z,t = 0) = C_{c} & \text{ or } & T(x,y,z, t = 0) = T_{c}\\
    C(x,y = 0,z, t) = C_{c} & \text{ or } & T(x,y = 0,z, t) = T_{c}\\
    C(x,y = H,z, t) = C_{s} & \text{ or } & T(x,y = H, z,t) = T_{s}\\
    \dfrac{\partial C}{\partial x} (x = 0, y, z,t) = 0 & \text{ or } & \dfrac{\partial T}{\partial x}(x = 0, y, z,t) = 0\\
    \dfrac{\partial C}{\partial x}(x = L, y, z,t) = \dfrac{q_x}{D} & \text{ or } & \dfrac{\partial T}{\partial x}(x = L, y, z,t) = \dfrac{q_x}{k}\\
    \dfrac{\partial C}{\partial z} (x, y, z = 0,t) = 0 & \text{ or } & \dfrac{\partial T}{\partial z}(x, y, z=0,t) = 0\\
    \dfrac{\partial C}{\partial z}(x, y, z = T_z,t) = \dfrac{q_z}{D} & \text{ or } & \dfrac{\partial T}{\partial z}(x, y, z = T_z,t) = \dfrac{q_z}{k}
\end{matrix}
$$

### Pennes equation

The bio-heat equation is defined as:

\begin{equation}
    \rho c_{p}\frac{\partial T}{\partial t} = k\nabla^2 T + S + Q_m + \omega_b\rho_b c_{b} (T_b - T)
\end{equation}

with the following definition of flux

\begin{equation}
    \bar{q} = -k\nabla T
\end{equation}

where \\(T\\) is temperature, \\(t\\) is time, \\(\rho\\) is density, \\(c_{p}\\) is specific heat, \\(k\\) is heat conductivity, \\(S\\) is source, \\(Q_m\\) is metabolic heat generation, \\(\omega_b\\) is blood perfusion, \\(\rho_b\\) is blodd density, \\(c_{b}\\) is blood specific heat, and \\(T_b\\) is blood temperature.

#### **1) One-dimension**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/pennes 1D TT.md %})

$$
\begin{matrix}
    T(y, t = 0) = T_{c}\\
    T(y = 0, t) = T_{c}\\
    T(y = H, t) = T_{s}
\end{matrix}
$$

#### **2) Two-dimensions**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/pennes 2D TTqq.md %})

$$
\begin{matrix}
    T(x,y, t = 0) = T_{c}\\
    T(x,y = 0, t) = T_{c}\\
    T(x,y = H, t) = T_{s}\\
    \dfrac{\partial T}{\partial x}(x = 0, y, t) = 0\\
    \dfrac{\partial T}{\partial x}(x = L, y, t) = \dfrac{q_x}{k}
\end{matrix}
$$


#### **3) Three-dimensions**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/pennes 3D TTqqqq.md %})

$$
\begin{matrix}
    T(x,y,z, t = 0) = T_{c}\\
    T(x,y = 0,z, t) = T_{c}\\
    T(x,y = H, z,t) = T_{s}\\
    \dfrac{\partial T}{\partial x}(x = 0, y, z,t) = 0\\
    \dfrac{\partial T}{\partial x}(x = L, y, z,t) = \dfrac{q_x}{k}\\
    \dfrac{\partial T}{\partial z}(x, y, z=0,t) = 0\\
    \dfrac{\partial T}{\partial z}(x, y, z = T_z,t) = \dfrac{q_z}{k}
\end{matrix}
$$
