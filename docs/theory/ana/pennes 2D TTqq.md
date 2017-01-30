---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page_eqAMS_Disqus
permalink: theory/ana/pennes-2D-TTqq.html
---

# Analytical solution of the Pennes equation in time-domain for two-dimensions
<span style="color:#697473">Jan 30, 2017</span> by [**Hugo Milan**](https://hugomilan.github.io/)

You can download the algorithm `D2_BHE_f.m` for Octave/Matlab that can solve this problem [here](https://github.com/hugomilan/tlmbht/tree/master/src/octave/analytical/2D) and you can see instructions in [how to use it here]({% link theory/ana/pennes 2D TTqq.md%}#using-the-algorithm). If you want to see the final solution, [go to Solution]({% link theory/ana/pennes 2D TTqq.md%}#solution).

In this page, we will solve the dynamic Pennes equation in two-dimensions using the principles of superposition and separation of variables. We build on the previous solution of the Pennes equation in one-dimension described [here]({{ site.baseurl }}{% link theory/ana/pennes 2D TTqq.md%}) to solve this two-dimensional problem. The problem we will solve is restricted to the following initial and boundary conditions:

$$
\begin{matrix}
    T(x,y, t = 0) = T_{c}\\
    T(x,y = 0, t) = T_{c}\\
    T(x,y = H, t) = T_{s}\\
    \dfrac{\partial T}{\partial x}(x = 0, y, t) = 0\\
    \dfrac{\partial T}{\partial x}(x = L, y, t) = \dfrac{q_x}{k}
\end{matrix}
$$

The problem geometry for the pennes equation can be represented as shown below.
<img src="{{ site.baseurl }}/assets/images/theory/ana/heat 2D TTqq.png" alt="Geometry of the two-dimensional problem for Pennes equation" width="500">

This geometry is a plate in a three-dimensional space, as you can see in the three-dimensional model below.
<div class="sketchfab-embed-wrapper"><iframe width="640" height="480" src="https://sketchfab.com/models/d8000a11836843989111d9b9b7c03b0f/embed" frameborder="0" allowvr allowfullscreen mozallowfullscreen="true" webkitallowfullscreen="true" onmousewheel=""></iframe>

<p style="font-size: 13px; font-weight: normal; margin: 5px; color: #4A4A4A;">
    <a href="https://sketchfab.com/models/d8000a11836843989111d9b9b7c03b0f?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">Plate 0.75 x 1</a>
    by <a href="https://sketchfab.com/hugomilan?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">hugomilan</a>
    on <a href="https://sketchfab.com?utm_medium=embed&utm_source=website&utm_campain=share-popup" target="_blank" style="font-weight: bold; color: #1CAAD9;">Sketchfab</a>
</p>
</div>

### Governing equations

The Pennes equation is defined as:

\begin{equation}
    \rho c_{p}\frac{\partial T}{\partial t} = k\left( \frac{\partial^2 T}{\partial x^2} + \frac{\partial^2 T}{\partial y^2} \right) +  S + Q_m + \omega_b\rho_b c_{b} (T_b - T)
    \label{eq:Pennes}
\end{equation}

where \\(T\\) is temperature, \\(t\\) is time, \\(\rho\\) is density, \\(c_{p}\\) is specific heat, \\(k\\) is heat conductivity, and \\(S\\) is source, \\(Q_m\\) is metabolic heat generation, \\(\omega_b\\) is blood perfusion, \\(\rho_b\\) is blodd density, \\(c_{b}\\) is blood specific heat, and \\(T_b\\) is blood temperature.


### Solving

I will use the principle of suporposition so that:

\begin{equation}
    T(y,t) = T_c + \phi_a(y) + \phi_b(y)\tau_b(t) + \phi_c(x, y) + \phi_d(x,y)\tau_d(t)
    \label{eq:Sup}
\end{equation}

and the initial and boundary conditions for these problems are:

$$
\begin{matrix}
    & T(x, y,t) & \phi_a(y) & \phi_b(y)\tau_b(t) & \phi_c(x,y) & \phi_d(x,y)\tau_d(t)\\
    t = 0 & T = T_c & - & \phi_b\tau_b = -\phi_a & - & \phi_d\tau_d = -\phi_c\\
    y = 0 & T = T_c & \phi_a = 0 & \phi_b\tau_b = 0 & \phi_c = 0 & \phi_d\tau_d = 0\\
    y = H & T = T_s & \phi_a = T_s - T_c & \phi_b\tau_b = 0 & \phi_c = 0 & \phi_d\tau_d = 0\\
    x = 0 & \dfrac{\partial T}{\partial x} = 0 & - & - & \dfrac{\partial \phi_c}{\partial x} = 0 & \tau_d\dfrac{\partial \phi_d}{\partial x} = 0\\
    x = L & \dfrac{\partial T}{\partial x} = \dfrac{q_x}{k} & - & - & \dfrac{\partial \phi_c}{\partial x} = \dfrac{q_x}{k} & \tau_d\dfrac{\partial \phi_d}{\partial x} = 0
\end{matrix}
$$

Defining \\(W_b = \omega_b\rho_b c_{b} \\) and applying Eq. \ref{eq:Sup} in \ref{eq:Pennes} yields

\begin{equation}
    \left( k\frac{\partial^2 \phi_a}{\partial y^2} - W_b\phi_a + S + Q_m + W_b(T_b - T_c) \right) \ldots \\\
    \ldots - \left( \rho c_{p}\phi_b\frac{\partial \tau_b}{\partial t} - k\tau_b\frac{\partial^2 \phi_b}{\partial y^2} + W_b\phi_b\tau_b \right) = \ldots \\\
    \ldots \left( -k \frac{\partial^2 \phi_c}{\partial x^2} - k \frac{\partial^2 \phi_c}{\partial y^2} + W_b\phi_c \right) \ldots \\\
    \ldots + \left( \rho c_{p}\phi_d\frac{\partial \tau_d}{\partial t} - k \tau_d\frac{\partial^2 \phi_d}{\partial x^2} - k \tau_d\frac{\partial^2 \phi_d}{\partial y^2} + W_b\phi_d\tau_d\right) 
\end{equation}

which implies that we can solve Eqs. \ref{eq:a}-\ref{eq:d}
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



#### **Solving Eq. \ref{eq:a} and \ref{eq:b}**

The solution of Eq. \ref{eq:a} and \ref{eq:b} was previously covered when we [solved the pennes equation in one-dimension.]({{ site.baseurl }}{% link theory/ana/pennes 1D TT.md%})




#### **Solving Eq. \ref{eq:c}**

We will use separation of variables to solve Eq. \ref{eq:c}. In separation of variables, we assume that the solution is the multiplication of a function of \\(x\\) ( \\(\phi_{cx}(x)\\) ) and \\(y\\) ( \\(\phi_{cy}(y)\\) ). Hence,

\begin{equation}
    \frac{1}{\phi_{cx}}\frac{\partial^2 \phi_{cx}}{\partial x^2} - \frac{W_b}{k}= -\frac{1}{\phi_{cy}}\frac{\partial^2 \phi_{cy}}{\partial y^2} = \gamma_n^2
\end{equation}


The solution of \\(\phi_{cy}\\) can be expressed as:

\begin{equation}
    \phi_{cy} = c_6 \sin{\left(\gamma_n y\right)} + c_7 \cos{\left(\gamma_n y\right)}
\end{equation}

Applying the boundary condition at \\(y = 0\\) we get \\(c_7 = 0\\). Applying the boundary condition at \\(y = H\\) we get:

\begin{equation}
    \sin{\left(\gamma_n H\right)} = 0
    \label{eq:gamman}
\end{equation}

which implies that 

\begin{equation}
    \gamma_n = \frac{n\pi}{H}
\end{equation}

with \\(n = 1, 2, 3, \dotsc\\) We start the counting from \\(1\\) because \\(n = 0\\) yields \\(\phi_{cy} = 0\\). The solution of \\(\phi_{cx}\\) can be expressed as:

\begin{equation}
    \phi_{cx} = c_8 \sinh{\left(x \sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\right)} + c_9 \cosh{\left(x\sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\right)}
\end{equation}

Applying the boundary condition at \\(x = 0\\) we get \\(c_8 = 0\\). Then, the solution that we are looking for is:

\begin{equation}
    \phi_c = \sum_{n=1}^{\infty} c_n \cosh{\left(x\sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_n y\right)}
    \label{eq:c:almost}
\end{equation}

Now, we apply the non-homogeneous boundary condition at \\(x = L\\):

\begin{equation}
    \frac{\partial\phi_c}{\partial x}(x = L) = \sum_{n=1}^{\infty} c_n \sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\sinh{\left( L\sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_n y\right)} = \dfrac{q_x}{k}
    \label{eq:c:cn}
\end{equation}

To solve Eq. \ref{eq:c:cn}, we multiply both sides by an [orthogonal function](https://en.wikipedia.org/wiki/Orthogonal_functions) of the function that multiplies \\(c_n\\) and integrate it from \\(y = 0\\) to \\(y = H\\) to elimate the \\(y\\) dependence. That is, we need a function that is orthogonal to \\(\sin{\left(\gamma_n y\right)}\\). The function that we are looking for is \\(\sin{\left(\gamma_m y\right)}\\). Multiplying both sides by this function yields

\begin{equation}
    \int_{y=0}^{y=H}\sum_{n=1}^{\infty}c_n \sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_n y\right)}\sin{\left(\gamma_m y\right)} dy = \ldots\\\
    \ldots \int_{y=0}^{y=H}\dfrac{q_x}{k}\sin{\left(\lambda_n y\right)} dy
    \label{eq:b:cn2}
\end{equation}

Since we have

\begin{equation}
    \int_{y=0}^{y=H} \sin{\left(\gamma_n y\right)}\sin{\left(\gamma_m y\right)} dy = 0
    \label{eq:mnn}
\end{equation}

for \\(n \neq m\\), and

\begin{equation}
    \int_{y=0}^{y=H} \sin{\left(\gamma_n y\right)}\sin{\left(\gamma_m y\right)} dy = \frac{H}{2}
    \label{eq:mn}
\end{equation}

for \\(n = m\\), we can drop the summation and solve Eq. \ref{eq:b:cn2} for each \\(c_n\\):

\begin{equation}
    c_n =  \dfrac{\displaystyle \dfrac{q_x}{k}\int_{y=0}^{y=H}\sin{\left(\lambda_n y\right)} dy}{ \displaystyle \sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_n^2 +\dfrac{W_b}{k}}\right)}\int_{y=0}^{y=H}\sin^2{\left(\lambda_m y\right)} dy  }
    \label{eq:b:cn3}
