---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page_eqAMS_Disqus
title: Analytical solution of the Hyperbolic Diffusion/Heat equation in time-domain for one-dimension
permalink: theory/ana/Hheat/Hheat-1D-TT.html
---

<span style="color:#697473">Feb 7, 2017</span> by [**Hugo Milan**](https://hugomilan.github.io/)

You can download the algorithm `D1_H_HEAT_f.m` for Octave/Matlab that can solve this problem [here](https://github.com/hugomilan/tlmbht/tree/master/src/octave/analytical/1D) and you can see instructions in [how to use it here]({{ site.baseurl }}{% link theory/ana/Hheat/Hheat 1D TT.md%}#using-the-algorithm). If you want to see the final solution, [go to Solution]({{ site.baseurl }}{% link theory/ana/Hheat/Hheat 1D TT.md%}#solution).

In this page, we will solve the dynamic hyperbolic diffusion/heat equation in one-dimension using the principles of superposition and separation of variables. The problem we will solve is restricted to the following initial and boundary conditions:

$$
\begin{matrix}
    C(y, t = 0) = C_{c} & \text{ or } & T(y, t = 0) = T_{c}\\
    \dfrac{\partial C}{\partial t}(y, t = 0) = 0 & \text{ or } & \dfrac{\partial T}{\partial t}(y, t = 0) = 0\\
    C(y = 0, t) = C_{c} & \text{ or } & T(y = 0, t) = T_{c}\\
    C(y = H, t) = C_{s} & \text{ or } & T(y = H, t) = T_{s}
\end{matrix}
$$

The problem geometry for the hyperbolic diffusion equation can be represented as shown below.
<img src="{{ site.baseurl }}/assets/images/theory/ana/diff 1D TT.png" alt="Geometry of the one-dimensional problem for hyperbolic diffusion equation" width="300">


The problem geometry for the hyperbolic heat equation can be represented as shown below.
<img src="{{ site.baseurl }}/assets/images/theory/ana/heat 1D TT.png" alt="Geometry of the one-dimensional problem for hyperbolic heat equation" width="300">

### Governing equations

Diffusion equation:

\begin{equation}
    \tau\frac{\partial^2 C}{\partial t^2} + \frac{\partial C}{\partial t} = D\nabla^2 C + S + \tau\frac{\partial S}{\partial t}
\end{equation}

where \\(C\\) is concentration, \\(t\\) is time, \\(D\\) is diffusivity, \\(\tau\\) is relaxation time, and \\(S\\) is source.

And we define the heat equation as:

\begin{equation}
    \tau\rho c_{p}\frac{\partial^2 T}{\partial t^2} + \rho c_{p}\frac{\partial T}{\partial t} = k\nabla^2 T + S + \tau\frac{\partial S}{\partial t}
    \label{eq:Heat}
\end{equation}

where \\(T\\) is temperature, \\(t\\) is time, \\(\rho\\) is density, \\(c_{p}\\) is specific heat, \\(k\\) is heat conductivity, \\(\tau\\) is thermal relaxation time, and \\(S\\) is source.

Note that the diffusion equation and the heat equation have the same form when \\(\rho c_{p} = 1\\).


### Solving

I will show the solution process for the heat equation. The solution process for the diffusion equation follows straightforwardly.

I will use the principle of suporposition so that:

\begin{equation}
    T(y,t) = T_c + \phi_a(y) + \phi_b(y)\tau_b(t)
    \label{eq:Sup}
\end{equation}

and the initial and boundary conditions for these problems are:

$$
\begin{matrix}
    & T(y,t) & \phi_a(y) & \phi_b(y)\tau_b(t) \\
    t = 0 & T = T_c & - & \phi_b\tau_b = -\phi_a\\
    t = 0 & \dfrac{\partial T}{\partial t} = 0 & - & \phi_b\dfrac{\partial \tau_b}{\partial t} = 0\\
    y = 0 & T = T_c & \phi_a = 0 & \phi_b\tau_b = 0\\
    y = H & T = T_s & \phi_a = T_s - T_c & \phi_b\tau_b = 0
\end{matrix}
$$


Assuming that \\(S\\) is constant and applying Eq. \ref{eq:Sup} in \ref{eq:Heat} yields

\begin{equation}
    k\frac{\partial^2 \phi_a}{\partial y^2} + S =
    \tau\rho c_{p}\phi_b\frac{\partial^2 \tau_b}{\partial t^2} + \rho c_{p}\phi_b\frac{\partial \tau_b}{\partial t} - k\tau_b\frac{\partial^2 \phi_b}{\partial y^2}
     = 0
\end{equation}

which implies that we can solve Eqs. \ref{eq:a} and \ref{eq:b}
\begin{equation}
    k\frac{\partial^2 \phi_a}{\partial y^2} + S = 0
    \label{eq:a}
\end{equation}

\begin{equation}
    \tau\rho c_{p}\phi_b\frac{\partial^2 \tau_b}{\partial t^2} + \rho c_{p}\phi_b\frac{\partial \tau_b}{\partial t} - k\tau_b\frac{\partial^2 \phi_b}{\partial y^2} = 0
    \label{eq:b}
\end{equation}



#### **Solving Eq. \ref{eq:a}**

Equation \ref{eq:a} can be solved by direct integration, with the following general solution

\begin{equation}
    \phi_a = -\frac{S}{2k}y^2 + c_1 y + c_2
\end{equation}

Applying the boundary condition at \\(y = 0\\) we get \\(c_2 = 0\\). Applying the boundary condition at \\(y=H\\) we get:
\begin{equation}
    c_1 = \frac{T_s - T_c}{H} + \frac{SH}{2k}
\end{equation}

So, the final solution for \\(\phi_a\\) is:

\begin{equation}
    \phi_a = -\frac{S}{2k}y^2 + \left( \frac{T_s - T_c}{H} + \frac{SH}{2k} \right) y
\end{equation}


#### **Solving Eq. \ref{eq:b}**

We will use separation of variables to solve Eq. \ref{eq:b}. In separation of variables, we assume that the solution is the multiplication of a function of \\(y\\) ( \\(\phi_b(y)\\) ) and \\(t\\) ( \\(\tau_b(t)\\) ). Hence,

\begin{equation}
    \frac{\tau\rho c_{p}}{k\tau_b}\frac{\partial^2 \tau_b}{\partial t^2} + \frac{\rho c_{p}}{k\tau_b}\frac{\partial \tau_b}{\partial t} = \frac{1}{\phi_b}\frac{\partial^2 \phi_b}{\partial y^2} = -\lambda_m^2
\end{equation}


The solution of \\(\phi_b\\) can be expressed as:

\begin{equation}
    \phi_b = c_3 \sin{\left(\lambda_m y\right)} + c_4 \cos{\left(\lambda_m y\right)}
\end{equation}

Applying the boundary condition at \\(y = 0\\) we get \\(c_4 = 0\\). Applying the boundary condition at \\(y=H\\) we get:

\begin{equation}
    \sin{\left(\lambda_m H\right)} = 0
    \label{eq:lambdam}
\end{equation}

which implies that 

\begin{equation}
    \lambda_m = \frac{m\pi}{H}
\end{equation}

with \\(m = 1, 2, 3, \dotsc\\) We start the counting from \\(1\\) because \\(m = 0\\) yields \\(\phi_b = 0\\). Now, defining \\(\alpha = k/(\rho c_p) \\), \\(\tau_b\\) has two possible general solutions:

\begin{eqnarray}
    \tau_{b1} &=& \exp{\left(\dfrac{-t}{2\tau}\right)}\left[ c_{51} \exp{\left( -t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)} \ldots \\\
    \ldots + c_{61} \exp{\left( t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)}\right]
    \label{eq:tb1} \\\
    \tau_{b2} &=& \exp{\left(\dfrac{-t}{2\tau}\right)}\left[ c_{52} \cos{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)} \ldots \\\
    \ldots + c_{62} \sin{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)}\right]
    \label{eq:tb2}
