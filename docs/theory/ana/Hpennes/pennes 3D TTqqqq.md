---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page_eqAMS_Disqus
title: Analytical solution of the Pennes equation in time-domain for three-dimensions
permalink: theory/ana/pennes/pennes-3D-TTqqqq.html
---

<span style="color:#697473">Jan 30, 2017</span> by [**Hugo Milan**](https://hugomilan.github.io/)

You can download the algorithm `D3_BHE_f.m` for Octave/Matlab that can solve this problem [here](https://github.com/hugomilan/tlmbht/tree/master/src/octave/analytical/3D) and you can see instructions in [how to use it here]({% link theory/ana/pennes/pennes 3D TTqqqq.md%}#using-the-algorithm). If you want to see the final solution, [go to Solution]({% link theory/ana/pennes/pennes 3D TTqqqq.md%}#solution).

In this page, we will solve the dynamic Pennes equation in three-dimensions using the principles of superposition and separation of variables. We build on the previous solution of the Pennes equation in two-dimensions described [here]({{ site.baseurl }}{% link theory/ana/pennes/pennes 2D TTqq.md%}) to solve this three-dimensional problem. The problem we will solve is restricted to the following initial and boundary conditions:

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

The problem geometry for the pennes equation can be represented as shown below.
<img src="{{ site.baseurl }}/assets/images/theory/ana/heat 3D TTqqqq.png" alt="Geometry of the three-dimensional problem for Pennes equation" width="500">

This geometry is a parallelepiped in a three-dimensional space, as you can see in the three-dimensional model below.
<div class="sketchfab-embed-wrapper"><iframe width="640" height="480" src="https://sketchfab.com/models/0ec8af1e3cc6475192fd38a75d4be8e7/embed" frameborder="0" allowvr allowfullscreen mozallowfullscreen="true" webkitallowfullscreen="true" onmousewheel=""></iframe>

<p style="font-size: 13px; font-weight: normal; margin: 5px; color: #4A4A4A;">
    <a href="https://sketchfab.com/models/0ec8af1e3cc6475192fd38a75d4be8e7?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">Parallelepiped 0.75 x 1 x 0.375</a>
    by <a href="https://sketchfab.com/hugomilan?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">hugomilan</a>
    on <a href="https://sketchfab.com?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">Sketchfab</a>
</p>
</div>

### Governing equations

The Pennes equation is defined as:

\begin{equation}
    \rho c_{p}\frac{\partial T}{\partial t} = k\left( \frac{\partial^2 T}{\partial x^2} + \frac{\partial^2 T}{\partial y^2} + \frac{\partial^2 T}{\partial z^2} \right) +  S + Q_m + \omega_b\rho_b c_{b} (T_b - T)
    \label{eq:Pennes}
\end{equation}

where \\(T\\) is temperature, \\(t\\) is time, \\(\rho\\) is density, \\(c_{p}\\) is specific heat, \\(k\\) is heat conductivity, and \\(S\\) is source, \\(Q_m\\) is metabolic heat generation, \\(\omega_b\\) is blood perfusion, \\(\rho_b\\) is blodd density, \\(c_{b}\\) is blood specific heat, and \\(T_b\\) is blood temperature.


### Solving

I will use the principle of suporposition so that:

\begin{equation}
    T(y,t) = T_c + \phi_a(y) + \phi_b(y)\tau_b(t) + \phi_c(x, y) + \phi_d(x,y)\tau_d(t) \ldots\\\
    \ldots + \phi_e(y,z) + \phi_f(y,z)\tau_f(t)
    \label{eq:Sup}
\end{equation}

and the initial and boundary conditions for these problems are:

$$
\begin{matrix}
    & T(x, y,t) & \phi_a(y) & \phi_b(y)\tau_b(t) & \phi_c(x,y) & \phi_d(x,y)\tau_d(t) & \phi_e(y,z) & \phi_f(y,z)\tau_f(t)\\
    t = 0 & T = T_c & - & \phi_b\tau_b = -\phi_a & - & \phi_d\tau_d = -\phi_c & - & \phi_f\tau_f = -\phi_d\\
    y = 0 & T = T_c & \phi_a = 0 & \phi_b\tau_b = 0 & \phi_c = 0 & \phi_d\tau_d = 0 & \phi_f = 0 & \phi_f\tau_f = 0\\
    y = H & T = T_s & \phi_a = T_s - T_c & \phi_b\tau_b = 0 & \phi_c = 0 & \phi_d\tau_d = 0 & \phi_f = 0 & \phi_f\tau_f = 0\\
    x = 0 & \dfrac{\partial T}{\partial x} = 0 & - & - & \dfrac{\partial \phi_c}{\partial x} = 0 & \tau_d\dfrac{\partial \phi_d}{\partial x} = 0 & - & - \\
    x = L & \dfrac{\partial T}{\partial x} = \dfrac{q_x}{k} & - & - & \dfrac{\partial \phi_c}{\partial x} = \dfrac{q_x}{k} & \tau_d\dfrac{\partial \phi_d}{\partial x} = 0 & - & - \\
    z = 0 & \dfrac{\partial T}{\partial z} = 0 & - & - & - & - & \dfrac{\partial \phi_e}{\partial z} = 0 & \tau_f\dfrac{\partial \phi_f}{\partial z} = 0\\
    z = T_z & \dfrac{\partial T}{\partial z} = \dfrac{q_z}{k} & - & - & - & - & \dfrac{\partial \phi_e}{\partial z} = \dfrac{q_z}{k} & \tau_f\dfrac{\partial \phi_f}{\partial z} = 0
\end{matrix}
$$


Defining \\(W_b = \omega_b\rho_b c_{b} \\) and applying Eq. \ref{eq:Sup} in \ref{eq:Pennes} yields

\begin{equation}
    \left( k\frac{\partial^2 \phi_a}{\partial y^2} - W_b\phi_a + S + Q_m + W_b(T_b - T_c) \right) \ldots \\\
    \ldots - \left( \rho c_{p}\phi_b\frac{\partial \tau_b}{\partial t} - k\tau_b\frac{\partial^2 \phi_b}{\partial y^2} + W_b\phi_b\tau_b \right) \ldots \\\
    \ldots + \left( k \frac{\partial^2 \phi_c}{\partial x^2} + k \frac{\partial^2 \phi_c}{\partial y^2} - W_b\phi_c \right) \ldots \\\
    \ldots - \left( \rho c_{p}\phi_d\frac{\partial \tau_d}{\partial t} - k \tau_d\frac{\partial^2 \phi_d}{\partial x^2} - k \tau_d\frac{\partial^2 \phi_d}{\partial y^2} + W_b\phi_d\tau_d\right) = \ldots \\\
    \ldots \left( - k \frac{\partial^2 \phi_e}{\partial y^2} - k \frac{\partial^2 \phi_e}{\partial z^2} + W_b\phi_e \right) \ldots \\\
    \ldots + \left( \rho c_{p}\phi_f\frac{\partial \tau_f}{\partial t} - k \tau_f\frac{\partial^2 \phi_f}{\partial y^2} - k \tau_f\frac{\partial^2 \phi_f}{\partial z^2} + W_b\phi_f\tau_f\right) 
\end{equation}

which implies that we can solve Eqs. \ref{eq:a}-\ref{eq:f}
\begin{equation}
    k\frac{\partial^2 \phi_a}{\partial y^2} - W_b\phi_a + S + Q_m + W_b(T_b - T_c) = 0
    \label{eq:a}
\end{equation}

\begin{equation}
    \rho c_{p}\phi_b\frac{\partial \tau_b}{\partial t} - k\tau_b\frac{\partial^2 \phi_b}{\partial y^2} + W_b\phi_b\tau_b = 0
    \label{eq:b}
\end{equation}

\begin{equation}
    k \frac{\partial^2 \phi_c}{\partial x^2} + k \frac{\partial^2 \phi_c}{\partial y^2} - W_b\phi_c = 0
    \label{eq:c}
\end{equation}

\begin{equation}
    \rho c_{p}\phi_d\frac{\partial \tau_d}{\partial t} - k \tau_d\frac{\partial^2 \phi_d}{\partial x^2} - k \tau_d\frac{\partial^2 \phi_d}{\partial y^2} + W_b\phi_d\tau_d = 0
    \label{eq:d}
\end{equation}

\begin{equation}
    k \frac{\partial^2 \phi_e}{\partial y^2} + k \frac{\partial^2 \phi_e}{\partial z^2} - W_b\phi_f = 0
    \label{eq:e}
\end{equation}

\begin{equation}
    \rho c_{p}\phi_f\frac{\partial \tau_f}{\partial t} - k \tau_f\frac{\partial^2 \phi_f}{\partial y^2} - k \tau_f\frac{\partial^2 \phi_f}{\partial z^2} + W_b\phi_f\tau_f = 0
    \label{eq:f}
\end{equation}



#### **Solving Eq. \ref{eq:a} and \ref{eq:b}**

The solution of Eq. \ref{eq:a} and \ref{eq:b} was previously covered when we [solved the pennes equation in one-dimension.]({{ site.baseurl }}{% link theory/ana/pennes/pennes 1D TT.md%})



#### **Solving Eq. \ref{eq:c} and \ref{eq:d}**

The solution of Eq. \ref{eq:c} and \ref{eq:d} was previously covered when we [solved the pennes equation in two-dimensions.]({{ site.baseurl }}{% link theory/ana/pennes/pennes 2D TTqq.md%})

#### **Solving Eq. \ref{eq:e}**

The solution procedure for Eq. \ref{eq:e} is exactly the same as the solution procedure for Eq. \ref{eq:c} showed [here.]({{ site.baseurl }}{% link theory/ana/pennes/pennes 2D TTqq.md%}) Hence, changing the nomenclature from the solution of Eq. \ref{eq:c}, the solution of Eq. \ref{eq:e} is:

\begin{equation}
    \mu_u = \frac{ \left( 2u - 1 \right) \pi}{H}
\end{equation}

\begin{equation}
    \phi_e = \sum_{u=1}^{\infty} \frac{4q_z\cosh{\left(z\sqrt{\mu_u^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\mu_u y\right)}}{Hk\mu_u\sqrt{\mu_u^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\mu_u^2 +\dfrac{W_b}{k}}\right)} }
\end{equation}

#### **Solving Eq. \ref{eq:f}**

The solution procedure for Eq. \ref{eq:f} is exactly the same as the solution procedure for Eq. \ref{eq:d} showed [here.]({{ site.baseurl }}{% link theory/ana/pennes/pennes 2D TTqq.md%}) Hence, changing the nomenclature from the solution of Eq. \ref{eq:d}, the solution of Eq. \ref{eq:f} is:

\begin{equation}
    \nu_v = \frac{v\pi}{T_z}
\end{equation}

\begin{equation}
    \phi_f\tau_f = \sum_{u=1}^{\infty} -\frac{4q_z\sin{ \left( \mu_u y\right) }}{HT_zk\mu_u}
    \left\\{ \frac{\exp{\left[ -\alpha t \left( \mu_u^2 + \dfrac{W_b}{k}\right) \right]}}{ \mu_u^2 + \dfrac{W_b}{k} }
    \ldots\\\
    \ldots + 
    \sum_{v=1}^{\infty} \left( -1 \right)^v\frac{2\cos{ \left( \nu_v z \right) }}{\nu_v^2 + \mu_u^2 + \dfrac{W_b}{k}} \exp{\left[ -\alpha t \left(  \nu_v^2 + \mu_u^2 + \dfrac{W_b}{k}\right) \right]}\right\\}
\end{equation}

### Solution

The final solution is:


with \\(\alpha = k/( \rho c_p ) \\), and \\(\phi_a(y)\\) and \\(\phi_b(y)\tau_b(t)\\) expressed as:

\begin{equation}
    \phi_a = \frac{Q_t}{W_b} + \left\\{ \left( T_s - T_c \right) + \dfrac{Q_t}{W_b}\left[ \cosh{ \left( H\sqrt{\dfrac{W_b}{k}} \right) } - 1\right] \right\\}\frac{\sinh{ \left( y\sqrt{\dfrac{W_b}{k}} \right)}}{\sinh{ \left( H\sqrt{\dfrac{W_b}{k}} \right) }} \ldots\\\
     \ldots - \frac{Q_t}{W_b} \cosh{ \left( y\sqrt{\dfrac{W_b}{k}} \right) }
\end{equation}

\begin{equation}
    \lambda_m = \frac{m\pi}{H}
\end{equation}

\begin{eqnarray}
    c_m &=& c_{m1} + c_{m2} + c_{m3}\\\
    c_{m1} &=& \frac{2Q_t}{HW_b\lambda_m}\left[\left( -1 \right)^m - 1 \right]\\\
    c_{m2} &=& \left( -1 \right)^m \frac{2\lambda_m c_1}{H} \frac{\sinh{ \left( H\sqrt{\dfrac{W_b}{k}} \right)}  }{ \dfrac{W_b}{k} + \lambda_m^2}\\\
    c_{m3} &=& \frac{2Q_t\lambda_m}{HW_b} \frac{ 1 - \left( -1 \right)^m\cosh{ \left( H\sqrt{\dfrac{W_b}{k}} \right)}
    }{ \dfrac{W_b}{k} + \lambda_m^2}
\end{eqnarray}

\begin{equation}
    \phi_b\tau_b = \sum_{m=1}^{\infty}c_m \sin{\left(\lambda_m y\right)} \exp{\left[-\alpha t \left(\lambda_m^2 + \dfrac{W_b}{k} \right)\right]}
\end{equation}

\begin{equation}
    \gamma_o = \frac{ \left( 2o - 1 \right) \pi}{H}
\end{equation}

\begin{equation}
    \phi_c = \sum_{o=1}^{\infty} \frac{4q_x\cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_o y\right)}}{Hk\gamma_o\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)} }
    \label{eq:c:solution}
