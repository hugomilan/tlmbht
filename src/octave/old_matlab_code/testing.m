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

% uncomment for testing the data provided in the paper
qfluxX = 1e4;
qfluxZ = qfluxX*100;
Tc = 37;
Ts = 150;
time = 0.5; % seconds


vals = 0:50;
x = vals.*1e-3/max(vals);
y = vals.*0.75e-3/max(vals);
z = vals.*0.5e-3/max(vals);
[Ttemp, qxAnatemp, qyAnatemp] = D3_BHE_f(x,  ...
    y, z, 1e-3, 0.75e-3, 0.5e-3, time, ...
    qfluxX, qfluxZ, Ts, Tc, k, ro, cp, wb, pb, cb, Tb, Qmet, 50, 50, 50, 1);
%% ploting for 3D data
if size(Ttemp,3) > 1    
    for tz = 1:size(Ttemp,3)
        mesh(x,y,Ttemp(:,:,tz));
        pause(0.01)
    end
end