\end{eqnarray}

Which solution should we use? Eq. \ref{eq:tb1} or \ref{eq:tb2}? Well, we need a solution that gives us a real value because, in our framework, we are assuming that temperature is real (yes, some theoretical studies support that temperature can be complex but we will not dive into this). So, 

\begin{equation}
    \tau_b = \left\\{ \begin{array}{lr}
    \tau_{b1} & 4\alpha\lambda_m^2\tau - 1 \lt 0\\\
    \tau_{b2} & 4\alpha\lambda_m^2\tau - 1 \le 0
\end{array} \right\.
\end{equation}

Now, we apply the initial condition \\(\dfrac{\partial \tau_b}{\partial t} = 0 \\):

\begin{eqnarray}
    \frac{\partial\tau_{b1}}{\partial t} &=& \dfrac{1}{2\tau}\exp{\left(\dfrac{-t}{2\tau}\right)}\left[ -c_{51}\left(  \sqrt{1-4\alpha\lambda_m^2\tau} + 1 \right) \exp{\left( -t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)} \ldots \\\
    \ldots + c_{61} \left(  \sqrt{1-4\alpha\lambda_m^2\tau} - 1 \right) \exp{\left( t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)}\right]\\\
    \frac{\partial\tau_{b2}}{\partial t} &=& \dfrac{1}{2\tau}\exp{\left(\dfrac{-t}{2\tau}\right)}\left[ -\left( c_{52} - c_{62}\sqrt{4\alpha\lambda_m^2\tau - 1}\right)\cos{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)} \ldots \\\
    \ldots - \left( c_{62} + c_{52}\sqrt{4\alpha\lambda_m^2\tau - 1}\right) \sin{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)}\right]
