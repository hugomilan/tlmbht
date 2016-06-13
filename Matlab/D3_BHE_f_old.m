function [T, qx, qy, qz, phi_a, qy_phi_a, YTau, qy_YTau, ...
    phi_b, qx_phi_b, qy_phi_b, sigma, qx_sigma, qy_sigma, ...
    sigma_z, qz_sigma_z, qy_sigma_z, phi_z, qz_phi_z, qy_phi_z] = ...
    D3_BHE_f(x, y, z, L, H, Tz, time, qfluxX, qfluxZ, Ts, Tc, k, p, cp, wb, pb, cb, Ta, ...
    qmet, minf, ninf, pinf, flagMorV)
% This file is part of TLMBHT. See details of this function below.
% Copyright (C) 2016 Hugo Fernando Maia Milan
%
% Acknowledgment: Brazilian National Counsel of Technological and Scientific
% Development (Proc. 203312/2014-7) for scholarship to HFMM
%
%
%
% This function solves the analytical problem presented in our paper:
% Milan HFM, Gebremedhin KG. Transmission-line modeling (TLM) triangular
% node for bioheat transfer, 2016. The correct reference will be added here
% when the paper is published.
%
% The inputs to this function are:
% x (vector with the positions in x to get outputs, m). Vector column.
% y (vector with the positions in y to get outputs, m). Vector column.
% z (vector with the positions in z to get outputs, m). Vector column.
% L (tissue's length in x, m)
% H (tissue's length in y, m)
% Tz (tissue's length in z, m)
% time (time instant to get outputs, s)
% qfluxX (heat flux at x = L, W/m2)
% qfluxZ (heat flux at z = Tz, W/m2)
% Ts (temperature at y = H, oC)
% Tc (temperature at y = 0 and also the initial temperature, oC)
% k (thermal conductivity, W/K-m)
% p (tissue's density, kg/m3)
% cp (tissue's specific heat, J/K-kg)
% wb (blood's perfusion, s-1)
% pb (blood's density, kg/m3)
% cp (blood's specific heat, J/K-kg)
% Ta (arterial blood temperature, oC)
% qmet (tissue's metabolic heat, W/m3)
% minf (number of repetition of the index m). If in doubt, chose 50 for
% fast (and still accurate) results or 500 for more accurate results.
%
% ninf (number of repetition of the index n). If in doubt, chose 50 for
% fast (and still accurate) results or 500 for more accurate results.
%
% pinf (number of repetition of the index p). If in doubt, chose 50 for
% fast (and still accurate) results or 500 for more accurate results.
%
% flagMorV says if we should consider the x, y, z inputs as matrix (flagMorV =
% 1), this is, calculate for all y considering each x, or vectors (flagMorV = 2),
% this is, consider the pair x, y, z as one point of interest. If vectors,
% then size(x,2) == size(y,2) == size(z,2).
%
%
%
%
%
% this algorithm uses the principle of superposition to make
% T = sigma_z(y,z,t) + phi_z(y,z) + T2D(x,y,t)
%
%
%
% This algorithm is not intended for wb = 0. However, if you input that, we
% will make wb = eps (the minimum value that your computer can use). This
% approximation barely will lead to results different from when wb = 0.


if flagMorV == 1
    sizey = size(y,2);
    sizex = size(x,2);
    sizez = size(z,2);
elseif flagMorV == 2
    sizey = size(y,2);
    sizex = size(x,2);
    sizez = size(z,2);
    if sizey ~= sizex || sizey ~= sizez
        error(['flagMorV = 2 but the sizes of the vectors were not equal. ', ...
            'For instance, size(x,2) = ', num2str(sizex), ...
            ', size(y,2) = ', num2str(sizey), ' and size(z,2) = ', num2str(sizez), ...
            '. They should be equal for flagMorV = 2.', ...
            'type help D3_BHE_f for more information.'])
    end
end

if wb == 0
    wb = eps;
end




% solving for the 3D problem


% adding one to minf and ninf in order to make, at least, the input value
% in the while loop.
minf = minf + 1; ninf = ninf + 1; pinf = pinf + 1;



% defining constants
alpha = k/(p*cp); % thermal difusivity (m2/s)
Wb = wb*pb*cb; % heat dissipation through blood perfusion
Wbk1 = Wb/k; % constant for solution of phi_a

QWb = QT/Wb; % constant for solution of phi_a
Wbk = sqrt(Wb/k); % constant for solution of phi_a
WbkH = Wbk*H; % constant for solution of phi_a
c1 = ( theta_inf + QWb*( cosh(WbkH) - 1 ) )/sinh(WbkH); % constant for solution of phi_a



% the logic is different for flagMorV = 1 or flagMorV = 2. Just on the
% for-loop of ax, ay below and in the size of the matrix. The rest of the code is equal

switch flagMorV
    case 1
        
        
        
        
        % variable for solution of phi_z(y,z). The boundary conditions are: y = 0 => theta = 0; y = H =>
        % theta = 0; z = 0 => d_theta/dz = 0; z = Tz => d_theta/dz = qfluxZ/k. It is
        % not time dependent
        phi_z = zeros(sizey,sizez);
        qz_phi_z = zeros(sizey,sizez);
        qy_phi_z = zeros(sizey,sizez);
        
        % variable for solution of sigma(y,z,t). The boundary conditions are: y = 0 => theta = 0; y = H =>
        % theta = 0; z = 0 => d_theta/dz = 0; z = Tz => d_theta/dz = 0. The initial condition is: t = 0 => - phi_b(y).
        sigma_z = zeros(sizey,sizez);
        qz_sigma_z = zeros(sizey,sizez);
        qy_sigma_z = zeros(sizey,sizez);
        
        
        
        for m = 1:2:minf
            % We make use of symmetry to increase m by 2 and speed up the solution.
            
            lambda = m*pi/H; % comes from separation of variables. Used for solution of YTau, phi_b, sigma
            gamma = sqrt(Wb/k + lambda^2); % appears because of blood perfusion.
            
            
            
            % this is exclusive to sigma and for n = 0.
            C7m0 = -4*qfluxZ/( lambda*k*H*Tz*gamma^2 )*exp( -( Wb/k + lambda^2 )*alpha*time );
            
            % n appears just for sigma_z
            sigma_temp = zeros(1,sizez); % this is a temporary variable used for speeding up the solution
            qz_sigma_z_temp = zeros(1,sizez); % this is a temporary variable used for speeding up the solution
            for p = 1:2:pinf
                
                delta = p*pi/Tz; % comes from separation of variables
                
                % we are using the problem's symmetry to speed up the solution.
                % Then, we make n and n + 1 at once.
                C5mn = 8*qfluxX/( lambda*k*H*L )/( gamma^2 + eta^2 )*exp( -( Wb/k + lambda^2 + eta^2 )*alpha*time );
                
                eta2 = (n + 1)*pi/L;
                C5mn2 = -8*qfluxX/( lambda*k*H*L )/( gamma^2 + eta2^2 )*exp( -( Wb/k + lambda^2 + eta2^2 )*alpha*time );
                
                sigma_temp = C5mn*cos( eta*x ) + C5mn2*cos( eta2*x ) + sigma_temp;
                qx_sigma_temp = k*( C5mn*sin( eta*x )*eta + C5mn2*sin( eta2*x )*eta2 ) + qx_sigma_temp;
                
            end
            
            %     C4m = 2*qfluxX/( lambda*gamma*k*H*sinh( lambda*L ) )*( 1 - cos( lambda*H ) );
            %     this can be simplified because it just occurs when m is odd. m is
            %     usually m = m + 1 but we are going to make m = m + 2.
            %     In addition, because sinh is unstable, we are using its
            %     exponential form in conjunction with the calculation of C4mT
            
            C4mT = 4*qfluxX/( lambda*H*k*gamma )*( 1./( exp( gamma*( L - x ) ) - exp( -gamma*( L + x))) + ...
                1./( exp( gamma*( L + x ) ) - exp( -gamma*( L - x))));
            
            C4mqx = 4*qfluxX/( lambda*H )*( 1./( exp( gamma*( L - x ) ) - exp( -gamma*( L + x))) - ...
                1./( exp( gamma*( L + x ) ) - exp( -gamma*( L - x))));
            
            % now we calculate sigma and phi_b
            for ax = 1:sizex
                for ay = 1:sizey
                    
                    phi_b(ay,ax) = C4mT(ax)*sin( lambda*y(ay) ) + phi_b(ay,ax);
                    qx_phi_b(ay,ax) = -C4mqx(ax)*sin( lambda*y(ay) ) + qx_phi_b(ay,ax);
                    qy_phi_b(ay,ax) = -k*lambda*C4mT(ax)*cos( lambda*y(ay) ) + qy_phi_b(ay,ax);
                    
                    sigma(ay,ax) = ( C5m0 + sigma_temp(ax) )*sin( lambda*y(ay) ) + sigma(ay,ax);
                    qx_sigma(ay,ax) = qx_sigma_temp(ax)*sin( lambda*y(ay) ) + qx_sigma(ay,ax);
                    qy_sigma(ay,ax) = -( C5m0 + sigma_temp(ax) )*k*lambda*cos( lambda*y(ay) ) + qy_sigma(ay,ax);
                    
                end
            end
            
            
            
            % at the end we proceed to the calculation of YTau. Different from the
            % others solutions, YTau also requires evens m. Then, first we solve for m
            % odd
            C3m1 = 2*QWb/( H*lambda )*( cos( lambda*H ) - 1 );
            C3m2 = 2*c1/H*cos( lambda*H )*sinh(WbkH)/( lambda + Wb/( k*lambda ) );
            C3m3 = 2*QWb/H*( 1 - cos( lambda*H )*cosh(WbkH) )/( lambda + Wb/( k*lambda ) );
            
            C3m = C3m1 + C3m2 + C3m3;
            
            YTau = C3m*sin( lambda*y )*exp( -(lambda^2 + Wb/k)*alpha*time ) + YTau;
            qy_YTau = -k*C3m*lambda*cos(lambda*y)*exp( -(lambda^2 + Wb/k)*alpha*time ) + qy_YTau;
            
            % now we make m even
            m = m + 1; lambda = m*pi/H;
            
            C3m1 = 2*QWb/( H*lambda )*( cos( lambda*H ) - 1 );
            C3m2 = 2*c1/H*cos( lambda*H )*sinh(WbkH)/( lambda + Wb/( k*lambda ) );
            C3m3 = 2*QWb/H*( 1 - cos( lambda*H )*cosh(WbkH) )/( lambda + Wb/( k*lambda ) );
            
            C3m = C3m1 + C3m2 + C3m3;
            
            YTau = C3m*sin(lambda*y)*exp( -(lambda^2 + Wb/k)*alpha*time ) + YTau;
            qy_YTau = -k*C3m*lambda*cos(lambda*y)*exp( -(lambda^2 + Wb/k)*alpha*time ) + qy_YTau;
            
            
        end
        
        
        
        % at the end we sum the solutions.
        
        % here we create the matrix for T, qx, qy
        T = zeros(sizey,sizex);
        qx = zeros(sizey,sizex);
        qy = zeros(sizey,sizex);
        
        % and here we sum up the solutions
        for ax = 1:sizex
            for ay = 1:sizey
                
                T(ay,ax) = sigma(ay,ax) + phi_b(ay,ax) + YTau(ay) + phi_a(ay) + Tc;
                qx(ay,ax) = qx_sigma(ay,ax) + qx_phi_b(ay,ax);
                qy(ay,ax) = qy_sigma(ay,ax) + qy_phi_b(ay,ax) + qy_YTau(ay) + qy_phi_a(ay);
                
            end
        end
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        % vector case
    case 2
        
        
        % variable for solution of phi_z(y,z). The boundary conditions are: y = 0 => theta = 0; y = H =>
        % theta = 0; z = 0 => d_theta/dz = 0; z = Tz => d_theta/dz = qfluxZ/k. It is
        % not time dependent
        phi_z = zeros(1,sizex);
        qz_phi_z = zeros(1,sizex);
        qy_phi_z = zeros(1,sizex);
        
        % variable for solution of sigma(y,z,t). The boundary conditions are: y = 0 => theta = 0; y = H =>
        % theta = 0; z = 0 => d_theta/dz = 0; z = Tz => d_theta/dz = 0. The initial condition is: t = 0 => - phi_b(y).
        sigma_z = zeros(1,sizex);
        qz_sigma_z = zeros(1,sizex);
        qy_sigma_z = zeros(1,sizex);
        
        
        for m = 1:2:minf
            % We make use of symmetry to increase m by 2 and speed up the solution.
            
            lambda = m*pi/H; % comes from separation of variables. Used for solution of YTau, phi_b, sigma
            gamma = sqrt(Wbk1 + lambda^2); % appears because of blood perfusion.
            
            
            
            % this is exclusive to sigma and for n = 0.
            C7m0 = -4*qfluxZ/( lambda*k*H*Tz*gamma^2 )*exp( -( Wbk1 + lambda^2 )*alpha*time );
            
            % n appears just for sigma
            sigma_z_temp = zeros(1,sizex); % this is a temporary variable used for speeding up the solution
            qz_sigma_z_temp = zeros(1,sizex); % this is a temporary variable used for speeding up the solution
            for p = 1:2:pinf
                delta = p*pi/Tz; % comes from separation of variables
                
                % we are using the problem's symmetry to speed up the solution.
                % Then, we make n and n + 1 at once.
                C7mp = 8*qfluxZ/( lambda*k*H*Tz )/( gamma^2 + delta^2 )*exp( -( Wbk1 + lambda^2 + delta^2 )*alpha*time );
                
                delta2 = (p + 1)*pi/Tz;
                C7mp2 = -8*qfluxZ/( lambda*k*H*Tz )/( gamma^2 + delta2^2 )*exp( -( Wbk1 + lambda^2 + delta2^2 )*alpha*time );
                
                sigma_z_temp = C7mp*cos( delta*z ) + C7mp2*cos( delta2*z ) + sigma_z_temp;
                qz_sigma_z_temp = k*( C7mp*sin( delta*z )*delta + C7mp2*sin( delta2*z )*delta2 ) + qz_sigma_z_temp;
                
            end
            
            %     C6m = 2*qfluxZ/( lambda*gamma*k*H*sinh( lambda*L ) )*( 1 - cos( lambda*H ) );
            %     this can be simplified because it just occurs when m is odd. m is
            %     usually m = m + 1 but we are going to make m = m + 2.
            %     In addition, because sinh is unstable, we are using its
            %     exponential form in conjunction with the calculation of C4mT
            
            C6mT = 4*qfluxZ/( lambda*H*k*gamma )*( 1./( exp( gamma*( Tz - z ) ) - exp( -gamma*( Tz + z))) + ...
                1./( exp( gamma*( Tz + z ) ) - exp( -gamma*( Tz - z))));
            
            C6mqz = 4*qfluxZ/( lambda*H )*( 1./( exp( gamma*( Tz - z ) ) - exp( -gamma*( Tz + z))) - ...
                1./( exp( gamma*( Tz + z ) ) - exp( -gamma*( Tz - z))));
            
            % now we calculate sigma and phi_b
            phi_z = C6mT.*sin( lambda*y ) + phi_z;
            qz_phi_z = -C6mqz.*sin( lambda*y ) + qz_phi_z;
            qy_phi_z = -k*lambda*C6mT.*cos( lambda*y ) + qy_phi_z;
            
            sigma_z = ( C7m0 + sigma_z_temp ).*sin( lambda*y ) + sigma_z;
            qz_sigma_z = qz_sigma_z_temp.*sin( lambda*y ) + qz_sigma_z;
            qy_sigma_z = -( C7m0 + sigma_z_temp )*k*lambda.*cos( lambda*y ) + qy_sigma_z;
        end
        
        
        
        % at the end we sum the solutions.
        
        % getting the value for the 2D problem
        [T, qx, qy, phi_a, qy_phi_a, YTau, qy_YTau, ...
            phi_b, qx_phi_b, qy_phi_b, sigma, qx_sigma, qy_sigma] = ...
            D2_BHE_f(x, y, L, H, time, qfluxX, Ts, Tc, k, p, cp, wb, pb, cb, Ta, ...
            qmet, minf, ninf, flagMorV);
        T = T + sigma_z + phi_z;
        qy = qy + qy_sigma_z + qy_phi_z;
        qz = qz_sigma_z + qz_phi_z;
        
        
        
        
end



end
