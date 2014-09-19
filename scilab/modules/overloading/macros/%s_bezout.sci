// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [g,u] = %s_bezout(a,b)
    //   g = bezout(a,b) is the greatest common divisor of a and b.
    //       a and b must contain non-negative integer scalars.
    //   [g,U] = bezout(a,b) also returns a (2x2) unimodular matrix U such that:
    //   [a,b]*U = [g,0].
    //   These are useful for solving Diophantine equations and computing
    //   Hermite transformations.

    u = [1 0 a];
    v = [0 1 b];
    while v(3)<>0
        q = floor(u(3)/v(3));
        t = u - v*q;
        u = v;
        v = t;
    end
    g = u(3);
    u=[u(1) -v(1);u(2) -v(2)]

endfunction
