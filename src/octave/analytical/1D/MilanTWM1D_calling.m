% Values for testing.
k = 0.2;
qflux = 4000;
tau = 30;
p = 1000;
cp = 4200;
wb = 1e-4;
pb = p;
cb = cp;
Ti = 37;
Ta = 36*0;
Tinf = 20;
L = 0.015;
Qm = 500*0;
Qr = 0;
x = ((0:100)*L)/100;



ninf = 5000;
time = 0.1;
timeEnd = 300;
T = zeros((timeEnd/time)+1, size(x, 2));

for t = 0:(timeEnd/time)
    T(t+1,:) = MilanTWM1D(x, L, time*t, Tinf, Ti, k, p, cp, tau, wb, pb, cb, Ta, ...
        Qm, Qr, ninf);
    
    plot( x, T(t+1,:) )
    ylim([0.9*Tinf 1.1*Ti])
    xlim([0 L])
    pause(0.01)
end