\end{equation}

The solutions of the integral on the numerator is:

\begin{equation}
   \int_{y=0}^{y=H}  \sin{\left(\gamma_n y\right)} dy = \frac{1}{\gamma_n}\left[1 - \cos{ \left( \gamma_n H\right) } \right]
\end{equation}


From the definition of \\(\gamma_n\\) in Eq. \ref{eq:gamman},

\begin{equation}
    \int_{y=0}^{y=H}  \sin{\left(\gamma_n y\right)} dy = \frac{1}{\gamma_n}\left[1 - \left( -1 \right)^n \right]
    \label{eq:c:int}
\end{equation}

From Eq. \ref{eq:c:int} it is obvious that the summation in Eq. \ref{eq:c:almost} will be non-zero only for odd values of \\(n\\). Hence, we define

\begin{eqnarray}
    n &=& 2o - 1\\\
    \gamma_o &=& \frac{ \left( 2o - 1 \right) \pi}{H}
\end{eqnarray}

Therefore, the final solution of Eq. \ref{eq:c} is:

\begin{equation}
    \phi_c = \sum_{o=1}^{\infty} \frac{4q_x\cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_o y\right)}}{Hk\gamma_o\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)} } 
    \label{eq:c:final}
\end{equation}

Although Eq. \ref{eq:c:final} can be used when \\(W_b=0\\), I recommend the use of the solutions for the [heat equation shown here]({{ site.baseurl }}{% link theory/ana/heat 2D TTqq.md %}) for these cases.