\end{eqnarray}


\begin{eqnarray}
    c_{51} &=& c_{61} \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau} - 1}{\sqrt{1-4\alpha\lambda_m^2\tau} + 1} \\\
    c_{52} &=& c_{62}\sqrt{4\alpha\lambda_m^2\tau - 1}
\end{eqnarray}

So,

\begin{eqnarray}
    \tau_{b1} &=& c_{61} \exp{\left(\dfrac{-t}{2\tau}\right)}\left[ \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau} - 1}{\sqrt{1-4\alpha\lambda_m^2\tau} + 1} \exp{\left( -t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)} \ldots \\\
    \ldots + \exp{\left( t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)}\right]
    \label{eq:tb1p} \\\
    \tau_{b2} &=& c_{62}\exp{\left(\dfrac{-t}{2\tau}\right)}\left[ \sqrt{4\alpha\lambda_m^2\tau - 1} \cos{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)} \ldots \\\
    \ldots + \sin{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)}\right]
    \label{eq:tb2p}
\end{eqnarray}

Therefore, the solution that we are looking for is:

\begin{equation}
    \phi_b\tau_b = \sum_{m=1}^{\infty}c_m \sin{\left(\lambda_m y\right)} \tau_b
    \label{eq:b:almost}
\end{equation}

Now, we apply the initial condition and get

\begin{eqnarray}
    \sum_{m=1}^{\infty}2c_m \sin{\left(\lambda_m y\right)} \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{\sqrt{1-4\alpha\lambda_m^2\tau} + 1} &=& \ldots \\\
    \ldots -\phi_a = \frac{S}{2k}y^2 - \left( \frac{T_s - T_c}{H} + \frac{SH}{2k} \right) y
    \label{eq:tb1:cm} \\\
    \sum_{m=1}^{\infty}c_m \sin{\left(\lambda_m y\right)}\sqrt{4\alpha\lambda_m^2\tau - 1} &=& \ldots \\\
    \ldots -\phi_a = \frac{S}{2k}y^2 - \left( \frac{T_s - T_c}{H} + \frac{SH}{2k} \right) y
    \label{eq:tb2:cm}
\end{eqnarray}

