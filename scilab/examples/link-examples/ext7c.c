/* Copyright INRIA */
#include "../../routines/stack-c.h"

/******************************************
 *     example 7 
 *     creating vector c in scilab internal stack 
 *     -->link('ext7c.o','ext7c','C') 
 *     -->a=[1,2,3]; b=[2,3,4]; 
 *     c does not exist (c made by the call to matz) 
 *     -->call('ext7c',a,1,'d',b,2,'d','out',1); 
 *     c now exists 
 *     -->c=a+2*b 
 ******************************************/

static int c1 = 1;
static int c3 = 3;

int ext7c(a, b)
     double *a, *b;
{
  double c[3],w;
  int k,f=4;
  for (k = 0; k < 3; ++k) 
    c[k] = a[k] + b[k] * 2.;
  /** sending c[] to scilab variable c (size [1,3]) **/
  WriteMatrix("c",&c1,&c3,c);
  /** sending f characters of "test" to Scilab variable d **/
  WriteString("d",&f,"test");
  return(0);
}

