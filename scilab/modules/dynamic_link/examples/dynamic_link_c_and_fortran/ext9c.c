/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */


/*********************************************
 *     argument function for ode
 *     input variables n, t, y
 *     n = dimension of state vector y
 *     t = time
 *     y = state variable
 *     output variable = ydot
 *     ================================
 *     external routine must
 *     load ydot[0] with d/dt ( y(1)(t) )
 *          ydot[1] with d/dt ( y(2)(t) )
 *          ...
 *     i.e. ydot vector of derivative of state y
 *     ================================
 *     Example:
 *     call this ext5 routine:
 *     ode([1;0;0],0,[0.4,4],'ext9c')
 *     ================================
 *     With dynamic link:
 *     -->link('ext9c.o','ext9c','C')
 *     -->ode([1;0;0],0,[0.4,4],'ext9c')
 *********************************************/

int ext9c(int *n, double *t, double *y, double *ydot)
{
    ydot[0] = y[0] * -.04 + y[1] * 1e4 * y[2];
    ydot[2] = y[1] * 3e7 * y[1];
    ydot[1] = -ydot[0] - ydot[2];
    return (0);
}



