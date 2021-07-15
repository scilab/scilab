// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Unitary test for conversion of Matlab function: ISMATRIX

syntaxes = [
    "ismatrix([])"
    "ismatrix(1)"
    "ismatrix(pi)"
    "ismatrix(''a'')"
    "ismatrix(''abc'')"
    "ismatrix([1 2])"
    "ismatrix([1;2])"
    "ismatrix([1 2 ; 3 4])"
    "ismatrix(rand(1,1,3))"
    "%m2scideclare A|1 1|Double|Real"
    "ismatrix(A)"
    "%m2scideclare B|1 1 3|?|?"
    "ismatrix(B)"
    "%ismatrix(B(:,2,1))"    // TODO
    "ismatrix(@sin)"        // Issue with wrong @ replacement
    ];

mfile = TMPDIR + "/ismatrix";
mputl(syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR);
mprintf("%s\n", mgetl(mfile + ".sci"));