#### **Solving Eq. \ref{eq:d}**

We will use separation of variables to solve Eq. \ref{eq:d}. In separation of variables, we assume that the solution is the multiplication of a function of \\(x\\) ( \\(\phi_{dx}(x)\\) ) and \\(y\\) ( \\(\phi_{dy}(y)\\) ) and \\(t\\) \\( ( \tau_d(t) ) \\). Hence,

\begin{equation}
    \frac{\rho c_p}{k \tau_d}\frac{\partial \tau_d}{\partial t} + \frac{W_b}{k} = \frac{1}{\phi_{dx}}\frac{\partial^2 \phi_{dx}}{\partial x^2} + \frac{1}{\phi_{dy}}\frac{\partial^2 \phi_{dy}}{\partial y^2} = - \eta_p^2
\end{equation}

Defining \\(\alpha = k/( \rho c_p ) \\), the solution of \\( \tau_d \\) is expressed as:

\begin{equation}
    \tau_d = c_{10}\exp{\left[ -\alpha t\left(\eta_p^2 + \frac{W_b}{k} \right) \right]}
\end{equation}


Now, to solve for \\( \phi_d \\), we define:

\begin{equation}
    \frac{1}{\phi_{dx}}\frac{\partial^2 \phi_{dx}}{\partial x^2} = - \frac{1}{\phi_{dy}}\frac{\partial^2 \phi_{dy}}{\partial y^2} - \eta_p^2 = - \beta_r^2
