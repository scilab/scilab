//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 480 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=480
//
// <-- Short Description -->
//    Bug Report Id: 12070200362710754
//    [u]intN() and iconvert() do not handle hypermatrices. It would be nice if they would.
//    There is a workaround -
//    hypermat(a.dims,int8(a.entries)) - but it is somehow a detour.
//
//    On Scilab CVS with " intN(), iconvert() " function
//    The Error Messages are:
//     -->a=hypermat([3,3,1],1:9);
//
//    -->int8(a)
//            !--error    53
//    invalid input (waiting for real or complex matrix)
//
//    Commands:
//
//    Enrico Segre on Linux version  distribution RH9 with   as window manager
//    Israel  July 27, 2003 at 10:7:54

a=matrix(1:9, [1,3,3]);
assert_checktrue(execstr("int8(a)", "errcatch")==0);
