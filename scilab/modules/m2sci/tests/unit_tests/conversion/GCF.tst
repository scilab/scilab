// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Unitary test for conversion of Matlab function: GCF

Syntaxes = [
    "idf = gcf"
    "%m2scideclare idf|1 1|Handle|Figure"
    ];

Ref = [
    ""
    "idf = gcf()"
    ];

mfile = TMPDIR + "/_gcf";
mputl(Syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, Ref);