\end{equation}

\begin{equation}
    \beta_r = \frac{r\pi}{L}
\end{equation}

\begin{equation}
    \phi_d\tau_d = \sum_{o=1}^{\infty} -\frac{4q_x\sin{ \left( \gamma_o y\right) }}{HLk\gamma_o}
    \left\\{ \frac{\exp{\left[ -\alpha t \left( \gamma_o^2 + \dfrac{W_b}{k}\right) \right]}}{ \gamma_o^2 + \dfrac{W_b}{k} }
    \ldots\\\
    \ldots + 
    \sum_{r=1}^{\infty} \left( -1 \right)^r\frac{2\cos{ \left( \beta_r x \right) }}{\beta_r^2 + \gamma_o^2 + \dfrac{W_b}{k}} \exp{\left[ -\alpha t \left(  \beta_r^2 + \gamma_o^2 + \dfrac{W_b}{k}\right) \right]}\right\\}
    \label{eq:d:solution}
\end{equation}

\begin{equation}
    \mu_u = \frac{ \left( 2u - 1 \right) \pi}{H}
\end{equation}

\begin{equation}
    \phi_e = \sum_{u=1}^{\infty} \frac{4q_z\cosh{\left(z\sqrt{\mu_u^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\mu_u y\right)}}{Hk\mu_u\sqrt{\mu_u^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\mu_u^2 +\dfrac{W_b}{k}}\right)} }
    \label{eq:e:solution}
