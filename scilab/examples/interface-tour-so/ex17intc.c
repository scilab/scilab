#include "stack-c.h"

/**************************************************
 * Shows how to back convert data to Scilab internal formal 
 * when data was modified when extracted (for example when 
 * using 'i','r','z' in GetRhsVar ) 
 **************************************************/

int intex17c_1(fname)
  char* fname;
{ 
  int m,n,m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,l5;
  int it1,lc1, l;

  CheckRhs(1,1);
  CheckLhs(1,1);

  GetRhsVar(1,"l",&m,&n,&l);
  CheckLength(1,m,5);
  GetListRhsCVar(1,1,"i",&it1,&m1,&n1,&l1,&lc1);
  GetListRhsVar(1,2,"i",&m2,&n2,&l2);
  GetListRhsVar(1,3,"c",&m3,&n3,&l3);
  GetListRhsVar(1,4,"r",&m4,&n4,&l4);
  GetListRhsVar(1,5,"z",&m5,&n5,&l5);

  /* first list argument cannot be returned directly 
   * since the previous GetListRhsVar have performed 
   * data conversion. But we can back-convert data 
   * before returning the list. 
   */ 

  /* Backconvert to scilab internal default format */

  ConvertData("i",m1*n1*(it1+1),l1);
  ConvertData("i",m2*n2,l2);
  ConvertData("c",m3*n3,l3);
  ConvertData("r",m4*n4,l4);
  ConvertData("z",m5*n5,l5);

  LhsVar(1)=1;
  return(0);
}
