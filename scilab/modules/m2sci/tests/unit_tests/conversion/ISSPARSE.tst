// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// Unitary test for conversion of Matlab function: ISSPARSE

exec("SCI/modules/m2sci/tests/unit_tests/utils/m2sciTestConversion.sci", -1);

if ~m2sciTestConversion("ISSPARSE") then pause;end

Syntaxes = [
    "%m2scideclare a|3 5|Sparse|Real"     ""
    "issparse(a)"                         "or(type(a)==[5,6])"
    "%m2scideclare b|3 5|Sparse|?"        ""
    "issparse(b)"                         "or(type(b)==[5,6])"
    "%m2scideclare c|3 5|Boolean|?"       ""
    "issparse(c)"                         "or(type(c)==[5,6])"
    "%m2scideclare d|3 5|Boolean|Sparse"  ""
    "issparse(d)"                         "or(type(d)==[5,6])"
    "%m2scideclare e|3 5|?|?"             ""
    "issparse(e)"                         "or(type(e)==[5,6])"
    ];


mfile = TMPDIR + "/OPERATORS_special_syntax";
mputl(Syntaxes(:,1), mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, [""; Syntaxes(:,2)]);