\end{equation}

Hence, the solution of \\( \phi_{dx} \\) is:

\begin{equation}
    \phi_{dx} = c_{11}\sin{ \left( \beta_r x \right) } + c_{12}\cos{ \left( \beta_r x \right) }
\end{equation}

Applying the boundary condition at \\(x=0\\) yields \\(c_{11}=0\\). Applying the boundary condition at \\(x=L\\):

\begin{equation}
    \frac{\partial\phi_{dx}}{\partial x}(x=L) = 0 = \sin{ \left( \beta_r L \right) }
\end{equation}

which implies that

\begin{equation}
    \beta_r = \frac{r\pi}{L}
    \label{eq:d:beta}
\end{equation}

with \\(r=0,1,2,\ldots\\) We start counting in 0 because \\(r=0\\) does not necessarily imply \\(\phi_{dx}=0\\). Now, the solution of \\(\phi_{dy}\\) is:

\begin{equation}
    \phi_{dy} = c_{13}\sin{ \left( y\sqrt{\eta_p^2 - \beta_r^2} \right) } + c_{14}\cos{ \left( y\sqrt{\eta_p^2 - \beta_r^2} \right) }
\end{equation}

Applying the boundary condition at \\(y=0\\) yields \\(c_{13}=0\\). Defining \\(\delta_s^2 = \eta_p^2 - \beta_r^2\\) and applying the boundary condition at \\(y=H\\) yields:

\begin{equation}
    \sin{ \left( H\delta_s \right) } = 0
\end{equation}

which implies that

\begin{equation}
    \beta_s = \frac{s\pi}{H}
\end{equation}

with \\(s=1,2,3,\ldots\\) Therefore, the solution that we are looking for is:

\begin{equation}
    \phi_d\tau_d = \sum_{r=0}^{\infty}\sum_{s=1}^{\infty}c_{rs}\cos{ \left( \beta_r x \right) }\sin{ \left( \delta_s y\right) }\exp{\left[ -\alpha\ t \left(  \beta_r^2 + \delta_s^2 + \frac{W_b}{k} \right) \right]}
\end{equation}

Now, we apply the initial condition

\begin{equation}
    \sum_{r=0}^{\infty}\sum_{s=1}^{\infty}c_{rs}\cos{ \left( \beta_r x \right) }\sin{ \left( \delta_s y\right) } = \ldots \\\
    \ldots -\sum_{o=1}^{\infty} \frac{4q_x\cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_o y\right)}}{Hk\gamma_o\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)} } 
    \label{eq:d:crs}
\end{equation}

