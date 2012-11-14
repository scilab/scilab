// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3014 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3014
//
// <-- Short Description -->
//    I used variable n in the equation for first argument of integrate(),
//    but the value of n are not considered in the calculation result.
//    Both calculation results with n=2 and n=4 are same.
//    I checked the source of integrate( ) and I found the reason of this bug.
//    The variable n and m are used in integrate(). And function func is also used.
//    These cause that if we use n, m, or func in the string for the first or second
//    argument of integrate( ), the calculation result is not correct.
//    So we can not use n, m, and func for the arguments of integrate( ).
//    I think, the name of variables in integrate( ) source had better start with '_'.


n=2;
solcomputed=integrate('x^n', 'x', 0, 1);
solexpected=1/3;
if (abs(solcomputed-solexpected)>%eps) then pause,end