\end{equation}

\begin{equation}
    \nu_v = \frac{v\pi}{T_z}
\end{equation}

\begin{equation}
    \phi_f\tau_f = \sum_{u=1}^{\infty} -\frac{4q_z\sin{ \left( \mu_u y\right) }}{HT_zk\mu_u}
    \left\\{ \frac{\exp{\left[ -\alpha t \left( \mu_u^2 + \dfrac{W_b}{k}\right) \right]}}{ \mu_u^2 + \dfrac{W_b}{k} }
    \ldots\\\
    \ldots + 
    \sum_{v=1}^{\infty} \left( -1 \right)^v\frac{2\cos{ \left( \nu_v z \right) }}{\nu_v^2 + \mu_u^2 + \dfrac{W_b}{k}} \exp{\left[ -\alpha t \left(  \nu_v^2 + \mu_u^2 + \dfrac{W_b}{k}\right) \right]}\right\\}
    \label{eq:f:solution}
\end{equation}


The flux/heat flux can be obtained by derivating these equations with respect to \\(x\\), \\(y\\), and \\(z\\).

Note that these solutions are not suitable for \\(W_b = 0\\). In this case, the correct solutions are for the [heat equation shown here.]({{ site.baseurl }}{% link theory/ana/heat/heat 3D TTqqqq.md %})