Once more, to solve Eq. \ref{eq:d:crs}, we are looking for [orthogonal function](https://en.wikipedia.org/wiki/Orthogonal_functions) of the functions that multiplies \\(c_{rs}\\). We want to integrate these functions from \\(x = 0\\) to \\(x = L\\) to elimate the \\(x\\) dependence and from \\(y = 0\\) to \\(y = H\\) to elimate the \\(y\\) dependence. That is, we need a function that is orthogonal to \\(\cos{\left(\beta_r x\right)}\sin{\left(\delta_s y\right)}\\). The function that we are looking for is \\(\cos{\left(\beta_u x\right)}\sin{\left(\delta_v y\right)}\\). Multiplying both sides by this function yields

\begin{equation}
    \int_{x=0}^{x=L}\int_{y=0}^{y=H}\sum_{r=0}^{\infty}\sum_{s=1}^{\infty}c_{rs}\cos{ \left( \beta_r x \right) }\sin{ \left( \delta_s y\right) }\cos{\left(\beta_u x\right)}\sin{\left(\delta_v y\right)} dydx= \ldots \\\
    \ldots -\int_{x=0}^{x=L}\int_{y=0}^{y=H}\sum_{o=1}^{\infty} \frac{4q_x\cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_o y\right)}}{Hk\gamma_o\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)} }\cos{\left(\beta_u x\right)}\sin{\left(\delta_v y\right)}dydx
    \label{eq:int1}
\end{equation}

From Eq. \ref{eq:mnn} and \ref{eq:mn}, the left-hand side of Eq. \ref{eq:int1} will be non-zero when \\(\delta_s = \delta_v\\). Moreover, the right-hand side will be non-zero when \\(\gamma_o = \delta_v\\), so, we have \\(\delta_s = \gamma_o\\), which implies that Eq. \ref{eq:int1} will be non-zero for \\(s = o\\) and simplified to

\begin{equation}
    \int_{x=0}^{x=L}\sum_{r=0}^{\infty}c_{rs}\cos{ \left( \beta_r x \right) }\cos{\left(\beta_u x\right)} dx = \ldots \\\
    \ldots \int_{x=0}^{x=L} \frac{4q_x\cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}}{Hk\gamma_o\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)} } \cos{\left(\beta_u x\right)}dx
    \label{eq:int2}
\end{equation}


Since we have

\begin{equation}
    \int_{x=0}^{x=L} \cos{\left(\beta_r x\right)}\cos{\left(\beta_u x\right)} dx = 0
    \label{eq:ruu}
\end{equation}

for \\(r \neq u\\), and

\begin{equation}
    \int_{x=0}^{x=L} \cos{\left(\beta_r x\right)}\cos{\left(\beta_u x\right)} dx = \frac{L}{2}
    \label{eq:ru}
\end{equation}

for \\(r = u\\), we can drop the summation over \\(r\\) and solve Eq. \ref{eq:int2} for each \\(c_{rs}\\):

\begin{equation}
    c_{rs} = -\dfrac{\displaystyle  \int_{x=0}^{x=L} \frac{4q_x\cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}}{Hk\gamma_o\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)} } \cos{\left(\beta_u x\right)}dx  }{ \displaystyle  \int_{x=0}^{x=L}\cos^2{ \left( \beta_r x \right) } dx  }
    \label{eq:int3}
\end{equation}

The solution of the integral in the numerator is 
\begin{equation}
    \int_{x=0}^{x=L} \cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\cos{\left(\beta_r x\right)}dx = \ldots\\\
    \ldots \frac{\\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left( L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\cos{\left(\beta_r L\right)} + \beta_r\cosh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\beta_r L\right)}}{\gamma_o^2 + \dfrac{W_b}{k} + \beta_r^2}
\end{equation}

which, from the definition of \\(\beta_r\\) (Eq. \ref{eq:d:beta}), becomes:
\begin{equation}
    \int_{x=0}^{x=L} \cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\cos{\left(\beta_r x\right)}dx = \ldots\\\
    \ldots\left( -1 \right)^r\frac{\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}}{\gamma_o^2 + \dfrac{W_b}{k} + \beta_r^2}
\end{equation}

Therefore, the final solution of Eq. \ref{eq:d} is:

\begin{equation}
    \phi_d\tau_d = \sum_{r=0}^{\infty}\sum_{o=1}^{\infty} -\left( -1 \right)^r\frac{8q_x}{HLk\gamma_o \left( \gamma_o^2 + \dfrac{W_b}{k} + \beta_r^2 \right)} \cos{ \left( \beta_r x \right) }\sin{ \left( \gamma_o y\right) }\exp{\left[ -\alpha\ t \left(  \beta_r^2 + \gamma_o^2 + \dfrac{W_b}{k}\right) \right]}
    \label{eq:d:final}
\end{equation}

