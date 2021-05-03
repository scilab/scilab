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
// Unitary test for conversion of Matlab function: DEAL

Syntaxes = [
    "[a,b,c] = deal(2)"
    "[a,b,c] = deal(1:3);"
    "[a,b,c] = deal(1+32);"
    "[a,b,c] = deal(1+324);"
    "[a,b,c] = deal(gca());"
    "[a,b,c] = deal([pi,i]);"
    "[a,b,c] = deal(i, sin(1:3), ''AB'');"
    "b = [''CD'';''EF'';''GH''];"
    "%[a,b(3)] = deal(sin(1:3), ''AB'');"       // http://bugzilla.scilab.org/699
    "%[a.myfield, b] = deal(sin(1:3), ''AB'');" // http://bugzilla.scilab.org/699
    ];
Ref = [
    ""
    "[a,b,c] = (2,2,2)"
    "[a,b,c] = (1:3,1:3,1:3); "
    "[a,b,c] = (1+32,1+32,1+32); "
    "[a,b,c] = (1+324,1+324,1+324); "
    "%v0 = gca(); [a,b,c] = (%v0,%v0,%v0); "
    "%v0 = [%pi,%i]; [a,b,c] = (%v0,%v0,%v0); "
    "[a,b,c] = (%i,sin(1:3),""AB""); "
    "b = [""CD"";""EF"";""GH""]; "
    "//[a,b(3)] = deal(sin(1:3), ''''AB'''');"
    "//[a.myfield, b] = deal(sin(1:3), ''''AB'''');"
    ];

mfile = TMPDIR + "/_deal";
mputl(Syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, Ref);
