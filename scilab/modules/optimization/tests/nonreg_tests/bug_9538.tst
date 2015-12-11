// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 9538 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9538
//
// <-- Short Description -->
// optimbase_checkshape() only allowed row vectors as return from the cost function.
// Allowed column vectors.
//

// Load the program as it used to work. The fix for this bug does not apply to polygon.sce as it is.
exec("SCI/modules/optimization/demos/neldermead/polygon.sce", -1);

// Now redefine largesmallpolygon() without the "transposition for neldermead"
// and check that we get the same result as before.
function [ f , c , index ] = largesmallpolygon ( x , index )
    // Bibliography
    //   "Benchmarking Optimization Software with Cops"
    //   Dolan, Moré, 2001
    //   Section 1, "Largest Small Polygon"
    //
    //   "Biggest Little Polygon"
    //   http://mathworld.wolfram.com/BiggestLittlePolygon.html
    //
    //   Audet, C. "Optimisation globale structurée: propriétés, équivalences et résolution."
    //   Thèse de Doctorat. Montréal, Canada: École Polytechnique de Montréal, 1997.
    //   http://www.gerad.ca/Charles.Audet.
    //
    // Known optimal values are
    // A6 = 0.677981 (Wolfram Mathworld)
    // A8 = 0.726869 (Wolfram Mathworld)
    // A25 = 0.77974 (Dolan & Moré - SNOPT)
    // A50 = 0.784016 (Dolan & Moré - SNOPT)
    // A75 = 0.784769 (Dolan & Moré - SNOPT)
    // A100 = 0.785040 (Dolan & Moré - SNOPT)
    //
    nv = size(x,"*")/2
    f = []
    c = []
    // nv: number of vertices
    // x : a (2*nv)-by-1 matrix of doubles, where
    // r is in [0,1]
    // t is in [0,pi]
    r = x(1:nv)
    t = x(nv+1:2*nv)

    //polygon_draw(r ,t , nv);
    //pause

    if ( index == 2 | index == 6 ) then
        f = polygon_area(r ,t , nv)
        f = -f
    end
    if ( index == 5 | index == 6 ) then
        c = zeros(nv^2+nv-1,1)
        // Set the diameters
        d = polygon_diameters(r ,t , nv)
        d = matrix(d,nv^2,1)
        c(1:nv^2) = 1-d
        // Set the angles
        c(nv^2+1:nv^2+nv-1) = t(2:nv)-t(1:nv-1)
        // Transpose for neldermead
        //c = c'
    end
endfunction


nv = 6;
rand("seed" , 0);
[A,r,t] = findlargestpolygon (nv);

assert_checkalmostequal(A, 0.677981, [], 1e-1);
