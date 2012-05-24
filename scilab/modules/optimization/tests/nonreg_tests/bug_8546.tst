// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8546 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8546
//
// <-- Short Description -->
// The neldermead function may call f outside the bounds.
//

function [ f , index ] = myquad ( x , index )
    mprintf("index=%d, x=[%.2f %.2f]\n",index,x(1),x(2))
    if ( or(x < 1) | or(x > 2) ) then
        error("Point not in bounds")
    end
    f = x(1)^2 + x(2)^2
endfunction
rand("seed" , 0)
x0 = [1.2 1.9].';
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",myquad);
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-boundsmin",[1 1]);
nm = neldermead_configure(nm,"-boundsmax",[2 2]);
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_search(nm);
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal(xopt,[1;1],1.e-6);
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal(fopt,2,1.e-4);
nm = neldermead_destroy(nm);
