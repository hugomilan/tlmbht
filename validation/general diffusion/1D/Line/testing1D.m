% This code shows and compares all the outputs received

% Configuration for Octave:
more off %turns pagination off

% Inputs
Tc = 37;
Ts = 150;

% Characteristics for the mediums. They must be vectors
ro = 1200; % tissue density (kg/m3)
cp = 3200; % specific heat (J/(K-kg))
k = 0.3; % thermal conductivity (W/(K-m))
Ti = 37; % initial temperature

wb = 1e-4; % blood perfusion (s-1)
pb = 1052; % blood density (kg/m3)
cb = 3600; % blood specific heat (J/(K-kg))
Tb = 37; % temperature of the blood (oC)

Qmet = 500*0; % internal heat generation (W/m3)

% last position that has temperature values
number_Temp = save_1(1)*numbers_1(1) + save_1(2)*numbers_1(3);

% last position that has heat flux values
number_Flux = number_Temp + save_1(3)*numbers_1(3);

% last position that contains points positions
number_Points = save_1(1)*numbers_1(1) + ( save_1(2) || save_1(3) )*numbers_1(3);

% first position that has the position at the interface
number_Points_Flux = save_1(1)*numbers_1(1) + 1;

figure_defined = 0;


for (time_Ana = 0.01:0.01:10)


[Ttemp, qxAnatemp] = D1_BHE_f(Points_Output_1(1:number_Points,1)', 1e-3, time_Ana, ...
             Ts, Tc, k, ro, cp, wb, pb, cb, Tb, Qmet, 50);

             plot(Points_Output_1(1:numbers_1(1),1), Ttemp(1:numbers_1(1))', '*b')
             hold on 
             plot(Points_Output_1((1 + numbers_1(1)*save_1(1) ):number_Temp,1), Ttemp((1 + numbers_1(1)*save_1(1) ):number_Temp)', 'ob')
             hold off

             fflush(stdout);
             pause(0.1)
             
             
end
