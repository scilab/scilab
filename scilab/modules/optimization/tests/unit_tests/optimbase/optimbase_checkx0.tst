// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->


//
// gould.nonconvex --
//   The Gould test case with additional inequality constraints.
// Arguments
//    x : the point where to compute the cost
//    index : a flag which states what is to compute
//    * if index=1, or no index, returns the value of the cost
//      function (default case)
//    * if index=2, returns the value of the nonlinear inequality
//      constraints, as a row array
//    * if index=3, returns an array which contains
//      at index #0, the value of the cost function
//      at index #1 to the end is the list of the values of the nonlinear
//      constraints
//  Discussion:
//    The problem is to minimize a cost function with 4 non linear constraints.
//    This is Problem 4.1 in Subrahmanyam, extracted from Gould.
//    Non convex.
//    The constraint region is a narrow winding (half-moon shaped) valley.
//    Solution showed with tolerance 1.e-8.
//
//  Reference:
//    An extension of the simplex method to constrained
//    nonlinear optimization
//    M.B. Subrahmanyam
//    Journal of optimization theory and applications
//    Vol. 62, August 1989
//
//    Gould F.J.
//    Nonlinear Tolerance Programming
//    Numerical methods for Nonlinear optimization
//    Edited by F.A. Lootsma, pp 349-366, 1972
//
function [ f , c , index ] = gouldnonconvex ( x , index )
    f = []
    c = []
    if ( index==2 | index==6 ) then
        f = (x(1) - 10.0 )^3 + ( x(2) - 20.0 ) ^ 3
    end
    if ( index==5 | index==6 ) then
        c1 = x(1) - 13.0
        c2 = ( x(1) - 5.0 )^2  + (x(2) - 5.0 )^2 - 100.0
        c3 = -( x(1) - 6.0 )^2 - (x(2) - 5.0 )^2 + 82.81
        c4 = x(2)
        c = [c1 c2 c3 c4]
    end
endfunction
//
// Test optimbase_checkx0 method
//
// Test without anything
opt = optimbase_new ();
opt = optimbase_configure ( opt , "-numberofvariables",2);
opt = optimbase_configure ( opt , "-verbose",1);
opt = optimbase_checkx0 ( opt );
opt = optimbase_destroy(opt);
//
// Test with satisfied/unsatisfied bounds constraints
opt = optimbase_new ();
opt = optimbase_configure ( opt , "-numberofvariables",2);
opt = optimbase_configure ( opt , "-verbose",1);
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
opt = optimbase_configure ( opt , "-x0", [1.0 1.0]' );
opt = optimbase_checkx0 ( opt );
opt = optimbase_configure ( opt , "-x0",[-6.0 1.0]');
instr = "opt = optimbase_checkx0 ( opt )";
lclmsg = gettext("%s: Initial guess is not feasible.");
assert_checkerror(instr,lclmsg,[],"optimbase_checkx0" );
opt = optimbase_destroy(opt);
//
// Test with satisfied/unsatisfied nonlinear inequality constraints
opt = optimbase_new ();
opt = optimbase_configure ( opt , "-numberofvariables",2);
opt = optimbase_configure ( opt , "-verbose",1);
opt = optimbase_configure ( opt , "-nbineqconst",4);
opt = optimbase_configure ( opt , "-function" , gouldnonconvex );
opt = optimbase_configure ( opt , "-x0" , [ 14.0950013 , 0.8429636 ]');
opt = optimbase_checkx0 ( opt );
opt = optimbase_configure ( opt , "-x0" , [ 14.0950013 , 0.0 ]');
instr = "opt = optimbase_checkx0 ( opt )";
lclmsg = gettext("%s: Initial guess is not feasible.");
assert_checkerror(instr,lclmsg,[],"optimbase_checkx0" );
opt = optimbase_destroy(opt);

