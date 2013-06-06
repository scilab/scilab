// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 477 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=477
//
// <-- Short Description -->
//    Bug Report Id: 12044200362419444
//     %hm_maxi and %hm_mini are indeed currently defined with a _single_ return argument. IMHO they should be extended analogously to max, min and %hm_find, returning optionally either a vector of indices of the max/min.
//
//    On Scilab CVS with " %hm_maxi, %hm_mini " function
//    The Error Messages are:
//     -->a=hypermat([3,3,1],1:9);
//
//    -->[m]=max(a)
//     i  =
//
//     9.
//
//    -->[m,i]=max(a)
//                 !--error    59
//    incorrect # of outputs in the function
//    arguments are :
//     x
//
//    -->[m,i,j,k]=max(a)
//                     !--error    78
// ...

a   = hypermat([1,3,3],1:9);
assert_checkequal(max(a), 9);

result = execstr("[n,i]=max(a)","errcatch","n");
assert_checkequal(result, 0);
assert_checkequal(n, 9);
assert_checkfalse(or(i<>[1,3,3]));
errmsg=msprintf(_("%s: Wrong number of output argument(s): %d to %d expected.\n"), "max", 1, 2);
assert_checkerror("[m,i,j,k]=max(a)", errmsg, 78);
