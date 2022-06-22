#include "curves.h"
#include <math.h>

double c1(double x){
  return ((-2*x)+(0.0000003*x*x*x)); 
}

double c2(double x){
  return (sqrt(x+2*fabs(x))-sqrt(fabs(x))); 
}

double c3(double x){
  return (sin(x)); 
}