To solve Eqs. \ref{eq:tb1:cm} and \ref{eq:tb2:cm}, we multiply both sides by an [orthogonal function](https://en.wikipedia.org/wiki/Orthogonal_functions) of the function that multiplies \\(c_m\\) and integrate it from \\(y = 0\\) to \\(y = H\\) to elimate the \\(y\\) dependence. That is, we need a function that is orthogonal to \\(\sin{\left(\lambda_m y\right)}\\). The function that we are looking for is \\(\sin{\left(\lambda_n y\right)}\\). Multiplying both sides by this function yields


\begin{eqnarray}
    \int_{y=0}^{y=H}\sum_{m=1}^{\infty}2c_m \sin{\left(\lambda_m y\right)}\sin{\left(\lambda_n y\right)}  \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{\sqrt{1-4\alpha\lambda_m^2\tau} + 1}dy &=& \ldots \\\
    \ldots \int_{y=0}^{y=H}\left[ \frac{S}{2k}y^2 - \left( \frac{T_s - T_c}{H} + \frac{SH}{2k} \right) y \right]\sin{\left(\lambda_n y\right)} dy
    \label{eq:tb1:cm2} \\\
    \int_{y=0}^{y=H}\sum_{m=1}^{\infty}c_m \sin{\left(\lambda_m y\right)}\sin{\left(\lambda_n y\right)} \sqrt{4\alpha\lambda_m^2\tau - 1} dy &=& \ldots \\\
    \ldots \int_{y=0}^{y=H}\left[ \frac{S}{2k}y^2 - \left( \frac{T_s - T_c}{H} + \frac{SH}{2k} \right) y \right]\sin{\left(\lambda_n y\right)} dy
    \label{eq:tb2:cm2}
\end{eqnarray}

Since we have

\begin{equation}
    \int_{y=0}^{y=H} \sin{\left(\lambda_m y\right)}\sin{\left(\lambda_n y\right)} dy = 0
\end{equation}

for \\(m \neq n\\), and

\begin{equation}
    \int_{y=0}^{y=H} \sin{\left(\lambda_m y\right)}\sin{\left(\lambda_n y\right)} dy = \frac{H}{2}
    \label{eq:mn}
\end{equation}

for \\(m = n\\), we can drop the summation and solve Eqs. \ref{eq:tb1:cm2} and \ref{eq:tb2:cm2} for each \\(c_m\\):


\begin{eqnarray}
    c_m &=& \dfrac{\displaystyle \int_{y=0}^{y=H}\left[ \dfrac{S}{2k}y^2 - \left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) y \right]\sin{\left(\lambda_m y\right)} dy}{ \displaystyle 2\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{\sqrt{1-4\alpha\lambda_m^2\tau} + 1} \int_{y=0}^{y=H}\sin^2{\left(\lambda_m y\right)} dy  }
    \label{eq:tb1:cm3} \\\
    c_m  &=& \dfrac{\displaystyle \int_{y=0}^{y=H}\left[ \dfrac{S}{2k}y^2 - \left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) y \right]\sin{\left(\lambda_m y\right)} dy}{ \displaystyle \sqrt{4\alpha\lambda_m^2\tau - 1} \int_{y=0}^{y=H}\sin^2{\left(\lambda_m y\right)} dy  }
    \label{eq:tb2:cm3}
\end{eqnarray}

So, solving for the first integral,

\begin{equation}
    \dfrac{S}{2k}\int_{y=0}^{y=H}y^2\sin{\left(\lambda_m y\right)} dy = \dfrac{S}{2k} \frac{ \left( 2 - \lambda_m^2H^2 \right)\cos{\left(\lambda_m H\right)} + 2\lambda_m H \sin{\left(\lambda_m H\right)} - 2}{\lambda_m^3}
    \label{eq:b:cmf1}
\end{equation}

From the definition of \\(\lambda_m\\) in Eq. \ref{eq:lambdam},


\begin{equation}
    \dfrac{S}{2k}\int_{y=0}^{y=H}y^2\sin{\left(\lambda_m y\right)} dy = \dfrac{S}{2k} \frac{ \left( -1 \right)^m \left( 2 - \lambda_m^2H^2 \right) -2 }{\lambda_m^3}
    \label{eq:b:cmf1b}
\end{equation}

Now, solving the last integral:

\begin{equation}
    -\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right)\int_{y=0}^{y=H}y\sin{\left(\lambda_m y\right)} dy = \ldots \\\
    \ldots -\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) \frac{\sin{\left(\lambda_m H\right)} - \lambda_m H\cos{\left(\lambda_m H\right)}}{\lambda_m^2}
    \label{eq:b:cmf2}
\end{equation}


which, from the definition of \\(\lambda_m\\) in Eq. \ref{eq:lambdam}, becomes

\begin{equation}
    -\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) \int_{y=0}^{y=H}y\sin{\left(\lambda_m y\right)} dy = \left( -1 \right)^m\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) \frac{H}{\lambda_m}
    \label{eq:b:cmf2b}
