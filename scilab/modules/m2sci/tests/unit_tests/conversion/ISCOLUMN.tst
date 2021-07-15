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
// Unitary test for conversion of Matlab function: ISCOLUMN

Syntaxes = [
    "iscolumn([])"
    "iscolumn(2)"
    "iscolumn(gca())"
    "iscolumn([2 3])"
    "iscolumn([2 3]==4)"
    "a = iscolumn([2 3]'')"
    "%m2scideclare a|1 1|Boolean|Boolean"
    ];

mfile = TMPDIR + "/iscolumn";
mputl(Syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
res = mgetl(mfile + ".sci");
ref = [
  ""
  "iscolumn([])"
  "iscolumn(2)"
  "iscolumn(gca())"
  "iscolumn([2,3])"
  "iscolumn([2,3]==4)"
  "a = iscolumn([2,3]'')"
  ];
assert_checkequal(res, ref);
