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
// Unitary test for conversion of Matlab function: ISMAC


Syntaxes = [
    "ismac"         "getos()==""Darwin"""
    "tf = ismac"    "tf = getos()==""Darwin"""
    ];

mfile = TMPDIR + "/_ismac";
mputl(Syntaxes(:,1), mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
res = mgetl(mfile + ".sci");

assert_checkequal(res, [""; Syntaxes(:,2)]);