\end{equation}

From Eqs. \ref{eq:mn}, \ref{eq:b:cmf1b}, and, \ref{eq:b:cmf2b}, the final solution for \\(\phi_b(y)\tau_b(t)\\) is:

\begin{equation}
    \phi_b\tau_b = \exp{\left(\dfrac{-t}{2\tau}\right)}\sum_{m=1}^{\infty}\left[ \dfrac{S}{Hk}  \frac{ \left( -1 \right)^m\left( 2 - \lambda_m^2H^2 \right) -2 }{\lambda_m^3} \ldots \\\
    \ldots + \left( -1 \right)^m\frac{2}{\lambda_m}\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) \right]\sin{\left(\lambda_m y\right)} \times \ldots \\\
    \left\\{  I \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau} + 1}{2\sqrt{1-4\alpha\lambda_m^2\tau}}\left[ \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau} - 1}{\sqrt{1-4\alpha\lambda_m^2\tau} + 1} \exp{\left( -t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)} \\\
    \exp{\left( t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)} \right] + \\\
    \left( 1 - I \right) \dfrac{1}{\sqrt{4\alpha\lambda_m^2\tau - 1}} \left[ \sqrt{4\alpha\lambda_m^2\tau - 1} \cos{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)} \\\
    \sin{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)} \right]\right\\}
    \label{eq:b:final}
\end{equation}

where

\begin{equation}
    I = \left\\{ \begin{array}{lr}
    1 & 4\alpha\lambda_m^2\tau - 1 \lt 0\\\
    0 & 4\alpha\lambda_m^2\tau - 1 \ge 0
\end{array} \right\.
\end{equation}

and 

\begin{equation}
\lim_{4\alpha\lambda_m^2\tau - 1\rightarrow 0^+} \phi_b\tau_b = \exp{\left(\dfrac{-t}{2\tau}\right)}\sum_{m=1}^{\infty}\left[ \dfrac{S}{Hk}  \frac{ \left( -1 \right)^m\left( 2 - \lambda_m^2H^2 \right) -2 }{\lambda_m^3} \ldots \\\
    \ldots + \left( -1 \right)^m\frac{2}{\lambda_m}\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) \right]\sin{\left(\lambda_m y\right)}\times \left\\{ 1 + \dfrac{1}{2\tau} \right\\}
\end{equation}
 
Note that this solution is not suitable for \\(\tau = 0\\). In this case, the correct solution is for the [heat equation shown here.]({{ site.baseurl }}{% link theory/ana/heat/heat 1D TT.md %})

### Solution

The final solution is:

\begin{equation}
    T(y,t) = T_c + \phi_a(y) + \phi_b(y)\tau_b(t)
\end{equation}

with \\(\alpha = k/( \rho c_p ) \\), and \\(\phi_a(y)\\) and \\(\phi_b(y)\tau_b(t)\\) expressed as:

\begin{equation}
    \phi_a = -\frac{S}{2k}y^2 + \left( \frac{T_s - T_c}{H} + \frac{SH}{2k} \right) y
\end{equation}

\begin{equation}
    \lambda_m = \frac{m\pi}{H}
\end{equation}

\begin{equation}
    \phi_b\tau_b = \exp{\left(\dfrac{-t}{2\tau}\right)}\sum_{m=1}^{\infty}\left[ \dfrac{S}{Hk}  \frac{ \left( -1 \right)^m\left( 2 - \lambda_m^2H^2 \right) -2 }{\lambda_m^3} \ldots \\\
    \ldots + \left( -1 \right)^m\frac{2}{\lambda_m}\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) \right]\sin{\left(\lambda_m y\right)} \times \ldots \\\
    \left\\{  I \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau} + 1}{2\sqrt{1-4\alpha\lambda_m^2\tau}}\left[ \dfrac{\sqrt{1-4\alpha\lambda_m^2\tau} - 1}{\sqrt{1-4\alpha\lambda_m^2\tau} + 1} \exp{\left( -t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)} \\\
    \exp{\left( t\dfrac{\sqrt{1-4\alpha\lambda_m^2\tau}}{2\tau} \right)} \right] + \\\
    \left( 1 - I \right) \dfrac{1}{\sqrt{4\alpha\lambda_m^2\tau - 1}} \left[ \sqrt{4\alpha\lambda_m^2\tau - 1} \cos{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)} \\\
    \sin{\left( t\dfrac{\sqrt{4\alpha\lambda_m^2\tau - 1}}{2\tau} \right)} \right]\right\\}
