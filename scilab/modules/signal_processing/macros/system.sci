// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1988 - INRIA - C. Bunks
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
function [x1, y] = system(x0, f, g, h, q, r)
    //<x1,y>=system(x0,f,g,h,q,r)
    //define system macro which generates the next
    //observation given the old state
    //  x0 :Input state vector
    //  f  :System matrix
    //  g  :Input matrix
    //  h  :Output matrix
    //  q  :Input noise covariance matrix
    //  r  :Output noise covariance matrix
    //  x1 :Output state vector
    //  y  :Output observation
    //System recursively calculates
    //
    //     x1=f*x0+g*u
    //      y=h*x0+v
    //
    //where u is distributed N(0,q)
    //and v is distribute N(0,r).

    [lhs, rhs] = argn(0);
    if rhs == 0 then
        error(999, msprintf(_("%s: Wrong number of input argument(s).\n"), "system"));
    end

    rand("normal");
    q2 = chol(q);
    r2 = chol(r);
    u = q2' * rand(ones(x0));
    v = r2' * rand(ones(x0));
    x1 = f * x0 + g * u;
    y = h * x0 + v;
endfunction
