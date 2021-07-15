// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Unitary test for conversion of Matlab function: ISVECTOR

exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciTestConversion.sci", -1);

assert_checktrue(m2sciTestConversion("ISVECTOR"));

syntaxes = [
    "isvector([])"
    "isvector(1)"
    "isvector(pi)"
    "isvector(''abc'')"
    "isvector(gca())"
    "isvector(rand(1,1,3))"
    "if ~isvector(pi + rand(1,1,3))"
    "   a=1"
    "end"
    ];

mfile = TMPDIR + "/isvector";
mputl(syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR);
mprintf("%s\n", mgetl(mfile + ".sci"));
