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
//      at index #1, the value of the cost function
//      at index #2 to the end is the list of the values of the nonlinear
//      inequality constraints
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
// The same cost function as before, with an
// additional argument, which contains parameters of the
// cost function and constraints.
// In this case, the mydata variable is passed
// explicitely by the optimization class.
// So the actual name "mydata" does not matter
// and whatever variable name can be used.
//
function [ f , c , index , mydata ] = gouldnonconvex2 ( x , index , mydata )
    f = []
    c = []
    if ( index==2 | index==6 ) then
        f = (x(1) - mydata.f1 )^3 + ( x(2) - mydata.f2 ) ^ 3
    end
    if ( index==5 | index==6 ) then
        c1 = x(1) - mydata.a1
        c2 = ( x(1) - 5.0 )^2  + (x(2) - 5.0 )^2 - mydata.a2
        c3 = -( x(1) - 6.0 )^2 - (x(2) - 5.0 )^2 + mydata.a3
        c4 = x(2)
        c = [c1 c2 c3 c4]
    end
endfunction
//
// Test optimbase_isfeasible method
//
// Test with bounds
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-verbose",1);
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
[ opt , isfeasible ] = optimbase_isfeasible ( opt ,  [0.0 0.0] );
assert_checkequal ( isfeasible , 1 );
[ opt , isfeasible ] = optimbase_isfeasible ( opt ,  [-6.0 0.0] );
assert_checkequal ( isfeasible , 0 );
[ opt , isfeasible ] = optimbase_isfeasible ( opt ,  [0.0 6.0] );
assert_checkequal ( isfeasible , 0 );
opt = optimbase_destroy(opt);
//
// Test with nonlinear inequality constraints
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-verbose",1);
opt = optimbase_configure(opt,"-nbineqconst",4);
opt = optimbase_configure ( opt , "-function" , gouldnonconvex );
[ opt , isfeasible ] = optimbase_isfeasible ( opt ,  [ 14.0950013 , 0.8429636 ] );
assert_checkequal ( isfeasible , 1 );
[ opt , isfeasible ] = optimbase_isfeasible ( opt ,  [ 14.0950013 , 0.0 ] );
assert_checkequal ( isfeasible , -1 );
opt = optimbase_destroy(opt);
//
// Test with nonlinear inequality constraints and additional argument in cost function
// Set verbose to 1 to check that verbose mode is operationnal.
mystuff = struct();
mystuff.f1 = 10.0;
mystuff.f2 = 20.0;
mystuff.a1 = 13.0;
mystuff.a2 = 100.0;
mystuff.a3 = 82.81;
opt = optimbase_new ();
opt = optimbase_configure ( opt , "-numberofvariables",2);
opt = optimbase_configure ( opt , "-verbose",1);
opt = optimbase_configure ( opt , "-nbineqconst",4);
opt = optimbase_configure ( opt , "-function" , list(gouldnonconvex2,mystuff) );
[ opt , isfeasible ] = optimbase_isfeasible ( opt ,  [ 14.0950013 , 0.8429636 ] );
assert_checkequal ( isfeasible , 1 );
[ opt , isfeasible ] = optimbase_isfeasible ( opt ,  [ 14.0950013 , 0.0 ] );
assert_checkequal ( isfeasible , -1 );
opt = optimbase_destroy(opt);