\end{equation}

where

\begin{equation}
    I = \left\\{ \begin{array}{lr}
    1 & 4\alpha\lambda_m^2\tau - 1 \lt 0\\\
    0 & 4\alpha\lambda_m^2\tau - 1 \ge 0
\end{array} \right\.
\end{equation}

and 

\begin{equation}
\lim_{4\alpha\lambda_m^2\tau - 1\rightarrow 0^+} \phi_b\tau_b = \exp{\left(\dfrac{-t}{2\tau}\right)}\sum_{m=1}^{\infty}\left[ \dfrac{S}{Hk}  \frac{ \left( -1 \right)^m\left( 2 - \lambda_m^2H^2 \right) -2 }{\lambda_m^3} \ldots \\\
    \ldots + \left( -1 \right)^m\frac{2}{\lambda_m}\left( \dfrac{T_s - T_c}{H} + \dfrac{SH}{2k} \right) \right]\sin{\left(\lambda_m y\right)}\times \left\\{ 1 + \dfrac{1}{2\tau} \right\\}
\end{equation}
 
Note that this solution is not suitable for \\(\tau = 0\\). In this case, the correct solution is for the [heat equation shown here.]({{ site.baseurl }}{% link theory/ana/heat/heat 1D TT.md %})

The flux/heat flux can be obtained by derivating these equations with respect to \\(y\\).

### Using the Algorithm

The algorithm `D1_H_HEAT_f.m` was written as a function in Octave/Matlab and you can obtain it [here](https://github.com/hugomilan/tlmbht/tree/master/src/octave/analytical/1D). To use it, you will need to define the material properties and the maximum value of \\(m\\).

As an example, we can use the following script:

{% highlight matlab %}
% Thermal properties
Tc = 100; % core and initial temperature (oC)
Ts = 1000; % superficial temperature (oC)
rho = 2700; % density (kg/m3)
cp = 900; % specific heat (J/(K-kg))
k = 205; % thermal conductivity (W/(K-m))
tau = 10; % thermal relaxation time (s) 
S = 5000; % internal heat source (W/m3)
    
% Material properties
H = 1e-3; % m
y = 0:(H/100):H;
    
% number of iterations of m
minf = 500;
    
% time of the solution
t = 0.5e-3;
    
[T, q] = D1_H_HEAT_f(y, H, t, Ts, Tc, k, rho, cp, tau, S, minf);
    
plot(y, T)
    
figure;
    
plot(y, q)
{% endhighlight %}

which generates

<img src="{{ site.baseurl }}/assets/images/theory/ana/heat 1D TT plot.png" alt="Solution of the problem for heat equation">

### Limitations

Every solution method has limitations. The most noticeable limitation is that we have to perform infinity sums to get predictions using solutions from separation of variables. Infinity sums, however, cannot be performed in numerical computations, which leads us to truncate the calculations for large values of \\(m\\). How large this number have to be will depend on the problem geometry and on the material properties but, if theses number are not large enough, the calculations will manifest oscillations that will be most evident for predicted fluxes. When observed that \\(m\\) has to be increased, a rule-of-thumb is to double its valuee, re-calculate the predicionts, and observe if the oscillations have decreased to acceptable intensities.

### Done

Now, you can go to:

* [Analytical solutions menu]({{ site.baseurl }}{% link theory/ana/index.md %})
* [Analytical solutions of Hyperbolic Diffusion and Heat equations menu]({{ site.baseurl }}{% link theory/ana/Hheat/index.md %})
* [Analytical solution in two-dimensions]({{ site.baseurl }}{% link theory/ana/Hheat/Hheat 2D TTqq.md %})
* [FIXME TLM validation using this solution for hyperbolic diffusion equation]({{ site.baseurl }}{% link vte/validation/diff 1D line.md %})
* [FIXME TLM validation using this solution for hyperbolic heat equation]({{ site.baseurl }}{% link vte/validation/heat 1D line.md %})


