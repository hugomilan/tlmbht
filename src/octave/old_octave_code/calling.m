% This file is part of TLMBHT. See details of this function below.
% Version 0.1. Copyright © 2015 to 2016 by Cornell University.
% Written by Hugo Fernando Maia Milan.
% All Rights Reserved. Refer to the license file for details.
%
% Acknowledgment: Brazilian National Counsel of Technological and Scientific
% Development (Proc. 203312/2014-7) for scholarship to HFMM
%
%
tic


%% input variables
% nameToOpen: Opens the selected stored file. See variables loadNow and readNow.
% nameToOpen = 'plate_4BC_40e.tms.msh'; % 2D
% nameToOpen = 'plate_4BC_40e.tms.mat'; % 2D
% nameToOpen = 'plate_4BC_682e.tms.mat'; % 2D
%nameToOpen = 'plate_4BC_10766e.tms.mat'; % 2D
% nameToOpen = 'plate_6BC_206e.tms.mat'; % 3D
% nameToOpen = 'plate_6BC_1368e.tms.mat'; % 3D
nameToOpen = 'plate_6BC_69941e.tms.mat'; % 3D

% 1: will load into the workspace the data contained in the nameToOpen
% otherwise: do nothing
loadNow = 1;

% 1: will read the .msh file (chosen by an select dialog) and will save
% the data with the name in the variable nameToOpen.
% Otherwise: do nothing
readNow = 0;


% 1: it will solve the inverse matrix equation for the mesh and find the
% steady-state solution for the incident voltage.
% otherwise: will run the temporal evolution of the system.
SteadyState = 1;

% the time-steps interval that the algorithm will calculate the temperature
% and heat fluxes in the mesh. Must be a natural number greater than 0
intervalForCalculationTempAndHeat = 1;

% these values are necessary only when the algorithm will solve the
% time-domain problem. However, these variables require inputs
% and dt > 0.
dt = 0.0001; % time-step (s)
% the final time for the simulation (s)
tend = 4; % final time for the simulation

SolveSteadyStateAfter = 0; % will solve for steady state after it has solved
% for the time-domain problem



% The geometrical input for this algorithm is the mesh generated using
% Gmsh. In Gmsh, you can put tag numbers in some places of the mesh. These
% tag numbers are used in the software in order to understand what is
% medium, what variables do they have, and what is boundary condition.

% everything is construct based on the tags values. You must take the tag
% value and define if it is a medium where temperature and heat flux are
% calculated or if it is boundary where heat flux or temperatures are
% fixed.


% enter here the tags' number that are mediums and specify
% the values for the constants down there for each medium. Different
% mediums tags can have the same constants values, it is not a problem.
% Please, structure these variables as vectors like this [1, 2, 3, 4, 5].
% If a medium tag is not specified here, the algorithm won't consider them
% and the boundary between this medium tag and other mediums (if
% applicable) will be considered as adiabatic--however, it is not guaranted.

% for 2D tests
%mediumtags = 7;
%for 3D tests
mediumtags = 33;

% Characteristics for the mediums. They must be vectors
ro = 1200; % tissue density (kg/m3)
cp = 3200; % specific heat (J/(K-kg))
k = 0.3; % thermal conductivity (W/(K-m))
Ti = 37; % initial temperature

wb = 1e-4; % blood perfusion (s-1)
pb = 1052; % blood density (kg/m3)
cb = 3600; % blood specific heat (J/(K-kg))
Tb = 37; % temperature of the blood (oC)

Qmet = 500; % internal heat generation (W/m3)

% for 2D tests

% Enter the tag numbers that are boundary conditions.
%boundarytags = [10 11 8]; % tag vector for the boundary conditions
%boundarytype = [1  1  2]; % 1 - temperature; 2 - heat flux;
% 3 - Newtons cooling law.
%boundaryvalues = [Ti 150 1e5;
%    0 0 0]; % pertinent values refered to the boundary types.
% for 1: temperature;
% for 2: heat flux;
% for 3: temperature; heat transfer coefficient (W/(K-m2)). Input them in
% matrix format

