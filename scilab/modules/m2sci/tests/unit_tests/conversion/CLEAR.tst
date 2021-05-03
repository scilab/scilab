// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Unitary test for conversion of Matlab function: CLEAR

// ---------------------------------------------


Syntaxes = [
    "% Matlab version: 6.5.0.180913a (R13)"
    "clear"
    "clear name"
    "clear name1 name2 name3"
    "clear global name"
    "clear all"
    "clear classes"
    "clear functions"
    "clear global"
    "clear import"
    "clear variables"
    "clear(''name1'',''name2'',''name3'')"
    ];

Ref = [
    ""
    "// Matlab version: 6.5.0.180913a (R13)"
    "clear"
    "clear(""name"")"
    "clear(""name1"",""name2"",""name3"")"
    "clearglobal(""name"")"
    "clear"
    "// !! L.7: No Scilab equivalent for: clear(""classes"")"
    "clear(""classes"")"
    "// !! L.8: No Scilab equivalent for: clear(""functions"")"
    "clear(""functions"")"
    "clearglobal"
    "// !! L.10: No Scilab equivalent for: clear(""import"")"
    "clear(""import"")"
    "clear"
    "clear(""name1"",""name2"",""name3"")"
    ];

mfile = TMPDIR + "/_clear";
mputl(Syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, Ref);

