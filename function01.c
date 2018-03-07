#include <math.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <omp.h>
double main(int argc, char *argv[]) { 
const double pi = 4. * atan(1.);
double BHE__temperature;
sscanf(argv[1], "%lf", &BHE__temperature);
double BHE__heat_flux;
sscanf(argv[2], "%lf", &BHE__heat_flux);
double STD__x;
sscanf(argv[3], "%lf", &STD__x);
double STD__y;
sscanf(argv[4], "%lf", &STD__y);
double STD__z;
sscanf(argv[5], "%lf", &STD__z);
double BHE__time;
sscanf(argv[6], "%lf", &BHE__time);
double BHE__time_step;
sscanf(argv[7], "%lf", &BHE__time_step);
double output_f1;

double temp1, temp2, temp3;
temp1 = cos(pi*BHE__time)*exp(BHE__temperature);
temp2 = BHE__heat_flux/BHE__time_step;
temp3 = 50;
output_f1 = temp1 + temp2 + temp3;

return output_f1;
}
