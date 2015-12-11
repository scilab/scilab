// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 9537 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9537
//
// <-- Short Description -->
// optimbase_configure() only allowed row vector as initial value.
// Allowed column vector by transposing it.
//

// Load the program as it used to work. The fix for this bug does not apply to polygon.sce as it is.

exec("SCI/modules/optimization/demos/neldermead/polygon.sce", -1);

// Now redefine largesmallpolygon() without transposing x0 in neldermead_configure(nm,"-x0",x0');
// and check that we get the same result as before.
prot = funcprot();
funcprot(0);
function [A,r,t] = findlargestpolygon (nv)
    // Finds the largest smallest polygon with nv vertices
    // A : a 1-by-1 matrix of doubles, the area
    // r : a nv-by-1 matrix of doubles, the radius
    // t : a nv-by-1 matrix of doubles, the angle
    radius = 0.45;
    [r,t] = polygon_regular (nv);
    r = radius*r;
    x0 = [r;t];
    index = 6;
    [ f0 , c0 , index ] = largesmallpolygon ( x0 , index );
    mprintf("Current area = %f\n",-f0);
    mprintf("Constraint satisfaction = %f (expected positive)\n",min(c0));
    //
    // Setup bounds
    rmin = zeros(nv,1);
    rmax = ones(nv,1);
    tmin = -ones(nv,1)*%pi;
    tmax = ones(nv,1)*%pi;
    xmin=[rmin;tmin];
    xmax=[rmax;tmax];
    //
    nm = neldermead_new ();
    nm = neldermead_configure(nm,"-numberofvariables",2*nv);
    nm = neldermead_configure(nm,"-function",largesmallpolygon);
    nm = neldermead_configure(nm,"-x0",x0);
    nm = neldermead_configure(nm,"-maxiter",2000);
    nm = neldermead_configure(nm,"-maxfunevals",2000);
    nm = neldermead_configure(nm,"-method","box");
    nm = neldermead_configure(nm,"-boundsmin",xmin);
    nm = neldermead_configure(nm,"-boundsmax",xmax);
    nm = neldermead_configure(nm,"-nbineqconst",nv^2+nv-1);
    //
    // Check that the cost function is correctly connected.
    [ nm , result ] = neldermead_function ( nm , x0 );
    //
    // Perform optimization
    nm = neldermead_search(nm, "off");
    fopt = neldermead_get(nm,"-fopt")
    A = -fopt
    xopt = neldermead_get(nm,"-xopt")
    r = xopt(1:nv)
    t = xopt(nv+1:$)
    nm = neldermead_destroy(nm)
endfunction
funcprot(prot);

nv = 6;
rand("seed" , 0);
[A,r,t] = findlargestpolygon (nv);
assert_checkalmostequal(A, 0.677981, [], 1e-1);
