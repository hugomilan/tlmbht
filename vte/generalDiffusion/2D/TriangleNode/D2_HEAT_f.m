function [T, qx, qy, phi_a, qy_phi_a, phi_b_tau_b, qy_phi_b_tau_b, ...
    phi_c, qx_phi_c, qy_phi_c, phi_d_tau_d, qx_phi_d_tau_d, qy_phi_d_tau_d] = ...
    D2_HEAT_f(x, y, L, H, t, Ts, Tc, qx, k, p, cp, Qext, minf, oinf, rinf)
    
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
% y (vector with the positions in y to get outputs, m). Vector column.
% L (material's length, m)
% H (material's thickness, m)
% t (time instant to get outputs, s)
% Ts (temperature at x = L, oC)
% Tc (initial condition temperature and also temperature at x = 0, oC)
% qx (heat flux at x = L, W/m2)
% k (thermal conductivity, W/K-m)
% p (material's density, kg/m3)
% cp (material's specific heat, J/K-kg)
% Qext (external heat source, W/m3)
% minf (number of repetition of the index m). If in doubt, chose 50 for
%     fast (and still accurate) results or 500 for more accurate results.
% oinf (number of repetition of the index o). If in doubt, chose 50 for
%     fast (and still accurate) results or 500 for more accurate results.
% rinf (number of repetition of the index r). If in doubt, chose 50 for
%     fast (and still accurate) results or 500 for more accurate results.
%
% The outputs of this function are:
% T (temperature at the given positions, oC)
% qx (heat flux in the x direction calculated at the given positions, W/m2)
% qy (heat flux in the y direction calculated at the given positions, W/m2)
% phi_a (temperatures calculated using the function phi_a at the given y positions, oC)
% qy_phi_a (heat flux in the y direction calculated using the function phi_a at the given positions, W/m2)
% phi_b_tau_b (temperatures calculated using the function phi_b_tau_b at the given positions, oC)
% qy_phi_b_tau_b (heat flux in the y direction calculated using the function phi_b_tau_b at the given positions, W/m2)
% phi_c (temperatures calculated using the function phi_c at the given positions, oC)
% qx_phi_c (heat flux in the x direction calculated using the function phi_c at the given positions, W/m2)
% qy_phi_c (heat flux in the y direction calculated using the function phi_c at the given positions, W/m2)
% phi_d_tau_d (temperatures calculated using the function phi_d_tau_d at the given positions, oC)
% qx_phi_d_tau_d (heat flux in the x direction calculated using the function phi_d_tau_d at the given positions, W/m2)
% qy_phi_d_tau_d (heat flux in the y direction calculated using the function phi_d_tau_d at the given positions, W/m2)
%
%
%
%
%
% this algorithm uses the principle of superposition to make
% T = Tc + phi_a(x) + phi_b(x)tau_b(t) + phi_c(x,y) + phi_d(x,y)tau_d(t)

% 
% File:   D2_HEAT_f.m
% Author: Hugo Fernando Maia Milan
% Email:  hugofernando@gmail.com
%
% Created on Jan 30, 2017.
%
%
%
% Revision history:
% 
% Date: 
% Who: 
% Description: 
%

sizey = size(y,2);
sizex = size(x,2);
if sizey ~= sizex
    error(['size(y,2) = ', num2str(sizey), ...
        ' and size(x,2) = ', num2str(sizex), '. They should be equal.', ...
        'type help D2_HEAT_f for more information.'])
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
% The initial condition is: t = 0 => phi_b_tau_b = - phi_a(y).
phi_b_tau_b = zeros(1,size(y,2));
qy_phi_b_tau_b = zeros(1,size(y,2));

for m = 1:minf
    lambda = m*pi/H; % comes from separation of variables. Used for solution of phi_b_tau_b
    
    C3m1 = c1*2/lambda*(-1)^m;
    C3m2 = Qext/(H*k)*( (-1)^m*( 2 - lambda^2*H^2 ) - 2 )/lambda^3;
    
    C3m = C3m1 + C3m2;
    
    phi_b_tau_b = C3m*sin( lambda*y )*exp( -lambda^2*alpha*t ) + phi_b_tau_b;
    qy_phi_b_tau_b = -k*C3m*lambda*cos(lambda*y)*exp( -lambda^2*alpha*t ) + qy_phi_b_tau_b;
    
end


% variable for solution of phi_c(x,y). The boundary conditions are:
% y = 0 => phi_c = 0.
% y = H => phi_c = 0.
% x = 0 => d(phi_c)/dx = 0.
% x = L => d(phi_c)/dx = qx/k.
phi_c = zeros(1,size(y,2));
qx_phi_c = zeros(1,size(y,2));
qy_phi_c = zeros(1,size(y,2));

% variable for solution of phi_d(x,y)tau_d(t). The boundary conditions are:
% y = 0 => phi_d(x,y)tau_d(t) = 0.
% y = H => phi_d(x,y)tau_d(t) = 0.
% x = 0 => d(phi_d(x,y)tau_d(t))/dx = 0.
% x = L => d(phi_d(x,y)tau_d(t))/dx = 0.
% The initial condition is: t = 0 => phi_d(x,y)tau_d(t) = - phi_c(x,y).
phi_d_tau_d = zeros(1,size(y,2));
qx_phi_d_tau_d = zeros(1,size(y,2));
qy_phi_d_tau_d = zeros(1,size(y,2));


for o = 1:oinf
    gamma = (2*o - 1)*pi/H; % comes from separation of variables. Used for solution of phi_c and phi_d_tau_d
    
    phi_c = 4*qx/( H*k*gamma^2*sinh(gamma*L) )*cosh(gamma*x).*sin(gamma*y) + phi_c;
    qx_phi_c = -4*qx/( H*gamma*sinh(gamma*L) )*sinh(gamma*x).*sin(gamma*y) + qx_phi_c;
    qy_phi_c = -4*qx/( H*gamma*sinh(gamma*L) )*cosh(gamma*x).*cos(gamma*y) + qy_phi_c;
    
    for r = 0:rinf
    
        beta = r*pi/L; % comes from separation of variables. Used for solution of phi_d_tau_d
    
        phi_d_tau_d = -(-1)^r*8*qx/(H*L*k*gamma*(gamma^2 + beta^2))*cos(beta*x).*sin(gamma*y)*exp(-alpha*t*(gamma^2 + beta^2)) + phi_d_tau_d;
        qx_phi_d_tau_d = -(-1)^r*beta*8*qx/(H*L*gamma*(gamma^2 + beta^2))*sin(beta*x).*sin(gamma*y)*exp(-alpha*t*(gamma^2 + beta^2)) + qx_phi_d_tau_d;
        qy_phi_d_tau_d = (-1)^r*8*qx/(H*L*(gamma^2 + beta^2))*cos(beta*x).*cos(gamma*y)*exp(-alpha*t*(gamma^2 + beta^2)) + qy_phi_d_tau_d;
        
    end
        
end




% at the end we sum the solutions.
T = Tc + phi_a + phi_b_tau_b + phi_c + phi_d_tau_d;
qx = qx_phi_c + qx_phi_d_tau_d;
qy = qy_phi_a + qy_phi_b_tau_b + qy_phi_c + qy_phi_d_tau_d;