### Using the Algorithm

The algorithm `D3_BHE_f.m` was written as a function in Octave/Matlab and you can obtain it [here](https://github.com/hugomilan/tlmbht/tree/master/src/octave/analytical/3D). To use it, you will need to define the material properties and the maximum values for \\(m\\), \\(o\\), \\(r\\), \\(u\\), and \\(v\\).

As an example, we can use the following script:

{% highlight matlab %}
% Thermal properties
Tc = 37; % core and initial temperature (oC)
Ts = 150; % superficial temperature (oC)
qx = 1e5; % heat flux at x = L (W/m2)
qz = 2e5; % heat flux at z = Tz (W/m2)
rho = 1200; % tissue density (kg/m3)
cp = 3200; % specific heat (J/(K-kg))
k = 0.3; % thermal conductivity (W/(K-m))
wb = 1e-4; % blood perfusion (s-1)
rhob = 1052; % blood density (kg/m3)
cb = 3600; % blood specific heat (J/(K-kg))
Tb = 37; % temperature of the blood (oC)

Qmet = 500; % internal heat generation (W/m3)
% If there is another heat source S besides Qmet, sum it to Qmet.
% For example, if Qmet = 250 and S = 250, you would input Qmet = 500.
    
% Material properties
H = 1e-3; % m
L = 0.75e-3; % m
Tz = 0.375e-3; % m
yprime = 0:(H/100):H;
xprime = 0:(L/100):L;
zprime = Tz/2;
y = repmat( yprime, 1, size(xprime,2)); % creating a subspace for y
x = repelems( xprime, [1:size(xprime,2); ...
        size(yprime,2)*ones(1,size(xprime,2))]); % creating a subspace for x
    
% we will be observing the restuls at only one z level.
z = zprime*ones(1, size(x,2));

% number of iterations
minf = 50; % for m
oinf = 100; % for o
rinf = 50; % for r
uinf = 100; % for u
vinf = 50; % for v
    
% time of the solution
t = 0.5;
    
[Tprime, qxprime, qyprime, qzprime] = D3_BHE_f(x, y, z, L, H, Tz, ...
        t, Ts, Tc, qx, qz, k, rho, cp, wb, rhob, cb, Tb, Qmet, ...
        minf, oinf, rinf, uinf, vinf);

T = reshape(Tprime, size(yprime,2), size(xprime,2));
qxout = reshape(qxprime, size(yprime,2), size(xprime,2));
qyout = reshape(qyprime, size(yprime,2), size(xprime,2));
qzout = reshape(qzprime, size(yprime,2), size(xprime,2));
    
surf(xprime, yprime, T)
    
figure;
    
surf(xprime, yprime, qxout)

figure;
    
surf(xprime, yprime, qyout)

figure;
    
surf(xprime, yprime, qzout )

figure;
    
surf(xprime, yprime, sqrt(qxout.^2 + qyout.^2 + qzout.^2) )
{% endhighlight %}

which generates

<img src="{{ site.baseurl }}/assets/images/theory/ana/pennes 3D TTqqqq plot.png" alt="Solution of the problem for Pennes equation">

### Limitations

Every solution method has limitations. The most noticeable limitation is that we have to perform infinity sums to get predictions using solutions from separation of variables. Infinity sums, however, cannot be performed in numerical computations, which leads us to truncate the calculations for large values of \\(m\\), \\(o\\), \\(r\\), \\(u\\), and \\(v\\). How large these numbers have to be will depend on the problem geometry and on the material properties but, if theses number are not large enough, the calculations will manifest oscillations that will be most evident for predicted fluxes positioned near the flux boundary conditions (that is, near \\(x = L\\) and \\(z = T_z\\) in this solution). When observed that \\(m\\), \\(o\\), \\(r\\), \\(u\\), and/or \\(v\\) have to be increased, a rule-of-thumb is to double their values, re-calculate the predicionts, and observe if the oscillations have decreased to acceptable intensities.

Another surprising limitation is that the flux boundary conditions at \\(x = L\\) and \\(z = T_z\\) *are not* satisfied when \\(y = 0\\) or \\(y = H\\). You can derivate Eqs. \ref{eq:c:solution} and \ref{eq:d:solution} with respect to \\(x\\) and Eqs. \ref{eq:e:solution} and \ref{eq:f:solution} with respect to \\(z\\) to observe that the term \\(\sin{\left(\gamma_o y\right)}\\) vanishes when \\(y = 0\\) or \\(y = H\\) no matter what is the value of the boundary condition. The solution, however, is accurate for \\(y \rightarrow 0\\) and \\(y \rightarrow H\\) and the concentration/temperature is not affected by this peculiarity.

### Done

Now, you can go to:

* [Analytical solutions menu]({{ site.baseurl }}{% link theory/ana/index.md %})
* [Analytical solutions of Pennes equations menu]({{ site.baseurl }}{% link theory/ana/pennes/index.md %})
* [TLM validation using this solution]({{ site.baseurl }}{% link vte/validation/pennes 3D tetrahedron.md %})
