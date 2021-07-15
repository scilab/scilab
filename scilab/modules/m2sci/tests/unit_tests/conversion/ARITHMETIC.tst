// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- LONG TIME EXECUTION -->
//
// <-- Short Description -->
// Unitary test for conversion of Matlab function: ARITHMETIC operators

Syntaxes = [
    "% Matlab version: "    "// Matlab version: "
    "% 7.6.0.324 (R2008a)"  "// 7.6.0.324 (R2008a)"
    "a = 1+1;"              "a = 1+1; "
    "b = 1++1;"             "b = 1+1; "
    "c = 1+-1;"             "c = 1+-1; "
    "d = 1*+1;"             "d = 1*1; "
    "e = 1/+1"              "e = 1/1"
    "f = 1*-1;"             "f = 1*-1; "
    "g = 1/-1"              "g = 1/-1"
    "h = 1--1;"             "h = 1--1; "
    "k = 1-+1;"             "k = 1-1; "
    "l = 1++-+-+-2;"        "l = 1+-2; "
    "m = 1-1;"              "m = 1-1; "
    ];

mfile = TMPDIR + "/_arithmetic";
mputl(Syntaxes(:,1), mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, ["" ; Syntaxes(:,2)]);

