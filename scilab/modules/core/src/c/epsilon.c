#include "epsilon.h"
#include "machine.h" // C2F

extern double C2F(dlamch)(const char[1]);

double epsilon(void){
  return C2F(dlamch)("e");
}
