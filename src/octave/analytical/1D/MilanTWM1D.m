function [T] = ...
    MilanTWM1D(x, L, t, T_inf, Ti, k, p, cp, tau, wb, pb, cb, Ta, ...
    Qm, Qr, ninf)
    
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
% x (vector with the positions in x to get outputs, m). Vector column.
% L (tissue's thickness, m)
% time (time instant to get outputs, s)
% Tinf (temperature at x = L, oC)
% Ti (initial condition temperature and also temperature at x = 0, oC)
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
% Ta (arterial blood temperature, oC)
% Qm (tissue's metabolic heat, W/m3)
% Qr (constant external heat, W/m3)
% ninf (number of repetition of the index n). If in doubt, chose 500 for
%     fast (and still accurate) results or 5000 for more accurate results.
%
%
%
%
%
% this algorithm uses the principle of superposition to calculate
% T = ...
%
%
%
% This algorithm is not intended for wb = 0. However, if you input that, we
% will make Wb = eps (the minimum value that your computer can use). This
% approximation barely will lead to results different from when wb = 0.


if wb == 0
    wb = eps;
end


% defining constants
alpha = k/(p*cp); % thermal diffusivity (m^2/s)
Wb = wb*pb*cb;
Wbk = Wb/k;
Wbk2 = sqrt(Wbk);
Qt = Qm + Qr + Wb*Ta;
QWb = Qt/Wb; % constant for solution of phi_a
alpha = k/(p*cp); % thermal difusivity (m2/s)
alpha2 = alpha*Wb/k + 1/tau;
alpha22 = alpha2^2;

phia_inf = T_inf - Ti;

c1 = ( phia_inf + QWb*( cosh(Wbk*L) - 1 ) )/sinh(Wbk*L); % constant for solution of phi_a

% defining phibGammab
phibGammab = zeros(size(x,1),1);

for n = 1:ninf

  lambdan = n*pi/L;
  betan = 4*alpha/tau*(Wb/k + lambdan^2);
  
  An1 = 2*QWb/( L*lambdan )*( cos( lambdan*L ) - 1 );
  An2 = 2*c1/L*cos( lambdan*L )*sinh(Wbk*L)/( lambdan + Wb/( k*lambdan ) );
  An3 = 2*QWb/L*( 1 - cos( lambdan*L )*cosh(Wbk*L) )/( lambdan + Wb/( k*lambdan ) );
  
  An = An1 + An2 + An3;

 if ( alpha22 > betan )
    
    gamman = 0.5*sqrt(alpha22 - betan);
    Bn = ( 2*gamman + alpha2 )/( 2*gamman - alpha2 );
    
    phibGammab = phibGammab + An/(1 + Bn)*( exp(-gamman*t) + Bn*exp(gamman*t) ) *sin(lambdan*x);
    
 else
    
    omegan = 0.5*sqrt(betan - alpha22);
    Cn = alpha2/(2*omegan);
    
    phibGammab = phibGammab + An*( cos(omegan*t) + Cn*sin(omegan*t) ) *sin(lambdan*x);
    
 end
 
%    phibGammab = phibGammab + phibGammab.*sin(lambdan*x)
 
end

phibGammab = exp(-alpha2/2*t)*phibGammab;

c1 = (phia_inf + Qt/Wb*( cosh(Wbk2*L) - 1 ) )/sinh(Wbk2*L);
phia = Qt/Wb + c1*sinh(Wbk2*x) - Qt/Wb*cosh(Wbk2*x);

theta = phia + phibGammab;
T = theta + Ti;