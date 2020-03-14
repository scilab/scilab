// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 16365 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16365
//
// <-- Short Description -->
// median(m,"r") and median(m,"c") were wrong
// ===================================================================

m = [
   2.   8.   9.   6.   9.   4.
   2.   3.   4.   6.   8.   6.
   4.   5.   7.   6.   6.   5.
   5.   5.   6.   3.   5.   0.
   4.   0.   9.   0.   3.   7.
   1.   6.   9.   1.   3.   6.
   9.   9.   3.   2.   7.   5.
   5.   0.   9.   9.   8.   4.
   ];

//--> gsort(m,"r","i")
// ans  =
//   1.   0.   3.   0.   3.   0.
//   2.   0.   4.   1.   3.   4.
//   2.   3.   6.   2.   5.   4.
//   4.   5.   7.   3.   6.   5.
//   4.   5.   9.   6.   7.   5.
//   5.   6.   9.   6.   8.   6.
//   5.   8.   9.   6.   8.   6.
//   9.   9.   9.   9.   9.   7.
ref=[4    5    8    4.5  6.5  5 ];
assert_checkequal(median(m,"r"), ref);

//--> gsort(m,"c","i")
// ans  =
//   2.   4.   6.   8.   9.   9.
//   2.   3.   4.   6.   6.   8.
//   4.   5.   5.   6.   6.   7.
//   0.   3.   5.   5.   5.   6.
//   0.   0.   3.   4.   7.   9.
//   1.   1.   3.   6.   6.   9.
//   2.   3.   5.   7.   9.   9.
//   0.   4.   5.   8.   9.   9.
ref = [7 5 5.5 5 3.5 4.5 6 6.5]';
assert_checkequal(median(m,"c"), ref);
