function [T, qy, phi_a, qy_phi_a, phi_b_tau_b, qy_phi_b_tau_b] = ...
    D1_H_BHE_f(y, H, t, Ts, Tc, k, p, cp, tau, wb, pb, cb, Tb, ...
    Qm, minf)
    
% This file is part of TLMBHT. See details of this function below.
% Version 0.1. Copyright © 2015 to 2017 by Cornell University.
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
% tau (thermal relaxation time, or, another definition more appropriated for 
%     biological tissues, "characteristic time needed for accumulating the thermal
%     energy required for propagative transfer to the nearest element within the 
%     nonhomogeneous inner structures", defition from Liu, Chen and Xu, 1999; s)
% wb (blood's perfusion, s-1)
% pb (blood's density, kg/m3)
% cp (blood's specific heat, J/K-kg)
% Tb (blood's temperature, oC)
% Qmet (tissue's metabolic heat, W/m3)
% minf (number of repetition of the index m). If in doubt, chose 500 for
%     fast (and still accurate) results or 5000 for more accurate results.
%
%
%
%
%
% this algorithm uses the principle of superposition to calculate
% T = Tc + phi_a(x) + phi_b(x)tau_b(t)
%
%
%
% This algorithm is not intended for wb = 0. For this case, you should use
% D1_H_HEAT_f
%

% 
% File:   D1_H_BHE_f.m
% Author: Hugo Fernando Maia Milan
% Email:  hugofernando@gmail.com
%
% Created on Jan 08, 2017.
%
%
%
% Revision history:
% 
% Date: Feb 07, 2017
% Who: Hugo Milan
% Description: making the nomenclature compatible with the analytical solution in the website
%


if wb == 0;
  disp('The input has wb = 0, which is not suitable to use in this solution. Please, use the function D1_H_HEAT_f instead.');
  return;
end

% defining constants
alpha = k/(p*cp); % thermal diffusivity (m^2/s)
Wb = wb*pb*cb;
Wbk = Wb/k;
Wbk2 = sqrt(Wbk);
Qt = Qm + Wb*Tb;
QWb = Qt/Wb; % constant for solution of phi_a
alpha = k/(p*cp); % thermal difusivity (m2/s)
alpha2 = alpha*Wb/k + 1/tau;
alpha22 = alpha2^2;

theta_inf = Ts - Tc;
% solution phi_a(y). The boundary conditions are:
% y = 0 => phi_a = 0.
% y = H => phi_a = theta_inf.
c1 = (theta_inf + QWb*( cosh(Wbk2*H) - 1 ) )/sinh(Wbk2*H); % constant for solution of phi_a
phi_a = QWb + c1*sinh(Wbk2*y) - QWb*cosh(Wbk2*y);
qy_phi_a = - k*c1*Wbk2*cosh(Wbk2*y) + k*QWb*Wbk2*sinh(Wbk2*y);

% variable for solution of phi_b(y)tau_b(t). The boundary conditions are: 
% y = 0 => phi_b(y)tau_b(t) = 0.
% y = H => phi_b(y)tau_b(t) = theta_inf.
% The initial conditions are:
% t = 0 => phi_b_tau_b = - phi_a(y).
% t = 0 => d( phi_b_tau_b )/dt = 0.
phi_b_tau_b = zeros(1,size(y,2));
qy_phi_b_tau_b = zeros(1,size(y,2));

for m = 1:minf

  lambda = m*pi/H;
  beta = 4*alpha/tau*(Wb/k + lambda^2);
  
  Am1 = 2*QWb/( H*lambda )*( cos( lambda*H ) - 1 );
  Am2 = 2*c1/H*cos( lambda*H )*sinh(Wbk*H)/( lambda + Wb/( k*lambda ) );
  Am3 = 2*QWb/H*( 1 - cos( lambda*H )*cosh(Wbk*H) )/( lambda + Wb/( k*lambda ) );
  
  Am = Am1 + Am2 + Am3;

 if ( alpha22 > beta )
    
    gamma = 0.5*sqrt(alpha22 - beta);
    Bm = ( 2*gamma + alpha2 )/( 2*gamma - alpha2 );
    
    phi_b_tau_b = phi_b_tau_b + Am/(1 + Bm)*( exp(-gamma*t) + Bm*exp(gamma*t) ) *sin(lambda*y);
    qy_phi_b_tau_b = qy_phi_b_tau_b + Am/(1 + Bm)*( exp(-gamma*t) + Bm*exp(gamma*t) ) * lambda*cos(lambda*y);
    
 else
    
    omega = 0.5*sqrt(beta - alpha22);
    Cm = alpha2/(2*omega);
    
    phi_b_tau_b = phi_b_tau_b + Am*( cos(omega*t) + Cm*sin(omega*t) ) *sin(lambda*y);
    qy_phi_b_tau_b = qy_phi_b_tau_b + Am*( cos(omega*t) + Cm*sin(omega*t) ) * lambda*cos(lambda*y);
    
 end 
end

phi_b_tau_b = exp(-alpha2/2*t)*phi_b_tau_b;
qy_phi_b_tau_b = exp(-alpha2/2*t)*qy_phi_b_tau_b;

% at the end we sum the solutions.
T = Tc + phi_a + phi_b_tau_b;
qy = qy_phi_a + qy_phi_b_tau_b;
