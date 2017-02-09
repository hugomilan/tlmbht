function [T, qy, phi_a, qy_phi_a, phi_b_tau_b, qy_phi_b_tau_b] = ...
    D1_H_HEAT_f(y, H, t, Ts, Tc, k, p, cp, tau, Qext, minf)
    
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
% tau (thermal relaxation time; s)
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
% This algorithm is not intended for tau = 0. For this case, you should use
% D1_HEAT_f
%
%

% 
% File:   D1_H_HEAT_f.m
% Author: Hugo Fernando Maia Milan
% Email:  hugofernando@gmail.com
%
% Created on Feb 07, 2017.
%
%
%
% Revision history:
% 
% Date: 
% Who: 
% Description: 
%


if tau == 0;
  disp('The input has tau = 0, which is not suitable to use in this solution. Please, use the function D1_HEAT_f instead.');
  return;
end

% defining constants
alpha = k/(p*cp); % thermal difusivity (m2/s)
theta_inf = Ts - Tc; % boundary condition for solution of phi_a

c1 = ( theta_inf/H + Qext*H/(2*k) ); % constant for solution of phi_a

% solution phi_a(y). The boundary conditions are:
% y = 0 => phi_a = 0.
% y = H => phi_a = theta_inf.
phi_a = -Qext/(2*k)*y.^2 + c1*y;
qy_phi_a = Qext*y - k*c1;

% variable for solution of phi_b(y)tau_b(t). The boundary conditions are: 
% y = 0 => phi_b(y)tau_b(t) = 0.
% y = H => phi_b(y)tau_b(t) = theta_inf.
% The initial conditions are:
% t = 0 => phi_b_tau_b = - phi_a(y).
% t = 0 => d( phi_b_tau_b )/dt = 0.
phi_b_tau_b = zeros(1,size(y,2));
qy_phi_b_tau_b = zeros(1,size(y,2));

for m = 1:minf

  lambda = m*pi/H; % comes from separation of variables. Used for solution of phi_b_tau_b
    
    C3m1 = c1*2/lambda*(-1)^m;
    C3m2 = Qext/(H*k)*( (-1)^m*( 2 - lambda^2*H^2 ) - 2 )/lambda^3;
    
    C3m = C3m1 + C3m2;

 if ( 4*alpha*lambda^2*tau - 1 < 0 )
    
    phi_b_tau_b = C3m*sin( lambda*y )*...
        ( sqrt(1 - 4*alpha*lambda^2*tau) + 1 )/(2*sqrt(1 - 4*alpha*lambda^2*tau))*(...
        ( sqrt(1 - 4*alpha*lambda^2*tau) - 1 )/( sqrt(1 - 4*alpha*lambda^2*tau) + 1 )*exp(-t*sqrt(1 - 4*alpha*lambda^2*tau)/(2*tau) )...
        +  exp(t*sqrt(1 - 4*alpha*lambda^2*tau)/(2*tau) ) ...
        ) + phi_b_tau_b;
    qy_phi_b_tau_b = -k*C3m*lambda*cos(lambda*y)*...
        ( sqrt(1 - 4*alpha*lambda^2*tau) + 1 )/(2*sqrt(1 - 4*alpha*lambda^2*tau))*(...
        ( sqrt(1 - 4*alpha*lambda^2*tau) - 1 )/( sqrt(1 - 4*alpha*lambda^2*tau) + 1 )*exp(-t*sqrt(1 - 4*alpha*lambda^2*tau)/(2*tau) ) +  exp(t*sqrt(1 - 4*alpha*lambda^2*tau)/(2*tau) ) ...
        ) + qy_phi_b_tau_b;
    
 elseif ( 4*alpha*lambda^2*tau - 1 > 0 )
    
    phi_b_tau_b = C3m*sin( lambda*y )*(...
    cos(t*sqrt(4*alpha*lambda^2*tau - 1)/(2*tau)) ...
    + 1/sqrt(4*alpha*lambda^2*tau - 1)*sin(t*sqrt(4*alpha*lambda^2*tau - 1)/(2*tau)) ...
    ) + phi_b_tau_b;
    qy_phi_b_tau_b = -k*C3m*lambda*cos(lambda*y)*(...
    cos(t*sqrt(4*alpha*lambda^2*tau - 1)/(2*tau)) ...
    + 1/sqrt(4*alpha*lambda^2*tau - 1)*sin(t*sqrt(4*alpha*lambda^2*tau - 1)/(2*tau)) ...
    ) + qy_phi_b_tau_b;
 
 % this can only be 4*alpha*lambda^2*tau - 1 == 0
 else
    phi_b_tau_b = C3m*sin( lambda*y )*( 1 + 1/(2*tau) ) + phi_b_tau_b;
    qy_phi_b_tau_b = -k*C3m*lambda*cos(lambda*y)*( 1 + 1/(2*tau) ) + qy_phi_b_tau_b;
 end 
end

phi_b_tau_b = exp(-t/(2*tau))*phi_b_tau_b;
qy_phi_b_tau_b = exp(-t/(2*tau))*qy_phi_b_tau_b;

% at the end we sum the solutions.
T = Tc + phi_a + phi_b_tau_b;
qy = qy_phi_a + qy_phi_b_tau_b;
