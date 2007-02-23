#include "temptst.h"

N1<double> callsquare(N1<double> n1) {
  double x = n1.sqr();
  return N1<double>(x);
}