Although Eq. \ref{eq:d:final} can be used when \\(W_b=0\\), I recommend the use of the solutions for the [heat equation shown here]({{ site.baseurl }}{% link theory/ana/heat 2D TTqq.md %}) for these cases.

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
    \phi_b\tau_b = \sum_{m=1}^{\infty}c_m \sin{\left(\lambda_m y\right)} \exp{\left[-\alpha t \left(\lambda_m + \sqrt{\dfrac{W_b}{k}} \right)\right]}
\end{equation}

\begin{equation}
    \gamma_o = \frac{ \left( 2o - 1 \right) \pi}{H}
\end{equation}

\begin{equation}
    \phi_c = \sum_{o=1}^{\infty} \frac{4q_x\cosh{\left(x\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)}\sin{\left(\gamma_o y\right)}}{Hk\gamma_o\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\sinh{\left(L\sqrt{\gamma_o^2 +\dfrac{W_b}{k}}\right)} }
\end{equation}

\begin{equation}
    \beta_r = \frac{r\pi}{L}
\end{equation}

\begin{equation}
    \phi_d\tau_d = \sum_{r=0}^{\infty}\sum_{o=1}^{\infty} -\left( -1 \right)^r\frac{8q_x}{HLk\gamma_o \left( \gamma_o^2 + \dfrac{W_b}{k} + \beta_r^2 \right)} \cos{ \left( \beta_r x \right) }\sin{ \left( \gamma_o y\right) }\exp{\left[ -\alpha\ t \left(  \beta_r^2 + \gamma_o^2 + \dfrac{W_b}{k}\right) \right]}
\end{equation}


The flux/heat flux can be obtained by derivating these equations with respect to \\(x\\) and \\(y\\).

Note that these solutions are not suitable for \\(W_b = 0\\). In this case, the correct solutions are for the [heat equation shown here.]({{ site.baseurl }}{% link theory/ana/heat 2D TTqq.md %})

### Using the Algorithm

The algorithm `D2_BHE_f.m` was written as a function in Octave/Matlab and you can obtain it [here](https://github.com/hugomilan/tlmbht/tree/master/src/octave/analytical/2D). To use it, you will need to define the material properties and the maximum values for \\(m\\), \\(o\\), and \\(r\\).

As an example, we can use the following script:

{% highlight matlab %}
% Thermal properties
Tc = 37; % core and initial temperature (oC)
Ts = 150; % superficial temperature (oC)
qx = 1e5; % heat flux at x = L (W/m2)
rho = 1200; % tissue density (kg/m3)
cp = 3200; % specific heat (J/(K-kg))
k = 0.3; % thermal conductivity (W/(K-m))
Ti = 37; % initial temperature
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
yprime = 0:(H/100):H;
xprime = 0:(L/100):L;
y = repmat( yprime, 1, size(xprime,2)); % creating a subspace for y
x = repelems( xprime, [1:size(xprime,2); ...
        size(yprime,2)*ones(1,size(xprime,2))]) ; % creating a subspace for x
    
% number of iterations
minf = 50; % for m
oinf = 100; % for o
rinf = 50; % for r
    
% time of the solution
t = 0.5;
    
[Tprime, qxprime, qyprime] = D2_BHE_f(x, y, L, H, t, Ts, Tc, qx, ...
        k, rho, cp, wb, rhob, cb, Tb, Qmet, minf, oinf, rinf);

T = reshape(Tprime, size(yprime,2), size(xprime,2));
qxout = reshape(qxprime, size(yprime,2), size(xprime,2));
qyout = reshape(qyprime, size(yprime,2), size(xprime,2));
    
surf(xprime, yprime, T)
    
figure;
    
surf(xprime, yprime, qxout)

figure;
    
surf(xprime, yprime, qyout)

figure;
    
surf(xprime, yprime, sqrt(qxout.^2 + qyout.^2) )
{% endhighlight %}

which generates

<img src="{{ site.baseurl }}/assets/images/theory/ana/pennes 2D TTqq plot.png" alt="Solution of the problem for Pennes equation">

### Done

Now, you can go to:

* [Analytical solutions menu]({{ site.baseurl }}{% link theory/ana/index.md %})
* [Analytical solution in three-dimensions]({{ site.baseurl }}{% link theory/ana/pennes 3D TTqqqq.md %})


