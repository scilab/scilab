#include <string.h> 
#include <stdio.h>
#include "stack-c.h"
#include "lib/pipo.h"

int int_f4(char *fname) 
{
  static int l1,m1, n1;
  CheckRhs(1,1);
  CheckLhs(1,1); 
  GetRhsVar(1, "i", &m1, &n1, &l1);
  CheckScalar(1,m1,n1); 
  *istk(l1)= foo(*istk(l1)); 
  *istk(l1)= bar(*istk(l1)); 
  LhsVar(1) = 1;
  return 0;
}
