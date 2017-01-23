function [T, qx, phi_a, qx_phi_a, YTau, qx_YTau] = ...
    D1_BHE_f(x, H, t, Ts, Tc, k, p, cp, wb, pb, cb, Ta, ...
    Qmet, minf)
    
% This file is part of TLMBHT. See details of this function below.
% Copyright © 2015 to 2017 by Cornell University.
% Written by Hugo Fernando Maia Milan.
% All Rights Reserved. Refer to the license file for details.
%
% Acknowledgment: Brazilian National Counsel of Technological and Scientific
% Development (Proc. 203312/2014-7) for scholarship to HFMM
%
%
%
% 
% The inputs to this function are:
% x (vector with the positions in x to get outputs, m). Vector column.
% H (tissue's thickness, m)
% t (time instant to get outputs, s)
% Ts (temperature at x = L, oC)
% Tc (initial condition temperature and also temperature at x = 0, oC)
% k (thermal conductivity, W/K-m)
% p (tissue's density, kg/m3)
% cp (tissue's specific heat, J/K-kg)
% wb (blood's perfusion, s-1)
% pb (blood's density, kg/m3)
% cp (blood's specific heat, J/K-kg)
% Ta (arterial blood temperature, oC)
% Qm (tissue's metabolic heat, W/m3)
% minf (number of repetition of the index m). If in doubt, chose 50 for
%     fast (and still accurate) results or 500 for more accurate results.
%
%
%
%
%
% this algorithm uses the principle of superposition to make
% T = Tc + phi_a(x) + YTau(x,t)
%
%
%
% This algorithm is not intended for wb = 0. However, if you input that, we
% will make wb = eps (the minimum value that your computer can use). This
% approximation barely will lead to results different from when wb = 0.


if wb == 0;
    wb = eps;
end

% defining constants
alpha = k/(p*cp); % thermal difusivity (m2/s)
Wb = wb*pb*cb; % heat dissipation through blood perfusion
QT = Qmet + Wb*( Ta - Tc ); % total heat generation
theta_inf = Ts - Tc; % boundary condition for solution of phi_a
QWb = QT/Wb; % constant for solution of phi_a
Wbk = sqrt(Wb/k); % constant for solution of phi_a
WbkH = Wbk*H; % constant for solution of phi_a
c1 = ( theta_inf + QWb*( cosh(WbkH) - 1 ) )/sinh(WbkH); % constant for solution of phi_a

% solution phi_a(y). The boundary conditions are: x = 0 => theta = 0; x = H =>
% theta = theta_inf.
phi_a = QWb + c1*sinh(Wbk*x) - QWb*cosh(Wbk*x);
qx_phi_a = - k*c1*Wbk*cosh(Wbk*x) + k*QWb*Wbk*sinh(Wbk*x);

% variable for solution of YTau(x,t). The boundary conditions are: x = 0 => theta = 0; x = H =>
% theta = theta_inf. The initial condition is: t = 0 => - phi_a(x).
YTau = zeros(1,size(x,2));
qx_YTau = zeros(1,size(x,2));


for m = 1:minf
    % We make use of symmetry to increase m by 2 and speed up the solution.
    
    lambda = m*pi/H; % comes from separation of variables. Used for solution of YTau
    gamma = sqrt(Wb/k + lambda^2); % appears because of blood perfusion.
    
    C3m1 = 2*QWb/( H*lambda )*( cos( lambda*H ) - 1 );
    C3m2 = 2*c1/H*cos( lambda*H )*sinh(WbkH)/( lambda + Wb/( k*lambda ) );
    C3m3 = 2*QWb/H*( 1 - cos( lambda*H )*cosh(WbkH) )/( lambda + Wb/( k*lambda ) );
    
    C3m = C3m1 + C3m2 + C3m3;
    
    YTau = C3m*sin( lambda*x )*exp( -(lambda^2 + Wb/k)*alpha*t ) + YTau;
    qx_YTau = -k*C3m*lambda*cos(lambda*x)*exp( -(lambda^2 + Wb/k)*alpha*t ) + qx_YTau;
    
end



% at the end we sum the solutions.
T = YTau + phi_a + Tc;
qx = qx_YTau + qx_phi_a;