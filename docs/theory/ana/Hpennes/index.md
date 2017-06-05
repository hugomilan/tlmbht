---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page_eqAMS_Disqus
title: Analytical solutions of Pennes equation
permalink: theory/ana/pennes/index.html
---

<span style="color:#697473">Feb 7, 2017</span> by [**Hugo Milan**](https://hugomilan.github.io/)

The Pennes bio-heat equation in time-domain is defined as:

\begin{equation}
    \rho c_{p}\frac{\partial T}{\partial t} = k\nabla^2 T + S + Q_m + \omega_b\rho_b c_{b} (T_b - T)
\end{equation}

with the following definition of flux

\begin{equation}
    \bar{q} = -k\nabla T
\end{equation}

where \\(T\\) is temperature, \\(t\\) is time, \\(\rho\\) is density, \\(c_{p}\\) is specific heat, \\(k\\) is heat conductivity, \\(S\\) is source, \\(Q_m\\) is metabolic heat generation, \\(\omega_b\\) is blood perfusion, \\(\rho_b\\) is blodd density, \\(c_{b}\\) is blood specific heat, and \\(T_b\\) is blood temperature.

#### **1) One-dimension**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/pennes/pennes 1D TT.md %})

$$
\begin{matrix}
    T(y, t = 0) = T_{c}\\
    T(y = 0, t) = T_{c}\\
    T(y = H, t) = T_{s}
\end{matrix}
$$

#### **2) Two-dimensions**

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/pennes/pennes 2D TTqq.md %})

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

[Click here to see how to solve the problem with the following initial condition and boundary conditions:]({{ site.baseurl }}{% link theory/ana/pennes/pennes 3D TTqqqq.md %})

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


You can go to:

* [Analytical solutions menu]({{ site.baseurl }}{% link theory/ana/index.md %})
