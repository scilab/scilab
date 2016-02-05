// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [g,u] = %i_bezout(a,b)
    //   g = bezout(a,b) is the greatest common divisor of  a and b.
    //       a and b must contain non-negative   integer scalars.
    //   [g,U] = bezout(a,b) also returns a (2x2) unimodular matrix U such that:
    //   [a,b]*U = [g,0].
    //   These are useful for solving Diophantine equations and computing
    //   Hermite transformations.

    it=max(inttype(a),inttype(b))
    a=iconvert(a,it);b=iconvert(b,it)
    u = [iconvert([1 0],it) a];
    v = [iconvert([0 1],it) b];
    zero=iconvert(0,it)
    while v(3)<>zero
        q = u(3)/v(3);
        t = u - v*q;
        u = v;
        v = t;
    end
    g = u(3);
    u=[u(1) -v(1);u(2) -v(2)]

endfunction
