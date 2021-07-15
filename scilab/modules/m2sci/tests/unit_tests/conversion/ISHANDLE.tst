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
// Unitary test for conversion of Matlab function: ISHANDLE

Syntaxes = [
    "% Matlab version: 6.5.1.199709 (R13) Service Pack 1"
    "%m2scideclare h|? ?|?|?"
    "tf = ishandle(h)"
    "%m2scideclare h1|? ?|Handle"
    "tf1 = ishandle(h1)"
    "ishandle(@sin)     % Builtin"
    "ishandle(@gca)     % Macro"
    ];

Ref = [
    ""
    "// Matlab version: 6.5.1.199709 (R13) Service Pack 1"
    ""
    "tf = type(h)==9"
    ""
    "tf1 = type(h1)==9"
    "type(sin)==9  // Builtin"
    "type(gca)==9  // Macro"
    ];

mfile = TMPDIR + "/_ishandle";
mputl(Syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, Ref);
