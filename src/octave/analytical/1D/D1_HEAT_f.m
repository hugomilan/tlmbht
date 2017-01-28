function [T, qx, phi_a, qx_phi_a, YTau, qx_YTau] = ...
    D1_BHE_f(x, H, t, Ts, Tc, k, p, cp, Qext, minf)
    
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
% Qext (external heat source, W/m3)
% minf (number of repetition of the index m). If in doubt, chose 50 for
%     fast (and still accurate) results or 500 for more accurate results.
%
%
%
%
%
% this algorithm uses the principle of superposition to make
% T = Tc + phi_a(x) + YTau(x,t)


% defining constants
alpha = k/(p*cp); % thermal difusivity (m2/s)
theta_inf = Ts - Tc; % boundary condition for solution of phi_a

c1 = ( theta_inf/H + Qext*H/(2*k)); % constant for solution of phi_a

% solution phi_a(x). The boundary conditions are: x = 0 => theta = 0; x = H =>
% theta = theta_inf.
phi_a = -Qext/(2*k)*x.^2 + c1*x;
qx_phi_a = Qext*x - k*c1;

% variable for solution of YTau(x,t). The boundary conditions are: x = 0 => theta = 0; x = H =>
% theta = theta_inf. The initial condition is: t = 0 => - phi_a(x).
YTau = zeros(1,size(x,2));
qx_YTau = zeros(1,size(x,2));

for m = 1:minf
    % We make use of symmetry to increase m by 2 and speed up the solution.
    
    lambda = m*pi/H; % comes from separation of variables. Used for solution of YTau
    
    C3m1 = c1*2/lambda*(-1)^m;
    C3m2 = Qext/(H*k)*( (-1)^m*( 2 - lambda^2*H^2 ) - 2 )/lambda^3;
    
    C3m = C3m1 + C3m2;
    
    YTau = C3m*sin( lambda*x )*exp( -lambda^2*alpha*t ) + YTau;
    qx_YTau = -k*C3m*lambda*cos(lambda*x)*exp( -lambda^2*alpha*t ) + qx_YTau;
    
end



% at the end we sum the solutions.
T = YTau + phi_a + Tc;
qx = qx_YTau + qx_phi_a;
