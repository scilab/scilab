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
// Unitary test for conversion of Matlab function: GRAYMON

Syntaxes = [
    "graymon"   "gdf().color_map = [0.75;0.5;0.25] * ones(1,3); "
    ];

mfile = TMPDIR + "/_graymon";
mputl(Syntaxes(:,1), mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, ["" ; Syntaxes(:,2)]);
