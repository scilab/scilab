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
// <-- LONG TIME EXECUTION -->
//
// <-- Short Description -->
// Unitary test for conversion of ARITHMETIC_special_syntax:
//
// Old versions of Scilab (before Scilab 4 I think) did not manage
// multiple operators such as in the following cases
// Modifications were made in m2sci_syntax.sci

Syntaxes = [
    "a = 1++2;"                "a = 1+2; "
    "d = 1-+2;"                "d = 1-2; "
    "e = 1+-+2;  % was 1-2"    "e = 1+-2;   // was 1-2"
    "f = 1-+-2;  % was 1+2"    "f = 1--2;   // was 1+2"
    "g = 1---2;"               "g = 1-2; "
    "h = 1+++2;"               "h = 1+2; "
    "k = 1*+2;"                "k = 1*2; "
    "l = 1/+2;"                "l = 1/2; "
    "% No longer simplified"   "// No longer simplified"
    "b = 1--2;  % was 1+2"     "b = 1--2;   // was 1+2"
    "c = 1+-2;  % was 1-2"     "c = 1+-2;   // was 1-2"
    ];


mfile = TMPDIR + "/OPERATORS_special_syntax";
mputl(Syntaxes(:,1), mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, [""; Syntaxes(:,2)]);