%for 3D tests
boundarytags = [15 18 21 24 27 30]; % tag vector for the boundary conditions
boundarytype = [2  2  2  2  1  1]; % 1 - temperature; 2 - heat flux;
% 3 - Newtons cooling law.
% boundaryvalues = [0 2e4 0 1e4 Ti 150;
boundaryvalues = [0 2.5e4 0 5e4 Ti 150;
    0 0   0 0   0  0]; % pertinent values refered to the boundary types.
% for 1: temperature;
% for 2: heat flux;
% for 3: temperature; heat transfer coefficient (W/(K-m2)). Input them in
% matrix format

% uncomment for testing the data provided in the paper
Tc = boundaryvalues(1,1);
Ts = boundaryvalues(1,2);
qflux = boundaryvalues(1,3);

% uncomment for 3D
Tc = boundaryvalues(1,5);
Ts = boundaryvalues(1,6);
qfluxX = boundaryvalues(1,4);
qfluxZ = boundaryvalues(1,2);

%% calling functions

if readNow == 1
    [errormsg, nN, vertexmat, ntags, nEm1, ...
        nEm2, nEm3, nEm4, nEm5, nEm6, nEm7, nEm15] = openmsh;
    
    if strcmp(errormsg,'')
        save(nameToOpen,'errormsg', 'nN', 'vertexmat', 'ntags', 'nEm1', ...
            'nEm2', 'nEm3', 'nEm4', 'nEm5', ...
            'nEm6', 'nEm7', 'nEm15');
        disp('The mesh was read and saved with success');
    else
        disp(errormsg);
        return;
    end
    
elseif loadNow == 1
    load(nameToOpen)
    disp('The mesh was opened with success')
end



% this function create the matrix for time-domain calculation or
% steady-state calculation
[M, E, TM, TE, Vi0, positions, ...
    qLTM, qLTE, projec, etasM, AtOrVt] = proproc(nN, vertexmat, ntags, nEm1, ... % mesh information
    nEm2, nEm3, nEm4, nEm5, nEm6, nEm7, nEm15, ... % mesh information
    dt, mediumtags, ro, cp, k, Ti, wb, pb, cb, Tb, ... % problem's information
    Qmet, boundarytags, boundarytype, boundaryvalues); % problem's information
disp(['values calculated']);

%% solving the problem

if SteadyState ~= 1
    if (intervalForCalculationTempAndHeat ~= 1)
        
        Esum = speye(size(M));
        Munit = M;
        
        for nEffective = 1:intervalForCalculationTempAndHeat
            Esum = Esum + M;
            M = Munit*M;
        end
        
        E = Esum*E;
    end
    
    ktotal = ceil(tend/dt)/intervalForCalculationTempAndHeat;
    Vi = Vi0;
    qLT = qLTM*Vi + qLTE;
    
    % error for validation
    errorTLM = zeros(2,ktotal); % line 1: temperature; line 2: heat flux
    for kt = 1:ktotal
        Vi = M*Vi + E;
        
        T = TM*Vi + TE;
        %
        %         eta = qLT;
        %
        qLT = qLTM*Vi + qLTE;
        %
        %         eta = abs(eta./qLT - 1);
        %         eta1 = eta.*etasM(:,1);
        %         eta2 = eta.*etasM(:,2);
        %
        %         uncomment the following line in order to have more than one graph in each
        %         plot. You must pause in order to see the graphs as the solution
        %         is developed
        %
        %         hold on
%         pause(0.01)
        %
        %         temperature
%                 plot(positions(:,1),T,'*')
%         plot(positions(:,2),T,'*')
        %         plot(positions(:,3),T,'*')
        %
        %         heat flux
        %         plot(positions((end - size(qLT,1) + 1):end,1),qLT,'*')
        %         plot(positions((end - size(qLT,1) + 1):end,2),qLT,'*')
        %         plot(positions((end - size(qLT,1) + 1):end,3),qLT,'*')
        %
        %         etas. They are complimentaries. If eta2(n) = 0, position n is a boundary
        %         condition, otherwise eta1 analyses one side of the triangular face and
        %         eta2 analyses the other side.
        %         plot(positions((end - size(eta,1) + 1):end,1), eta1, '*');
        %         plot(positions((end - size(eta,1) + 1):end,1), eta2, '*');
        
        %        These are commands used in the TLM validation. Only uncomment them
        %        if you want to repeat the validation
        [Ttemp, qxAnatemp, qyAnatemp, qzAnatemp] = D3_BHE_f(positions(:,1),  ...
            positions(:,2), positions(:,3), 1e-3, 0.75e-3, 0.5e-3, kt*dt*intervalForCalculationTempAndHeat, ...
            qfluxX, qfluxZ, Ts, Tc, k, ro, cp, wb, pb, cb, Tb, Qmet, 50, 50, 50, 2);
%        
%        qLTAna = qxAnatemp((end - size(qLT,1) + 1):end).*projec(:,1) + ...
%            qyAnatemp((end - size(qLT,1) + 1):end).*projec(:,2) + ...
%            qzAnatemp((end - size(qLT,1) + 1):end).*projec(:,3);
        
        %
%         hold on
        
        %         plot(positions(:,1),Ttemp,'*')
                 plot(positions(:,1),(T - Ttemp)./Ttemp*100,'*')
%        errorTLM(1,kt) = mean(abs(T - Ttemp)./Ttemp*100);
%         errorTLM(1,kt)
%         plot(positions(:,2),Ttemp,'*')
        %         plot(positions(:,3),Ttemp,'*')
        
        %         plot(positions((end - size(qLT,1) + 1):end,1),qLTAna,'*')
        %         plot(positions((end - size(qLT,1) + 1):end,1),(qLT - qLTAna),'*')
        %         plot(positions((end - size(qLT,1) + 1):end,2),qLTAna,'*')
        %         plot(positions((end - size(qLT,1) + 1):end,3),qLTAna,'*')
%        errorTLM(2,kt) =  mean(abs(qLT - qLTAna));
%         errorTLM(2,kt)
        
%         hold off
        
        disp(['Time: ', num2str(kt*dt*intervalForCalculationTempAndHeat), ' s; Mesh: ' nameToOpen '. dt = ' num2str(dt) ' s.'])
        fflush(stdout);
        %         pause
        
        
        
    end
    
    % statistics for this result
    disp(['Mesh: ' nameToOpen '. dt = ' num2str(dt) ' s.'])
    disp('Mesh statistics:')
    disp(['Number of nodes = ' num2str(size(AtOrVt,1)) '; number of ports = ' num2str(size(qLT,1))])
    disp(['Volume: mean = ' num2str(mean(AtOrVt)) ' m3; standard deviation = ' num2str(std(AtOrVt)) ' m3'])
    disp(' ')
    disp(' ')
    disp(['Absolute percentage error of the temperature: mean = ' num2str(mean(errorTLM(1,:))) ' %; standard deviation = ' num2str(std(errorTLM(1,:))) ' %'])
    disp(['Absolute difference of the heat flux: mean = ' num2str(mean(errorTLM(2,:))) ' W/m2; standard deviation = ' num2str(std(errorTLM(2,:))) ' W/m2'])
    
    if SolveSteadyStateAfter == 1
        Vi = (speye(size(M)) - M)\E;
        TSS = TM*Vi + TE;
        qLTSS = qLTM*Vi + qLTE;
        
        [Ttemp, qxAnatemp, qyAnatemp, qzAnatemp] = D3_BHE_f(positions(:,1),  ...
            positions(:,2), positions(:,3), 1e-3, 0.75e-3, 0.5e-3, 1e10, ...
            qfluxX, qfluxZ, Ts, Tc, k, ro, cp, wb, pb, cb, Tb, Qmet, 50, 50, 50, 2);
        
        qLTAna = qxAnatemp((end - size(qLT,1) + 1):end).*projec(:,1) + ...
            qyAnatemp((end - size(qLT,1) + 1):end).*projec(:,2) + ...
            qzAnatemp((end - size(qLT,1) + 1):end).*projec(:,3);
        
        errorTLMSSTemp = abs(TSS - Ttemp)./Ttemp*100;
        errorTLMSSHeatFlux = abs(qLTSS - qLTAna);
        
        disp(' ')
        disp(' ')
        disp('Statistics for steady state solution')
        disp(['Absolute error of the temperature: mean = ' num2str(mean(errorTLMSSTemp)) ' %; standard deviation = ' num2str(std(errorTLMSSTemp)) ' %'])
        disp(['Absolute difference of the heat flux: mean = ' num2str(mean(errorTLMSSHeatFlux)) ' W/m2; standard deviation = ' num2str(std(errorTLMSSHeatFlux)) ' W/m2'])
    end
else
    Vi = (speye(size(M)) - M)\E;
    T = TM*Vi + TE;
    qLT = qLTM*Vi + qLTE;
    
    %         temperature
    %         plot(positions(:,1),T,'*')
%     plot(positions(:,2),T,'*')
    %         plot(positions(:,3),T,'*')
    %
    %         heat flux
    %         plot(positions((end - size(qLT,1) + 1):end,1),qLT,'*')
    %         plot(positions((end - size(qLT,1) + 1):end,2),qLT,'*')
    %         plot(positions((end - size(qLT,1) + 1):end,3),qLT,'*')
    
    
    [Ttemp, qxAnatemp, qyAnatemp, qzAnatemp] = D3_BHE_f(positions(:,1),  ...
        positions(:,2), positions(:,3), 1e-3, 0.75e-3, 0.5e-3, tend*1e10, ...
        qfluxX, qfluxZ, Ts, Tc, k, ro, cp, wb, pb, cb, Tb, Qmet, 50, 50, 50, 2);
    
    qLTAna = qxAnatemp((end - size(qLT,1) + 1):end).*projec(:,1) + ...
        qyAnatemp((end - size(qLT,1) + 1):end).*projec(:,2) + ...
        qzAnatemp((end - size(qLT,1) + 1):end).*projec(:,3);
    
    errorTLMSSTemp = abs(T - Ttemp)./Ttemp*100;
    errorTLMSSHeatFlux = abs(qLT - qLTAna);
    
%     hold on
    
    %         plot(positions(:,1),Ttemp,'*')
    %         plot(positions(:,1),(T - Ttemp)./Ttemp*100,'*')
%     plot(positions(:,2),Ttemp,'*')
    %         plot(positions(:,3),Ttemp,'*')
    
    %         plot(positions((end - size(qLT,1) + 1):end,1),qLTAna,'*')
    %         plot(positions((end - size(qLT,1) + 1):end,1),(qLT - qLTAna),'*')
    %         plot(positions((end - size(qLT,1) + 1):end,2),qLTAna,'*')
    %         plot(positions((end - size(qLT,1) + 1):end,3),qLTAna,'*')
    
    disp(['Mesh: ' nameToOpen '. Steady state solution.'])
    disp('Mesh statistics:')
    disp(['Number of nodes = ' num2str(size(AtOrVt,1)) '; number of ports = ' num2str(size(qLT,1))])
    disp(['Volume: mean = ' num2str(mean(AtOrVt)) ' m3; standard deviation = ' num2str(std(AtOrVt)) ' m3'])
    disp(' ')
    disp(' ')
    disp('Statistics for steady state solution')
    disp(['Absolute error of the temperature: mean = ' num2str(mean(errorTLMSSTemp)) ' %; standard deviation = ' num2str(std(errorTLMSSTemp)) ' %'])
    disp(['Absolute difference of the heat flux: mean = ' num2str(mean(errorTLMSSHeatFlux)) ' W/m2; standard deviation = ' num2str(std(errorTLMSSHeatFlux)) ' W/m2'])
end

%% plot functions

% uncomment the following line in order to have more than one graph in each
% plot.

% hold on

% temperature
% hold off
% plot(positions(:,1),T,'*')
% plot(positions(:,2),T,'*')
% plot(positions(:,3),T,'*')

% heat flux
% plot(positions((end - size(qLT,1) + 1):end,1),qLT,'*')
% plot(positions((end - size(qLT,1) + 1):end,2),qLT,'*')
% plot(positions((end - size(qLT,1) + 1):end,3),qLT,'*')

% etas. They are complimentaries. If eta2(n) = 0, position n is a boundary
% condition, otherwise eta1 analyses one side of the triangular face and
% eta2 analyses the other side.
% plot(positions((end - size(eta,1) + 1):end,1), eta1, '*');
% plot(positions((end - size(eta,1) + 1):end,1), eta2, '*');

% hold off
toc
