/* Copyright Inria/Enpc */

/*********************************************
 *     external fonction for ode 
 *     input variables n, t, y 
 *     n = dimension of state vector y 
 *     t = time 
 *     y = state variable 
 *     output variable = ydot 
 *    ================================
 *     external routine must 
 *     load ydot[0] with d/dt ( y(1)(t) ) 
 *          ydot[1] with d/dt ( y(2)(t) ) 
 *          ... 
 *     i.e. ydot vector of derivative of state y 
 *    ================================
 *     With dynamic link: 
 *     link('ext10c.o','ext10c','C') 
 *    ================================
 *     passing a parameter to ext6 routine by a list: 
 *     -->param=[0.04,10000,3d+7]; 
 *     -->y=ode([1;0;0],0,[0.4,4],list('ext10c',param)) 
 *     param is retrieved in ext6 by: 
 *     param[0]=y[ *n ] , param[1]=y[ *n + 1] etc 
 *     with this calling sequence y is a n+np vector 
 *     where np=dimension of scilab variable param 
 *********************************************/

int ext10c(n, t, y, ydot)
     int *n;
     double *t, *y, *ydot;
{
  static double param[3];
  param[0] = y[*n + 0];
  param[1] = y[*n + 1];
  param[2] = y[*n + 2];
  ydot[0] = -param[0] * y[0] + param[1] * y[1] * y[2];
  ydot[2] = param[2] * y[1] * y[1];
  ydot[1] = -ydot[0] - ydot[2];
  return(0);
} 

