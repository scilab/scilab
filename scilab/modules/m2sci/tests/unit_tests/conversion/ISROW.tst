// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Unitary test for conversion of Matlab function: ISROW

Syntaxes = [
    "isrow([])"
    "isrow(2)"
    "isrow(gca())"
    "isrow([2 3])"
    "isrow([2 3]==4)"
    "a = isrow([2 3]'')"
    "%m2scideclare a|1 1|Boolean|Boolean"
    ];

mfile = TMPDIR + "/isrow";
mputl(Syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
res = mgetl(mfile + ".sci");
ref = [
  ""
  "isrow([])"
  "isrow(2)"
  "isrow(gca())"
  "isrow([2,3])"
  "isrow([2,3]==4)"
  "a = isrow([2,3]'')"
  ];
assert_checkequal(res, ref);
