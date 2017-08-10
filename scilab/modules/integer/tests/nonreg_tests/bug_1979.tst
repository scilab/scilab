//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1979 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1979
//
// <-- Short Description -->
//    I think that something is forgotten in the integer treatment in Scilab :
//
//    -->a=int32(1);b=int32(6);
//    -->a:b
//    a:b
//        !--error 4
//    undefined variable : %i_b_i

a = int32(1);
b = int32(6);
c = a:b;
d = int32([1 2 3 4 5 6]);

if  or( c <> d ) then pause,end
