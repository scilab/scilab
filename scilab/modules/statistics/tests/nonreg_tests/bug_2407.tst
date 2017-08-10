//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA -Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2407 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2407
//
// <-- Short Description -->
//    stdev and st_deviation are not available for hypermatrix.
//
//    When trying to calculate stdev of an hypermatrix, an error occurs
//    systematically. The stdev of an hypermatrix should be managed by the %
//    hm_st_deviation function in Scilab.
//    After a quick look at the code of this function, i think i've
//    understood the origin of the bug. There is a mix between two variables :
//    N and d.
//    For example, at line 8, instruction "p1=prod(dims(1:d-1));" should be
//    replace by p1=prod(dims(1:N-1));
//    I think that the replacement of d by N in all the function could solve
//    the problem.
//
//    Regards.

a = zeros(3,3,3);
b = stdev(a);

if b <> 0 then pause,end
