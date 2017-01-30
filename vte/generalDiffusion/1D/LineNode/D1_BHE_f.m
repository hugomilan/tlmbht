function [T, qy, phi_a, qy_phi_a, phi_b_tau_b, qy_phi_b_tau_b] = ...
    D1_BHE_f(y, H, t, Ts, Tc, k, p, cp, wb, pb, cb, Tb, ...
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
% y (vector with the positions in y to get outputs, m). Vector column.
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
% Tb (blood's temperature, oC)
% Qm (tissue's metabolic heat, W/m3)
% minf (number of repetition of the index m). If in doubt, chose 50 for
%     fast (and still accurate) results or 500 for more accurate results.
%
%
% The outputs of this function are:
% T (temperature at the given positions, oC)
% qy (heat flux in the y direction calculated at the given positions, W/m2)
% phi_a (temperatures calculated using the function phi_a at the given positions, oC)
% qy_phi_a (heat flux in the y direction calculated using the function phi_a at the given positions, W/m2)
% phi_b_tau_b (temperatures calculated using the function phi_b_tau_b at the given positions, oC)
% qy_phi_b_tau_b (heat flux in the y direction calculated using the function phi_b_tau_b at the given positions, W/m2)
%
%
% this algorithm uses the principle of superposition to make
% T = Tc + phi_a(x) + phi_b(x)tau_b(t)
%
%
%
% This algorithm is not intended for wb = 0. For this case, you should use
% D1_HEAT_f
%

% 
% File:   D1_BHE_f.m
% Author: Hugo Fernando Maia Milan
% Email:  hugofernando@gmail.com
%
% Created on Dec 09, 2015.
%
%
%
% Revision history:
% 
% Date: Jan 30, 2017
% Who: Hugo Milan
% Description: making the nomenclature compatible with the analytical solution in the website
%

if wb == 0;
  disp('The input has wb = 0, which is not suitable to use in this solution. Please, use the function D1_HEAT_f instead.');
  return;
end

% defining constants
alpha = k/(p*cp); % thermal difusivity (m2/s)
Wb = wb*pb*cb; % heat dissipation through blood perfusion
QT = Qmet + Wb*( Tb - Tc ); % total heat generation
theta_inf = Ts - Tc; % boundary condition for solution of phi_a
QWb = QT/Wb; % constant for solution of phi_a
Wbk = sqrt(Wb/k); % constant for solution of phi_a
WbkH = Wbk*H; % constant for solution of phi_a
c1 = ( theta_inf + QWb*( cosh(WbkH) - 1 ) )/sinh(WbkH); % constant for solution of phi_a

% solution phi_a(y). The boundary conditions are:
% y = 0 => phi_a = 0.
% y = H => phi_a = theta_inf.
phi_a = QWb + c1*sinh(Wbk*y) - QWb*cosh(Wbk*y);
qy_phi_a = - k*c1*Wbk*cosh(Wbk*y) + k*QWb*Wbk*sinh(Wbk*y);

% variable for solution of phi_b(y)tau_b(t). The boundary conditions are: 
% y = 0 => phi_b(y)tau_b(t) = 0.
% y = H => phi_b(y)tau_b(t) = theta_inf.
% The initial condition is: t = 0 => phi_b_tau_b = - phi_a(y).
phi_b_tau_b = zeros(1,size(y,2));
qy_phi_b_tau_b = zeros(1,size(y,2));


for m = 1:minf    
    lambda = m*pi/H; % comes from separation of variables. Used for solution of phi_b_tau_b
    gamma = sqrt(Wb/k + lambda^2); % appears because of blood perfusion.
    
    C3m1 = 2*QWb/( H*lambda )*( (-1)^m - 1 );
    C3m2 = 2*(-1)^m*lambda*c1/H*sinh(WbkH)/( lambda^2 + Wb/k );
    C3m3 = 2*QWb/H*lambda*( 1 - (-1)^m*cosh(WbkH) )/( lambda^2 + Wb/k );
    
    C3m = C3m1 + C3m2 + C3m3;
    
    phi_b_tau_b = C3m*sin( lambda*y )*exp( -(lambda^2 + Wb/k)*alpha*t ) + phi_b_tau_b;
    qy_phi_b_tau_b = -k*C3m*lambda*cos(lambda*y)*exp( -(lambda^2 + Wb/k)*alpha*t ) + qy_phi_b_tau_b;
    
end



% at the end we sum the solutions.
T = Tc + phi_a + phi_b_tau_b;
qy = qy_phi_a + qy_phi_b_tau_b;
